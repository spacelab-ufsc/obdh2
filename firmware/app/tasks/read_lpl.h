/*
 * read_lpl.h
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
 * \brief Read LPL data task definition.
 * 
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 1.0.0
 * 
 * \date 2025/09/28
 * 
 * \defgroup read_lpl Read LPL
 * \ingroup tasks
 * \{
 */

#ifndef READ_LPL_H_
#define READ_LPL_H_

#include <FreeRTOS.h>
#include <task.h>

#define TASK_READ_LPL_NAME                      "Read LPL"          /**< Task name. */
#define TASK_READ_LPL_STACK_SIZE                500                 /**< Stack size in bytes. */
#define TASK_READ_LPL_PRIORITY                  3                   /**< Task priority. */
#define TASK_READ_LPL_PERIOD_MS                 (10000)             /**< Task period in milliseconds. */
#define TASK_READ_LPL_INITIAL_DELAY_MS          5500                /**< Delay, in milliseconds, before the first execution. */
#define TASK_READ_LPL_INIT_TIMEOUT_MS           5000                /**< Wait time to initialize the task in milliseconds. */

/**
 * \brief Read LPL handle.
 */
extern xTaskHandle xTaskReadLPLHandle;

/**
 * \brief Read LPL data task.
 *
 * \return None.
 */
void vTaskReadLPL(void *p);

#endif /* READ_LPL_H_ */

/** \} End of read_LPL group */
