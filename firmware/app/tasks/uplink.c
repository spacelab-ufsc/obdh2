/*
 * uplink.c
 * 
 * Copyright (C) 2020, SpaceLab.
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
 * along with OBDH 2.0. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Uplink task implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.4.15
 * 
 * \date 18/04/2020
 * 
 * \addtogroup uplink
 * \{
 */

#include <system/sys_log/sys_log.h>
#include <devices/radio/radio.h>

#include "uplink.h"
#include "startup.h"

xTaskHandle xTaskUplinkHandle;

void vTaskUplink(void *pvParameters)
{
    /* Wait startup task to finish */
    xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_UPLINK_INIT_TIMEOUT_MS));

    /* Delay before the first cycle */
    vTaskDelay(pdMS_TO_TICKS(TASK_UPLINK_INITIAL_DELAY_MS));

    while(1)
    {
        uint8_t data[129];
        uint8_t len = 50;

        if (radio_recv(data, len, TASK_UPLINK_PERIOD_MS) > 0)
        {
            /* Process packets here */
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_UPLINK_NAME, "Error during data reception! Trying again in ");
            sys_log_print_uint(TASK_UPLINK_PERIOD_MS);
            sys_log_print_msg(" ms...");
            sys_log_new_line();

            /* Wait TASK_UPLINK_PERIOD_MS milliseconds to try again */
            vTaskDelay(pdMS_TO_TICKS(TASK_UPLINK_PERIOD_MS));
        }
    }
}

/** \} End of uplink group */
