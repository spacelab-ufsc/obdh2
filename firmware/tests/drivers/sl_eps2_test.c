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
 * \version 0.7.19
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

#include <stdlib.h>

#include <drivers/i2c/i2c.h>
#include <drivers/gpio/gpio.h>
#include <drivers/sl_eps2/sl_eps2.h>

#define SL_EPS2_I2C_PORT        I2C_PORT_1
#define SL_EPS2_I2C_CLOCK_HZ    100000UL
#define SL_EPS2_I2C_EN_PIN      GPIO_PIN_17
#define SL_EPS2_I2C_RDY_PIN     GPIO_PIN_20
#define SL_EPS2_I2C_ADR         0x36

sl_eps2_config_t conf = {0};

unsigned int generate_random(unsigned int l, unsigned int r);

uint8_t crc8(uint8_t *data, uint8_t len);

void read_reg(uint8_t adr, uint32_t val);

static void sl_eps2_init_test(void **state)
{
    /* I2C initialization */
    expect_value(__wrap_i2c_init, port, SL_EPS2_I2C_PORT);
    expect_value(__wrap_i2c_init, config.speed_hz, SL_EPS2_I2C_CLOCK_HZ);

    will_return(__wrap_i2c_init, 0);

    /* Enable pin initialization */
    expect_value(__wrap_gpio_init, pin, SL_EPS2_I2C_EN_PIN);
    expect_value(__wrap_gpio_init, config.mode, GPIO_MODE_OUTPUT);

    will_return(__wrap_gpio_init, 0);

    /* Ready pin initialization */
    expect_value(__wrap_gpio_init, pin, SL_EPS2_I2C_RDY_PIN);
    expect_value(__wrap_gpio_init, config.mode, GPIO_MODE_INPUT);

    will_return(__wrap_gpio_init, 0);

    /* Set state */
    expect_value(__wrap_gpio_set_state, pin, SL_EPS2_I2C_EN_PIN);
    expect_value(__wrap_gpio_set_state, level, true);

    will_return(__wrap_gpio_set_state, 0);

    /* Get state */
    expect_value(__wrap_gpio_get_state, pin, SL_EPS2_I2C_RDY_PIN);

    will_return(__wrap_gpio_get_state, 1);

    uint8_t data[256] = {UINT8_MAX};

    data[0] = 48;   /* Device ID register */
    data[1] = crc8(data, 1);

    /* I2C write */
    expect_value(__wrap_i2c_write, port, SL_EPS2_I2C_PORT);
    expect_value(__wrap_i2c_write, adr, SL_EPS2_I2C_ADR);
    expect_memory(__wrap_i2c_write, data, (void*)data, 2);
    expect_value(__wrap_i2c_write, len, 2);

    will_return(__wrap_i2c_write, 0);

    /* Get state */
    expect_value(__wrap_gpio_get_state, pin, SL_EPS2_I2C_RDY_PIN);

    will_return(__wrap_gpio_get_state, 1);

    /* I2C read */
    expect_value(__wrap_i2c_read, port, SL_EPS2_I2C_PORT);
    expect_value(__wrap_i2c_read, adr, SL_EPS2_I2C_ADR);
    expect_value(__wrap_i2c_read, len, 5);

    data[0] = 0x00;
    data[1] = 0x00;
    data[2] = 0xEE;
    data[3] = 0xE2;
    data[4] = crc8(data, 4);

    uint16_t i = 0;
    for(i=0; i<5; i++)
    {
        will_return(__wrap_i2c_read, data[i]);
    }

    will_return(__wrap_i2c_read, 0);

    /* Get state */
    expect_value(__wrap_gpio_get_state, pin, SL_EPS2_I2C_RDY_PIN);

    will_return(__wrap_gpio_get_state, 1);

    assert_return_code(sl_eps2_init(conf), 0);
}

static void sl_eps2_check_device_test(void **state)
{
    uint8_t data[256] = {UINT8_MAX};

    data[0] = 48;   /* Device ID register */
    data[1] = crc8(data, 1);

    /* I2C write */
    expect_value(__wrap_i2c_write, port, SL_EPS2_I2C_PORT);
    expect_value(__wrap_i2c_write, adr, SL_EPS2_I2C_ADR);
    expect_memory(__wrap_i2c_write, data, (void*)data, 2);
    expect_value(__wrap_i2c_write, len, 2);

    will_return(__wrap_i2c_write, 0);

    /* Get state */
    expect_value(__wrap_gpio_get_state, pin, SL_EPS2_I2C_RDY_PIN);

    will_return(__wrap_gpio_get_state, 1);

    /* I2C read */
    expect_value(__wrap_i2c_read, port, SL_EPS2_I2C_PORT);
    expect_value(__wrap_i2c_read, adr, SL_EPS2_I2C_ADR);
    expect_value(__wrap_i2c_read, len, 5);

    data[0] = 0x00;
    data[1] = 0x00;
    data[2] = 0xEE;
    data[3] = 0xE2;
    data[4] = crc8(data, 4);

    uint16_t i = 0;
    for(i=0; i<5; i++)
    {
        will_return(__wrap_i2c_read, data[i]);
    }

    will_return(__wrap_i2c_read, 0);

    /* Get state */
    expect_value(__wrap_gpio_get_state, pin, SL_EPS2_I2C_RDY_PIN);

    will_return(__wrap_gpio_get_state, 1);

    assert_return_code(sl_eps2_check_device(conf), 0);
}

