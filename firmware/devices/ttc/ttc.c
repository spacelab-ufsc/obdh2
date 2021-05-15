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
 * \version 0.6.22
 * 
 * \date 2020/02/01
 * 
 * \addtogroup ttc
 * \{
 */

#include <stdbool.h>

#include <system/sys_log/sys_log.h>
#include <drivers/spi/spi.h>

#include "ttc.h"

ttc_config_t ttc_0_config = {0};
ttc_config_t ttc_1_config = {0};

bool ttc_0_is_open = false;
bool ttc_1_is_open = false;

int ttc_init(ttc_e dev)
{
    ttc_config_t ttc_config = {0};

    switch(dev)
    {
        case TTC_0:
            if (ttc_0_is_open)
            {
                return 0;   /* TTC 0 device already initialized */
            }

            ttc_0_config.port                   = SPI_PORT_0;
            ttc_0_config.cs_pin                 = SPI_CS_0;
            ttc_0_config.port_config.speed_hz   = 100000;
            ttc_0_config.port_config.mode       = SPI_MODE_0;
            ttc_0_config.id                     = SL_TTC2_RADIO_0;

            ttc_config = ttc_0_config;

            break;
        case TTC_1:
            if (ttc_1_is_open)
            {
                return 0;   /* TTC 1 device already initialized */
            }

            ttc_1_config.port                   = SPI_PORT_0;
            ttc_1_config.cs_pin                 = SPI_CS_1;
            ttc_1_config.port_config.speed_hz   = 100000;
            ttc_1_config.port_config.mode       = SPI_MODE_0;
            ttc_1_config.id                     = SL_TTC2_RADIO_1;

            ttc_config = ttc_1_config;

            break;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, TTC_MODULE_NAME, "Error initializing the TTC device! Invalid device!");
            sys_log_new_line();

            return -1;
    }

    sys_log_print_event_from_module(SYS_LOG_INFO, TTC_MODULE_NAME, "Initializing TTC device ");
    sys_log_print_uint(ttc_config.id);
    sys_log_print_msg("...");
    sys_log_new_line();

    if (sl_ttc2_init(ttc_config) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TTC_MODULE_NAME, "Error initializing the TTC device ");
        sys_log_print_uint(ttc_config.id);
        sys_log_print_msg("!");
        sys_log_new_line();

        return -1;
    }

    uint8_t hw_ver = 0;
    if (sl_ttc2_read_hardware_version(ttc_config, &hw_ver) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TTC_MODULE_NAME, "Error reading the hardware version of the TTC device ");
        sys_log_print_uint(ttc_config.id);
        sys_log_print_msg("!");
        sys_log_new_line();

        return -1;
    }

    uint32_t fw_ver = 0;
    if (sl_ttc2_read_firmware_version(ttc_config, &fw_ver) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TTC_MODULE_NAME, "Error reading the firmware version of the TTC device ");
        sys_log_print_uint(ttc_config.id);
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

    switch(dev)
    {
        case TTC_0:     ttc_0_is_open = true;
        case TTC_1:     ttc_1_is_open = true;
    }

    return 0;
}

int ttc_get_data(ttc_e dev, ttc_data_t *data)
{
    ttc_config_t ttc_config = {0};

    switch(dev)
    {
        case TTC_0:     ttc_config = ttc_0_config;  break;
        case TTC_1:     ttc_config = ttc_1_config;  break;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, TTC_MODULE_NAME, "Error initializing the TTC device! Invalid device!");
            sys_log_new_line();

            return -1;
    }

    if (sl_ttc2_check_device(ttc_config) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TTC_MODULE_NAME, "Error reading the data from the TTC device ");
        sys_log_print_uint(ttc_config.id);
        sys_log_print_msg("! No device detected!");
        sys_log_new_line();

        return -1;
    }

    if (sl_ttc2_read_hk_data(ttc_config, data) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TTC_MODULE_NAME, "Error reading the data from the TTC device ");
        sys_log_print_uint(ttc_config.id);
        sys_log_print_msg("!");
        sys_log_new_line();

        return -1;
    }

    return 0;
}

int ttc_send(ttc_e dev, uint8_t *data, uint16_t len)
{
    ttc_config_t ttc_config = {0};

    switch(dev)
    {
        case TTC_0:     ttc_config = ttc_0_config;  break;
        case TTC_1:     ttc_config = ttc_1_config;  break;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, TTC_MODULE_NAME, "Error initializing the TTC device! Invalid device!");
            sys_log_new_line();

            return -1;
    }

    if (sl_ttc2_check_device(ttc_config) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TTC_MODULE_NAME, "Error sending data to the TTC device ");
        sys_log_print_uint(ttc_config.id);
        sys_log_print_msg("! No device detected!");
        sys_log_new_line();

        return -1;
    }

    if (sl_ttc2_transmit_packet(ttc_config, data, len) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TTC_MODULE_NAME, "Error sending data to the TTC device ");
        sys_log_print_uint(ttc_config.id);
        sys_log_print_msg("!");
        sys_log_new_line();

        return -1;
    }

    return 0;
}

int ttc_recv(ttc_e dev, uint8_t *data, uint16_t *len)
{
    ttc_config_t ttc_config = {0};

    switch(dev)
    {
        case TTC_0:     ttc_config = ttc_0_config;  break;
        case TTC_1:     ttc_config = ttc_1_config;  break;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, TTC_MODULE_NAME, "Error sending data! Invalid device!");
            sys_log_new_line();

            return -1;
    }

    if (ttc_avail(dev) <= 0)
    {
        return -1;  /* No packet to receive! */
    }

    if (sl_ttc2_read_packet(ttc_config, data, len) != 0)
    {
        return -1;
    }

    return 0;
}

int ttc_avail(ttc_e dev)
{
    ttc_config_t ttc_config = {0};

    switch(dev)
    {
        case TTC_0:     ttc_config = ttc_0_config;  break;
        case TTC_1:     ttc_config = ttc_1_config;  break;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, TTC_MODULE_NAME, "Error checking packet availability! Invalid device!");
            sys_log_new_line();

            return -1;
    }

    return sl_ttc2_check_pkt_avail(ttc_config);
}

int ttc_enter_hibernation(ttc_e dev)
{
    ttc_config_t ttc_config = {0};

    switch(dev)
    {
        case TTC_0:     ttc_config = ttc_0_config;  break;
        case TTC_1:     ttc_config = ttc_1_config;  break;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, TTC_MODULE_NAME, "Error enabling hibernation! Invalid device!");
            sys_log_new_line();

            return -1;
    }

    return sl_ttc2_set_tx_enable(ttc_config, false);
}

int ttc_leave_hibernation(ttc_e dev)
{
    ttc_config_t ttc_config = {0};

    switch(dev)
    {
        case TTC_0:     ttc_config = ttc_0_config;  break;
        case TTC_1:     ttc_config = ttc_1_config;  break;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, TTC_MODULE_NAME, "Error disabling hibernation! Invalid device!");
            sys_log_new_line();

            return -1;
    }

    return sl_ttc2_set_tx_enable(ttc_config, true);
}

/** \} End of ttc group */
