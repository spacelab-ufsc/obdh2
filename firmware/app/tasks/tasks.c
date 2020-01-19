/*
 * tasks.c
 * 
 * Copyright (C) 2019, SpaceLab.
 * 
 * This file is part of OBDH 2.0.
 * 
 * OBDH 2.0 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * OBDH 2.0 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with OBDH 2.0. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Tasks implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.0
 * 
 * \date 02/11/2019
 * 
 * \addtogroup tasks
 * \{
 */

#include <FreeRTOS.h>
#include <task.h>

#include "tasks.h"
#include "startup.h"

void create_tasks()
{
    // Startup task
    xTaskCreate(vTaskStartup, TASK_STARTUP_NAME, TASK_STARTUP_STACK_SIZE, NULL, TASK_STARTUP_PRIORITY, &xTaskStartupHandle);

    if (xTaskStartupHandle == NULL)
    {
        // Error creating the startup task
    }

    // Watchdog reset task
    xTaskCreate(vTaskWatchdogReset, TASK_WATCHDOG_RESET_NAME, TASK_WATCHDOG_RESET_STACK_SIZE, NULL, TASK_WATCHDOG_RESET_PRIORITY, &xTaskWatchdogResetHandle);

    if (vTaskWatchdogReset == NULL)
    {
        // Error creating the watchdog reset task
    }
}

//! \} End of tasks group
