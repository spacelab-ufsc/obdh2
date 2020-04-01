/*
 * save_time.c
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
 * \brief Save system time task definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.3.2
 * 
 * \date 17/03/2020
 * 
 * \defgroup save_time Save Time
 * \ingroup tasks
 * \{
 */

#ifndef SAVE_TIME_H_
#define SAVE_TIME_H_

#include <FreeRTOS.h>
#include <task.h>

#define TASK_SAVE_TIME_NAME                 "SaveTime"          /**< Task name. */
#define TASK_SAVE_TIME_STACK_SIZE           128                 /**< Stack size in bytes. */
#define TASK_SAVE_TIME_PRIORITY             3                   /**< Task priority. */
#define TASK_SAVE_TIME_PERIOD_MS            (60*1000)           /**< Task period in milliseconds. */
#define TASK_SAVE_TIME_INIT_TIMEOUT_MS      0                   /**< Wait time to initialize the task in milliseconds. */

/**
 * \brief Save system time handle.
 */
extern xTaskHandle xTaskSaveTimeHandle;

/**
 * \brief Save system time task.
 *
 * \param[in] pvParameters is a value that will passed as the task's parameter.
 *
 * \return None.
 */
void vTaskSaveTime(void *pvParameters);

#endif /* SAVE_TIME_H_ */

/** \} End of save_time group */
