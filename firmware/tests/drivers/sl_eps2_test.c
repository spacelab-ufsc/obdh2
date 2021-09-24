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
}

static void sl_eps2_read_mppt_duty_cycle_test(void **state)
{
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
}

static void sl_eps2_read_battery_voltage_test(void **state)
{
}

static void sl_eps2_read_battery_current_test(void **state)
{
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
