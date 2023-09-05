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
 * \version 0.8.39
 * 
 * \date 2021/11/17
 * 
 * \addtogroup antenna_deployment
 * \{
 */

#include <config/config.h>
#include <system/sys_log/sys_log.h>

#include <devices/antenna/antenna.h>

#include <structs/satellite.h>

#include "antenna_deployment.h"

xTaskHandle xTaskAntennaDeploymentHandle;

void vTaskAntennaDeployment(void)
{
    /* Initial hibernation */
    if (!sat_data_buf.obdh.data.initial_hib_executed)
    {
        uint8_t initial_hib_time_counter = sat_data_buf.obdh.data.initial_hib_time_count;

        uint8_t i = 0;

        for(i = initial_hib_time_counter; i < CONFIG_ANTENNA_DEPLOYMENT_HIBERNATION_MIN; i++)
        {
            vTaskDelay(pdMS_TO_TICKS(60000U));

            sat_data_buf.obdh.data.initial_hib_time_count++;
        }

        sat_data_buf.obdh.data.initial_hib_executed = true;
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_ANTENNA_DEPLOYMENT_NAME, "Initial deployment already executed!");
        sys_log_new_line();
    }

    /* Antenna deployment */
    if (sat_data_buf.obdh.data.ant_deployment_counter < CONFIG_ANTENNA_DEPLOYMENT_ATTEMPTS)
    {
        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_ANTENNA_DEPLOYMENT_NAME, "Antenna deployment attempt number ");
        sys_log_print_uint(sat_data_buf.obdh.data.ant_deployment_counter + 1);
        sys_log_print_msg(" of ");
        sys_log_print_uint(CONFIG_ANTENNA_DEPLOYMENT_ATTEMPTS);
        sys_log_print_msg("...");
        sys_log_new_line();

        if (antenna_deploy(10U*1000U) != 0)
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_ANTENNA_DEPLOYMENT_NAME, "Error deploying the antenna!");
            sys_log_new_line();
        }

        sat_data_buf.obdh.data.ant_deployment_executed = true;

        sat_data_buf.obdh.data.ant_deployment_counter++;
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_ANTENNA_DEPLOYMENT_NAME, "All antenna deployments attempts executed! (");
        sys_log_print_uint(sat_data_buf.obdh.data.ant_deployment_counter + 1);
        sys_log_print_msg(")");
        sys_log_new_line();
    }

    /* Startup task status = Done */
    vTaskSuspend(xTaskAntennaDeploymentHandle);
}

/** \} End of antenna_deployment group */
