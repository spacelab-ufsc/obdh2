/*
 * mission_manager.c
 * 
 * Copyright The OBDH 2.0 Contributors.
 * 
 * This file is part of OBDH 2.0.
 * 
 * OBDH 2.0 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * OBDH 2.0 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with OBDH 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Mission Manager Task implementation
 * 
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 0.10.19
 * 
 * \date 2024/08/04
 * 
 * \addtogroup mission_man
 * \{
 */

#include <stdint.h>
#include <stdbool.h>

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <structs/satellite.h>
#include <structs/obdh_data.h>
#include <devices/payload/payload.h>
#include <devices/ttc/ttc.h>
#include <system/sys_log/sys_log.h>
#include <system/system.h>
#include <config/config.h>

#include "mission_manager.h"
#include "read_px.h"
#include "system_reset.h"

static inline void handle_event(const event_t *ev);
static inline int8_t handle_mode_change_rq(const uint8_t *args);
static inline int8_t activate_payload_rq(const uint8_t *args);
static inline int8_t deactivate_payload_rq(const uint8_t *args);

static int enable_ttc_tx(void);
static int disable_ttc_tx(void);

static bool in_hibernation = false;
static bool in_brazil = false;

QueueHandle_t event_queue;

TaskHandle_t xTaskMissionManagerHandle;

void vTaskMissionManager(void)
{
    event_t ev;
    sat_data_buf.state.c_edc = &sat_data_buf.edc_0;
    sat_data_buf.state.active_payload[0] = PAYLOAD_NONE;
    sat_data_buf.state.active_payload[1] = PAYLOAD_NONE;
    sat_data_buf.state.edc_active = false;

    while (1) 
    {
        if (xQueueReceive(event_queue, &ev, pdMS_TO_TICKS_64(TASK_MISSION_MANAGER_EV_NOTIFICATION_TIMEOUT)) == pdPASS) 
        {
            handle_event(&ev);
        }
        else 
        {
            sys_log_print_event_from_module(SYS_LOG_WARNING, TASK_MISSION_MANAGER_NAME, "Notification waiting timed out!");
            sys_log_new_line();
        }
    }
}

int8_t notify_event_to_mission_manager(const event_t *ev)
{
    int8_t err = 0;

    if (xQueueSendToBack(event_queue, ev, pdMS_TO_TICKS(10U)) != pdPASS)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, "Event Queue", "Failed to enqueue event");
        sys_log_new_line();

        err = -1;
    }

    return err;
}

void satellite_change_mode(const uint8_t mode)
{
    /* This ensures the mode change is done atomically */
    taskENTER_CRITICAL();
    sat_data_buf.obdh.data.mode = mode;
    sat_data_buf.obdh.data.ts_last_mode_change = system_get_time();
    taskEXIT_CRITICAL();
}

