/*
 * sl_eps2_test.c
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
 * \brief Unit test of the SpaceLab EPS 2.0 driver.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.12
 * 
 * \date 2021/09/02
 * 
 * \defgroup sl_eps2_unit_test SpaceLab EPS 2.0
 * \ingroup tests
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include <drivers/i2c/i2c.h>
#include <drivers/sl_eps2/sl_eps2.h>

static void sl_eps2_init_test(void **state)
{
}

static void sl_eps2_check_device_test(void **state)
{
}

static void sl_eps2_write_reg_test(void **state)
{
}

static void sl_eps2_read_reg_test(void **state)
{
}

static void sl_eps2_read_data_test(void **state)
{
}

static void sl_eps2_read_time_counter_test(void **state)
{
}

static void sl_eps2_read_temp_test(void **state)
{
}

static void sl_eps2_read_current_test(void **state)
{
}

static void sl_eps2_read_reset_cause_test(void **state)
{
}

static void sl_eps2_read_reset_counter_test(void **state)
{
}

static void sl_eps2_read_solar_panel_voltage_test(void **state)
{
}

static void sl_eps2_read_solar_panel_current_test(void **state)
{
}

static void sl_eps2_read_mppt_duty_cycle_test(void **state)
{
}

static void sl_eps2_read_main_bus_voltage_test(void **state)
{
}

static void sl_eps2_read_rtd_temperature_test(void **state)
{
}

static void sl_eps2_read_battery_voltage_test(void **state)
{
}

static void sl_eps2_read_battery_current_test(void **state)
{
}

static void sl_eps2_read_battery_charge_test(void **state)
{
}

static void sl_eps2_read_battery_monitor_temp_test(void **state)
{
}

static void sl_eps2_read_battery_monitor_status_test(void **state)
{
}

static void sl_eps2_read_battery_monitor_protection_test(void **state)
{
}

static void sl_eps2_read_battery_monitor_cycle_counter_test(void **state)
{
}

static void sl_eps2_read_battery_monitor_raac_test(void **state)
{
}

static void sl_eps2_read_battery_monitor_rsac_test(void **state)
{
}

static void sl_eps2_read_battery_monitor_rarc_test(void **state)
{
}

static void sl_eps2_read_battery_monitor_rsrc_test(void **state)
{
}

static void sl_eps2_read_heater_duty_cycle_test(void **state)
{
}

static void sl_eps2_read_hardware_version_test(void **state)
{
}

static void sl_eps2_read_firmware_version_test(void **state)
{
}

static void sl_eps2_set_mppt_mode_test(void **state)
{
}

static void sl_eps2_get_mppt_mode_test(void **state)
{
}

static void sl_eps2_set_heater_mode_test(void **state)
{
}

static void sl_eps2_get_heater_mode_test(void **state)
{
}

static void sl_eps2_delay_ms_test(void **state)
{
}

int main(void)
{
    const struct CMUnitTest sl_eps2_tests[] = {
        cmocka_unit_test(sl_eps2_init_test),
        cmocka_unit_test(sl_eps2_check_device_test),
        cmocka_unit_test(sl_eps2_write_reg_test),
        cmocka_unit_test(sl_eps2_read_reg_test),
        cmocka_unit_test(sl_eps2_read_data_test),
        cmocka_unit_test(sl_eps2_read_time_counter_test),
        cmocka_unit_test(sl_eps2_read_temp_test),
        cmocka_unit_test(sl_eps2_read_current_test),
        cmocka_unit_test(sl_eps2_read_reset_cause_test),
        cmocka_unit_test(sl_eps2_read_reset_counter_test),
        cmocka_unit_test(sl_eps2_read_solar_panel_voltage_test),
        cmocka_unit_test(sl_eps2_read_solar_panel_current_test),
        cmocka_unit_test(sl_eps2_read_mppt_duty_cycle_test),
        cmocka_unit_test(sl_eps2_read_main_bus_voltage_test),
        cmocka_unit_test(sl_eps2_read_rtd_temperature_test),
        cmocka_unit_test(sl_eps2_read_battery_voltage_test),
        cmocka_unit_test(sl_eps2_read_battery_current_test),
        cmocka_unit_test(sl_eps2_read_battery_charge_test),
        cmocka_unit_test(sl_eps2_read_battery_monitor_temp_test),
        cmocka_unit_test(sl_eps2_read_battery_monitor_status_test),
        cmocka_unit_test(sl_eps2_read_battery_monitor_protection_test),
        cmocka_unit_test(sl_eps2_read_battery_monitor_cycle_counter_test),
        cmocka_unit_test(sl_eps2_read_battery_monitor_raac_test),
        cmocka_unit_test(sl_eps2_read_battery_monitor_rsac_test),
        cmocka_unit_test(sl_eps2_read_battery_monitor_rarc_test),
        cmocka_unit_test(sl_eps2_read_battery_monitor_rsrc_test),
        cmocka_unit_test(sl_eps2_read_heater_duty_cycle_test),
        cmocka_unit_test(sl_eps2_read_hardware_version_test),
        cmocka_unit_test(sl_eps2_read_firmware_version_test),
        cmocka_unit_test(sl_eps2_set_mppt_mode_test),
        cmocka_unit_test(sl_eps2_get_mppt_mode_test),
        cmocka_unit_test(sl_eps2_set_heater_mode_test),
        cmocka_unit_test(sl_eps2_get_heater_mode_test),
        cmocka_unit_test(sl_eps2_delay_ms_test),
    };

    return cmocka_run_group_tests(sl_eps2_tests, NULL, NULL);
}

/** \} End of sl_eps2_test group */
