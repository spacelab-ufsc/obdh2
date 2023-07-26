/*
 * pos_det.c
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
 * \brief Position determination task definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.10.7
 * 
 * \date 2023/07/19
 * 
 * \defgroup pos_det Position Determination
 * \ingroup tasks
 * \{
 */

#ifndef POS_DET_H_
#define POS_DET_H_

#include <FreeRTOS.h>
#include <task.h>

#define TASK_POS_DET_NAME                       "Position"          /**< Task name. */
#define TASK_POS_DET_STACK_SIZE                 1024                /**< Stack size in bytes. */
#define TASK_POS_DET_PRIORITY                   2                   /**< Task priority. */
#define TASK_POS_DET_PERIOD_MS                  60000               /**< Task period in milliseconds. */
#define TASK_POS_DET_INIT_TIMEOUT_MS            1000                /**< Wait time to initialize the task in milliseconds. */

/**
 * \brief Position determination task handle.
 */
extern xTaskHandle xTaskPosDetHandle;

/**
 * \brief Position determination task.
 *
 * \return None.
 */
void vTaskPosDet(void);

#endif /* POS_DET_H_ */

/** \} End of pos_det group */
