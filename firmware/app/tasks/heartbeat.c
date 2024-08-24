/*
 * heartbeat.c
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
 * \brief Heartbeat task implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.47
 * 
 * \date 2020/01/20
 * 
 * \addtogroup heartbeat
 * \{
 */

#include <devices/leds/leds.h>

#include "heartbeat.h"
#include "startup.h"

xTaskHandle xTaskHeartbeatHandle;

void vTaskHeartbeat(void)
{
    /* Wait startup task to finish */
    xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_HEARTBEAT_INIT_TIMEOUT_MS));

    TickType_t last_cycle = xTaskGetTickCount();

    while(1)
    {
        led_toggle(LED_SYSTEM);

        vTaskDelayUntil(&last_cycle, pdMS_TO_TICKS(TASK_HEARTBEAT_PERIOD_MS));
    }
}

/** \} End of heartbeat group */
