/*
 * process_tc.h
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
 * \brief Process TC task definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 0.10.14
 * 
 * \date 2021/07/06
 * 
 * \defgroup process_tc Process TC
 * \ingroup tasks
 * \{
 */

#ifndef PROCESS_TC_H_
#define PROCESS_TC_H_

#include <FreeRTOS.h>
#include <task.h>

#define TASK_PROCESS_TC_NAME                "Process TC"        /**< Task name. */
#define TASK_PROCESS_TC_STACK_SIZE          500                 /**< Stack size in bytes. */
#define TASK_PROCESS_TC_PRIORITY            4                   /**< Task priority. */
#define TASK_PROCESS_TC_PERIOD_MS           1000                /**< Task period in milliseconds. */
#define TASK_PROCESS_TC_INITIAL_DELAY_MS    2000                /**< Delay, in milliseconds, before the first execution. */
#define TASK_PROCESS_TC_INIT_TIMEOUT_MS     (10*1000)           /**< Wait time to initialize the task in milliseconds. */

/**
 * \brief Process TC handle.
 */
extern xTaskHandle xTaskProcessTCHandle;

/**
 * \brief Process TC task.
 *
 * \return None.
 */
void vTaskProcessTC(void);

#endif /* PROCESS_TC_H_ */

/** \} End of process_tc group */
