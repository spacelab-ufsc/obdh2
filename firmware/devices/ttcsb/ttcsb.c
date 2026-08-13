/*
 * ttc.c
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
 * \brief TTC device implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.8.35
 * 
 * \date 2020/02/01
 * 
 * \addtogroup ttc
 * \{
 */

#include <stdbool.h>

#include <config/config.h>
#include <system/sys_log/sys_log.h>
#include <drivers/i2c/i2c.h>


#include "ttcsb.h"

static ttcsb_config_t ttcsb_0_config = {0};
static ttcsb_config_t ttcsb_1_config = {0};

int ttcsb_init(ttcsb_e dev)
{
    static bool ttcsb_0_is_open = false;
    static bool ttcsb_1_is_open = false;

    int err = -2;

    ttcsb_config_t ttcsb_config = {0};

    switch(dev)
    {
        case TTCSB_0:
            if (ttcsb_0_is_open)
            {
                err = 0;    /* TTC 0 device already initialized */
            }
            else
            {
                ttcsb_0_config.port                   = I2C_PORT_0;
                ttcsb_0_config.port_config.speed_hz   = 1000000UL; // TEMPORARY
                ttcsb_0_config.id                     = SL_TTCSB_RADIO_0;

                ttcsb_config = ttcsb_0_config;
            }

            break;
        case TTCSB_1:
            if (ttcsb_1_is_open)
            {
                err = 0;    /* TTC 1 device already initialized */
            }
            else
            {
                ttcsb_1_config.port                   = I2C_PORT_0;
                ttcsb_1_config.port_config.speed_hz   = 1000000UL; // TEMPORARY
                ttcsb_1_config.id                     = SL_TTCSB_RADIO_1;

                ttcsb_config = ttcsb_1_config;
            }

            break;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, TTCSB_MODULE_NAME, "Error initializing the TTC device! Invalid device!");
            sys_log_new_line();

            err = -1;

            break;
    }

    if (err == -2)
    {
        sys_log_print_event_from_module(SYS_LOG_INFO, TTCSB_MODULE_NAME, "Initializing TTC device ");
        sys_log_print_uint(ttcsb_config.id);
        sys_log_print_msg("...");
        sys_log_new_line();

        if (sl_ttcsb_init(ttcsb_config) == 0)
        {
            uint8_t hw_ver = 0;

            if (sl_ttcsb_read_hardware_version(ttcsb_config, &hw_ver) == 0)
            {
                uint32_t fw_ver = 0;

                if (sl_ttcsb_read_firmware_version(ttcsb_config, &fw_ver) == 0)
                {
                    sys_log_print_event_from_module(SYS_LOG_INFO, TTCSB_MODULE_NAME, "SpaceLab TTC 2.0 detected! (hw=");
                    sys_log_print_uint(hw_ver);
                    sys_log_print_msg(", fw=");
                    sys_log_print_uint(fw_ver);
                    sys_log_print_msg(")");
                    sys_log_new_line();

                    switch(dev)
                    {
                        case TTCSB_0:     ttcsb_0_is_open = true;   break;
                        case TTCSB_1:     ttcsb_1_is_open = true;   break;
                        default:                                break;
                    }

                    err = 0;
                }
                else
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TTCSB_MODULE_NAME, "Error reading the firmware version of the TTC SBAND device ");
                    sys_log_print_uint(ttcsb_config.id);
                    sys_log_print_msg("!");
                    sys_log_new_line();

                    err = -1;
                }
            }
            else
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TTCSB_MODULE_NAME, "Error reading the hardware version of the TTC SBAND device ");
                sys_log_print_uint(ttcsb_config.id);
                sys_log_print_msg("!");
                sys_log_new_line();

                err = -1;
            }
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TTCSB_MODULE_NAME, "Error initializing the TTC SBAND device ");
            sys_log_print_uint(ttcsb_config.id);
            sys_log_print_msg("!");
            sys_log_new_line();

            err = -1;
        }
    }

    return err;
}

int ttcsb_set_param(ttcsb_e dev, ttc_param_id_t param, uint32_t val)
{
    int err = -1;

    switch(dev)
    {
        case TTCSB_0:     err = sl_ttcsb_write_reg(ttcsb_0_config, param, val);   break;
        case TTCSB_1:     err = sl_ttcsb_write_reg(ttcsb_1_config, param, val);   break;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, TTCSB_MODULE_NAME, "Error reading a parameter from the TTC device! Invalid device!");
            sys_log_new_line();

        break;
    }    
    return err;
}

int ttcsb_get_param(ttcsb_e dev, ttc_param_id_t param, uint32_t *val)
{
    int err = -1;

    switch(dev)
    {
        case TTCSB_0:     err = sl_ttcsb_read_reg(ttcsb_0_config, param, val);   break;
        case TTCSB_1:     err = sl_ttcsb_read_reg(ttcsb_1_config, param, val);   break;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, TTCSB_MODULE_NAME, "Error reading a parameter from the TTC device! Invalid device!");
            sys_log_new_line();

        break;
    }    
    return err;
}

int ttcsb_get_data(ttcsb_e dev, ttcsb_data_t *data)
{
    int err = -1;

    ttcsb_config_t ttcsb_config = {0};

    switch(dev)
    {
        case TTCSB_0:     ttcsb_config = ttcsb_0_config;  err = 0;    break;
        case TTCSB_1:     ttcsb_config = ttcsb_1_config;  err = 0;    break;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, TTCSB_MODULE_NAME, "Error initializing the TTC device! Invalid device!");
            sys_log_new_line();

            break;
    }

    if (err == 0)
    {
        if (sl_ttcsb_check_device(ttcsb_config) == 0) 
        {
            uint64_t err_id = 0;
            if (sl_ttcsb_read_hk_data(ttcsb_config, data, &err_id) != 0) 
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TTCSB_MODULE_NAME, "Error reading the data from the TTC device ");
                sys_log_print_uint(ttcsb_config.id);
                sys_log_print_msg("!");
                sys_log_new_line();

                err = -1;
            }
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TTCSB_MODULE_NAME, "Error reading the data from the TTC device ");
            sys_log_print_uint(ttcsb_config.id);
            sys_log_print_msg("! No device detected!");
            sys_log_new_line();
        }
    }

    return err;
}

/** \} End of ttc group */
