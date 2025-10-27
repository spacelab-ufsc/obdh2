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

static void lpl_duty_cycle_control(lpl_t *dev, uint16_t *period)
{
    uint16_t c = *period;

    if (c < 5U)
    {
        (void)lpl_enable(dev);
        (*period)++; 
    }
    else if ((c >= 5U) && (c < 10U))
    {
        (void)lpl_disable(dev);
        (*period)++; 
    }
    else
    {
        (void)lpl_enable(dev);
        *period = 0U;
    }
}

void vTaskReadLPL(void *p)
{
    (void)p;

    lpl_t dev = {0};
    int retval = 0;
    uint16_t period = 0U;

    /* Wait startup task to finish */
    (void)xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_READ_LPL_INIT_TIMEOUT_MS));

    /* Delay before the first cycle */
    vTaskDelay(pdMS_TO_TICKS(TASK_READ_LPL_INITIAL_DELAY_MS));

    if (lpl_init(&dev) == 0)
    {
        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_READ_LPL_NAME, "LPL device initialized sucessfully!");
        sys_log_new_line();

        lpl_register_handle(&dev);
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_READ_LPL_NAME, "Failed to initialize LPL uart interface!");
        sys_log_new_line();
    }

    if (lpl_load_from_fram(&dev) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_READ_LPL_NAME, "Failed to read last packet from FRAM!");
        sys_log_new_line();
    }

    TickType_t last_cycle = xTaskGetTickCount();

    while(1)
    {
        retval = lpl_available(&dev);

        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_READ_LPL_NAME, "LPL have ");
        sys_log_print_int((int32_t)retval);
        sys_log_print_msg(" bytes available to read!");
        sys_log_new_line();

        if (retval > 0)
        {
            if (lpl_recv(&dev, 500U) > 0)
            {
                if (lpl_store_to_fram(&dev) != 0)
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_READ_LPL_NAME, "Failed to store LPL packet to FRAM!");
                    sys_log_new_line();
                }

                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_READ_LPL_NAME, "Received packet: ");
                sys_log_dump_hex(dev.packet, sizeof(dev.packet));
                sys_log_new_line();
            }
            else
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_READ_LPL_NAME, "Failed to read data from LPL!");
                sys_log_new_line();
            }
        }

        lpl_duty_cycle_control(&dev, &period);

        vTaskDelayUntil(&last_cycle, pdMS_TO_TICKS(TASK_READ_LPL_PERIOD_MS));
    }
}

/** \} End of read_ttc group */
