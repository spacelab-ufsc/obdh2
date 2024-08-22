/*
 * op_ctrl.c
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
 * \brief Operation Control Task implementation
 * 
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 0.10.19
 * 
 * \date 2024/08/04
 * 
 * \addtogroup op_ctrl 
 * \{
 */

#include <stdint.h>
#include <stdbool.h>

#include <structs/satellite.h>
#include <structs/obdh_data.h>
#include <devices/payload/payload.h>
#include <devices/ttc/ttc.h>
#include <system/sys_log/sys_log.h>
#include <FreeRTOS.h>
#include <task.h>

#include "op_ctrl.h"
#include "read_px.h"
#include "system_reset.h"

static inline void handle_notification(uint32_t notify_value);

static int enable_main_edc(void);
static int enable_px(void);
static int disable_curr_payload(void);
static int enable_ttc_tx(void);
static int disable_ttc_tx(void);

static bool in_hibernation = false;

TaskHandle_t xTaskOpCtrlHandle;

void vTaskOpCtrl(void)
{
    uint32_t notify_value;
    sat_data_buf.state.c_edc = &sat_data_buf.edc_0;

    while (1) 
    {
        if (xTaskNotifyWait(0UL, UINT32_MAX, &notify_value, pdMS_TO_TICKS_64(TASK_OP_CTRL_NOTIFICATION_TIMEOUT)) == pdTRUE) 
        {
            handle_notification(notify_value);
        }
        else 
        {
            sys_log_print_event_from_module(SYS_LOG_WARNING, TASK_OP_CTRL_NAME, "Notification waiting timedout!");
            sys_log_new_line();
        }
    }
}

void notify_op_ctrl(uint32_t notification_flag)
{
    xTaskNotify(xTaskOpCtrlHandle, notification_flag, eSetBits);
}

void satellite_change_mode(const uint8_t mode)
{
    /* This ensures the mode change is done atomically */
    taskENTER_CRITICAL();
    sat_data_buf.obdh.data.mode = mode;
    sat_data_buf.obdh.data.ts_last_mode_change = system_get_time();
    taskEXIT_CRITICAL();
}

int8_t override_op_mode(const uint8_t mode)
{
    int8_t err = 0;

    switch (mode) 
    {
        case OBDH_MODE_NORMAL:
        {
            sat_data_buf.state.edc_active = true;
            satellite_change_mode(mode);

            if (enable_main_edc() != 0)
            {
                err = -1;
            }

            break;
        }
        case OBDH_MODE_HIBERNATION:
        {
            in_hibernation = true;
            satellite_change_mode(mode);

            if (disable_ttc_tx() != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_OP_CTRL_NAME, "Failed to disable TTC TX!!");
                sys_log_new_line();
            }

            break;
        }
        case OBDH_MODE_STAND_BY:
        {
            sat_data_buf.state.edc_active = false;
            satellite_change_mode(mode);

            if (disable_curr_payload() != 0)
            {
                err = -1;
            }

            break;
        }
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_OP_CTRL_NAME, "Invalid Mode was given!");
            sys_log_new_line();
            err = -1;
            break;
    }

    return err;
}

