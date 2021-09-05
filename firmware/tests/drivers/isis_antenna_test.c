/*
 * isis_antenna_test.c
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
 * \brief Unit test of the ISIS Antenna driver.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.12
 * 
 * \date 2021/09/01
 * 
 * \defgroup isis_antenna_unit_test ISIS Antenna
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
#include <drivers/isis_antenna/isis_antenna.h>

#define ISIS_ANTENNA_IIC_PORT       I2C_PORT_2
#define ISIS_ANTENNA_IIC_CLOCK_HZ   100000UL
#define ISIS_ANTENNA_IIC_ADR        0x31

unsigned int generate_random(unsigned int l, unsigned int r);

static void isis_antenna_init_test(void **state)
{
    expect_value(__wrap_i2c_init, port, ISIS_ANTENNA_IIC_PORT);
    expect_value(__wrap_i2c_init, config.speed_hz, ISIS_ANTENNA_IIC_CLOCK_HZ);

    will_return(__wrap_i2c_init, 0);

    assert_return_code(isis_antenna_init(), 0);
}

static void isis_antenna_arm_test(void **state)
{
    uint8_t cmd = 0xAD;

    expect_value(__wrap_i2c_write, port, ISIS_ANTENNA_IIC_PORT);
    expect_value(__wrap_i2c_write, adr, ISIS_ANTENNA_IIC_ADR);
    expect_memory(__wrap_i2c_write, data, (void*)&cmd, 1);
    expect_value(__wrap_i2c_write, len, 1);

    will_return(__wrap_i2c_write, 0);

    cmd = 0xC3;

    expect_value(__wrap_i2c_write, port, ISIS_ANTENNA_IIC_PORT);
    expect_value(__wrap_i2c_write, adr, ISIS_ANTENNA_IIC_ADR);
    expect_memory(__wrap_i2c_write, data, (void*)&cmd, 1);
    expect_value(__wrap_i2c_write, len, 1);

    will_return(__wrap_i2c_write, 0);

    expect_value(__wrap_i2c_read, port, ISIS_ANTENNA_IIC_PORT);
    expect_value(__wrap_i2c_read, adr, ISIS_ANTENNA_IIC_ADR);
    expect_value(__wrap_i2c_read, len, 2);

    will_return(__wrap_i2c_read, 0x01);
    will_return(__wrap_i2c_read, 0x00);

    will_return(__wrap_i2c_read, 0);

    assert_return_code(isis_antenna_arm(), 0);
}

static void isis_antenna_disarm_test(void **state)
{
    uint8_t cmd = 0xAC;

    expect_value(__wrap_i2c_write, port, ISIS_ANTENNA_IIC_PORT);
    expect_value(__wrap_i2c_write, adr, ISIS_ANTENNA_IIC_ADR);
    expect_memory(__wrap_i2c_write, data, (void*)&cmd, 1);
    expect_value(__wrap_i2c_write, len, 1);

    will_return(__wrap_i2c_write, 0);

    cmd = 0xC3;

    expect_value(__wrap_i2c_write, port, ISIS_ANTENNA_IIC_PORT);
    expect_value(__wrap_i2c_write, adr, ISIS_ANTENNA_IIC_ADR);
    expect_memory(__wrap_i2c_write, data, (void*)&cmd, 1);
    expect_value(__wrap_i2c_write, len, 1);

    will_return(__wrap_i2c_write, 0);

    expect_value(__wrap_i2c_read, port, ISIS_ANTENNA_IIC_PORT);
    expect_value(__wrap_i2c_read, adr, ISIS_ANTENNA_IIC_ADR);
    expect_value(__wrap_i2c_read, len, 2);

    will_return(__wrap_i2c_read, 0x00);
    will_return(__wrap_i2c_read, 0x00);

    will_return(__wrap_i2c_read, 0);

    assert_return_code(isis_antenna_disarm(), 0);
}

static void isis_antenna_start_sequential_deploy_test(void **state)
{
    uint8_t sec = generate_random(1, 255);

    uint8_t cmd[2] = {0xA5, sec};

    expect_value(__wrap_i2c_write, port, ISIS_ANTENNA_IIC_PORT);
    expect_value(__wrap_i2c_write, adr, ISIS_ANTENNA_IIC_ADR);
    expect_memory(__wrap_i2c_write, data, (void*)cmd, 2);
    expect_value(__wrap_i2c_write, len, 2);

    will_return(__wrap_i2c_write, 0);

    assert_return_code(isis_antenna_start_sequential_deploy(sec), 0);
}

static void isis_antenna_start_independent_deploy_test(void **state)
{
    uint8_t sec = generate_random(1, 255);

    /* Antenna 1 with independent deploy */
    uint8_t cmd[2] = {0xBA, sec};

    expect_value(__wrap_i2c_write, port, ISIS_ANTENNA_IIC_PORT);
    expect_value(__wrap_i2c_write, adr, ISIS_ANTENNA_IIC_ADR);
    expect_memory(__wrap_i2c_write, data, (void*)cmd, 2);
    expect_value(__wrap_i2c_write, len, 2);

    will_return(__wrap_i2c_write, 0);

    assert_return_code(isis_antenna_start_independent_deploy(ISIS_ANTENNA_ANT_1, sec, ISIS_ANTENNA_INDEPENDENT_DEPLOY_WITH_OVERRIDE), 0);

    /* Antenna 2 with independent deploy */
    cmd[0] = 0xBB;

    expect_value(__wrap_i2c_write, port, ISIS_ANTENNA_IIC_PORT);
    expect_value(__wrap_i2c_write, adr, ISIS_ANTENNA_IIC_ADR);
    expect_memory(__wrap_i2c_write, data, (void*)cmd, 2);
    expect_value(__wrap_i2c_write, len, 2);

    will_return(__wrap_i2c_write, 0);

    assert_return_code(isis_antenna_start_independent_deploy(ISIS_ANTENNA_ANT_2, sec, ISIS_ANTENNA_INDEPENDENT_DEPLOY_WITH_OVERRIDE), 0);

    /* Antenna 3 with independent deploy */
    cmd[0] = 0xBC;

    expect_value(__wrap_i2c_write, port, ISIS_ANTENNA_IIC_PORT);
    expect_value(__wrap_i2c_write, adr, ISIS_ANTENNA_IIC_ADR);
    expect_memory(__wrap_i2c_write, data, (void*)cmd, 2);
    expect_value(__wrap_i2c_write, len, 2);

    will_return(__wrap_i2c_write, 0);

    assert_return_code(isis_antenna_start_independent_deploy(ISIS_ANTENNA_ANT_3, sec, ISIS_ANTENNA_INDEPENDENT_DEPLOY_WITH_OVERRIDE), 0);

    /* Antenna 4 with independent deploy */
    cmd[0] = 0xBD;

    expect_value(__wrap_i2c_write, port, ISIS_ANTENNA_IIC_PORT);
    expect_value(__wrap_i2c_write, adr, ISIS_ANTENNA_IIC_ADR);
    expect_memory(__wrap_i2c_write, data, (void*)cmd, 2);
    expect_value(__wrap_i2c_write, len, 2);

    will_return(__wrap_i2c_write, 0);

    assert_return_code(isis_antenna_start_independent_deploy(ISIS_ANTENNA_ANT_4, sec, ISIS_ANTENNA_INDEPENDENT_DEPLOY_WITH_OVERRIDE), 0);
}

