/*
 * read_eps.c
 * 
 * Copyright (C) 2021, SpaceLab.
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
 * \brief Read EPS data task implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.8.5
 * 
 * \date 2021/05/08
 * 
 * \addtogroup read_eps
 * \{
 */

#include <stdint.h>
#include <system/sys_log/sys_log.h>
#include <devices/eps/eps.h>

#include <structs/satellite.h>

#include "read_eps.h"
#include "startup.h"

#define READ_EPS_MAX_RETRIES        (3U)
#define READ_EPS_RETRY_DELAY_MS     (500U)

xTaskHandle xTaskReadEPSHandle;

void vTaskReadEPS(void)
{
    /* Wait startup task to finish */
    xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_READ_EPS_INIT_TIMEOUT_MS));

    TickType_t last_cycle = xTaskGetTickCount();

    while(1)
    {
        int8_t err = 0;
        uint8_t retry_count = READ_EPS_MAX_RETRIES;

        do {
            err = eps_init();

            if (err != 0)
            {
                retry_count--;
                vTaskDelay(pdMS_TO_TICKS(READ_EPS_RETRY_DELAY_MS));
            }
        } while ((err != 0) && (retry_count > 0U));

        if (retry_count == 0U)
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_READ_EPS_NAME, "Max retries reached trying to initialize EPS!!!");
            sys_log_new_line();
        }

        err = 0;
        retry_count = READ_EPS_MAX_RETRIES;

        do {
            err = eps_get_data(&sat_data_buf.eps.data);

            if (err == 0)
            {
                sat_data_buf.eps.timestamp = system_get_time();
            }
            else 
            {
                retry_count--;
                vTaskDelay(pdMS_TO_TICKS(READ_EPS_RETRY_DELAY_MS));
            }
        } while ((err != 0) && (retry_count > 0U));

        if (retry_count == 0U)
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_READ_EPS_NAME, "Max retries reached trying to read data from EPS!!!");
            sys_log_new_line();
        }

        vTaskDelayUntil(&last_cycle, pdMS_TO_TICKS(TASK_READ_EPS_PERIOD_MS));
    }
}

/** \} End of read_eps group */