static inline void handle_event(const event_t *ev)
{
    switch (ev->event)
    {
        case EV_NOTIFY_IN_BRAZIL:
        {
            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_MISSION_MANAGER_NAME, "Satellite in Brazil");
            sys_log_new_line();
            
            in_brazil = true;

            if (!sat_data_buf.obdh.data.manual_mode_on)
            {
                /* Enable Main EDC and update state */
                payload_t main;

                switch (sat_data_buf.obdh.data.main_edc) 
                {
                    case CONFIG_PL_ID_EDC_1:
                        main = PAYLOAD_EDC_0;
                        break;
                    case CONFIG_PL_ID_EDC_2:
                        main = PAYLOAD_EDC_1;
                        break;
                    default:
                        main = PAYLOAD_NONE;
                        break;
                }

                if (main != PAYLOAD_NONE)
                {
                    if (sat_data_buf.state.active_payload[0] != main)
                    {
                        if (sat_data_buf.state.edc_active)
                        {
                            (void)payload_disable(sat_data_buf.state.active_payload[0]);
                        }

                        /* Update satellite state */
                        sat_data_buf.state.active_payload[0] = main;
                        sat_data_buf.state.c_edc = (main == PAYLOAD_EDC_0) ? &sat_data_buf.edc_0 : &sat_data_buf.edc_1;
                        sat_data_buf.state.edc_active = true;

                        (void)payload_enable(main);

                        if (!in_hibernation)
                        {
                            satellite_change_mode(OBDH_MODE_NORMAL);
                        }
                    }
                }
            }

            break;
        }
        case EV_NOTIFY_OUT_OF_BRAZIL:
        {
            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_MISSION_MANAGER_NAME, "Satellite out of Brazil");
            sys_log_new_line();

            in_brazil = false;

            if (!sat_data_buf.obdh.data.manual_mode_on)
            {
                if (sat_data_buf.state.edc_active && (sat_data_buf.state.active_payload[0] != PAYLOAD_NONE))
                {
                    (void)payload_disable(sat_data_buf.state.active_payload[0]);
                    sat_data_buf.state.active_payload[0] = PAYLOAD_NONE;
                    sat_data_buf.state.edc_active = false;
                }

                #if defined (CONFIG_DEV_PAYLOAD_X_ENABLED) && defined (CONFIG_TASK_PAYLOAD_X_ENABLED) && (CONFIG_DEV_PAYLOAD_X_ENABLED == 1) && (CONFIG_TASK_PAYLOAD_X_ENABLED == 1)
                    /* Activate PX */
                    if (sat_data_buf.state.active_payload[1] == PAYLOAD_NONE)
                    {
                        const uint32_t px_active_time_ms = (uint32_t)PAYLOAD_X_EXPERIMENT_PERIOD_MS;
                        sat_data_buf.state.active_payload[1] = PAYLOAD_X;

                        (void)payload_enable(PAYLOAD_X);

                        /* Send notifcation to read_px task to initialize experiment */
                        xTaskNotify(xTaskReadPXHandle, px_active_time_ms, eSetValueWithOverwrite);

                        if (!in_hibernation)
                        {
                            satellite_change_mode(OBDH_MODE_NORMAL);
                        }
                    }
                #else
                    if (!in_hibernation)
                    {
                        satellite_change_mode(OBDH_MODE_STAND_BY);
                    }
                #endif
            }

            break;
        }
        #if defined (CONFIG_DEV_PAYLOAD_X_ENABLED) && defined (CONFIG_TASK_PAYLOAD_X_ENABLED) && (CONFIG_DEV_PAYLOAD_X_ENABLED == 1) && (CONFIG_TASK_PAYLOAD_X_ENABLED == 1)
            case EV_NOTIFY_PX_FINISHED:
            {
                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_MISSION_MANAGER_NAME, "PX Exp finished");
                sys_log_new_line();

                if (!sat_data_buf.obdh.data.manual_mode_on)
                {
                    /* Activate PX */
                    if (sat_data_buf.state.active_payload[1] == PAYLOAD_X)
                    {
                        sat_data_buf.state.active_payload[1] = PAYLOAD_NONE;

                        (void)payload_disable(PAYLOAD_X);

                        if (!in_hibernation)
                        {
                            satellite_change_mode(OBDH_MODE_STAND_BY);
                        }
                    }
                }

                break;
            }
        #endif
        case EV_NOTIFY_MODE_CHANGE_RQ:
        {
            handle_mode_change_rq(ev->args);
            break;
        }
        case EV_NOTIFY_ACTIVATE_PAYLOAD_RQ:
        {
            activate_payload_rq(ev->args);
            break;
        }
        case EV_NOTIFY_DEACTIVATE_PAYLOAD_RQ:
        {
            deactivate_payload_rq(ev->args);
            break;
        }
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_MISSION_MANAGER_NAME, "Invalid event notifcation was made!");
            sys_log_new_line();
    }
}

