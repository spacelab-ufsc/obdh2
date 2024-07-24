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

#define TASK_HEALTH_CHECK_MEM_NAME                       "HealthCheck Mem"       /**< Task name. */
#define TASK_HEALTH_CHECK_MEM_STACK_SIZE                 512                     /**< Stack size in bytes. */
#define TASK_HEALTH_CHECK_MEM_PRIORITY                   4                       /**< Task priority. */
#define TASK_HEALTH_CHECK_MEM_INIT_TIMEOUT_MS            5000UL                  /**< Task priority. */

#define TASK_HEALTH_CHECK_MEM_LOAD_CFG_NAME              "HealthCheck MemCfg"    /**< Task name. */
#define TASK_HEALTH_CHECK_MEM_LOAD_CFG_STACK_SIZE        512                     /**< Stack size in bytes. */
#define TASK_HEALTH_CHECK_MEM_LOAD_CFG_PRIORITY          4                       /**< Task priority. */
#define TASK_HEALTH_CHECK_MEM_LOAD_CFGINIT_TIMEOUT_MS    120000UL                /**< Task priority. */

/**
 * \brief Health Check Memory Task Handle
 */
extern TaskHandle_t xTaskHealthCheckMemHandle;

/**
 * \brief Health Check Memory Config Load Task Handle
 */
extern TaskHandle_t xTaskHealthCheckMemCfgLoadHandle;

/**
 * \brief Health Check Memory Task 
 *
 * \return None.
 */
void vTaskHealthCheckMem(void);

/**
 * \brief Health Check Memory Config Load Task 
 *
 * \return None.
 */
void vTaskHealthCheckMemCfgLoad(void);

#endif 

/** \} End of mem_check group */
