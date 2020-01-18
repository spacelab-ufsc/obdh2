/*
 * periodic_downlink.h
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
 * \brief Periodic downlink task definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.0
 * 
 * \date 27/10/2019
 * 
 * \defgroup periodic_downlink Periodic Downlink
 * \ingroup tasks
 * \{
 */

#ifndef PERIODIC_DOWNLINK_H_
#define PERIODIC_DOWNLINK_H_

#include <FreeRTOS.h>
#include <task.h>

#define TASK_PERIODIC_DOWNLINK_NAME                 "PeriodicDownlink"  /**< Task name. */
#define TASK_PERIODIC_DOWNLINK_STACK_SIZE           4096                /**< Stack size in bytes. */
#define TASK_PERIODIC_DOWNLINK_PRIORITY             5                   /**< Task priority. */
#define TASK_PERIODIC_DOWNLINK_PERIOD_MS            60000               /**< Task period in milliseconds. */
#define TASK_PERIODIC_DOWNLINK_INITIAL_DELAY_MS     1000                /**< Delay, in milliseconds, before the first execution. */

/**
 * \brief Periodic downlink handle.
 */
extern xTaskHandle xTaskPeriodicDownlinkHandle;

/**
 * \brief Periodic downlink task.
 *
 * \param[in] pvParameters is a value that will passed as the task's parameter.
 *
 * \return None.
 */
void vTaskPeriodicDownlink(void *pvParameters);

#endif // PERIODIC_DOWNLINK_H_

//! \} End of periodic_downlink group