static void sl_eps2_write_reg_test(void **state)
{
    uint8_t data[256] = {UINT8_MAX};

    data[0] = generate_random(0, UINT8_MAX);
    data[1] = generate_random(0, UINT8_MAX);
    data[2] = generate_random(0, UINT8_MAX);
    data[3] = generate_random(0, UINT8_MAX);
    data[4] = generate_random(0, UINT8_MAX);
    data[5] = crc8(data, 5);

    /* I2C write */
    expect_value(__wrap_i2c_write, port, SL_EPS2_I2C_PORT);
    expect_value(__wrap_i2c_write, adr, SL_EPS2_I2C_ADR);
    expect_memory(__wrap_i2c_write, data, (void*)data, 6);
    expect_value(__wrap_i2c_write, len, 6);

    will_return(__wrap_i2c_write, 0);

    /* Get state */
    expect_value(__wrap_gpio_get_state, pin, SL_EPS2_I2C_RDY_PIN);

    will_return(__wrap_gpio_get_state, 1);

    uint32_t val = (uint32_t)(data[1] << 24) |
                   (uint32_t)(data[2] << 16) |
                   (uint32_t)(data[3] << 8) |
                   (uint32_t)(data[4] << 0);

    assert_return_code(sl_eps2_write_reg(conf, data[0], val), 0);
}

static void sl_eps2_read_reg_test(void **state)
{
    uint8_t data[256] = {UINT8_MAX};
    uint8_t adr = generate_random(0, UINT8_MAX);

    data[0] = adr;
    data[1] = crc8(data, 1);

    /* I2C write */
    expect_value(__wrap_i2c_write, port, SL_EPS2_I2C_PORT);
    expect_value(__wrap_i2c_write, adr, SL_EPS2_I2C_ADR);
    expect_memory(__wrap_i2c_write, data, (void*)data, 2);
    expect_value(__wrap_i2c_write, len, 2);

    will_return(__wrap_i2c_write, 0);

    /* Get state */
    expect_value(__wrap_gpio_get_state, pin, SL_EPS2_I2C_RDY_PIN);

    will_return(__wrap_gpio_get_state, 1);

    /* I2C read */
    expect_value(__wrap_i2c_read, port, SL_EPS2_I2C_PORT);
    expect_value(__wrap_i2c_read, adr, SL_EPS2_I2C_ADR);
    expect_value(__wrap_i2c_read, len, 5);

    data[0] = generate_random(0, UINT8_MAX);
    data[1] = generate_random(0, UINT8_MAX);
    data[2] = generate_random(0, UINT8_MAX);
    data[3] = generate_random(0, UINT8_MAX);
    data[4] = crc8(data, 4);

    uint16_t i = 0;
    for(i=0; i<5; i++)
    {
        will_return(__wrap_i2c_read, data[i]);
    }

    will_return(__wrap_i2c_read, 0);

    /* Get state */
    expect_value(__wrap_gpio_get_state, pin, SL_EPS2_I2C_RDY_PIN);

    will_return(__wrap_gpio_get_state, 1);

    uint32_t val = UINT32_MAX;

    assert_return_code(sl_eps2_read_reg(conf, adr, &val), 0);

    assert_int_equal(data[0], (val >> 24) & 0xFF);
    assert_int_equal(data[1], (val >> 16) & 0xFF);
    assert_int_equal(data[2], (val >> 8) & 0xFF);
    assert_int_equal(data[3], (val >> 0) & 0xFF);
}