static inline int8_t handle_mode_change_rq(const uint8_t *args)
{
    int8_t err = 0;

    switch (args[0]) 
    {
        case OBDH_MODE_NORMAL:
        {
            if (in_hibernation)
            {
                if (enable_ttc_tx() == 0)
                {
                    in_hibernation = false;
                }
                else
                {
                    err = -1;
                }
            }

            if ((sat_data_buf.state.active_payload[0] == PAYLOAD_NONE) && (err == 0))
            {
                payload_t main;

                switch (sat_data_buf.obdh.data.main_edc) 
                {
                    case CONFIG_PL_ID_EDC_1:
                        main = PAYLOAD_EDC_0;
                        break;
                    case CONFIG_PL_ID_EDC_2:
                        main = PAYLOAD_EDC_1;
                        break;
                    default:
                        main = PAYLOAD_NONE;
                        break;
                }

                if (main != PAYLOAD_NONE)
                {
                    sat_data_buf.state.active_payload[0] = main;
                    sat_data_buf.state.c_edc = (main == PAYLOAD_EDC_0) ? &sat_data_buf.edc_0 : &sat_data_buf.edc_1;
                    sat_data_buf.state.edc_active = true;

                    (void)payload_enable(main);
                }
                else 
                {
                    err = -1;
                }
            }

            if (err == 0)
            {
                satellite_change_mode(OBDH_MODE_NORMAL);
            }

            break;
        }
        case OBDH_MODE_HIBERNATION:
        {
            taskENTER_CRITICAL();

            in_hibernation = true;
            sat_data_buf.obdh.data.mode_duration = (((sys_time_t)args[1] << 8) | (sys_time_t)args[2]) * 60UL * 60UL;

            taskEXIT_CRITICAL();

            satellite_change_mode(OBDH_MODE_HIBERNATION);

            (void)disable_ttc_tx();
            break;
        }
        case OBDH_MODE_STAND_BY:
        {
            if (sat_data_buf.state.active_payload[0] != PAYLOAD_NONE)
            {
                (void)payload_disable(sat_data_buf.state.active_payload[0]);
                sat_data_buf.state.active_payload[0] = PAYLOAD_NONE;
                sat_data_buf.state.edc_active = false;
            }

            if (sat_data_buf.state.active_payload[1] != PAYLOAD_NONE)
            {
                (void)payload_disable(sat_data_buf.state.active_payload[1]);
                sat_data_buf.state.active_payload[1] = PAYLOAD_NONE;
            }
            
            satellite_change_mode(OBDH_MODE_STAND_BY);

            break;
        }
        case OBDH_WAKE_UP:
        {
            in_hibernation = false;
            satellite_change_mode(OBDH_MODE_HIBERNATION);

            if (in_brazil || (sat_data_buf.state.active_payload[1] != PAYLOAD_NONE))
            {
                satellite_change_mode(OBDH_MODE_NORMAL);
            }
            else 
            {
                satellite_change_mode(OBDH_MODE_STAND_BY);
            }

            (void)enable_ttc_tx();
            break;
        }
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_MISSION_MANAGER_NAME, "Invalid Mode received");
            sys_log_new_line();
            err = -1;
            break;
    }

    return err;
}

static inline int8_t activate_payload_rq(const uint8_t *args)
{
    int8_t err = 0;
    uint8_t pl = args[0];

    switch (pl) 
    {
        case CONFIG_PL_ID_EDC_1: 
        {
            if (sat_data_buf.state.active_payload[0] == PAYLOAD_EDC_1)
            {
                (void)payload_disable(sat_data_buf.state.active_payload[0]);
            }

            /* Update satellite state */
            sat_data_buf.state.active_payload[0] = PAYLOAD_EDC_0;
            sat_data_buf.state.c_edc = &sat_data_buf.edc_0;
            sat_data_buf.state.edc_active = true;

            (void)payload_enable(PAYLOAD_EDC_0);

            if (!in_hibernation)
            {
                satellite_change_mode(OBDH_MODE_NORMAL);
            }

            break;
        }
        case CONFIG_PL_ID_EDC_2: 
        {
            if (sat_data_buf.state.active_payload[0] == PAYLOAD_EDC_0)
            {
                (void)payload_disable(sat_data_buf.state.active_payload[0]);
            }

            /* Update satellite state */
            sat_data_buf.state.active_payload[0] = PAYLOAD_EDC_1;
            sat_data_buf.state.c_edc = &sat_data_buf.edc_1;
            sat_data_buf.state.edc_active = true;

            (void)payload_enable(PAYLOAD_EDC_1);

            if (!in_hibernation)
            {
                satellite_change_mode(OBDH_MODE_NORMAL);
            }

            break;
        }
        #if (CONFIG_DEV_PAYLOAD_X_ENABLED == 1) && (CONFIG_TASK_PAYLOAD_X_ENABLED == 1)
            case CONFIG_PL_ID_PAYLOAD_X: 
            {
                if (sat_data_buf.state.active_payload[1] == PAYLOAD_NONE)
                {
                    const uint32_t px_active_time_ms = (uint32_t)PAYLOAD_X_EXPERIMENT_PERIOD_MS;
                    sat_data_buf.state.active_payload[1] = PAYLOAD_X;

                    (void)payload_enable(PAYLOAD_X);

                    /* Send notifcation to read_px task to initialize experiment */
                    xTaskNotify(xTaskReadPXHandle, px_active_time_ms, eSetValueWithOverwrite);

                    if (!in_hibernation)
                    {
                        satellite_change_mode(OBDH_MODE_NORMAL);
                    }
                }
                break;
            }
        #endif
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_MISSION_MANAGER_NAME, "Invalid payload received");
            sys_log_new_line();
            err = -1;
            break;

    }

    return err;
}

