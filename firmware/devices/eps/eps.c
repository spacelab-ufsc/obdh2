/*
 * eps.c
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
 * \brief EPS device implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.2.6
 * 
 * \date 01/02/2020
 * 
 * \addtogroup eps
 * \{
 */

#include <devices/logger/logger.h>

#include <drivers/sl_eps2/sl_eps2.h>

#include "eps.h"

sl_eps2_config_t eps_config;

int eps_init()
{
    logger_print_event_from_module(LOGGER_INFO, EPS_MODULE_NAME, "Initializing EPS device...");
    logger_new_line();

    eps_config.i2c_port     = I2C_PORT_1;
    eps_config.i2c_config   = (i2c_config_t){.speed_hz=100000};
    eps_config.en_pin       = GPIO_PIN_17;
    eps_config.ready_pin    = GPIO_PIN_20;

    int err = sl_eps2_init(eps_config);
    if (err != 0)
    {
        logger_print_event_from_module(LOGGER_ERROR, EPS_MODULE_NAME, "Error during the initialization! (error ");
        logger_print_dec(err);
        logger_print_msg(")");
        logger_new_line();

        return -1;
    }

    return 0;
}

int eps_get_bat_voltage(eps_bat_voltage_t *bat_volt)
{
    int err_0 = sl_eps2_read_battery_voltage(eps_config, SL_EPS2_BATTERY_CELL_0, &bat_volt->cell_0);

    int err_1 = sl_eps2_read_battery_voltage(eps_config, SL_EPS2_BATTERY_CELL_1, &bat_volt->cell_1);

    if (err_0 != 0)
    {
        logger_print_event_from_module(LOGGER_ERROR, EPS_MODULE_NAME, "Error reading the battery voltage from cell 0! (error ");
        logger_print_dec(err_0);
        logger_print_msg(")");
        logger_new_line();

        return -1;
    }

    if (err_0 != 0)
    {
        logger_print_event_from_module(LOGGER_ERROR, EPS_MODULE_NAME, "Error reading the battery voltage from cell 1! (error ");
        logger_print_dec(err_1);
        logger_print_msg(")");
        logger_new_line();

        return -1;
    }

    return 0;
}

int eps_get_bat_current(uint32_t *bat_cur)
{
    return -1;
}

int eps_get_bat_charge(uint32_t *charge)
{
    int err = sl_eps2_read_battery_charge(eps_config, charge);

    if (err != 0)
    {
        logger_print_event_from_module(LOGGER_ERROR, EPS_MODULE_NAME, "Error reading the battery charge! (error ");
        logger_print_dec(err);
        logger_print_msg(")");
        logger_new_line();

        return -1;
    }

    return 0;
}

int eps_get_data(eps_data_t *data)
{
    int err = 0;

    if (eps_get_bat_voltage(&data->bat_voltage) != 0)
    {
        err = -1;
    }

    if (eps_get_bat_charge(&data->bat_charge))
    {
        err = -1;
    }

    return err;
}

/** \} End of eps group */
