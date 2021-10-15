/*
 * read_antenna.h
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
 * \brief Read antenna data task definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.8.6
 * 
 * \date 2021/10/13
 * 
 * \defgroup read_antenna Read Antenna
 * \ingroup tasks
 * \{
 */

#ifndef READ_ANTENNA_H_
#define READ_ANTENNA_H_

#include <FreeRTOS.h>
#include <task.h>

#define TASK_READ_ANTENNA_NAME                  "Read Antenna"      /**< Task name. */
#define TASK_READ_ANTENNA_STACK_SIZE            150                 /**< Stack size in bytes. */
#define TASK_READ_ANTENNA_PRIORITY              3                   /**< Task priority. */
#define TASK_READ_ANTENNA_PERIOD_MS             (60000)             /**< Task period in milliseconds. */
#define TASK_READ_ANTENNA_INIT_TIMEOUT_MS       2000                /**< Wait time to initialize the task in milliseconds. */

/**
 * \brief Read Antenna handle.
 */
extern xTaskHandle xTaskReadAntennaHandle;

/**
 * \brief Read antenna data task.
 *
 * \return None.
 */
void vTaskReadAntenna(void);

#endif /* READ_ANTENNA_H_ */

/** \} End of read_antenna group */
