/*
 * payload_edc.c
 * 
 * Copyright (C) 2020, SpaceLab.
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
 * \brief Payload EDC device implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.3.16
 * 
 * \date 18/02/2020
 * 
 * \addtogroup payload_edc
 * \{
 */

#include <system/sys_log/sys_log.h>
#include <drivers/edc/edc.h>

#include "payload_edc.h"

int payload_edc_init()
{
    edc_config_t config;

    config.port = I2C_PORT_0;
    config.bitrate = 400000;

    if (edc_init(config) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_EDC_MODULE_NAME, "Error initializing the device!");
        sys_log_new_line();

        return -1;
    }

    if (payload_edc_disable() != 0)
    {
        return -1;
    }

    edc_hk_t hk_data;

    if (edc_get_hk(&hk_data) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_EDC_MODULE_NAME, "Error pausing the PTT task!");
        sys_log_new_line();

        return -1;
    }

    sys_log_print_event_from_module(SYS_LOG_INFO, PAYLOAD_EDC_MODULE_NAME, "Initialization done! (");
    sys_log_print_uint(hk_data.temp);
    sys_log_print_msg(" oC, ");
    sys_log_print_uint(hk_data.voltage_supply);
    sys_log_print_msg(" mV, ");
    sys_log_print_uint(hk_data.current_supply);
    sys_log_print_msg(" mA)");
    sys_log_new_line();

    return 0;
}

int payload_edc_get_raw_state(uint8_t *data)
{
    int len = edc_get_state_pkg(data);

    if (len < 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_EDC_MODULE_NAME, "Error reading state from EDC!");
        sys_log_new_line();
    }

    return len;
}

int payload_edc_get_raw_housekeeping(uint8_t *data)
{
    int len = edc_get_hk_pkg(data);

    if (len < 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_EDC_MODULE_NAME, "Error reading housekeeping data from EDC!");
        sys_log_new_line();
    }

    return len;
}

int payload_edc_enable()
{
    if (edc_resume_ptt_task() != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_EDC_MODULE_NAME, "Error enabling EDC!");
        sys_log_new_line();

        return -1;
    }

    return 0;
}

int payload_edc_disable()
{
    if (edc_pause_ptt_task() != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_EDC_MODULE_NAME, "Error disabling EDC!");
        sys_log_new_line();

        return -1;
    }

    return 0;
}

/** \} End of leds group */
