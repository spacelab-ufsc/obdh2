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
 * \version 0.7.45
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

    will_return(__wrap_i2c_read, 0x00);
    will_return(__wrap_i2c_read, 0x01);

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
    uint8_t cmd = 0xC3;

    expect_value(__wrap_i2c_write, port, ISIS_ANTENNA_IIC_PORT);
    expect_value(__wrap_i2c_write, adr, ISIS_ANTENNA_IIC_ADR);
    expect_memory(__wrap_i2c_write, data, (void*)&cmd, 1);
    expect_value(__wrap_i2c_write, len, 1);

    will_return(__wrap_i2c_write, 0);

    expect_value(__wrap_i2c_read, port, ISIS_ANTENNA_IIC_PORT);
    expect_value(__wrap_i2c_read, adr, ISIS_ANTENNA_IIC_ADR);
    expect_value(__wrap_i2c_read, len, 2);

    uint16_t raw_status = generate_random(0, UINT16_MAX);

    will_return(__wrap_i2c_read, (uint8_t)(raw_status >> 8));
    will_return(__wrap_i2c_read, (uint8_t)(raw_status & 0xFF));

    will_return(__wrap_i2c_read, 0);

    uint16_t status = UINT16_MAX;

    assert_return_code(isis_antenna_read_deployment_status_code(&status), 0);

    assert_int_equal(raw_status, status);
}

static void isis_antenna_read_deployment_status_test(void **state)
{
    uint8_t cmd = 0xC3;

    expect_value(__wrap_i2c_write, port, ISIS_ANTENNA_IIC_PORT);
    expect_value(__wrap_i2c_write, adr, ISIS_ANTENNA_IIC_ADR);
    expect_memory(__wrap_i2c_write, data, (void*)&cmd, 1);
    expect_value(__wrap_i2c_write, len, 1);

    will_return(__wrap_i2c_write, 0);

    expect_value(__wrap_i2c_read, port, ISIS_ANTENNA_IIC_PORT);
    expect_value(__wrap_i2c_read, adr, ISIS_ANTENNA_IIC_ADR);
    expect_value(__wrap_i2c_read, len, 2);

    uint16_t raw_status = generate_random(0, UINT16_MAX);

    will_return(__wrap_i2c_read, (uint8_t)(raw_status >> 8));
    will_return(__wrap_i2c_read, (uint8_t)(raw_status & 0xFF));

    will_return(__wrap_i2c_read, 0);

    isis_antenna_status_t status = {0};

    assert_return_code(isis_antenna_read_deployment_status(&status), 0);

    assert_int_equal(raw_status, status.code);
    assert_int_equal((raw_status >> 15) & 1, status.antenna_1.status);
    assert_int_equal((raw_status >> 14) & 1, status.antenna_1.timeout);
    assert_int_equal((raw_status >> 13) & 1, status.antenna_1.burning);
    assert_int_equal((raw_status >> 11) & 1, status.antenna_2.status);
    assert_int_equal((raw_status >> 10) & 1, status.antenna_2.timeout);
    assert_int_equal((raw_status >> 9) & 1, status.antenna_2.burning);
    assert_int_equal((raw_status >> 8) & 1, status.ignoring_switches);
    assert_int_equal((raw_status >> 7) & 1, status.antenna_3.status);
    assert_int_equal((raw_status >> 6) & 1, status.antenna_3.timeout);
    assert_int_equal((raw_status >> 5) & 1, status.antenna_3.burning);
    assert_int_equal((raw_status >> 4) & 1, status.independent_burn);
    assert_int_equal((raw_status >> 3) & 1, status.antenna_4.status);
    assert_int_equal((raw_status >> 2) & 1, status.antenna_4.timeout);
    assert_int_equal((raw_status >> 1) & 1, status.antenna_4.burning);
    assert_int_equal((raw_status >> 0) & 1, status.armed);
}

