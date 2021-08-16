/*
 * antenna.c
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
 * \brief Antenna device implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.3
 * 
 * \date 2019/11/01
 * 
 * \addtogroup antenna
 * \{
 */

#include <config/config.h>
#include <system/sys_log/sys_log.h>
#include <drivers/isis_antenna/isis_antenna.h>

#include "antenna.h"

int antenna_init(void)
{
#if ISIS_ANTENNA_ENABLED == 1
    sys_log_print_event_from_module(SYS_LOG_INFO, ANTENNA_MODULE_NAME, "Initializing...");
    sys_log_new_line();

    if (isis_antenna_init() != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, ANTENNA_MODULE_NAME, "Error during the initialization!");
        sys_log_new_line();

        return -1;
    }

    isis_antenna_status_t status = {0};

    if (isis_antenna_read_deployment_status(&status) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, ANTENNA_MODULE_NAME, "Error reading the antenna status!");
        sys_log_new_line();

        return -1;
    }

    sys_log_print_event_from_module(SYS_LOG_INFO, ANTENNA_MODULE_NAME, "Antenna 1 status=");
    sys_log_print_msg(status.antenna_1.status == ISIS_ANTENNA_STATUS_DEPLOYED ? "DEPLOYED" : "NOT DEPLOYED");
    sys_log_new_line();

    sys_log_print_event_from_module(SYS_LOG_INFO, ANTENNA_MODULE_NAME, "Antenna 2 status=");
    sys_log_print_msg(status.antenna_2.status == ISIS_ANTENNA_STATUS_DEPLOYED ? "DEPLOYED" : "NOT DEPLOYED");
    sys_log_new_line();

    sys_log_print_event_from_module(SYS_LOG_INFO, ANTENNA_MODULE_NAME, "Antenna 3 status=");
    sys_log_print_msg(status.antenna_3.status == ISIS_ANTENNA_STATUS_DEPLOYED ? "DEPLOYED" : "NOT DEPLOYED");
    sys_log_new_line();

    sys_log_print_event_from_module(SYS_LOG_INFO, ANTENNA_MODULE_NAME, "Antenna 4 status=");
    sys_log_print_msg(status.antenna_4.status == ISIS_ANTENNA_STATUS_DEPLOYED ? "DEPLOYED" : "NOT DEPLOYED");
    sys_log_new_line();

    return 0;
#else
    sys_log_print_event_from_module(SYS_LOG_ERROR, ANTENNA_MODULE_NAME, "No driver to initialize!");
    sys_log_new_line();

    return -1;
#endif /* ANTENNA_DRIVER */
}

int antenna_get_status(void)
{
    return -1;
}

int antenna_deploy(void)
{
    return -1;
}

/** \} End of antenna group */
