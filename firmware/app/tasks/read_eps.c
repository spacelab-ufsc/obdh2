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
 * \version 0.7.47
 * 
 * \date 2021/05/08
 * 
 * \addtogroup read_eps
 * \{
 */

#include <system/sys_log/sys_log.h>
#include <devices/eps/eps.h>

#include <structs/satellite.h>

#include "read_eps.h"
#include "startup.h"

xTaskHandle xTaskReadEPSHandle;

void vTaskReadEPS(void)
{
    /* Wait startup task to finish */
    xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_READ_EPS_INIT_TIMEOUT_MS));

    while(1)
    {
        TickType_t last_cycle = xTaskGetTickCount();

        if (eps_init() != 0)
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_READ_EPS_NAME, "Error initializing the EPS device!");
            sys_log_new_line();
        }

        if (eps_get_data(&sat_data_buf.eps) != 0)
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_READ_EPS_NAME, "Error reading data from the EPS device!");
            sys_log_new_line();
        }

        vTaskDelayUntil(&last_cycle, pdMS_TO_TICKS(TASK_READ_EPS_PERIOD_MS));
    }
}

/** \} End of read_eps group */
