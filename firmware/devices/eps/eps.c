/*
 * eps.c
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
 * \brief EPS device implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0.0
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

static sl_eps2_config_t eps_config = {0};

static bool eps_is_open = false;

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

int eps_set_param(eps_param_id_t param, uint32_t val)
{
    return sl_eps2_write_reg(eps_config, param, val);
}

int eps_get_param(eps_param_id_t param, uint32_t *val)
{
    return sl_eps2_read_reg(eps_config, param, val);
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

void eps_print_data(const eps_data_t *data)
{
    int16_t tmp = 0;

    sys_log_print_event_from_module(SYS_LOG_INFO, EPS_MODULE_NAME, "uC Temperature: ");
    sys_log_print_uint((uint32_t)data->temperature_uc);
    sys_log_print_msg(" oC");
    sys_log_new_line();

    sys_log_print_event_from_module(SYS_LOG_INFO, EPS_MODULE_NAME, "SP -Y|+X voltage: ");
    sys_log_print_uint((uint32_t)data->solar_panel_voltage_my_px);
    sys_log_print_msg(" mV");
    sys_log_new_line();

    sys_log_print_event_from_module(SYS_LOG_INFO, EPS_MODULE_NAME, "SP -Y|+X voltage: ");
    sys_log_print_uint((uint32_t)data->solar_panel_voltage_my_px);
    sys_log_print_msg(" mV");
    sys_log_new_line();

    sl_eps2_delay_ms(10U);

    sys_log_print_event_from_module(SYS_LOG_INFO, EPS_MODULE_NAME, "SP -X|+Z voltage: ");
    sys_log_print_uint((uint32_t)data->solar_panel_voltage_mx_pz);
    sys_log_print_msg(" mV");
    sys_log_new_line();

    sys_log_print_event_from_module(SYS_LOG_INFO, EPS_MODULE_NAME, "SP -Z|+Y voltage: ");
    sys_log_print_uint((uint32_t)data->solar_panel_voltage_mz_py);
    sys_log_print_msg(" mV");
    sys_log_new_line();

    sys_log_print_event_from_module(SYS_LOG_INFO, EPS_MODULE_NAME, "SP +X current: ");
    sys_log_print_uint((uint32_t)data->solar_panel_current_px);
    sys_log_print_msg(" mA");
    sys_log_new_line();

    sl_eps2_delay_ms(10U);

    sys_log_print_event_from_module(SYS_LOG_INFO, EPS_MODULE_NAME, "SP -X current: ");
    sys_log_print_uint((uint32_t)data->solar_panel_current_mx);
    sys_log_print_msg(" mA");
    sys_log_new_line();

    sys_log_print_event_from_module(SYS_LOG_INFO, EPS_MODULE_NAME, "SP -Z current: ");
    sys_log_print_uint((uint32_t)data->solar_panel_current_mz);
    sys_log_print_msg(" mA");
    sys_log_new_line();

    sys_log_print_event_from_module(SYS_LOG_INFO, EPS_MODULE_NAME, "SP +Z current: ");
    sys_log_print_uint((uint32_t)data->solar_panel_current_pz);
    sys_log_print_msg(" mA");
    sys_log_new_line();

    sl_eps2_delay_ms(10U);

    sys_log_print_event_from_module(SYS_LOG_INFO, EPS_MODULE_NAME, "SP +Y current: ");
    sys_log_print_uint((uint32_t)data->solar_panel_current_py);
    sys_log_print_msg(" mA");
    sys_log_new_line();

    sys_log_print_event_from_module(SYS_LOG_INFO, EPS_MODULE_NAME, "SP -Y current: ");
    sys_log_print_uint((uint32_t)data->solar_panel_current_my);
    sys_log_print_msg(" mA");
    sys_log_new_line();

    sys_log_print_event_from_module(SYS_LOG_INFO, EPS_MODULE_NAME, "SP total voltage: ");
    sys_log_print_uint((uint32_t)data->solar_panel_output_voltage);
    sys_log_print_msg(" mV");
    sys_log_new_line();

    sl_eps2_delay_ms(10U);

    sys_log_print_event_from_module(SYS_LOG_INFO, EPS_MODULE_NAME, "Main bus voltage: ");
    sys_log_print_uint((uint32_t)data->main_power_bus_voltage);
    sys_log_print_msg(" mV");
    sys_log_new_line();

    sys_log_print_event_from_module(SYS_LOG_INFO, EPS_MODULE_NAME, "RTD 0 temperature: ");
    sys_log_print_uint((uint32_t)data->rtd_0_temperature);
    sys_log_print_msg(" oC");
    sys_log_new_line();

    sys_log_print_event_from_module(SYS_LOG_INFO, EPS_MODULE_NAME, "RTD 1 temperature: ");
    sys_log_print_uint((uint32_t)data->rtd_1_temperature);
    sys_log_print_msg(" oC");
    sys_log_new_line();

    sl_eps2_delay_ms(10U);

    sys_log_print_event_from_module(SYS_LOG_INFO, EPS_MODULE_NAME, "RTD 2 temperature: ");
    sys_log_print_uint((uint32_t)data->rtd_2_temperature);
    sys_log_print_msg(" oC");
    sys_log_new_line();

    sys_log_print_event_from_module(SYS_LOG_INFO, EPS_MODULE_NAME, "RTD 3 temperature: ");
    sys_log_print_uint((uint32_t)data->rtd_3_temperature);
    sys_log_print_msg(" oC");
    sys_log_new_line();

    sys_log_print_event_from_module(SYS_LOG_INFO, EPS_MODULE_NAME, "RTD 4 temperature: ");
    sys_log_print_uint((uint32_t)data->rtd_4_temperature);
    sys_log_print_msg(" oC");
    sys_log_new_line();

    sl_eps2_delay_ms(10U);

    sys_log_print_event_from_module(SYS_LOG_INFO, EPS_MODULE_NAME, "RTD 5 temperature: ");
    sys_log_print_uint((uint32_t)data->rtd_5_temperature);
    sys_log_print_msg(" oC");
    sys_log_new_line();

    sys_log_print_event_from_module(SYS_LOG_INFO, EPS_MODULE_NAME, "RTD 6 temperature: ");
    sys_log_print_uint((uint32_t)data->rtd_6_temperature);
    sys_log_print_msg(" oC");
    sys_log_new_line();

    sys_log_print_event_from_module(SYS_LOG_INFO, EPS_MODULE_NAME, "Battery voltage: ");
    sys_log_print_uint((uint32_t)data->battery_voltage);
    sys_log_print_msg(" mV");
    sys_log_new_line();

    sl_eps2_delay_ms(10U);

    sys_log_print_event_from_module(SYS_LOG_INFO, EPS_MODULE_NAME, "Battery current: ");
    tmp = (int16_t)data->battery_current;
    sys_log_print_int((int32_t)tmp);
    sys_log_print_msg(" mA");
    sys_log_new_line();

    sys_log_print_event_from_module(SYS_LOG_INFO, EPS_MODULE_NAME, "Battery average current: ");
    tmp = (int16_t)data->battery_average_current;
    sys_log_print_int((int32_t)tmp);
    sys_log_print_msg(" mA");
    sys_log_new_line();

    sys_log_print_event_from_module(SYS_LOG_INFO, EPS_MODULE_NAME, "Battery accumalated current: ");
    sys_log_print_uint((uint32_t)data->battery_acc_current);
    sys_log_print_msg(" mAh");
    sys_log_new_line();
}

/** \} End of eps group */
