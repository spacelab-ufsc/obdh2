/*
 * beacon.c
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
 * \brief Beacon task implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.3.1
 * 
 * \date 27/10/2019
 * 
 * \addtogroup beacon
 * \{
 */

#include "beacon.h"

xTaskHandle xTaskBeaconHandle;

void vTaskBeacon(void *pvParameters)
{
    /* Delay before the first cycle */
    vTaskDelay(pdMS_TO_TICKS(TASK_BEACON_INITIAL_DELAY_MS));

    while(1)
    {
        TickType_t last_cycle = xTaskGetTickCount();

        vTaskDelayUntil(&last_cycle, pdMS_TO_TICKS(TASK_BEACON_PERIOD_MS));
    }
}

/** \} End of beacon group */