static void sl_eps2_read_data_test(void **state)
{
    sl_eps2_data_t data_val = {0};

    data_val.time_counter = generate_random(0, UINT32_MAX-1);
    read_reg(0, data_val.time_counter);

    data_val.temperature_uc = generate_random(0, UINT16_MAX);
    read_reg(1, (uint32_t)data_val.temperature_uc);

    data_val.current = generate_random(0, UINT16_MAX);
    read_reg(2, (uint32_t)data_val.current);

    data_val.last_reset_cause = generate_random(0, UINT8_MAX);
    read_reg(3, (uint32_t)data_val.last_reset_cause);

    data_val.reset_counter = generate_random(0, UINT16_MAX);
    read_reg(4, (uint32_t)data_val.reset_counter);

    data_val.solar_panel_voltage_my_px = generate_random(0, UINT16_MAX);
    read_reg(5, (uint32_t)data_val.solar_panel_voltage_my_px);

    data_val.solar_panel_voltage_mx_pz = generate_random(0, UINT16_MAX);
    read_reg(6, (uint32_t)data_val.solar_panel_voltage_mx_pz);

    data_val.solar_panel_voltage_mz_py = generate_random(0, UINT16_MAX);
    read_reg(7, (uint32_t)data_val.solar_panel_voltage_mz_py);

    data_val.solar_panel_output_voltage = generate_random(0, UINT16_MAX);
    read_reg(17, (uint32_t)data_val.solar_panel_output_voltage);

    data_val.solar_panel_current_my = generate_random(0, UINT16_MAX);
    read_reg(8, (uint32_t)data_val.solar_panel_current_my);

    data_val.solar_panel_current_py = generate_random(0, UINT16_MAX);
    read_reg(9, (uint32_t)data_val.solar_panel_current_py);

    data_val.solar_panel_current_mx = generate_random(0, UINT16_MAX);
    read_reg(10, (uint32_t)data_val.solar_panel_current_mx);

    data_val.solar_panel_current_px = generate_random(0, UINT16_MAX);
    read_reg(11, (uint32_t)data_val.solar_panel_current_px);

    data_val.solar_panel_current_mz = generate_random(0, UINT16_MAX);
    read_reg(12, (uint32_t)data_val.solar_panel_current_mz);

    data_val.solar_panel_current_pz = generate_random(0, UINT16_MAX);
    read_reg(13, (uint32_t)data_val.solar_panel_current_pz);

    data_val.mppt_1_duty_cycle = generate_random(0, UINT8_MAX);
    read_reg(14, (uint32_t)data_val.mppt_1_duty_cycle);

    data_val.mppt_2_duty_cycle = generate_random(0, UINT8_MAX);
    read_reg(15, (uint32_t)data_val.mppt_2_duty_cycle);

    data_val.mppt_3_duty_cycle = generate_random(0, UINT8_MAX);
    read_reg(16, (uint32_t)data_val.mppt_3_duty_cycle);

    data_val.main_power_bus_voltage = generate_random(0, UINT16_MAX);
    read_reg(18, (uint32_t)data_val.main_power_bus_voltage);

    data_val.rtd_0_temperature = generate_random(0, UINT16_MAX);
    read_reg(19, (uint32_t)data_val.rtd_0_temperature);

    data_val.rtd_1_temperature = generate_random(0, UINT16_MAX);
    read_reg(20, (uint32_t)data_val.rtd_1_temperature);

    data_val.rtd_2_temperature = generate_random(0, UINT16_MAX);
    read_reg(21, (uint32_t)data_val.rtd_2_temperature);

    data_val.rtd_3_temperature = generate_random(0, UINT16_MAX);
    read_reg(22, (uint32_t)data_val.rtd_3_temperature);

    data_val.rtd_4_temperature = generate_random(0, UINT16_MAX);
    read_reg(23, (uint32_t)data_val.rtd_4_temperature);

    data_val.rtd_5_temperature = generate_random(0, UINT16_MAX);
    read_reg(24, (uint32_t)data_val.rtd_5_temperature);

    data_val.rtd_6_temperature = generate_random(0, UINT16_MAX);
    read_reg(25, (uint32_t)data_val.rtd_6_temperature);

    data_val.battery_voltage = generate_random(0, UINT16_MAX);
    read_reg(26, (uint32_t)data_val.battery_voltage);

    data_val.battery_current = generate_random(0, UINT16_MAX);
    read_reg(27, (uint32_t)data_val.battery_current);

    data_val.battery_average_current = generate_random(0, UINT16_MAX);
    read_reg(28, (uint32_t)data_val.battery_average_current);

    data_val.battery_acc_current = generate_random(0, UINT16_MAX);
    read_reg(29, (uint32_t)data_val.battery_acc_current);

    data_val.battery_charge = generate_random(0, UINT16_MAX);
    read_reg(30, (uint32_t)data_val.battery_charge);

    data_val.battery_monitor_temperature = generate_random(0, UINT16_MAX);
    read_reg(31, (uint32_t)data_val.battery_monitor_temperature);

    data_val.battery_monitor_status = generate_random(0, UINT8_MAX);
    read_reg(32, (uint32_t)data_val.battery_monitor_status);

    data_val.battery_monitor_protection = generate_random(0, UINT8_MAX);
    read_reg(33, (uint32_t)data_val.battery_monitor_protection);

    data_val.battery_monitor_cycle_counter = generate_random(0, UINT8_MAX);
    read_reg(34, (uint32_t)data_val.battery_monitor_cycle_counter);

    data_val.raac = generate_random(0, UINT16_MAX);
    read_reg(35, (uint32_t)data_val.raac);

    data_val.rsac = generate_random(0, UINT16_MAX);
    read_reg(36, (uint32_t)data_val.rsac);

    data_val.rarc = generate_random(0, UINT8_MAX);
    read_reg(37, (uint32_t)data_val.rarc);

    data_val.rsrc = generate_random(0, UINT8_MAX);
    read_reg(38, (uint32_t)data_val.rsrc);

    data_val.battery_heater_1_duty_cycle = generate_random(0, UINT8_MAX);
    read_reg(39, (uint32_t)data_val.battery_heater_1_duty_cycle);

    data_val.battery_heater_2_duty_cycle = generate_random(0, UINT8_MAX);
    read_reg(40, (uint32_t)data_val.battery_heater_2_duty_cycle);

    data_val.mppt_1_mode = generate_random(0, UINT8_MAX);
    read_reg(43, (uint32_t)data_val.mppt_1_mode);

    data_val.mppt_2_mode = generate_random(0, UINT8_MAX);
    read_reg(44, (uint32_t)data_val.mppt_2_mode);

    data_val.mppt_3_mode = generate_random(0, UINT8_MAX);
    read_reg(45, (uint32_t)data_val.mppt_3_mode);

    data_val.battery_heater_1_mode = generate_random(0, UINT8_MAX);
    read_reg(46, (uint32_t)data_val.battery_heater_1_mode);

    data_val.battery_heater_2_mode = generate_random(0, UINT8_MAX);
    read_reg(47, (uint32_t)data_val.battery_heater_2_mode);

    sl_eps2_data_t data_res = {0};

    assert_return_code(sl_eps2_read_data(conf, &data_res), 0);

    assert_int_equal(data_val.time_counter,                     data_res.time_counter);
    assert_int_equal(data_val.temperature_uc,                   data_res.temperature_uc);
    assert_int_equal(data_val.current,                          data_res.current);
    assert_int_equal(data_val.last_reset_cause,                 data_res.last_reset_cause);
    assert_int_equal(data_val.reset_counter,                    data_res.reset_counter);
    assert_int_equal(data_val.solar_panel_voltage_my_px,        data_res.solar_panel_voltage_my_px);
    assert_int_equal(data_val.solar_panel_voltage_mx_pz,        data_res.solar_panel_voltage_mx_pz);
    assert_int_equal(data_val.solar_panel_voltage_mz_py,        data_res.solar_panel_voltage_mz_py);
    assert_int_equal(data_val.solar_panel_current_my,           data_res.solar_panel_current_my);
    assert_int_equal(data_val.solar_panel_current_py,           data_res.solar_panel_current_py);
    assert_int_equal(data_val.solar_panel_current_mx,           data_res.solar_panel_current_mx);
    assert_int_equal(data_val.solar_panel_current_px,           data_res.solar_panel_current_px);
    assert_int_equal(data_val.solar_panel_current_mz,           data_res.solar_panel_current_mz);
    assert_int_equal(data_val.solar_panel_current_pz,           data_res.solar_panel_current_pz);
    assert_int_equal(data_val.mppt_1_duty_cycle,                data_res.mppt_1_duty_cycle);
    assert_int_equal(data_val.mppt_2_duty_cycle,                data_res.mppt_2_duty_cycle);
    assert_int_equal(data_val.mppt_3_duty_cycle,                data_res.mppt_3_duty_cycle);
    assert_int_equal(data_val.solar_panel_output_voltage,       data_res.solar_panel_output_voltage);
    assert_int_equal(data_val.main_power_bus_voltage,           data_res.main_power_bus_voltage);
    assert_int_equal(data_val.rtd_0_temperature,                data_res.rtd_0_temperature);
    assert_int_equal(data_val.rtd_1_temperature,                data_res.rtd_1_temperature);
    assert_int_equal(data_val.rtd_2_temperature,                data_res.rtd_2_temperature);
    assert_int_equal(data_val.rtd_3_temperature,                data_res.rtd_3_temperature);
    assert_int_equal(data_val.rtd_4_temperature,                data_res.rtd_4_temperature);
    assert_int_equal(data_val.rtd_5_temperature,                data_res.rtd_5_temperature);
    assert_int_equal(data_val.rtd_6_temperature,                data_res.rtd_6_temperature);
    assert_int_equal(data_val.battery_voltage,                  data_res.battery_voltage);
    assert_int_equal(data_val.battery_current,                  data_res.battery_current);
    assert_int_equal(data_val.battery_average_current,          data_res.battery_average_current);
    assert_int_equal(data_val.battery_acc_current,              data_res.battery_acc_current);
    assert_int_equal(data_val.battery_charge,                   data_res.battery_charge);
    assert_int_equal(data_val.battery_monitor_temperature,      data_res.battery_monitor_temperature);
    assert_int_equal(data_val.battery_monitor_status,           data_res.battery_monitor_status);
    assert_int_equal(data_val.battery_monitor_protection,       data_res.battery_monitor_protection);
    assert_int_equal(data_val.battery_monitor_cycle_counter,    data_res.battery_monitor_cycle_counter);
    assert_int_equal(data_val.raac,                             data_res.raac);
    assert_int_equal(data_val.rsac,                             data_res.rsac);
    assert_int_equal(data_val.rarc,                             data_res.rarc);
    assert_int_equal(data_val.rsrc,                             data_res.rsrc);
    assert_int_equal(data_val.battery_heater_1_duty_cycle,      data_res.battery_heater_1_duty_cycle);
    assert_int_equal(data_val.battery_heater_2_duty_cycle,      data_res.battery_heater_2_duty_cycle);
    assert_int_equal(data_val.mppt_1_mode,                      data_res.mppt_1_mode);
    assert_int_equal(data_val.mppt_2_mode,                      data_res.mppt_2_mode);
    assert_int_equal(data_val.mppt_3_mode,                      data_res.mppt_3_mode);
    assert_int_equal(data_val.battery_heater_1_mode,            data_res.battery_heater_1_mode);
    assert_int_equal(data_val.battery_heater_2_mode,            data_res.battery_heater_2_mode);
}

