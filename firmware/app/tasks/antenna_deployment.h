/*
 * antenna_deployment.c
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
 * \brief Antenna deployment task definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.8.38
 * 
 * \date 2021/11/17
 * 
 * \defgroup antenna_deployment Antenna Deployment
 * \ingroup tasks
 * \{
 */

#ifndef ANTENNA_DEPLOYMENT_H_
#define ANTENNA_DEPLOYMENT_H_

#include <FreeRTOS.h>
#include <task.h>

#define TASK_ANTENNA_DEPLOYMENT_NAME                "Antenna Deployment"    /**< Task name. */
#define TASK_ANTENNA_DEPLOYMENT_STACK_SIZE          150                     /**< Stack size in bytes. */
#define TASK_ANTENNA_DEPLOYMENT_PRIORITY            5                       /**< Task priority. */
#define TASK_ANTENNA_DEPLOYMENT_PERIOD_MS           (45*60*1000)            /**< Task period in milliseconds. */

/**
 * \brief Antenna deployment handle.
 */
extern xTaskHandle xTaskAntennaDeploymentHandle;

/**
 * \brief Antenna deployment task.
 *
 * \return None.
 */
void vTaskAntennaDeployment(void);

#endif /* ANTENNA_DEPLOYMENT_H_ */

/** \} End of antenna_deployment group */
