/*
 * data_log.h
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
 * along with OBDH 2.0. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Data log task definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.6.39
 * 
 * \date 2021/05/24
 * 
 * \defgroup data_log Data Log
 * \ingroup tasks
 * \{
 */

#ifndef DATA_LOG_H_
#define DATA_LOG_H_

#include <FreeRTOS.h>
#include <task.h>

#define TASK_DATA_LOG_NAME                      "Data Log"          /**< Task name. */
#define TASK_DATA_LOG_STACK_SIZE                1000                /**< Stack size in bytes. */
#define TASK_DATA_LOG_PRIORITY                  3                   /**< Task priority. */
#define TASK_DATA_LOG_PERIOD_MS                 (600000UL)          /**< Task period in milliseconds. */
#define TASK_DATA_LOG_INIT_TIMEOUT_MS           2000                /**< Wait time to initialize the task in milliseconds. */

/**
 * \brief Data log handle.
 */
extern xTaskHandle xTaskDataLogHandle;

/**
 * \brief Data log task.
 *
 * \param[in] pvParameters is a value that will passed as the task's parameter.
 *
 * \return None.
 */
void vTaskDataLog(void *pvParameters);

#endif /* DATA_LOG_H_ */

/** \} End of data_log group */
