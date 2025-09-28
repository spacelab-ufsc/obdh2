/*
 * read_lpl.c
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
 * \brief Read LPL data task implementation.
 * 
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 1.0.0
 * 
 * \date 2025/09/28
 * 
 * \addtogroup read_lpl
 * \{
 */

#include <system/sys_log/sys_log.h>
#include <devices/lpl/lpl.h>

#include <structs/satellite.h>

#include "read_lpl.h"
#include "startup.h"

xTaskHandle xTaskReadLPLHandle;

void vTaskReadLPL(void *p)
{
    (void)p;

    lpl_t dev = {0};
    uint8_t raw[256];
    int retval = 0;

    /* Wait startup task to finish */
    (void)xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_READ_LPL_INIT_TIMEOUT_MS));

    /* Delay before the first cycle */
    vTaskDelay(pdMS_TO_TICKS(TASK_READ_LPL_INITIAL_DELAY_MS));

    if (lpl_init(&dev) == 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_READ_LPL_NAME, "LPL device initialized sucessfully!");
        sys_log_new_line();
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_READ_LPL_NAME, "Failed to initialize LPL uart interface!");
        sys_log_new_line();
    }

    TickType_t last_cycle = xTaskGetTickCount();

    while(1)
    {
        retval = lpl_available(&dev);

        if (retval > 0)
        {
            if (lpl_read(&dev, raw, (uint16_t)retval) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_READ_LPL_NAME, "Failed to read data from LPL!");
                sys_log_new_line();
            }

            /* Assembly Packet and store on FRAM */
        }

        vTaskDelayUntil(&last_cycle, pdMS_TO_TICKS(TASK_READ_LPL_PERIOD_MS));
    }
}

/** \} End of read_ttc group */