static void sl_eps2_read_time_counter_test(void **state)
{
    uint32_t val = generate_random(0, UINT32_MAX-1);

    read_reg(0, val);

    uint32_t res = UINT32_MAX;

    assert_return_code(sl_eps2_read_time_counter(conf, &res), 0);

    assert_int_equal(val, res);
}

static void sl_eps2_read_temp_test(void **state)
{
    uint16_t val = generate_random(0, UINT16_MAX);

    read_reg(1, (uint32_t)val);

    sl_eps2_temp_t res = UINT16_MAX;

    assert_return_code(sl_eps2_read_temp(conf, &res), 0);

    assert_int_equal(val, res);
}

static void sl_eps2_read_current_test(void **state)
{
    uint16_t val = generate_random(0, UINT16_MAX);

    read_reg(2, (uint32_t)val);

    sl_eps2_current_t res = UINT16_MAX;

    assert_return_code(sl_eps2_read_current(conf, &res), 0);

    assert_int_equal(val, res);
}

static void sl_eps2_read_reset_cause_test(void **state)
{
    uint8_t val = generate_random(0, UINT8_MAX);

    read_reg(3, (uint32_t)val);

    uint8_t res = UINT8_MAX;

    assert_return_code(sl_eps2_read_reset_cause(conf, &res), 0);

    assert_int_equal(val, res);
}

