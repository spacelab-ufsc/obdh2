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
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 1.0.0
 * 
 * \date 2021/11/17
 * 
 * \addtogroup antenna_deployment
 * \{
 */

#include "tasks/mission_manager.h"
#include <config/config.h>
#include <conops/conops.h>
#include <system/sys_log/sys_log.h>

#include <devices/antenna/antenna.h>

#include <structs/satellite.h>

#include "antenna_deployment.h"

xTaskHandle xTaskAntennaDeploymentHandle;

void vTaskAntennaDeployment(void *p)
{
    (void)p;

    /* Initial hibernation */
    if (!sat_data_buf.obdh.data.initial_hib_executed)
    {
        uint8_t initial_hib_time_counter = sat_data_buf.obdh.data.initial_hib_time_count;

        taskENTER_CRITICAL();
        sat_data_buf.obdh.data.hib_duration = (uint32_t)CONFIG_ANTENNA_DEPLOYMENT_HIBERNATION_MIN * 60UL;
        sat_data_buf.obdh.data.hibernation_on = true;
        taskEXIT_CRITICAL();

        uint8_t i = 0;
        for(i = initial_hib_time_counter; (i < CONFIG_ANTENNA_DEPLOYMENT_HIBERNATION_MIN) && (!sat_data_buf.obdh.data.initial_hib_executed); i++)
        {
            uint32_t hib_dur = CONFIG_ANTENNA_DEPLOYMENT_HIBERNATION_MIN - (uint32_t)i;
            sys_log_print_event_from_module(SYS_LOG_WARNING, TASK_ANTENNA_DEPLOYMENT_NAME, "Antenna deployment will happen in ");
            sys_log_print_uint(hib_dur);
            sys_log_print_msg(" minutes!");
            sys_log_new_line();

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
        sys_log_print_uint(sat_data_buf.obdh.data.ant_deployment_counter + 1U);
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

        const struct conops_event ant_deploy = {
            .ev_id = EV_DEPLOYMENT_COMPLETE,
            .ev_name = "DEPLOY_END",
            .src = 0U,
            .callback = NULL,
        };

        if (notify_event_to_mission_manager(&ant_deploy) != 0)
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_ANTENNA_DEPLOYMENT_NAME, "Failed to notify Mission Manager of Antenna Deploy!");
            sys_log_new_line();
        }
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_ANTENNA_DEPLOYMENT_NAME, "All antenna deployments attempts executed! (");
        sys_log_print_uint(sat_data_buf.obdh.data.ant_deployment_counter + 1U);
        sys_log_print_msg(")");
        sys_log_new_line();
    }

    /* Startup task status = Done */
    vTaskSuspend(xTaskAntennaDeploymentHandle);
}

/** \} End of antenna_deployment group */
