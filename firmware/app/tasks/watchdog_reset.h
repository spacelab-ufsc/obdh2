/*
 * watchdog_reset.c
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
 * \brief Watchdog reset task definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.0
 * 
 * \date 11/01/2020
 * 
 * \defgroup watchdog_reset Watchdog Reset
 * \ingroup tasks
 * \{
 */

#ifndef WATCHDOG_RESET_H_
#define WATCHDOG_RESET_H_

#include <FreeRTOS.h>
#include <task.h>

#define TASK_WATCHDOG_RESET_NAME                    "WatchdogReset"     /**< Task name. */
#define TASK_WATCHDOG_RESET_STACK_SIZE              128                 /**< Stack size in bytes. */
#define TASK_WATCHDOG_RESET_PRIORITY                1                   /**< Task priority. */
#define TASK_WATCHDOG_RESET_PERIOD_MS               100                 /**< Task period in milliseconds. */
#define TASK_WATCHDOG_RESET_INITIAL_DELAY_MS        0                   /**< Delay, in milliseconds, before the first execution. */

/**
 * \brief Watchdog reset handle.
 */
extern xTaskHandle xTaskWatchdogResetHandle;

/**
 * \brief Watchdog reset task.
 *
 * \param[in] pvParameters is a value that will passed as the task's parameter.
 *
 * \return None.
 */
void vTaskWatchdogReset(void *pvParameters);

#endif // WATCHDOG_RESET_H_

//! \} End of watchdog_reset group