static void sl_eps2_read_reset_counter_test(void **state)
{
    uint16_t val = generate_random(0, UINT16_MAX);

    read_reg(4, (uint32_t)val);

    uint16_t res = UINT16_MAX;

    assert_return_code(sl_eps2_read_reset_counter(conf, &res), 0);

    assert_int_equal(val, res);
}

static void sl_eps2_read_solar_panel_voltage_test(void **state)
{
    uint8_t sp = 0;
    for(sp=0; sp<UINT8_MAX; sp++)
    {
        uint16_t val = generate_random(0, UINT16_MAX);
        uint16_t res = UINT16_MAX;

        switch(sp)
        {
            case SL_EPS2_SOLAR_PANEL_3_0:   read_reg(5, (uint32_t)val);     break;
            case SL_EPS2_SOLAR_PANEL_1_4:   read_reg(6, (uint32_t)val);     break;
            case SL_EPS2_SOLAR_PANEL_5_2:   read_reg(7, (uint32_t)val);     break;
            case SL_EPS2_SOLAR_PANEL_ALL:   read_reg(17, (uint32_t)val);    break;
            default:
                assert_int_equal(sl_eps2_read_solar_panel_voltage(conf, sp, &res), -1);

                continue;
        }

        assert_return_code(sl_eps2_read_solar_panel_voltage(conf, sp, &res), 0);

        assert_int_equal(val, res);
    }
}

static void sl_eps2_read_solar_panel_current_test(void **state)
{
    uint8_t sp = 0;
    for(sp=0; sp<UINT8_MAX; sp++)
    {
        sl_eps2_current_t val = generate_random(0, UINT16_MAX);
        sl_eps2_current_t res = UINT16_MAX;

        switch(sp)
        {
            case SL_EPS2_SOLAR_PANEL_0:     read_reg(8, (uint32_t)val);     break;
            case SL_EPS2_SOLAR_PANEL_1:     read_reg(9, (uint32_t)val);     break;
            case SL_EPS2_SOLAR_PANEL_2:     read_reg(10, (uint32_t)val);    break;
            case SL_EPS2_SOLAR_PANEL_3:     read_reg(11, (uint32_t)val);    break;
            case SL_EPS2_SOLAR_PANEL_4:     read_reg(12, (uint32_t)val);    break;
            case SL_EPS2_SOLAR_PANEL_5:     read_reg(13, (uint32_t)val);    break;
            default:
                assert_int_equal(sl_eps2_read_solar_panel_current(conf, sp, &res), -1);

                continue;
        }

        assert_return_code(sl_eps2_read_solar_panel_current(conf, sp, &res), 0);

        assert_int_equal(val, res);
    }
}

static void sl_eps2_read_mppt_duty_cycle_test(void **state)
{
    uint8_t mppt = 0;
    for(mppt=0; mppt<UINT8_MAX; mppt++)
    {
        sl_eps2_duty_cycle_t val = generate_random(0, UINT8_MAX);
        sl_eps2_duty_cycle_t res = UINT8_MAX;

        switch(mppt)
        {
            case SL_EPS2_MPPT_1:    read_reg(14, (uint32_t)val);    break;
            case SL_EPS2_MPPT_2:    read_reg(15, (uint32_t)val);    break;
            case SL_EPS2_MPPT_3:    read_reg(16, (uint32_t)val);    break;
            default:
                assert_int_equal(sl_eps2_read_mppt_duty_cycle(conf, mppt, &res), -1);

                continue;
        }

        assert_return_code(sl_eps2_read_mppt_duty_cycle(conf, mppt, &res), 0);

        assert_int_equal(val, res);
    }
}

