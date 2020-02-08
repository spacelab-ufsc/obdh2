/*
 * startup.c
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
 * \brief Startup task implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.20
 * 
 * \date 04/12/2019
 * 
 * \addtogroup startup
 * \{
 */

#include <devices/watchdog/watchdog.h>
#include <devices/logger/logger.h>
#include <devices/leds/leds.h>
#include <devices/eps/eps.h>
#include <system/clocks.h>

#include "startup.h"

xTaskHandle xTaskStartupHandle;

EventGroupHandle_t task_startup_status;

void vTaskStartup(void *pvParameters)
{
    // Logger device initialization
    logger_init();

    // Print the FreeRTOS version
    logger_print_event_from_module(LOGGER_INFO, TASK_STARTUP_NAME, "FreeRTOS ");
    logger_print_msg(tskKERNEL_VERSION_NUMBER);
    logger_new_line();

    // Print the system clocks
    clocks_config_t clks = clocks_read();
    logger_print_event_from_module(LOGGER_INFO, TASK_STARTUP_NAME, "System clocks: MCLK=");
    logger_print_dec(clks.mclk_hz);
    logger_print_msg(" Hz, SMCLK=");
    logger_print_dec(clks.smclk_hz);
    logger_print_msg(" Hz, ACLK=");
    logger_print_dec(clks.aclk_hz);
    logger_print_msg(" Hz");
    logger_new_line();

    // LEDs device initialization
    leds_init();

    // EPS device initialization
    eps_init();

    // Startup task status = Done
    xEventGroupSetBits(task_startup_status, TASK_STARTUP_DONE);

    logger_print_event_from_module(LOGGER_INFO, TASK_STARTUP_NAME, "Boot completed with SUCCESS!");
    logger_new_line();

    vTaskSuspend(xTaskStartupHandle);
}

//! \} End of startup group
