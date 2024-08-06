/*
 * housekeeping.h
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
 * \brief Housekeeping task definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.10.17
 * 
 * \date 2021/04/27
 * 
 * \defgroup housekeeping Housekeeping
 * \ingroup tasks
 * \{
 */

#ifndef HOUSEKEEPING_H_
#define HOUSEKEEPING_H_

#include <FreeRTOS.h>
#include <task.h>

#define TASK_HOUSEKEEPING_NAME                  "Housekeeping"      /**< Task name. */
#define TASK_HOUSEKEEPING_STACK_SIZE            225                 /**< Stack size in bytes. */
#define TASK_HOUSEKEEPING_PRIORITY              3                   /**< Task priority. */
#define TASK_HOUSEKEEPING_PERIOD_MS             (60000)             /**< Task period in milliseconds. */
#define TASK_HOUSEKEEPING_INIT_TIMEOUT_MS       2000                /**< Wait time to initialize the task in milliseconds. */

/**
 * \brief Housekeeping handle.
 */
extern xTaskHandle xTaskHousekeepingHandle;

/**
 * \brief Housekeeping task.
 *
 * \return None.
 */
void vTaskHousekeeping(void);

#endif /* HOUSEKEEPING_H_ */

/** \} End of housekeeping group */
