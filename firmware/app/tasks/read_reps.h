/*
 * read_reps.h
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
 * \brief Read EPS data task definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Pedro Ferrari Barbosa <pedro.ferraribarbosa2007@gmail.com>
 * 
 * \version 1.0.0
 * 
 * \date 2026/12/11
 * 
 * \defgroup read_eps Read EPS
 * \ingroup tasks
 * \{
 */

 #ifndef READ_REPS_H_
 #define READ_REPS_H_

 #include <FreeRTOS.h>
 #include <task.h>

 #define TASK_READ_REPS_NAME                      "Read REPS"          /**< Task name. */
 #define TASK_READ_REPS_INIT_TIMEOUT_MS           5000                /**< Wait time to initialize the task in milliseconds. */

 /**
 * \brief Read REPS task handle
 */
 extern xTaskHandle xTaskReadRepsHandle;

 /** 
 * \brief Read REPS data task
 *
 * \return None.
 */
 void vTaskReadReps(void *p);

 #endif /* READ_REPS_H_ */

/** \} End of read_eps group */