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
#include <system/sys_log/sys_log.h>
#include <FreeRTOS.h>
#include <task.h>

#include "read_px.h"
#include "read_edc.h"
#include "op_ctrl.h"

static inline void handle_notification(uint32_t notify_value);

static int enable_main_edc(void);
static int enable_px(void);
static int disable_curr_payload(void);

static bool in_brazil = false;
static bool edc_active = false;
static bool in_hibernation = false;

TaskHandle_t xTaskOpCtrlHandle;

void vTaskOpCtrl(void)
{
    uint32_t notify_value;
    sat_data_buf.state.c_edc = &sat_data_buf.edc_0;

    TickType_t last_cycle = xTaskGetTickCount();

    while (1) 
    {
        if (xTaskNotifyWait(0UL, UINT32_MAX, &notify_value, 0UL) == pdTRUE) 
        {
            handle_notification(notify_value);
        }

        /* Hibernation mode check */
        if (sat_data_buf.obdh.data.mode == OBDH_MODE_HIBERNATION)
        {
            if ((sat_data_buf.obdh.data.ts_last_mode_change + sat_data_buf.obdh.data.mode_duration) <= system_get_time())
            {
                if (in_brazil || (sat_data_buf.state.active_payload == PAYLOAD_X))
                {
                    satellite_change_mode(OBDH_MODE_NORMAL);
                }
                else 
                {
                    satellite_change_mode(OBDH_MODE_STAND_BY);
                }

                in_hibernation = false;
            }
        }

        /* Nominal mode notification */
        if (in_brazil && edc_active)
        {
            /* Notifies to Read EDC task that the EDC is active.
             * Must happen because the task depends on the notification
             * to be pending for it to read EDC data. */
            xTaskNotifyGive(xTaskReadEDCHandle);
        }

        vTaskDelayUntil(&last_cycle, pdMS_TO_TICKS(TASK_OP_CTRL_PERIOD_MS));
    }
}

void notify_op_ctrl(uint32_t notification_flag)
{
    xTaskNotify(xTaskOpCtrlHandle, notification_flag, eSetBits);
}

void satellite_change_mode(uint8_t mode)
{
    /* This ensures the mode change is done atomically */
    taskENTER_CRITICAL();
    sat_data_buf.obdh.data.mode = mode;
    sat_data_buf.obdh.data.ts_last_mode_change = system_get_time();
    taskEXIT_CRITICAL();
}

static inline void handle_notification(uint32_t notify_value)
{
    uint32_t px_active_time_ms = (uint32_t)PAYLOAD_X_EXPERIMENT_PERIOD_MS;

    if ((notify_value & SAT_NOTIFY_ENTER_HIBERNATION) != 0U)
    {
        in_hibernation = true;
        satellite_change_mode(OBDH_MODE_HIBERNATION);
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
    }

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
                edc_active = true;
            }
        }
    }

    if ((notify_value & SAT_NOTIFY_OUT_OF_BRAZIL) != 0U)
    {
        edc_active = false;

        /* Stop the Read EDC task */
        xTaskNotify(xTaskReadEDCHandle, 0UL, eSetValueWithOverwrite);

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

static int disable_curr_payload(void)
{
    int err = -1;

    payload_t active_payload = sat_data_buf.state.active_payload;

    if (payload_disable(active_payload) == 0)
    {
        err = 0;
    }

    return err;
}

/** \} End of op_ctrl group */
