/*
 * antenna.c
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
 * \brief Antenna device implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 0.10.14
 * 
 * \date 2019/11/01
 * 
 * \addtogroup antenna
 * \{
 */

#include <stdbool.h>

#include <config/config.h>
#include <system/sys_log/sys_log.h>

#include <drivers/isis_antenna/isis_antenna.h>
#include <drivers/sl_antenna/sl_antenna.h>

#include "antenna.h"

#define ANTENNA_INDEPENDENT_DEPLOY_BURN_TIME_SEC        CONFIG_ANTENNA_INDEP_DEPLOY_BURN_TIME_SEC
#define ANTENNA_SEQUENTIAL_DEPLOY_BURN_TIME_SEC         CONFIG_ANTENNA_SEQ_DEPLOY_BURN_TIME_SEC

/**
 * \brief Prints the antenna status as a system log message.
 *
 * \param[in] status is the antenna data structure with the status info.
 *
 * \return None.
 */
static void antenna_print_status(isis_antenna_status_t status);

int antenna_init(void)
{
    static bool antenna_is_open = false;

    int err = -1;

    if (antenna_is_open)
    {
        err = 0;    /* Antenna device already initialized */
    }
    else
    {
    #if defined(CONFIG_DRV_ISIS_ANTENNA_ENABLED) && (CONFIG_DRV_ISIS_ANTENNA_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_INFO, ANTENNA_MODULE_NAME, "Initializing the antenna...");
        sys_log_new_line();

        if (isis_antenna_init() == 0)
        {
            int16_t temp_k = 0;

            if (isis_antenna_get_temperature_c(&temp_k) == 0)
            {
                sys_log_print_event_from_module(SYS_LOG_INFO, ANTENNA_MODULE_NAME, "Module temperature: ");
                sys_log_print_int(temp_k);
                sys_log_print_msg(" oC");
                sys_log_new_line();

                isis_antenna_status_t status = {0};

                if (isis_antenna_read_deployment_status(&status) == 0)
                {
                    antenna_print_status(status);

                    antenna_is_open = true;

                    err = 0;
                }
                else
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, ANTENNA_MODULE_NAME, "Error reading the antenna status!");
                    sys_log_new_line();
                }
            }
            else
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, ANTENNA_MODULE_NAME, "Error reading the antenna temperature!");
                sys_log_new_line();
            }
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, ANTENNA_MODULE_NAME, "Error during the initialization!");
            sys_log_new_line();
        }
    #elif defined(CONFIG_DRV_SL_ANTENNA_ENABLED) && (CONFIG_DRV_SL_ANTENNA_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_INFO, ANTENNA_MODULE_NAME, "Initializing the antenna...");
        sys_log_new_line();

        if (sl_antenna_init() == 0)
        {
            err = 0;
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, ANTENNA_MODULE_NAME, "Error during the initialization!");
            sys_log_new_line();
        }
    #else
        antenna_is_open = true;

        sys_log_print_event_from_module(SYS_LOG_ERROR, ANTENNA_MODULE_NAME, "No driver to initialize!");
        sys_log_new_line();
    #endif /* CONFIG_DRV_ANTENNA_DRIVER */
    }

    return err;
}

int antenna_get_data(antenna_data_t *data)
{
    int err = -1;

    data->status.code = UINT16_MAX;
    data->temperature = UINT16_MAX;

#if defined(CONFIG_DRV_ISIS_ANTENNA_ENABLED) && (CONFIG_DRV_ISIS_ANTENNA_ENABLED == 1)
    if (isis_antenna_get_data(data) == 0)
    {
        err = 0;
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, ANTENNA_MODULE_NAME, "Error reading the antenna data!");
        sys_log_new_line();
    }
#else
    sys_log_print_event_from_module(SYS_LOG_ERROR, ANTENNA_MODULE_NAME, "No driver to read the data!");
    sys_log_new_line();
#endif /* CONFIG_DRV_ANTENNA_DRIVER */

    return err;
}

