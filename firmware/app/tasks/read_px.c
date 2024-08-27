/*
 * read_px.c
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
 * \brief Read PX task implementation.
 *
 * \author Augusto Cezar Boldori Vassoler <augustovassoler@gmail.com>
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 0.10.19
 *
 * \date 2023/08/28
 *
 * \addtogroup read_px
 * \{
 */

#include <stdint.h>
#include <string.h>

#include <string.h>
#include <system/sys_log/sys_log.h>
#include <system/system.h>
#include <devices/payload/payload.h>
#include <drivers/px/px.h>
#include <structs/satellite.h>

#include "read_px.h"
#include "op_ctrl.h"
#include "startup.h"

xTaskHandle xTaskReadPXHandle;

void vTaskReadPX(void)
{
    payload_telemetry_t *const px = &sat_data_buf.payload_x;

    pl_px_buf_t px_buf = {0};
    px_buf.length = PX_PONG_BUF_SIZE;

    /* Wait startup task to finish */
    xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_READ_PX_INIT_TIMEOUT_MS));

    while(1)
    {
        uint32_t cancel_flag;
        uint32_t active_period_ms;

        BaseType_t result = xTaskNotifyWait(0UL, UINT32_MAX, &active_period_ms, pdMS_TO_TICKS(TASK_READ_PX_MAX_WAIT_TIME_MS));

        payload_t pl_px_active = sat_data_buf.state.active_payload;

        if ((pl_px_active == PAYLOAD_X) && (result == pdTRUE)) 
        {
            TickType_t last_cycle = xTaskGetTickCount();

            while (active_period_ms > 0U) 
            {
                /* Check notifications to break out of the loop if requested */
                if (xTaskNotifyWait(0UL, PAYLOAD_X_CANCEL_EXPERIMENT_FLAG, &cancel_flag, 0U) == pdTRUE)
                {
                    if ((cancel_flag & PAYLOAD_X_CANCEL_EXPERIMENT_FLAG) != 0U)
                    {
                        active_period_ms = 0U;
                        break;
                    }
                }

                /* Read data */
                if (payload_get_data(pl_px_active, PAYLOAD_X_PONG, px_buf.buffer, &px_buf.length) != 0)
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_READ_PX_NAME, "Error reading the ping-pong data!");

                    int32_t i = 0;
                    for(i = 0; i < px_buf.length; i++){
                        sys_log_print_uint(px_buf.buffer[i]);
                    }

                    sys_log_new_line();
                }
                else
                {
                    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_READ_PX_NAME, "Received Payload X packet:");
                    sys_log_new_line();

                    (void)memcpy(px->data, px_buf.buffer, PX_PONG_BUF_SIZE);

                    px->timestamp = system_get_time();

                    /*
                    uint8_t i = 0;
                    for(i=0;i<px_buf.length;i++){
                        sys_log_print_uint(px_buf.buffer[i]);
                    }
                    */
                }

                active_period_ms -= (uint32_t)TASK_READ_PX_PERIOD_MS;

                vTaskDelayUntil(&last_cycle, pdMS_TO_TICKS(TASK_READ_PX_PERIOD_MS));
            }

            notify_op_ctrl(SAT_NOTIFY_PX_FINISHED);
        }
    }
}

/** \} End of read_px group */


