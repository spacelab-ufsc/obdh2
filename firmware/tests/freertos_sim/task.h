/*
 * task.h
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
 * \brief FreeRTOS task simulation definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.6.4
 * 
 * \date 2021/04/27
 * 
 * \defgroup task_sim FreeRTOS task
 * \ingroup tests
 * \{
 */

#ifndef TASK_SIM_H_
#define TASK_SIM_H_

#include <stdint.h>
#include <unistd.h>

#include "FreeRTOS.h"

#define vTaskDelay(x)       usleep(1000*x)

/**
 * \brief Gets the system tick count since the begining.
 *
 * \return The tick count in milliseconds.
 */
TickType_t xTaskGetTickCount(void);

#endif /* TASK_SIM_H_ */

/** \} End of task_sim group */