int antenna_deploy(uint32_t timeout_ms)
{
    int err = 0;

    sys_log_print_event_from_module(SYS_LOG_INFO, ANTENNA_MODULE_NAME, "Executing the deployment routine with a timeout of ");
    sys_log_print_uint(timeout_ms);
    sys_log_print_msg(" ms...");
    sys_log_new_line();

#if defined(CONFIG_DRV_ISIS_ANTENNA_ENABLED) && (CONFIG_DRV_ISIS_ANTENNA_ENABLED == 1)
    /* Arm */
    sys_log_print_event_from_module(SYS_LOG_INFO, ANTENNA_MODULE_NAME, "Trying to arm the antenna module...");
    sys_log_new_line();

    uint32_t i = 0U;
    for(i = 0; i < timeout_ms; i += 1000U)
    {
        if (isis_antenna_arm() == 0)
        {
            sys_log_print_event_from_module(SYS_LOG_INFO, ANTENNA_MODULE_NAME, "The antenna is armed!");
            sys_log_new_line();

            break;
        }

        isis_antenna_delay_s(1U);
    }

    if (i >= timeout_ms)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, ANTENNA_MODULE_NAME, "Timeout reached trying to arm the antenna!");
        sys_log_new_line();

        err++;
    }

    /* Independent deployment */
    sys_log_print_event_from_module(SYS_LOG_INFO, ANTENNA_MODULE_NAME, "Trying an independent deploy sequence...");
    sys_log_new_line();

    isis_antenna_ant_t ant = ISIS_ANTENNA_ANT_1;
    for(ant = ISIS_ANTENNA_ANT_1; ant <= ISIS_ANTENNA_ANT_4; ant++)
    {
        if (isis_antenna_start_independent_deploy(ant, ANTENNA_INDEPENDENT_DEPLOY_BURN_TIME_SEC, ISIS_ANTENNA_INDEPENDENT_DEPLOY_WITH_OVERRIDE) != 0)
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, ANTENNA_MODULE_NAME, "Error during antenna ");
            sys_log_print_int(ant);
            sys_log_print_msg(" deployment!");
            sys_log_new_line();

            err++;
        }

        isis_antenna_delay_s(ANTENNA_INDEPENDENT_DEPLOY_BURN_TIME_SEC);
    }

    /* Sequential deployment */
    sys_log_print_event_from_module(SYS_LOG_INFO, ANTENNA_MODULE_NAME, "Trying a sequential deploy...");
    sys_log_new_line();

    if (isis_antenna_start_sequential_deploy(ANTENNA_SEQUENTIAL_DEPLOY_BURN_TIME_SEC) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, ANTENNA_MODULE_NAME, "Error during the sequential deployment!");
        sys_log_new_line();

        err++;
    }

    isis_antenna_delay_s(4U*ANTENNA_SEQUENTIAL_DEPLOY_BURN_TIME_SEC);

    /* Check if the antennas are deployed */
    antenna_data_t ant_data = {0};
    if (isis_antenna_read_deployment_status(&(ant_data.status)) == 0)
    {
        antenna_print_status(ant_data.status);
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, ANTENNA_MODULE_NAME, "Error reading the antenna status!");
        sys_log_new_line();

        err++;
    }

    /* Disarm */
    sys_log_print_event_from_module(SYS_LOG_INFO, ANTENNA_MODULE_NAME, "Disarming the antenna module...");
    sys_log_new_line();

    if (isis_antenna_disarm() != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, ANTENNA_MODULE_NAME, "Error disarming the antenna!");
        sys_log_new_line();

        err++;
    }
#elif defined(CONFIG_DRV_SL_ANTENNA_ENABLED) && (CONFIG_DRV_SL_ANTENNA_ENABLED == 1)
    sys_log_print_event_from_module(SYS_LOG_INFO, ANTENNA_MODULE_NAME, "Trying a sequential deploy...");
    sys_log_new_line();

    if (sl_antenna_start_sequential_deploy() != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, ANTENNA_MODULE_NAME, "Error during the sequential deployment!");
        sys_log_new_line();

        err = -1;
    }
#else
    sys_log_print_event_from_module(SYS_LOG_ERROR, ANTENNA_MODULE_NAME, "No driver to read the status!");
    sys_log_new_line();

    err = -1;
#endif /* CONFIG_DRV_ANTENNA_DRIVER */

    return err;
}

static void antenna_print_status(isis_antenna_status_t status)
{
    sys_log_print_event_from_module(SYS_LOG_INFO, ANTENNA_MODULE_NAME, "Antenna 1 status=");
    sys_log_print_msg((status.antenna_1.status == ISIS_ANTENNA_STATUS_DEPLOYED) ? "DEPLOYED" : "NOT DEPLOYED");
    sys_log_new_line();

    sys_log_print_event_from_module(SYS_LOG_INFO, ANTENNA_MODULE_NAME, "Antenna 2 status=");
    sys_log_print_msg((status.antenna_2.status == ISIS_ANTENNA_STATUS_DEPLOYED) ? "DEPLOYED" : "NOT DEPLOYED");
    sys_log_new_line();

    sys_log_print_event_from_module(SYS_LOG_INFO, ANTENNA_MODULE_NAME, "Antenna 3 status=");
    sys_log_print_msg((status.antenna_3.status == ISIS_ANTENNA_STATUS_DEPLOYED) ? "DEPLOYED" : "NOT DEPLOYED");
    sys_log_new_line();

    sys_log_print_event_from_module(SYS_LOG_INFO, ANTENNA_MODULE_NAME, "Antenna 4 status=");
    sys_log_print_msg((status.antenna_4.status == ISIS_ANTENNA_STATUS_DEPLOYED) ? "DEPLOYED" : "NOT DEPLOYED");
    sys_log_new_line();
}

/** \} End of antenna group */