static inline int8_t deactivate_payload_rq(const uint8_t *args)
{
    int8_t err = 0;
    uint8_t pl = args[0];

    switch (pl) 
    {
        case CONFIG_PL_ID_EDC_1: 
        {
            if (sat_data_buf.state.active_payload[0] == PAYLOAD_EDC_0)
            {
                sat_data_buf.state.active_payload[0] = PAYLOAD_NONE; 
                sat_data_buf.state.edc_active = false;

                (void)payload_disable(PAYLOAD_EDC_0);

                if ((sat_data_buf.obdh.data.mode == OBDH_MODE_NORMAL) && (sat_data_buf.state.active_payload[1] == PAYLOAD_NONE))
                {
                    satellite_change_mode(OBDH_MODE_STAND_BY);
                }
            }

            break;
        }
        case CONFIG_PL_ID_EDC_2: 
        {
            if (sat_data_buf.state.active_payload[0] == PAYLOAD_EDC_1)
            {
                sat_data_buf.state.active_payload[0] = PAYLOAD_NONE; 
                sat_data_buf.state.edc_active = false;

                (void)payload_disable(PAYLOAD_EDC_1);

                if ((sat_data_buf.obdh.data.mode == OBDH_MODE_NORMAL) && (sat_data_buf.state.active_payload[1] == PAYLOAD_NONE))
                {
                    satellite_change_mode(OBDH_MODE_STAND_BY);
                }
            }

            break;
        }
        #if (CONFIG_DEV_PAYLOAD_X_ENABLED == 1) && (CONFIG_TASK_PAYLOAD_X_ENABLED == 1)
            case CONFIG_PL_ID_PAYLOAD_X: 
            {
                if (sat_data_buf.state.active_payload[1] == PAYLOAD_X)
                {
                    sat_data_buf.state.active_payload[1] = PAYLOAD_NONE; 

                    /* Sends notification to read_px task to stop experiment */
                    xTaskNotify(xTaskReadPXHandle, PAYLOAD_X_CANCEL_EXPERIMENT_FLAG, eSetValueWithOverwrite);

                    (void)payload_disable(PAYLOAD_X);

                    if ((sat_data_buf.obdh.data.mode == OBDH_MODE_NORMAL) && (sat_data_buf.state.active_payload[0] == PAYLOAD_NONE))
                    {
                        satellite_change_mode(OBDH_MODE_STAND_BY);
                    }
                }

                break;
            }
        #endif
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_MISSION_MANAGER_NAME, "Invalid payload received");
            sys_log_new_line();
            err = -1;
            break;

    }

    return err;
}

static int enable_ttc_tx(void)
{
    int err = 0;

    if (ttc_set_param(TTC_0, SL_TTC2_REG_TX_ENABLE, 1U) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_MISSION_MANAGER_NAME, "Failed to enable TTC 0 TX");
        sys_log_new_line();
        err--;
    }

    if (ttc_set_param(TTC_1, SL_TTC2_REG_TX_ENABLE, 1U) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_MISSION_MANAGER_NAME, "Failed to enable TTC 1 TX");
        sys_log_new_line();
        err--;
    }

    return err;
}

static int disable_ttc_tx(void)
{
    int err = 0;

    if (ttc_set_param(TTC_0, SL_TTC2_REG_TX_ENABLE, 0U) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_MISSION_MANAGER_NAME, "Failed to disable TTC 0 TX");
        sys_log_new_line();
        err--;
    }

    if (ttc_set_param(TTC_1, SL_TTC2_REG_TX_ENABLE, 0U) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_MISSION_MANAGER_NAME, "Failed to disable TTC 1 TX");
        sys_log_new_line();
        err--;
    }

    return err;
}

/** \} End of mission_man group */
