/*
 * eps_wrap.c
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
 * \brief EPS device wrap implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.2
 * 
 * \date 2021/08/15
 * 
 * \addtogroup eps_wrap
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include "eps_wrap.h"

int __wrap_eps_init(void)
{
    return mock_type(int);
}

int __wrap_eps_get_bat_voltage(eps_bat_voltage_t *bat_volt)
{
    if (bat_volt != NULL)
    {
        *bat_volt = mock_type(eps_bat_voltage_t);
    }

    return mock_type(int);
}

int __wrap_eps_get_bat_current(uint32_t *bat_cur)
{
    if (bat_cur != NULL)
    {
        *bat_cur = mock_type(uint32_t);
    }

    return mock_type(int);
}

int __wrap_eps_get_bat_charge(uint32_t *charge)
{
    if (charge != NULL)
    {
        *charge = mock_type(uint32_t);
    }

    return mock_type(int);
}

int __wrap_eps_get_data(eps_data_t *data)
{
    if (data != NULL)
    {
        data->time_counter                  = mock_type(uint32_t);
        data->temperature_uc                = mock_type(sl_eps2_temp_t);
        data->current                       = mock_type(sl_eps2_current_t);
        data->last_reset_cause              = mock_type(uint8_t);
        data->reset_counter                 = mock_type(uint16_t);
        data->solar_panel_voltage_my_px     = mock_type(sl_eps2_voltage_t);
        data->solar_panel_voltage_mx_pz     = mock_type(sl_eps2_voltage_t);
        data->solar_panel_voltage_mz_py     = mock_type(sl_eps2_voltage_t);
        data->solar_panel_current_my        = mock_type(sl_eps2_current_t);
        data->solar_panel_current_py        = mock_type(sl_eps2_current_t);
        data->solar_panel_current_mx        = mock_type(sl_eps2_current_t);
        data->solar_panel_current_px        = mock_type(sl_eps2_current_t);
        data->solar_panel_current_mz        = mock_type(sl_eps2_current_t);
        data->solar_panel_current_pz        = mock_type(sl_eps2_current_t);
        data->mppt_1_duty_cycle             = mock_type(sl_eps2_duty_cycle_t);
        data->mppt_2_duty_cycle             = mock_type(sl_eps2_duty_cycle_t);
        data->mppt_3_duty_cycle             = mock_type(sl_eps2_duty_cycle_t);
        data->solar_panel_output_voltage    = mock_type(sl_eps2_voltage_t);
        data->main_power_bus_voltage        = mock_type(sl_eps2_voltage_t);
        data->rtd_0_temperature             = mock_type(sl_eps2_temp_t);
        data->rtd_1_temperature             = mock_type(sl_eps2_temp_t);
        data->rtd_2_temperature             = mock_type(sl_eps2_temp_t);
        data->rtd_3_temperature             = mock_type(sl_eps2_temp_t);
        data->rtd_4_temperature             = mock_type(sl_eps2_temp_t);
        data->rtd_5_temperature             = mock_type(sl_eps2_temp_t);
        data->rtd_6_temperature             = mock_type(sl_eps2_temp_t);
        data->battery_voltage               = mock_type(sl_eps2_voltage_t);
        data->battery_current               = mock_type(sl_eps2_current_t);
        data->battery_average_current       = mock_type(sl_eps2_current_t);
        data->battery_acc_current           = mock_type(sl_eps2_current_t);
        data->battery_charge                = mock_type(sl_eps2_charge_t);
        data->battery_monitor_temperature   = mock_type(sl_eps2_temp_t);
        data->battery_monitor_status        = mock_type(uint8_t);
        data->battery_monitor_protection    = mock_type(uint8_t);
        data->battery_monitor_cycle_counter = mock_type(uint8_t);
        data->raac                          = mock_type(sl_eps2_charge_t);
        data->rsac                          = mock_type(sl_eps2_charge_t);
        data->rarc                          = mock_type(uint8_t);
        data->rsrc                          = mock_type(uint8_t);
        data->battery_heater_1_duty_cycle   = mock_type(sl_eps2_duty_cycle_t);
        data->battery_heater_2_duty_cycle   = mock_type(sl_eps2_duty_cycle_t);
        data->mppt_1_mode                   = mock_type(uint8_t);
        data->mppt_2_mode                   = mock_type(uint8_t);
        data->mppt_3_mode                   = mock_type(uint8_t);
        data->battery_heater_1_mode         = mock_type(uint8_t);
        data->battery_heater_2_mode         = mock_type(uint8_t);
    }

    return mock_type(int);
}

/** \} End of eps_wrap group */
