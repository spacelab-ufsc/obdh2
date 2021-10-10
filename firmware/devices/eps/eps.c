/*
 * eps.c
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
 * along with OBDH 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief EPS device implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.22
 * 
 * \date 2020/02/01
 * 
 * \addtogroup eps
 * \{
 */

#include <stdbool.h>

#include <system/sys_log/sys_log.h>

#include <drivers/sl_eps2/sl_eps2.h>

#include "eps.h"

sl_eps2_config_t eps_config = {0};

bool eps_is_open = false;

int eps_init(void)
{
    int err = -1;

    if (eps_is_open)
    {
        err = 0;    /* EPS device already initialized */
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_INFO, EPS_MODULE_NAME, "Initializing EPS device...");
        sys_log_new_line();

        i2c_config_t i2c_conf = {0};

        i2c_conf.speed_hz = 100000UL;

        eps_config.i2c_port     = I2C_PORT_1;
        eps_config.i2c_config   = i2c_conf;
        eps_config.en_pin       = GPIO_PIN_17;
        eps_config.ready_pin    = GPIO_PIN_20;

        int err_drv = sl_eps2_init(eps_config);

        if (err_drv == 0)
        {
            eps_is_open = true;

            err = 0;
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, EPS_MODULE_NAME, "Error during the initialization! (error ");
            sys_log_print_int(err_drv);
            sys_log_print_msg(")");
            sys_log_new_line();
        }
    }

    return err;
}

int eps_get_bat_voltage(eps_voltage_t *bat_volt)
{
    int err = -1;

    if (eps_is_open)
    {
        int err_drv = sl_eps2_read_battery_voltage(eps_config, bat_volt);

        if (err_drv == 0)
        {
            err = 0;
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, EPS_MODULE_NAME, "Error reading the battery voltage! (error ");
            sys_log_print_int(err_drv);
            sys_log_print_msg(")");
            sys_log_new_line();
        }
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, EPS_MODULE_NAME, "Error reading the battery voltage! No initialized driver!");
        sys_log_new_line();
    }

    return err;
}

int eps_get_bat_current(eps_current_t *bat_cur)
{
    int err = -1;

    if (eps_is_open)
    {
        int err_drv = sl_eps2_read_battery_current(eps_config, SL_EPS2_BATTERY_CURRENT, bat_cur);

        if (err_drv == 0)
        {
            err = 0;
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, EPS_MODULE_NAME, "Error reading the battery current! (error ");
            sys_log_print_int(err_drv);
            sys_log_print_msg(")");
            sys_log_new_line();
        }
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, EPS_MODULE_NAME, "Error reading the battery current! No initialized driver!");
        sys_log_new_line();
    }

    return err;
}

int eps_get_bat_charge(eps_charge_t *charge)
{
    int err = -1;

    if (eps_is_open)
    {
        int err_drv = sl_eps2_read_battery_charge(eps_config, charge);

        if (err_drv == 0)
        {
            err = 0;
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, EPS_MODULE_NAME, "Error reading the battery charge! (error ");
            sys_log_print_int(err_drv);
            sys_log_print_msg(")");
            sys_log_new_line();
        }
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, EPS_MODULE_NAME, "Error reading the batery charge! No initialized driver!");
        sys_log_new_line();
    }

    return err;
}

int eps_get_data(eps_data_t *data)
{
    int err = -1;

    if (eps_is_open)
    {
        int err_drv = sl_eps2_read_data(eps_config, data);

        if (err_drv == 0)
        {
            err = 0;
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, EPS_MODULE_NAME, "Error reading the data! (error ");
            sys_log_print_int(err_drv);
            sys_log_print_msg(")");
            sys_log_new_line();
        }
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, EPS_MODULE_NAME, "Error reading the data! No initialized driver!");
        sys_log_new_line();
    }

    return err;
}

/** \} End of eps group */
