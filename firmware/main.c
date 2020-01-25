/*
 * main.c
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
 * \brief Main file.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.0
 * 
 * \date 25/10/2019
 * 
 * \defgroup main Main file
 * \{
 */

#include <FreeRTOS.h>
#include <task.h>

#include "system/clocks.h"
#include "app/tasks/tasks.h"

void main(void)
{
    // Watchdog device initialization
    watchdog_init();

    // System clocks configuration
    clocks_setup((clocks_config_t){.mclk_hz = 32000000UL, .smclk_hz=32000000UL, .aclk_hz=32768});

    // Create all the tasks
    create_tasks();

	// Start the scheduler
    vTaskStartScheduler();

	// Will only get here if there was insufficient memory to create the idle and/or timer task
    while(1);
}

//! \} End of main group
