/*
 * eps_test.c
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
 * \brief Unit test of the EPS device.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.21
 * 
 * \date 2021/08/06
 * 
 * \defgroup eps_unit_test EPS
 * \ingroup tests
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include <stdlib.h>

#include <devices/eps/eps.h>
#include <drivers/sl_eps2/sl_eps2.h>

#define EPS_I2C_PORT            I2C_PORT_1
#define EPS_I2C_SPEED_HZ        100000UL
#define EPS_EN_PIN              GPIO_PIN_17
#define EPS_READY_PIN           GPIO_PIN_20

unsigned int generate_random(unsigned int l, unsigned int r);

static void eps_init_test(void **state)
{
    expect_value(__wrap_sl_eps2_init, config.i2c_port, EPS_I2C_PORT);
    expect_value(__wrap_sl_eps2_init, config.i2c_config.speed_hz, EPS_I2C_SPEED_HZ);
    expect_value(__wrap_sl_eps2_init, config.en_pin, EPS_EN_PIN);
    expect_value(__wrap_sl_eps2_init, config.ready_pin, EPS_READY_PIN);

    will_return(__wrap_sl_eps2_init, 0);

    assert_return_code(eps_init(), 0);
}

static void eps_get_bat_voltage_test(void **state)
{
    expect_value(__wrap_sl_eps2_read_battery_voltage, config.i2c_port, EPS_I2C_PORT);
    expect_value(__wrap_sl_eps2_read_battery_voltage, config.i2c_config.speed_hz, EPS_I2C_SPEED_HZ);
    expect_value(__wrap_sl_eps2_read_battery_voltage, config.en_pin, EPS_EN_PIN);
    expect_value(__wrap_sl_eps2_read_battery_voltage, config.ready_pin, EPS_READY_PIN);

    eps_voltage_t test_data = generate_random(0, UINT16_MAX);

    will_return(__wrap_sl_eps2_read_battery_voltage, test_data);

    will_return(__wrap_sl_eps2_read_battery_voltage, 0);

    eps_voltage_t read_data = 0;

    assert_return_code(eps_get_bat_voltage(&read_data), 0);

    assert_int_equal(test_data, read_data);
}

static void eps_get_bat_current_test(void **state)
{
    expect_value(__wrap_sl_eps2_read_battery_current, config.i2c_port, EPS_I2C_PORT);
    expect_value(__wrap_sl_eps2_read_battery_current, config.i2c_config.speed_hz, EPS_I2C_SPEED_HZ);
    expect_value(__wrap_sl_eps2_read_battery_current, config.en_pin, EPS_EN_PIN);
    expect_value(__wrap_sl_eps2_read_battery_current, config.ready_pin, EPS_READY_PIN);
    expect_value(__wrap_sl_eps2_read_battery_current, cur, SL_EPS2_BATTERY_CURRENT);

    eps_current_t test_data = generate_random(0, UINT16_MAX);

    will_return(__wrap_sl_eps2_read_battery_current, test_data);

    will_return(__wrap_sl_eps2_read_battery_current, 0);

    eps_current_t read_data = 0;

    assert_return_code(eps_get_bat_current(&read_data), 0);

    assert_int_equal(test_data, read_data);
}

static void eps_get_bat_charge_test(void **state)
{
    expect_value(__wrap_sl_eps2_read_battery_charge, config.i2c_port, EPS_I2C_PORT);
    expect_value(__wrap_sl_eps2_read_battery_charge, config.i2c_config.speed_hz, EPS_I2C_SPEED_HZ);
    expect_value(__wrap_sl_eps2_read_battery_charge, config.en_pin, EPS_EN_PIN);
    expect_value(__wrap_sl_eps2_read_battery_charge, config.ready_pin, EPS_READY_PIN);

    eps_charge_t test_data = generate_random(0, UINT16_MAX);

    will_return(__wrap_sl_eps2_read_battery_charge, test_data);

    will_return(__wrap_sl_eps2_read_battery_charge, 0);

    eps_charge_t read_data = 0;

    assert_return_code(eps_get_bat_charge(&read_data), 0);

    assert_int_equal(test_data, read_data);
}

static void eps_get_data_test(void **state)
{
    eps_data_t test_data = {0};

    test_data.time_counter                  = generate_random(0, UINT32_MAX-1);
    test_data.temperature_uc                = generate_random(0, UINT16_MAX);
    test_data.current                       = generate_random(0, UINT16_MAX);
    test_data.last_reset_cause              = generate_random(0, UINT8_MAX);
    test_data.reset_counter                 = generate_random(0, UINT16_MAX);
    test_data.solar_panel_voltage_my_px     = generate_random(0, UINT16_MAX);
    test_data.solar_panel_voltage_mx_pz     = generate_random(0, UINT16_MAX);
    test_data.solar_panel_voltage_mz_py     = generate_random(0, UINT16_MAX);
    test_data.solar_panel_current_my        = generate_random(0, UINT16_MAX);
    test_data.solar_panel_current_py        = generate_random(0, UINT16_MAX);
    test_data.solar_panel_current_mx        = generate_random(0, UINT16_MAX);
    test_data.solar_panel_current_px        = generate_random(0, UINT16_MAX);
    test_data.solar_panel_current_mz        = generate_random(0, UINT16_MAX);
    test_data.solar_panel_current_pz        = generate_random(0, UINT16_MAX);
    test_data.mppt_1_duty_cycle             = generate_random(0, UINT8_MAX);
    test_data.mppt_2_duty_cycle             = generate_random(0, UINT8_MAX);
    test_data.mppt_3_duty_cycle             = generate_random(0, UINT8_MAX);
    test_data.solar_panel_output_voltage    = generate_random(0, UINT16_MAX);
    test_data.main_power_bus_voltage        = generate_random(0, UINT16_MAX);
    test_data.rtd_0_temperature             = generate_random(0, UINT16_MAX);
    test_data.rtd_1_temperature             = generate_random(0, UINT16_MAX);
    test_data.rtd_2_temperature             = generate_random(0, UINT16_MAX);
    test_data.rtd_3_temperature             = generate_random(0, UINT16_MAX);
    test_data.rtd_4_temperature             = generate_random(0, UINT16_MAX);
    test_data.rtd_5_temperature             = generate_random(0, UINT16_MAX);
    test_data.rtd_6_temperature             = generate_random(0, UINT16_MAX);
    test_data.battery_voltage               = generate_random(0, UINT16_MAX);
    test_data.battery_current               = generate_random(0, UINT16_MAX);
    test_data.battery_average_current       = generate_random(0, UINT16_MAX);
    test_data.battery_acc_current           = generate_random(0, UINT16_MAX);
    test_data.battery_charge                = generate_random(0, UINT16_MAX);
    test_data.battery_monitor_temperature   = generate_random(0, UINT16_MAX);
    test_data.battery_monitor_status        = generate_random(0, UINT8_MAX);
    test_data.battery_monitor_protection    = generate_random(0, UINT8_MAX);
    test_data.battery_monitor_cycle_counter = generate_random(0, UINT8_MAX);
    test_data.raac                          = generate_random(0, UINT16_MAX);
    test_data.rsac                          = generate_random(0, UINT16_MAX);
    test_data.rarc                          = generate_random(0, UINT8_MAX);
    test_data.rsrc                          = generate_random(0, UINT8_MAX);
    test_data.battery_heater_1_duty_cycle   = generate_random(0, UINT8_MAX);
    test_data.battery_heater_2_duty_cycle   = generate_random(0, UINT8_MAX);
    test_data.mppt_1_mode                   = generate_random(0, UINT8_MAX);
    test_data.mppt_2_mode                   = generate_random(0, UINT8_MAX);
    test_data.mppt_3_mode                   = generate_random(0, UINT8_MAX);
    test_data.battery_heater_1_mode         = generate_random(0, UINT8_MAX);
    test_data.battery_heater_2_mode         = generate_random(0, UINT8_MAX);

    expect_value(__wrap_sl_eps2_read_data, config.i2c_port, EPS_I2C_PORT);
    expect_value(__wrap_sl_eps2_read_data, config.i2c_config.speed_hz, EPS_I2C_SPEED_HZ);
    expect_value(__wrap_sl_eps2_read_data, config.en_pin, EPS_EN_PIN);
    expect_value(__wrap_sl_eps2_read_data, config.ready_pin, EPS_READY_PIN);

    will_return(__wrap_sl_eps2_read_data, test_data.time_counter);
    will_return(__wrap_sl_eps2_read_data, test_data.temperature_uc);
    will_return(__wrap_sl_eps2_read_data, test_data.current);
    will_return(__wrap_sl_eps2_read_data, test_data.last_reset_cause);
    will_return(__wrap_sl_eps2_read_data, test_data.reset_counter);
    will_return(__wrap_sl_eps2_read_data, test_data.solar_panel_voltage_my_px);
    will_return(__wrap_sl_eps2_read_data, test_data.solar_panel_voltage_mx_pz);
    will_return(__wrap_sl_eps2_read_data, test_data.solar_panel_voltage_mz_py);
    will_return(__wrap_sl_eps2_read_data, test_data.solar_panel_current_my);
    will_return(__wrap_sl_eps2_read_data, test_data.solar_panel_current_py);
    will_return(__wrap_sl_eps2_read_data, test_data.solar_panel_current_mx);
    will_return(__wrap_sl_eps2_read_data, test_data.solar_panel_current_px);
    will_return(__wrap_sl_eps2_read_data, test_data.solar_panel_current_mz);
    will_return(__wrap_sl_eps2_read_data, test_data.solar_panel_current_pz);
    will_return(__wrap_sl_eps2_read_data, test_data.mppt_1_duty_cycle);
    will_return(__wrap_sl_eps2_read_data, test_data.mppt_2_duty_cycle);
    will_return(__wrap_sl_eps2_read_data, test_data.mppt_3_duty_cycle);
    will_return(__wrap_sl_eps2_read_data, test_data.solar_panel_output_voltage);
    will_return(__wrap_sl_eps2_read_data, test_data.main_power_bus_voltage);
    will_return(__wrap_sl_eps2_read_data, test_data.rtd_0_temperature);
    will_return(__wrap_sl_eps2_read_data, test_data.rtd_1_temperature);
    will_return(__wrap_sl_eps2_read_data, test_data.rtd_2_temperature);
    will_return(__wrap_sl_eps2_read_data, test_data.rtd_3_temperature);
    will_return(__wrap_sl_eps2_read_data, test_data.rtd_4_temperature);
    will_return(__wrap_sl_eps2_read_data, test_data.rtd_5_temperature);
    will_return(__wrap_sl_eps2_read_data, test_data.rtd_6_temperature);
    will_return(__wrap_sl_eps2_read_data, test_data.battery_voltage);
    will_return(__wrap_sl_eps2_read_data, test_data.battery_current);
    will_return(__wrap_sl_eps2_read_data, test_data.battery_average_current);
    will_return(__wrap_sl_eps2_read_data, test_data.battery_acc_current);
    will_return(__wrap_sl_eps2_read_data, test_data.battery_charge);
    will_return(__wrap_sl_eps2_read_data, test_data.battery_monitor_temperature);
    will_return(__wrap_sl_eps2_read_data, test_data.battery_monitor_status);
    will_return(__wrap_sl_eps2_read_data, test_data.battery_monitor_protection);
    will_return(__wrap_sl_eps2_read_data, test_data.battery_monitor_cycle_counter);
    will_return(__wrap_sl_eps2_read_data, test_data.raac);
    will_return(__wrap_sl_eps2_read_data, test_data.rsac);
    will_return(__wrap_sl_eps2_read_data, test_data.rarc);
    will_return(__wrap_sl_eps2_read_data, test_data.rsrc);
    will_return(__wrap_sl_eps2_read_data, test_data.battery_heater_1_duty_cycle);
    will_return(__wrap_sl_eps2_read_data, test_data.battery_heater_2_duty_cycle);
    will_return(__wrap_sl_eps2_read_data, test_data.mppt_1_mode);
    will_return(__wrap_sl_eps2_read_data, test_data.mppt_2_mode);
    will_return(__wrap_sl_eps2_read_data, test_data.mppt_3_mode);
    will_return(__wrap_sl_eps2_read_data, test_data.battery_heater_1_mode);
    will_return(__wrap_sl_eps2_read_data, test_data.battery_heater_2_mode);

    will_return(__wrap_sl_eps2_read_data, 0);

    eps_data_t read_data = {0};

    assert_return_code(eps_get_data(&read_data), 0);

    assert_memory_equal((void*)&test_data, (void*)&read_data, sizeof(eps_data_t));
}

int main(void)
{
    const struct CMUnitTest eps_tests[] = {
        cmocka_unit_test(eps_init_test),
        cmocka_unit_test(eps_get_bat_voltage_test),
        cmocka_unit_test(eps_get_bat_current_test),
        cmocka_unit_test(eps_get_bat_charge_test),
        cmocka_unit_test(eps_get_data_test),
    };

    return cmocka_run_group_tests(eps_tests, NULL, NULL);
}

unsigned int generate_random(unsigned int l, unsigned int r)
{
    return (rand() % (r - l + 1)) + l;
}

/** \} End of eps_test group */
