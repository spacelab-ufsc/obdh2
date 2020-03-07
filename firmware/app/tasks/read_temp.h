/*
 * read_temp.c
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
 * \brief Read uC temperature task definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.2.16
 * 
 * \date 02/03/2020
 * 
 * \defgroup read_temp Read temperature
 * \ingroup tasks
 * \{
 */

#ifndef READ_TEMP_H_
#define READ_TEMP_H_

#include <FreeRTOS.h>
#include <task.h>

#define TASK_READ_TEMP_NAME                 "ReadTemp"          /**< Task name. */
#define TASK_READ_TEMP_STACK_SIZE           128                 /**< Stack size in bytes. */
#define TASK_READ_TEMP_PRIORITY             3                   /**< Task priority. */
#define TASK_READ_TEMP_PERIOD_MS            (60*1000)           /**< Task period in milliseconds. */
#define TASK_READ_TEMP_INIT_TIMEOUT_MS      2000                /**< Wait time to initialize the task in milliseconds. */

/**
 * \brief Watchdog reset handle.
 */
extern xTaskHandle xTaskReadTempHandle;

/**
 * \brief Read uC temperature task.
 *
 * \param[in] pvParameters is a value that will passed as the task's parameter.
 *
 * \return None.
 */
void vTaskReadTemp(void *pvParameters);

#endif /* READ_TEMP_H_ */

/** \} End of read_temp group */
