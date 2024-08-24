/*
 * watchdog_reset.h
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
 * \brief Watchdog reset task implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.47
 * 
 * \date 2020/01/11
 * 
 * \addtogroup watchdog_reset
 * \{
 */

#include <devices/watchdog/watchdog.h>

#include "watchdog_reset.h"

xTaskHandle xTaskWatchdogResetHandle;

void vTaskWatchdogReset(void)
{
    /* Delay before the first cycle */
    vTaskDelay(pdMS_TO_TICKS(TASK_WATCHDOG_RESET_INITIAL_DELAY_MS));

    TickType_t last_cycle = xTaskGetTickCount();

    while(1)
    {
        watchdog_reset();

        vTaskDelayUntil(&last_cycle, pdMS_TO_TICKS(TASK_WATCHDOG_RESET_PERIOD_MS));
    }
}

/** \} End of watchdog_reset group */
