/*
 * read_ttc.h
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
 * \brief Read TTC data task definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.6.23
 * 
 * \date 2021/05/14
 * 
 * \defgroup read_ttc Read TTC
 * \ingroup tasks
 * \{
 */

#ifndef READ_TTC_H_
#define READ_TTC_H_

#include <FreeRTOS.h>
#include <task.h>

#define TASK_READ_TTC_NAME                      "Read TTC"          /**< Task name. */
#define TASK_READ_TTC_STACK_SIZE                384                 /**< Stack size in bytes. */
#define TASK_READ_TTC_PRIORITY                  3                   /**< Task priority. */
#define TASK_READ_TTC_PERIOD_MS                 (60000)             /**< Task period in milliseconds. */
#define TASK_READ_TTC_INIT_TIMEOUT_MS           2000                /**< Wait time to initialize the task in milliseconds. */

/**
 * \brief Read TTC handle.
 */
extern xTaskHandle xTaskReadTTCHandle;

/**
 * \brief Read TTC data task.
 *
 * \param[in] pvParameters is a value that will passed as the task's parameter.
 *
 * \return None.
 */
void vTaskReadTTC(void *pvParameters);

#endif /* READ_TTC_H_ */

/** \} End of read_ttc group */