static void isis_antenna_read_deployment_status_code_test(void **state)
{
}

static void isis_antenna_read_deployment_status_test(void **state)
{
}

static void isis_antenna_get_antenna_status_test(void **state)
{
}

static void isis_antenna_get_antenna_timeout_test(void **state)
{
}

static void isis_antenna_get_burning_test(void **state)
{
}

static void isis_antenna_get_arming_status_test(void **state)
{
}

static void isis_antenna_get_raw_temperature_test(void **state)
{
}

static void isis_antenna_raw_to_temp_c_test(void **state)
{
}

static void isis_antenna_get_temperature_c_test(void **state)
{
}

static void isis_antenna_delay_s_test(void **state)
{
}

static void isis_antenna_delay_ms_test(void **state)
{
}

int main(void)
{
    const struct CMUnitTest isis_antenna_tests[] = {
        cmocka_unit_test(isis_antenna_init_test),
        cmocka_unit_test(isis_antenna_arm_test),
        cmocka_unit_test(isis_antenna_disarm_test),
        cmocka_unit_test(isis_antenna_start_sequential_deploy_test),
        cmocka_unit_test(isis_antenna_start_independent_deploy_test),
        cmocka_unit_test(isis_antenna_read_deployment_status_code_test),
        cmocka_unit_test(isis_antenna_read_deployment_status_test),
        cmocka_unit_test(isis_antenna_get_antenna_status_test),
        cmocka_unit_test(isis_antenna_get_antenna_timeout_test),
        cmocka_unit_test(isis_antenna_get_burning_test),
        cmocka_unit_test(isis_antenna_get_arming_status_test),
        cmocka_unit_test(isis_antenna_get_raw_temperature_test),
        cmocka_unit_test(isis_antenna_raw_to_temp_c_test),
        cmocka_unit_test(isis_antenna_get_temperature_c_test),
        cmocka_unit_test(isis_antenna_delay_s_test),
        cmocka_unit_test(isis_antenna_delay_ms_test),
    };

    return cmocka_run_group_tests(isis_antenna_tests, NULL, NULL);
}

unsigned int generate_random(unsigned int l, unsigned int r)
{
    return (rand() % (r - l + 1)) + l;
}

/** \} End of isis_antenna_test group */