static void isis_antenna_get_antenna_status_test(void **state)
{
    uint8_t cmd = 0xC3;

    uint8_t ant = 0;
    for(ant=0; ant<UINT8_MAX; ant++)
    {
        uint8_t ant_status = UINT8_MAX;

        switch(ant)
        {
            case ISIS_ANTENNA_ANT_1:    break;
            case ISIS_ANTENNA_ANT_2:    break;
            case ISIS_ANTENNA_ANT_3:    break;
            case ISIS_ANTENNA_ANT_4:    break;
            default:
                assert_int_equal(isis_antenna_get_antenna_status(ant, &ant_status), -1);
                continue;
        }

        expect_value(__wrap_i2c_write, port, ISIS_ANTENNA_IIC_PORT);
        expect_value(__wrap_i2c_write, adr, ISIS_ANTENNA_IIC_ADR);
        expect_memory(__wrap_i2c_write, data, (void*)&cmd, 1);
        expect_value(__wrap_i2c_write, len, 1);

        will_return(__wrap_i2c_write, 0);

        expect_value(__wrap_i2c_read, port, ISIS_ANTENNA_IIC_PORT);
        expect_value(__wrap_i2c_read, adr, ISIS_ANTENNA_IIC_ADR);
        expect_value(__wrap_i2c_read, len, 2);

        uint16_t raw_status = generate_random(0, UINT16_MAX);

        will_return(__wrap_i2c_read, (uint8_t)(raw_status >> 8));
        will_return(__wrap_i2c_read, (uint8_t)(raw_status & 0xFF));

        will_return(__wrap_i2c_read, 0);

        switch(ant)
        {
            case ISIS_ANTENNA_ANT_1:
                assert_return_code(isis_antenna_get_antenna_status(ant, &ant_status), 0);
                assert_int_equal((raw_status >> 15) & 1, ant_status);
                break;
            case ISIS_ANTENNA_ANT_2:
                assert_return_code(isis_antenna_get_antenna_status(ant, &ant_status), 0);
                assert_int_equal((raw_status >> 11) & 1, ant_status);
                break;
            case ISIS_ANTENNA_ANT_3:
                assert_return_code(isis_antenna_get_antenna_status(ant, &ant_status), 0);
                assert_int_equal((raw_status >> 7) & 1, ant_status);
                break;
            case ISIS_ANTENNA_ANT_4:
                assert_return_code(isis_antenna_get_antenna_status(ant, &ant_status), 0);
                assert_int_equal((raw_status >> 3) & 1, ant_status);
                break;
        }
    }
}

static void isis_antenna_get_antenna_timeout_test(void **state)
{
    uint8_t cmd = 0xC3;

    uint8_t ant = 0;
    for(ant=0; ant<UINT8_MAX; ant++)
    {
        uint8_t ant_timeout = UINT8_MAX;

        switch(ant)
        {
            case ISIS_ANTENNA_ANT_1:    break;
            case ISIS_ANTENNA_ANT_2:    break;
            case ISIS_ANTENNA_ANT_3:    break;
            case ISIS_ANTENNA_ANT_4:    break;
            default:
                assert_int_equal(isis_antenna_get_antenna_timeout(ant, &ant_timeout), -1);
                continue;
        }

        expect_value(__wrap_i2c_write, port, ISIS_ANTENNA_IIC_PORT);
        expect_value(__wrap_i2c_write, adr, ISIS_ANTENNA_IIC_ADR);
        expect_memory(__wrap_i2c_write, data, (void*)&cmd, 1);
        expect_value(__wrap_i2c_write, len, 1);

        will_return(__wrap_i2c_write, 0);

        expect_value(__wrap_i2c_read, port, ISIS_ANTENNA_IIC_PORT);
        expect_value(__wrap_i2c_read, adr, ISIS_ANTENNA_IIC_ADR);
        expect_value(__wrap_i2c_read, len, 2);

        uint16_t raw_status = generate_random(0, UINT16_MAX);

        will_return(__wrap_i2c_read, (uint8_t)(raw_status >> 8));
        will_return(__wrap_i2c_read, (uint8_t)(raw_status & 0xFF));

        will_return(__wrap_i2c_read, 0);

        switch(ant)
        {
            case ISIS_ANTENNA_ANT_1:
                assert_return_code(isis_antenna_get_antenna_timeout(ant, &ant_timeout), 0);
                assert_int_equal((raw_status >> 14) & 1, ant_timeout);
                break;
            case ISIS_ANTENNA_ANT_2:
                assert_return_code(isis_antenna_get_antenna_timeout(ant, &ant_timeout), 0);
                assert_int_equal((raw_status >> 10) & 1, ant_timeout);
                break;
            case ISIS_ANTENNA_ANT_3:
                assert_return_code(isis_antenna_get_antenna_timeout(ant, &ant_timeout), 0);
                assert_int_equal((raw_status >> 6) & 1, ant_timeout);
                break;
            case ISIS_ANTENNA_ANT_4:
                assert_return_code(isis_antenna_get_antenna_timeout(ant, &ant_timeout), 0);
                assert_int_equal((raw_status >> 2) & 1, ant_timeout);
                break;
        }
    }
}

