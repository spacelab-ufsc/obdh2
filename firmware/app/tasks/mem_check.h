/*
 * mem_check.h
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
 * \brief Memory Health Check Tasks definitions
 * 
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 0.10.18
 * 
 * \date 2024/07/24
 * 
 * \defgroup mem_check Memory Check
 * \ingroup tasks
 * \{
 */

#ifndef MEM_CHECK_H_
#define MEM_CHECK_H_

#include <FreeRTOS.h>
#include <task.h>

#define TASK_HEALTH_CHECK_MEM_NAME                       "HealthCheck Mem"      /**< Task name. */
#define TASK_HEALTH_CHECK_MEM_STACK_SIZE                 1024U                  /**< Stack size in bytes. */
#define TASK_HEALTH_CHECK_MEM_PRIORITY                   4U                     /**< Task priority. */
#define TASK_HEALTH_CHECK_MEM_INIT_TIMEOUT_MS            5000U                  /**< Task priority. */

/**
 * \brief Health Check Memory Task Handle
 */
extern TaskHandle_t xTaskHealthCheckMemHandle;

/**
 * \brief Health Check Memory Task 
 *
 * \return None.
 */
void vTaskHealthCheckMem(void);

#endif 

/** \} End of mem_check group */