static inline void handle_notification(uint32_t notify_value)
{
    static bool in_brazil = false;
    uint32_t px_active_time_ms = (uint32_t)PAYLOAD_X_EXPERIMENT_PERIOD_MS;

    if ((notify_value & SAT_NOTIFY_ENTER_HIBERNATION) != 0U)
    {
        in_hibernation = true;
        satellite_change_mode(OBDH_MODE_HIBERNATION);

        if (disable_ttc_tx() != 0)
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_OP_CTRL_NAME, "Failed to disable TTC TX!!");
            sys_log_new_line();
        }
    }

    if ((notify_value & SAT_NOTIFY_LEAVE_HIBERNATION) != 0U)
    {
        in_hibernation = false;

        if (in_brazil || (sat_data_buf.state.active_payload == PAYLOAD_X))
        {
            satellite_change_mode(OBDH_MODE_NORMAL);
        }
        else 
        {
            satellite_change_mode(OBDH_MODE_STAND_BY);
        }

        if (enable_ttc_tx() != 0)
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_OP_CTRL_NAME, "Failed to enable TTC TX!!");
            sys_log_new_line();
        }
    }

    if ((notify_value & SAT_NOTIFY_ENTER_MANUAL_MODE) != 0U)
    {
        sat_data_buf.obdh.data.manual_mode_on = true;
    }

    if ((notify_value & SAT_NOTIFY_LEAVE_MANUAL_MODE) != 0U)
    {
        sat_data_buf.obdh.data.manual_mode_on = false;
    }

    /* Automatic operation mode switch should only happen when manual mode is disable */
    if (!sat_data_buf.obdh.data.manual_mode_on)
    {
        if ((notify_value & SAT_NOTIFY_IN_BRAZIL) != 0U)
        {
            sys_log_print_event_from_module(SYS_LOG_WARNING, TASK_OP_CTRL_NAME, "Changing Satellite Mode to NOMINAL!");
            sys_log_new_line();

            if (!in_hibernation)
            {
                satellite_change_mode(OBDH_MODE_NORMAL);
            }

            /* It means the satellite just entered Brazilian territory*/
            if (!in_brazil) 
            {
                in_brazil = true;

                if (enable_main_edc() != 0)
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_OP_CTRL_NAME, "Failed to enable main EDC");
                    sys_log_new_line();
                }
                else 
                {
                    sat_data_buf.state.edc_active = true;
                }
            }
        }

        if ((notify_value & SAT_NOTIFY_OUT_OF_BRAZIL) != 0U)
        {
            sat_data_buf.state.edc_active = false;

            if (disable_curr_payload() != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_OP_CTRL_NAME, "Failed to disable active payload");
                sys_log_new_line();
            }

            if (in_brazil)
            {
                in_brazil = false;

                sys_log_print_event_from_module(SYS_LOG_WARNING, TASK_OP_CTRL_NAME, "Satellite is out of Brazil territory");
                sys_log_new_line();

                if (enable_px() != 0)
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_OP_CTRL_NAME, "Failed to enable Payload X");
                    sys_log_new_line();
                }
                else 
                {
                    xTaskNotify(xTaskReadPXHandle, px_active_time_ms, eSetValueWithOverwrite);
                }
            }
        }

        if ((notify_value & SAT_NOTIFY_PX_FINISHED) != 0U)
        {
            sys_log_print_event_from_module(SYS_LOG_WARNING, TASK_OP_CTRL_NAME, "Changing Satellite Mode to STAND BY!");
            sys_log_new_line();

            if (!in_hibernation)
            {
                satellite_change_mode(OBDH_MODE_STAND_BY);
            }

            if (disable_curr_payload() != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_OP_CTRL_NAME, "Failed to disable active payload");
                sys_log_new_line();
            }

            sat_data_buf.state.active_payload = PAYLOAD_NONE;
        }
    }
}

static int enable_main_edc(void)
{
    int err = -1;

    sat_data_buf.state.active_payload = sat_data_buf.state.main_edc;

    if (sat_data_buf.state.main_edc == PAYLOAD_EDC_0) 
    {
        payload_enable(PAYLOAD_EDC_0);
        sat_data_buf.state.c_edc = &sat_data_buf.edc_0;
        err = 0;
    }
    else 
    {
        payload_enable(PAYLOAD_EDC_1);
        sat_data_buf.state.c_edc = &sat_data_buf.edc_1;
        err = 0;
    }

    return err;
}

static int enable_px(void)
{
    int err = -1;

    sat_data_buf.state.active_payload = PAYLOAD_X;

    if (payload_enable(PAYLOAD_X) == 0)
    {
        err = 0;
    }

    return err;
}

static int enable_ttc_tx(void)
{
    int err = 0;

    if (ttc_set_param(TTC_0, SL_TTC2_REG_TX_ENABLE, 1U) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_OP_CTRL_NAME, "Failed to enable TTC 0 TX");
        sys_log_new_line();
        err--;
    }

    if (ttc_set_param(TTC_1, SL_TTC2_REG_TX_ENABLE, 1U) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_OP_CTRL_NAME, "Failed to enable TTC 1 TX");
        sys_log_new_line();
        err--;
    }

    return err;
}

static int disable_curr_payload(void)
{
    int err = -1;

    payload_t active_payload = sat_data_buf.state.active_payload;

    if (payload_disable(active_payload) == 0)
    {
        sat_data_buf.state.active_payload = PAYLOAD_NONE;
        err = 0;
    }

    return err;
}

static int disable_ttc_tx(void)
{
    int err = 0;

    if (ttc_set_param(TTC_0, SL_TTC2_REG_TX_ENABLE, 0U) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_OP_CTRL_NAME, "Failed to disable TTC 0 TX");
        sys_log_new_line();
        err--;
    }

    if (ttc_set_param(TTC_1, SL_TTC2_REG_TX_ENABLE, 0U) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_OP_CTRL_NAME, "Failed to disable TTC 1 TX");
        sys_log_new_line();
        err--;
    }

    return err;
}

/** \} End of op_ctrl group */
