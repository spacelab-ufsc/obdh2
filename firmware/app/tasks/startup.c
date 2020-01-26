/*
 * startup.c
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
 * \brief Startup task implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.0
 * 
 * \date 04/12/2019
 * 
 * \addtogroup startup
 * \{
 */

#include <devices/watchdog/watchdog.h>
#include <devices/logger/logger.h>
#include <devices/leds/leds.h>

#include "startup.h"

xTaskHandle xTaskStartupHandle;

EventGroupHandle_t task_startup_status;

void vTaskStartup(void *pvParameters)
{
    // Logger device initialization
    logger_init();

    // LEDs device initialization
    leds_init();

    // Startup task status = Done
    xEventGroupSetBits(task_startup_status, TASK_STARTUP_DONE);

    vTaskSuspend(xTaskStartupHandle);
}

//! \} End of startup group