static void sl_eps2_read_main_bus_voltage_test(void **state)
{
    sl_eps2_voltage_t val = generate_random(0, UINT16_MAX);

    read_reg(18, (uint32_t)val);

    sl_eps2_voltage_t res = UINT16_MAX;

    assert_return_code(sl_eps2_read_main_bus_voltage(conf, &res), 0);

    assert_int_equal(val, res);
}

static void sl_eps2_read_rtd_temperature_test(void **state)
{
    uint8_t rtd = 0;
    for(rtd=0; rtd<UINT8_MAX; rtd++)
    {
        sl_eps2_temp_t val = generate_random(0, UINT16_MAX);
        sl_eps2_temp_t res = UINT16_MAX;

        switch(rtd)
        {
            case SL_EPS2_RTD_0:     read_reg(19, (uint32_t)val);    break;
            case SL_EPS2_RTD_1:     read_reg(20, (uint32_t)val);    break;
            case SL_EPS2_RTD_2:     read_reg(21, (uint32_t)val);    break;
            case SL_EPS2_RTD_3:     read_reg(22, (uint32_t)val);    break;
            case SL_EPS2_RTD_4:     read_reg(23, (uint32_t)val);    break;
            case SL_EPS2_RTD_5:     read_reg(24, (uint32_t)val);    break;
            case SL_EPS2_RTD_6:     read_reg(25, (uint32_t)val);    break;
            default:
                assert_int_equal(sl_eps2_read_rtd_temperature(conf, rtd, &res), -1);

                continue;
        }

        assert_return_code(sl_eps2_read_rtd_temperature(conf, rtd, &res), 0);

        assert_int_equal(val, res);
    }
}

static void sl_eps2_read_battery_voltage_test(void **state)
{
    sl_eps2_voltage_t val = generate_random(0, UINT16_MAX);

    read_reg(26, (uint32_t)val);

    sl_eps2_voltage_t res = UINT16_MAX;

    assert_return_code(sl_eps2_read_battery_voltage(conf, &res), 0);

    assert_int_equal(val, res);
}

static void sl_eps2_read_battery_current_test(void **state)
{
    uint8_t cur = 0;
    for(cur=0; cur<UINT8_MAX; cur++)
    {
        sl_eps2_current_t val = generate_random(0, UINT16_MAX);
        sl_eps2_current_t res = UINT16_MAX;

        switch(cur)
        {
            case SL_EPS2_BATTERY_CURRENT:           read_reg(27, (uint32_t)val);    break;
            case SL_EPS2_BATTERY_AVERAGE_CURRENT:   read_reg(28, (uint32_t)val);    break;
            case SL_EPS2_BATTERY_ACC_CURRENT:       read_reg(29, (uint32_t)val);    break;
            default:
                assert_int_equal(sl_eps2_read_battery_current(conf, cur, &res), -1);

                continue;
        }

        assert_return_code(sl_eps2_read_battery_current(conf, cur, &res), 0);

        assert_int_equal(val, res);
    }
}

static void sl_eps2_read_battery_charge_test(void **state)
{
    sl_eps2_charge_t val = generate_random(0, UINT16_MAX);

    read_reg(30, (uint32_t)val);

    sl_eps2_charge_t res = UINT16_MAX;

    assert_return_code(sl_eps2_read_battery_charge(conf, &res), 0);

    assert_int_equal(val, res);
}

static void sl_eps2_read_battery_monitor_temp_test(void **state)
{
    sl_eps2_temp_t val = generate_random(0, UINT16_MAX);

    read_reg(31, (uint32_t)val);

    sl_eps2_temp_t res = UINT16_MAX;

    assert_return_code(sl_eps2_read_battery_monitor_temp(conf, &res), 0);

    assert_int_equal(val, res);
}

static void sl_eps2_read_battery_monitor_status_test(void **state)
{
    uint8_t val = generate_random(0, UINT8_MAX);

    read_reg(32, (uint32_t)val);

    uint8_t res = UINT8_MAX;

    assert_return_code(sl_eps2_read_battery_monitor_status(conf, &res), 0);

    assert_int_equal(val, res);
}

static void sl_eps2_read_battery_monitor_protection_test(void **state)
{
    uint8_t val = generate_random(0, UINT8_MAX);

    read_reg(33, (uint32_t)val);

    uint8_t res = UINT8_MAX;

    assert_return_code(sl_eps2_read_battery_monitor_protection(conf, &res), 0);

    assert_int_equal(val, res);
}

static void sl_eps2_read_battery_monitor_cycle_counter_test(void **state)
{
    uint8_t val = generate_random(0, UINT8_MAX);

    read_reg(34, (uint32_t)val);

    uint8_t res = UINT8_MAX;

    assert_return_code(sl_eps2_read_battery_monitor_cycle_counter(conf, &res), 0);

    assert_int_equal(val, res);
}

