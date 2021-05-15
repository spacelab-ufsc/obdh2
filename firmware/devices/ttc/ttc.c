/*
 * ttc.c
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
 * \brief TTC device implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.6.19
 * 
 * \date 2020/02/01
 * 
 * \addtogroup ttc
 * \{
 */

#include <system/sys_log/sys_log.h>

#include "ttc.h"

int ttc_init(ttc_config_t config)
{
    sys_log_print_event_from_module(SYS_LOG_INFO, TTC_MODULE_NAME, "Initializing TTC device ");
    sys_log_print_uint(config.id);
    sys_log_print_msg("...");
    sys_log_new_line();

    if (sl_ttc2_init(config) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TTC_MODULE_NAME, "Error initializing the TTC device ");
        sys_log_print_uint(config.id);
        sys_log_print_msg("!");
        sys_log_new_line();

        return -1;
    }

    uint8_t hw_ver = 0;
    if (sl_ttc2_read_hardware_version(config, &hw_ver) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TTC_MODULE_NAME, "Error reading the hardware version of the TTC device ");
        sys_log_print_uint(config.id);
        sys_log_print_msg("!");
        sys_log_new_line();

        return -1;
    }

    uint32_t fw_ver = 0;
    if (sl_ttc2_read_firmware_version(config, &fw_ver) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TTC_MODULE_NAME, "Error reading the firmware version of the TTC device ");
        sys_log_print_uint(config.id);
        sys_log_print_msg("!");
        sys_log_new_line();

        return -1;
    }

    sys_log_print_event_from_module(SYS_LOG_INFO, TTC_MODULE_NAME, "SpaceLab TTC 2.0 detected! (hw=");
    sys_log_print_uint(hw_ver);
    sys_log_print_msg(", fw=");
    sys_log_print_uint(fw_ver);
    sys_log_print_msg(")");
    sys_log_new_line();

    return 0;
}

int ttc_get_data(ttc_config_t config, ttc_data_t *data)
{
    if (sl_ttc2_check_device(config) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TTC_MODULE_NAME, "Error reading the data from the TTC device ");
        sys_log_print_uint(config.id);
        sys_log_print_msg("! No device detected!");
        sys_log_new_line();

        return -1;
    }

    if (sl_ttc2_read_hk_data(config, data) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TTC_MODULE_NAME, "Error reading the data from the TTC device ");
        sys_log_print_uint(config.id);
        sys_log_print_msg("!");
        sys_log_new_line();

        return -1;
    }

    return 0;
}

int ttc_send(ttc_config_t config, uint8_t *data, uint16_t len)
{
    if (sl_ttc2_check_device(config) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TTC_MODULE_NAME, "Error sending data to the TTC device ");
        sys_log_print_uint(config.id);
        sys_log_print_msg("! No device detected!");
        sys_log_new_line();

        return -1;
    }

    if (sl_ttc2_transmit_packet(config, data, len) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TTC_MODULE_NAME, "Error sending data to the TTC device ");
        sys_log_print_uint(config.id);
        sys_log_print_msg("!");
        sys_log_new_line();

        return -1;
    }

    return 0;
}

int ttc_recv(ttc_config_t config, uint8_t *data, uint16_t *len)
{
    if (ttc_avail(config) <= 0)
    {
        return -1;  /* No packet to receive! */
    }

    if (sl_ttc2_read_packet(config, data, len) != 0)
    {
        return -1;
    }

    return 0;
}

int ttc_avail(ttc_config_t config)
{
    return sl_ttc2_check_pkt_avail(config);
}

int ttc_enter_hibernation(ttc_config_t config)
{
    return sl_ttc2_set_tx_enable(config, false);
}

int ttc_leave_hibernation(ttc_config_t config)
{
    return sl_ttc2_set_tx_enable(config, true);
}

/** \} End of ttc group */
