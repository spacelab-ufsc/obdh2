/*
 * read_ttc.c
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
 * \brief Read TTC data task implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 0.10.14
 * 
 * \date 2021/05/14
 * 
 * \addtogroup read_ttc
 * \{
 */

#include <system/sys_log/sys_log.h>
#include <devices/ttc/ttc.h>

#include <structs/satellite.h>

#include "read_ttc.h"
#include "startup.h"

xTaskHandle xTaskReadTTCHandle;

void vTaskReadTTC(void)
{
    /* Wait startup task to finish */
    xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_READ_TTC_INIT_TIMEOUT_MS));

    /* Delay before the first cycle */
    vTaskDelay(pdMS_TO_TICKS(TASK_READ_TTC_INITIAL_DELAY_MS));

    while(1)
    {
        TickType_t last_cycle = xTaskGetTickCount();

        if (ttc_init(TTC_0) != 0)
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_READ_TTC_NAME, "Error initializing the TTC device!");
            sys_log_new_line();
        }

        if (ttc_init(TTC_1) != 0)
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_READ_TTC_NAME, "Error initializing the TTC device!");
            sys_log_new_line();
        }

        if (ttc_get_data(TTC_0, &sat_data_buf.ttc_0.data) == 0)
        {
            sat_data_buf.ttc_0.timestamp = system_get_time();
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_READ_TTC_NAME, "Error reading data from the TTC 0 device!");
            sys_log_new_line();
        }

        if (ttc_get_data(TTC_1, &sat_data_buf.ttc_1.data) == 0)
        {
            sat_data_buf.ttc_1.timestamp = system_get_time();
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_READ_TTC_NAME, "Error reading data from the TTC 1 device!");
            sys_log_new_line();
        }

        vTaskDelayUntil(&last_cycle, pdMS_TO_TICKS(TASK_READ_TTC_PERIOD_MS));
    }
}

/** \} End of read_ttc group */
