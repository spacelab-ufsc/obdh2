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
 *
 * \version 0.0.1
 *
 * \date 2023/08/28
 *
 * \addtogroup read_px
 * \{
 */

#include <system/sys_log/sys_log.h>
#include <devices/payload/payload.h>
#include <drivers/px/px.h>

#include "read_px.h"
#include "startup.h"

xTaskHandle xTaskReadPXHandle;

pl_px_buf_t px_buf = {0};

void vTaskReadPX(void)
{
    static payload_t pl_px_active = PAYLOAD_X;
    px_buf.length = PX_PONG_BUF_SIZE;

    /* Wait startup task to finish */
    xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_READ_PX_INIT_TIMEOUT_MS));

    while(1)
    {
        TickType_t last_cycle = xTaskGetTickCount();

        /* Read data */
        if (payload_get_data(pl_px_active, PAYLOAD_X_PONG, px_buf.buffer, &px_buf.length) != 0)
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_READ_PX_NAME, "Error reading the ping-pong data!");

            uint8_t i = 0;
            for(i=0;i<px_buf.length;i++){
                sys_log_print_uint(px_buf.buffer[i]);
            }

            sys_log_new_line();
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_READ_PX_NAME, "Received Payload X packet:");
            sys_log_new_line();

            /*
            uint8_t i = 0;
            for(i=0;i<px_buf.length;i++){
                sys_log_print_uint(px_buf.buffer[i]);
            }
            */
        }

        vTaskDelayUntil(&last_cycle, pdMS_TO_TICKS(TASK_READ_PX_PERIOD_MS));
    }
}

/** \} End of read_px group */