static void sl_eps2_read_battery_monitor_raac_test(void **state)
{
    sl_eps2_charge_t val = generate_random(0, UINT16_MAX);

    read_reg(35, (uint32_t)val);

    sl_eps2_charge_t res = UINT16_MAX;

    assert_return_code(sl_eps2_read_battery_monitor_raac(conf, &res), 0);

    assert_int_equal(val, res);
}

static void sl_eps2_read_battery_monitor_rsac_test(void **state)
{
    sl_eps2_charge_t val = generate_random(0, UINT16_MAX);

    read_reg(36, (uint32_t)val);

    sl_eps2_charge_t res = UINT16_MAX;

    assert_return_code(sl_eps2_read_battery_monitor_rsac(conf, &res), 0);

    assert_int_equal(val, res);
}

static void sl_eps2_read_battery_monitor_rarc_test(void **state)
{
    uint8_t val = generate_random(0, UINT8_MAX);

    read_reg(37, (uint32_t)val);

    uint8_t res = UINT8_MAX;

    assert_return_code(sl_eps2_read_battery_monitor_rarc(conf, &res), 0);

    assert_int_equal(val, res);
}

static void sl_eps2_read_battery_monitor_rsrc_test(void **state)
{
    uint8_t val = generate_random(0, UINT8_MAX);

    read_reg(38, (uint32_t)val);

    uint8_t res = UINT8_MAX;

    assert_return_code(sl_eps2_read_battery_monitor_rsrc(conf, &res), 0);

    assert_int_equal(val, res);
}

static void sl_eps2_read_heater_duty_cycle_test(void **state)
{
    uint8_t heater = 0;
    for(heater=0; heater<UINT8_MAX; heater++)
    {
        sl_eps2_duty_cycle_t val = generate_random(0, UINT8_MAX);
        sl_eps2_duty_cycle_t res = UINT8_MAX;

        switch(heater)
        {
            case SL_EPS2_HEATER_1:  read_reg(39, (uint32_t)val); break;
            case SL_EPS2_HEATER_2:  read_reg(40, (uint32_t)val); break;
            default:
                assert_int_equal(sl_eps2_read_heater_duty_cycle(conf, heater, &res), -1);

                continue;
        }

        assert_return_code(sl_eps2_read_heater_duty_cycle(conf, heater, &res), 0);

        assert_int_equal(val, res);
    }
}

static void sl_eps2_read_hardware_version_test(void **state)
{
    uint8_t val = generate_random(0, UINT8_MAX);

    read_reg(41, (uint32_t)val);

    uint8_t res = UINT8_MAX;

    assert_return_code(sl_eps2_read_hardware_version(conf, &res), 0);

    assert_int_equal(val, res);
}

static void sl_eps2_read_firmware_version_test(void **state)
{
    uint32_t val = generate_random(0, UINT32_MAX-1);

    read_reg(42, (uint32_t)val);

    uint32_t res = UINT32_MAX;

    assert_return_code(sl_eps2_read_firmware_version(conf, &res), 0);

    assert_int_equal(val, res);
}

static void sl_eps2_set_mppt_mode_test(void **state)
{
    uint8_t mppt = 0;
    for(mppt=0; mppt<UINT8_MAX; mppt++)
    {
        uint8_t mode = generate_random(SL_EPS2_MPPT_MODE_AUTOMATIC, SL_EPS2_MPPT_MODE_MANUAL);
        uint8_t data[8] = {UINT8_MAX};

        data[1] = 0;
        data[2] = 0;
        data[3] = 0;
        data[4] = mode;

        switch(mppt)
        {
            case SL_EPS2_MPPT_1:
                data[0] = 43;

                break;
            case SL_EPS2_MPPT_2:
                data[0] = 44;

                break;
            case SL_EPS2_MPPT_3:
                data[0] = 45;

                break;
            default:
                assert_int_equal(sl_eps2_set_mppt_mode(conf, mppt, mode), -1);

                continue;
        }

        data[5] = crc8(data, 5);

        expect_value(__wrap_i2c_write, port, SL_EPS2_I2C_PORT);
        expect_value(__wrap_i2c_write, adr, SL_EPS2_I2C_ADR);
        expect_memory(__wrap_i2c_write, data, (void*)data, 6);
        expect_value(__wrap_i2c_write, len, 6);

        will_return(__wrap_i2c_write, 0);

        /* Get state */
        expect_value(__wrap_gpio_get_state, pin, SL_EPS2_I2C_RDY_PIN);

        will_return(__wrap_gpio_get_state, 1);

        assert_return_code(sl_eps2_set_mppt_mode(conf, mppt, mode), 0);
    }
}

static void sl_eps2_get_mppt_mode_test(void **state)
{
    uint8_t mppt = 0;
    for(mppt=0; mppt<UINT8_MAX; mppt++)
    {
        uint8_t val = generate_random(0, UINT8_MAX);
        uint8_t res = UINT8_MAX;

        switch(mppt)
        {
            case SL_EPS2_MPPT_1:    read_reg(43, (uint32_t)val);    break;
            case SL_EPS2_MPPT_2:    read_reg(44, (uint32_t)val);    break;
            case SL_EPS2_MPPT_3:    read_reg(45, (uint32_t)val);    break;
            default:
                assert_int_equal(sl_eps2_get_mppt_mode(conf, mppt, &res), -1);

                continue;
        }

        assert_return_code(sl_eps2_get_mppt_mode(conf, mppt, &res), 0);

        assert_int_equal(val, res);
    }
}

