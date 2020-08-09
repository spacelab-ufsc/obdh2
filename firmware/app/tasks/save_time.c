/*
 * save_time.h
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
 * \brief Save system time task implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.4.13
 * 
 * \date 17/03/2020
 * 
 * \addtogroup save_time
 * \{
 */

#include <system/sys_log/sys_log.h>
#include <devices/media/media.h>
#include <config/config.h>

#include "save_time.h"
#include "startup.h"

xTaskHandle xTaskSaveTimeHandle;

void vTaskSaveTime(void *pvParameters)
{
    /* Wait startup task to finish */
    xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_SAVE_TIME_INIT_TIMEOUT_MS));

    while(1)
    {
        TickType_t last_cycle = xTaskGetTickCount();

        /* Read the current system time */
        uint32_t sys_tick = xTaskGetTickCount();

        /* Save the current system time */
        if (media_write(MEDIA_INT_FLASH, CONFIG_MEM_ADR_SYS_TIME, &sys_tick, 1))
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_SAVE_TIME_NAME, "Error writing the system time to the internal flash memory!");
            sys_log_new_line();
        }

        /* Check the non-volatile memory */
        uint32_t mem_sys_tick = 0xFF;
        if (media_read(MEDIA_INT_FLASH, CONFIG_MEM_ADR_SYS_TIME, &mem_sys_tick, 1))
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_SAVE_TIME_NAME, "Error writing the system time to the internal flash memory!");
            sys_log_new_line();
        }

        vTaskDelayUntil(&last_cycle, pdMS_TO_TICKS(TASK_SAVE_TIME_PERIOD_MS));
    }
}

/** \} End of save_time group */