static void isis_antenna_get_burning_test(void **state)
{
    uint8_t cmd = 0xC3;

    uint8_t ant = 0;
    for(ant=0; ant<UINT8_MAX; ant++)
    {
        uint8_t ant_burning = UINT8_MAX;

        switch(ant)
        {
            case ISIS_ANTENNA_ANT_1:    break;
            case ISIS_ANTENNA_ANT_2:    break;
            case ISIS_ANTENNA_ANT_3:    break;
            case ISIS_ANTENNA_ANT_4:    break;
            default:
                assert_int_equal(isis_antenna_get_burning(ant, &ant_burning), -1);
                continue;
        }

        expect_value(__wrap_i2c_write, port, ISIS_ANTENNA_IIC_PORT);
        expect_value(__wrap_i2c_write, adr, ISIS_ANTENNA_IIC_ADR);
        expect_memory(__wrap_i2c_write, data, (void*)&cmd, 1);
        expect_value(__wrap_i2c_write, len, 1);

        will_return(__wrap_i2c_write, 0);

        expect_value(__wrap_i2c_read, port, ISIS_ANTENNA_IIC_PORT);
        expect_value(__wrap_i2c_read, adr, ISIS_ANTENNA_IIC_ADR);
        expect_value(__wrap_i2c_read, len, 2);

        uint16_t raw_status = generate_random(0, UINT16_MAX);

        will_return(__wrap_i2c_read, (uint8_t)(raw_status >> 8));
        will_return(__wrap_i2c_read, (uint8_t)(raw_status & 0xFF));

        will_return(__wrap_i2c_read, 0);

        switch(ant)
        {
            case ISIS_ANTENNA_ANT_1:
                assert_return_code(isis_antenna_get_burning(ant, &ant_burning), 0);
                assert_int_equal((raw_status >> 13) & 1, ant_burning);
                break;
            case ISIS_ANTENNA_ANT_2:
                assert_return_code(isis_antenna_get_burning(ant, &ant_burning), 0);
                assert_int_equal((raw_status >> 9) & 1, ant_burning);
                break;
            case ISIS_ANTENNA_ANT_3:
                assert_return_code(isis_antenna_get_burning(ant, &ant_burning), 0);
                assert_int_equal((raw_status >> 5) & 1, ant_burning);
                break;
            case ISIS_ANTENNA_ANT_4:
                assert_return_code(isis_antenna_get_burning(ant, &ant_burning), 0);
                assert_int_equal((raw_status >> 1) & 1, ant_burning);
                break;
        }
    }
}

