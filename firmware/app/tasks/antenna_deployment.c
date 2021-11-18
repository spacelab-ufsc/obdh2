/*
 * antenna_deployment.h
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
 * along with OBDH 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Antenna deployment task implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.8.38
 * 
 * \date 2021/11/17
 * 
 * \addtogroup antenna_deployment
 * \{
 */

#include <devices/antenna/antenna.h>

#include "antenna_deployment.h"

xTaskHandle xTaskAntennaDeploymentHandle;

void vTaskAntennaDeployment(void)
{
    vTaskDelay(pdMS_TO_TICKS(TASK_ANTENNA_DEPLOYMENT_PERIOD_MS));

    /* TODO: Deployment routine */
}

/** \} End of antenna_deployment group */