static void sl_eps2_set_heater_mode_test(void **state)
{
    uint8_t heater = 0;
    for(heater=0; heater<UINT8_MAX; heater++)
    {
        uint8_t mode = generate_random(SL_EPS2_HEATER_MODE_AUTOMATIC, SL_EPS2_HEATER_MODE_MANUAL);
        uint8_t data[8] = {UINT8_MAX};

        data[1] = 0;
        data[2] = 0;
        data[3] = 0;
        data[4] = mode;

        switch(heater)
        {
            case SL_EPS2_HEATER_1:
                data[0] = 46;

                break;
            case SL_EPS2_HEATER_2:
                data[0] = 47;

                break;
            default:
                assert_int_equal(sl_eps2_set_heater_mode(conf, heater, mode), -1);

                continue;
        }

        data[5] = crc8(data, 5);

        expect_value(__wrap_i2c_write, port, SL_EPS2_I2C_PORT);
        expect_value(__wrap_i2c_write, adr, SL_EPS2_I2C_ADR);
        expect_memory(__wrap_i2c_write, data, (void*)data, 6);
        expect_value(__wrap_i2c_write, len, 6);

        will_return(__wrap_i2c_write, 0);

        /* Get state */
        expect_value(__wrap_gpio_get_state, pin, SL_EPS2_I2C_RDY_PIN);

        will_return(__wrap_gpio_get_state, 1);

        assert_return_code(sl_eps2_set_heater_mode(conf, heater, mode), 0);
    }
}

static void sl_eps2_get_heater_mode_test(void **state)
{
    uint8_t heater = 0;
    for(heater=0; heater<UINT8_MAX; heater++)
    {
        uint8_t val = generate_random(0, UINT8_MAX);
        uint8_t res = UINT8_MAX;

        switch(heater)
        {
            case SL_EPS2_HEATER_1:  read_reg(46, (uint32_t)val);    break;
            case SL_EPS2_HEATER_2:  read_reg(47, (uint32_t)val);    break;
            default:
                assert_int_equal(sl_eps2_get_heater_mode(conf, heater, &res), -1);

                continue;
        }

        assert_return_code(sl_eps2_get_heater_mode(conf, heater, &res), 0);

        assert_int_equal(val, res);
    }
}

int main(void)
{
    conf.i2c_port               = SL_EPS2_I2C_PORT;
    conf.i2c_config.speed_hz    = SL_EPS2_I2C_CLOCK_HZ;
    conf.en_pin                 = SL_EPS2_I2C_EN_PIN;
    conf.ready_pin              = SL_EPS2_I2C_RDY_PIN;

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
    };

    return cmocka_run_group_tests(sl_eps2_tests, NULL, NULL);
}

unsigned int generate_random(unsigned int l, unsigned int r)
{
    return (rand() % (r - l + 1)) + l;
}

uint8_t crc8(uint8_t *data, uint8_t len)
{
    uint8_t crc = 0;

    while(len--)
    {
        crc ^= *data++;

        uint8_t j = 0;
        for (j=0; j<8; j++)
        {
            crc = (crc << 1) ^ ((crc & 0x80)? 0x07 : 0);
        }

        crc &= 0xFF;
    }

    return crc;
}

void read_reg(uint8_t adr, uint32_t val)
{
    uint8_t data[256] = {UINT8_MAX};

    data[0] = adr;
    data[1] = crc8(data, 1);

    /* I2C write */
    expect_value(__wrap_i2c_write, port, SL_EPS2_I2C_PORT);
    expect_value(__wrap_i2c_write, adr, SL_EPS2_I2C_ADR);
    expect_memory(__wrap_i2c_write, data, (void*)data, 2);
    expect_value(__wrap_i2c_write, len, 2);

    will_return(__wrap_i2c_write, 0);

    /* Get state */
    expect_value(__wrap_gpio_get_state, pin, SL_EPS2_I2C_RDY_PIN);

    will_return(__wrap_gpio_get_state, 1);

    /* I2C read */
    expect_value(__wrap_i2c_read, port, SL_EPS2_I2C_PORT);
    expect_value(__wrap_i2c_read, adr, SL_EPS2_I2C_ADR);
    expect_value(__wrap_i2c_read, len, 5);

    data[0] = (val >> 24) & 0xFF;
    data[1] = (val >> 16) & 0xFF;
    data[2] = (val >> 8) & 0xFF;
    data[3] = (val >> 0) & 0xFF;
    data[4] = crc8(data, 4);

    uint16_t i = 0;
    for(i=0; i<5; i++)
    {
        will_return(__wrap_i2c_read, data[i]);
    }

    will_return(__wrap_i2c_read, 0);

    /* Get state */
    expect_value(__wrap_gpio_get_state, pin, SL_EPS2_I2C_RDY_PIN);

    will_return(__wrap_gpio_get_state, 1);
}

/** \} End of sl_eps2_test group */