static void isis_antenna_get_arming_status_test(void **state)
{
    uint8_t cmd = 0xC3;

    expect_value(__wrap_i2c_write, port, ISIS_ANTENNA_IIC_PORT);
    expect_value(__wrap_i2c_write, adr, ISIS_ANTENNA_IIC_ADR);
    expect_memory(__wrap_i2c_write, data, (void*)&cmd, 1);
    expect_value(__wrap_i2c_write, len, 1);

    will_return(__wrap_i2c_write, 0);

    expect_value(__wrap_i2c_read, port, ISIS_ANTENNA_IIC_PORT);
    expect_value(__wrap_i2c_read, adr, ISIS_ANTENNA_IIC_ADR);
    expect_value(__wrap_i2c_read, len, 2);

    uint16_t raw_status = generate_random(0, UINT16_MAX);

    will_return(__wrap_i2c_read, (uint8_t)(raw_status >> 8));
    will_return(__wrap_i2c_read, (uint8_t)(raw_status & 0xFF));

    will_return(__wrap_i2c_read, 0);

    bool ant_armed = false;

    assert_return_code(isis_antenna_get_arming_status(&ant_armed), 0);
    assert_int_equal((raw_status >> 0) & 1, (int)ant_armed);
}

static void isis_antenna_get_raw_temperature_test(void **state)
{
    uint8_t cmd = 0xC0;

    uint16_t raw = 0;
    for(raw=0; raw<1023; raw+=50)
    {
        expect_value(__wrap_i2c_write, port, ISIS_ANTENNA_IIC_PORT);
        expect_value(__wrap_i2c_write, adr, ISIS_ANTENNA_IIC_ADR);
        expect_memory(__wrap_i2c_write, data, (void*)&cmd, 1);
        expect_value(__wrap_i2c_write, len, 1);

        will_return(__wrap_i2c_write, 0);

        expect_value(__wrap_i2c_read, port, ISIS_ANTENNA_IIC_PORT);
        expect_value(__wrap_i2c_read, adr, ISIS_ANTENNA_IIC_ADR);
        expect_value(__wrap_i2c_read, len, 2);

        will_return(__wrap_i2c_read, (uint8_t)(raw >> 8));
        will_return(__wrap_i2c_read, (uint8_t)(raw & 0xFF));

        will_return(__wrap_i2c_read, 0);

        uint16_t temp = UINT16_MAX;

        assert_return_code(isis_antenna_get_raw_temperature(&temp), 0);

        assert_int_equal(temp, raw);
    }
}

static void isis_antenna_raw_to_temp_c_test(void **state)
{
    uint16_t raw = 0;
    for(raw=0; raw<UINT16_MAX; raw++)
    {
        int16_t temp_c = isis_antenna_raw_to_temp_c(raw);

//        if (raw > 130)
        if (raw > 413)
        {
            assert_in_range(temp_c + 50, -50 + 50, 150 + 50);
        }
        else
        {
            assert_int_equal(temp_c, INT16_MAX);
        }
    }
}

static void isis_antenna_get_temperature_c_test(void **state)
{
    uint8_t cmd = 0xC0;

    uint16_t raw = 0;
    for(raw=0; raw<1023; raw+=50)
    {
        expect_value(__wrap_i2c_write, port, ISIS_ANTENNA_IIC_PORT);
        expect_value(__wrap_i2c_write, adr, ISIS_ANTENNA_IIC_ADR);
        expect_memory(__wrap_i2c_write, data, (void*)&cmd, 1);
        expect_value(__wrap_i2c_write, len, 1);

        will_return(__wrap_i2c_write, 0);

        expect_value(__wrap_i2c_read, port, ISIS_ANTENNA_IIC_PORT);
        expect_value(__wrap_i2c_read, adr, ISIS_ANTENNA_IIC_ADR);
        expect_value(__wrap_i2c_read, len, 2);

        will_return(__wrap_i2c_read, (uint8_t)(raw >> 8));
        will_return(__wrap_i2c_read, (uint8_t)(raw & 0xFF));

        will_return(__wrap_i2c_read, 0);

        int16_t temp = INT16_MAX;

        assert_return_code(isis_antenna_get_temperature_c(&temp), 0);

//        if (raw > 130)
        if (raw > 413)
        {
            assert_in_range(temp + 50, -50 + 50, 150 + 50);
        }
        else
        {
            assert_int_equal(temp, INT16_MAX);
        }
    }
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
    };

    return cmocka_run_group_tests(isis_antenna_tests, NULL, NULL);
}

unsigned int generate_random(unsigned int l, unsigned int r)
{
    return (rand() % (r - l + 1)) + l;
}

/** \} End of isis_antenna_test group */
