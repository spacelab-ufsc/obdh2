/*
 * mode_check.h
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
 * \brief Operation Mode Health Check Tasks definitions
 *
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 *
 * \version 0.10.19
 *
 * \date 2024/08/09
 *
 * \defgroup mode_check Operation Mode Check
 * \ingroup tasks
 * \{
 */

#ifndef MODE_CHECK_H_
#define MODE_CHECK_H_

#include <FreeRTOS.h>
#include <task.h>

#define TASK_HEALTH_CHECK_MODE_NAME                       "HealthCheck Mode"     /**< Task name. */
#define TASK_HEALTH_CHECK_MODE_STACK_SIZE                 2048U                  /**< Stack size in bytes. */
#define TASK_HEALTH_CHECK_MODE_PRIORITY                   3U                     /**< Task priority. */

/**
 * \brief Health Check Operation Mode Task Handle
 */
extern TaskHandle_t xTaskHealthCheckModeHandle;

/**
 * \brief Health Check Operation Mode Task
 *
 * \return None.
 */
void vTaskHealthCheckMode(void);

#endif

/** \} End of mode_check group */
