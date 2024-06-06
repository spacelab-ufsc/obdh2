/*
 * system_reset.h
 * 
 * Copyright The OBDH 2.0 Contributors.
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
 * \brief Periodic system reset task definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.10.16
 * 
 * \date 2020/01/12
 * 
 * \defgroup system_reset System Reset
 * \ingroup tasks
 * \{
 */

#ifndef SYSTEM_RESET_H_
#define SYSTEM_RESET_H_

#include <FreeRTOS.h>
#include <task.h>

#define TASK_SYSTEM_RESET_NAME                      "SystemReset"           /**< Task name. */
#define TASK_SYSTEM_RESET_STACK_SIZE                128                     /**< Stack size in bytes. */
#define TASK_SYSTEM_RESET_PRIORITY                  2                       /**< Task priority. */
#define TASK_SYSTEM_RESET_PERIOD_MS                 (1000UL*60U*60U*10U)    /**< Task period in milliseconds. */

#define pdMS_TO_TICKS_64( xTimeInMs ) ( ( TickType_t ) ( ( ( uint64_t ) ( xTimeInMs ) * ( uint64_t ) configTICK_RATE_HZ ) / ( TickType_t ) 1000 ) )

/**
 * \brief Periodic system reset handle.
 */
extern xTaskHandle xTaskSystemResetHandle;

/**
 * \brief Periodic system reset task.
 *
 * This task resets the microcontroller.
 *
 * \return None.
 */
void vTaskSystemReset(void);

#endif /* SYSTEM_RESET_H_ */

/** \} End of system_reset group */
