/*
 * isis_antenna_test.c
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
 * along with OBDH 2.0. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Unit test of the ISIS Antenna driver.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.10.8
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
#include <drivers/gpio/gpio.h>
#include <drivers/isis_antenna/isis_antenna.h>

#define ISIS_ANTENNA_IIC_PORT       I2C_PORT_2
#define ISIS_ANTENNA_IIC_CLOCK_HZ   100000UL
#define ISIS_ANTENNA_IIC_ADR        0x31
#define ISIS_ANTENNA_IIC_EN_PIN     GPIO_PIN_18
#define ISIS_ANTENNA_IIC_RDY_PIN    GPIO_PIN_21

unsigned int generate_random(unsigned int l, unsigned int r);

void write_test(uint8_t *wd, uint16_t wd_len);

void read_test(uint8_t *rd, uint16_t rd_len);

static void isis_antenna_init_test(void **state)
{
    expect_value(__wrap_tca4311a_init, config.i2c_port,             ISIS_ANTENNA_IIC_PORT);
    expect_value(__wrap_tca4311a_init, config.i2c_config.speed_hz,  ISIS_ANTENNA_IIC_CLOCK_HZ);
    expect_value(__wrap_tca4311a_init, config.en_pin,               ISIS_ANTENNA_IIC_EN_PIN);
    expect_value(__wrap_tca4311a_init, config.ready_pin,            ISIS_ANTENNA_IIC_RDY_PIN);

    expect_value(__wrap_tca4311a_init, en, false);

    will_return(__wrap_tca4311a_init, 0);

    assert_return_code(isis_antenna_init(), 0);
}

static void isis_antenna_arm_test(void **state)
{
    uint8_t cmd = 0xAD;

    write_test(&cmd, 1);

    cmd = 0xC3;

    write_test(&cmd, 1);

    uint8_t ans[2] = {0x01, 0x00};

    read_test(ans, 2);

    assert_return_code(isis_antenna_arm(), 0);
}

static void isis_antenna_disarm_test(void **state)
{
    uint8_t cmd = 0xAC;

    write_test(&cmd, 1);

    cmd = 0xC3;

    write_test(&cmd, 1);

    uint8_t ans[2] = {0x00, 0x00};

    read_test(ans, 2);

    assert_return_code(isis_antenna_disarm(), 0);
}

static void isis_antenna_start_sequential_deploy_test(void **state)
{
    uint8_t sec = generate_random(1, 255);

    uint8_t cmd[2] = {0xA5, sec};

    write_test(cmd, 2);

    assert_return_code(isis_antenna_start_sequential_deploy(sec), 0);
}

static void isis_antenna_start_independent_deploy_test(void **state)
{
    uint8_t sec = generate_random(1, 255);

    /* Antenna 1 with independent deploy */
    uint8_t cmd[2] = {0xBA, sec};

    write_test(cmd, 2);

    assert_return_code(isis_antenna_start_independent_deploy(ISIS_ANTENNA_ANT_1, sec, ISIS_ANTENNA_INDEPENDENT_DEPLOY_WITH_OVERRIDE), 0);

    /* Antenna 2 with independent deploy */
    cmd[0] = 0xBB;

    write_test(cmd, 2);

    assert_return_code(isis_antenna_start_independent_deploy(ISIS_ANTENNA_ANT_2, sec, ISIS_ANTENNA_INDEPENDENT_DEPLOY_WITH_OVERRIDE), 0);

    /* Antenna 3 with independent deploy */
    cmd[0] = 0xBC;

    write_test(cmd, 2);

    assert_return_code(isis_antenna_start_independent_deploy(ISIS_ANTENNA_ANT_3, sec, ISIS_ANTENNA_INDEPENDENT_DEPLOY_WITH_OVERRIDE), 0);

    /* Antenna 4 with independent deploy */
    cmd[0] = 0xBD;

    write_test(cmd, 2);

    assert_return_code(isis_antenna_start_independent_deploy(ISIS_ANTENNA_ANT_4, sec, ISIS_ANTENNA_INDEPENDENT_DEPLOY_WITH_OVERRIDE), 0);
}

static void isis_antenna_read_deployment_status_code_test(void **state)
{
    uint8_t cmd = 0xC3;

    write_test(&cmd, 1);

    uint16_t raw_status = generate_random(0, UINT16_MAX);

    uint8_t ans[2] = {0};

    ans[0] = raw_status & 0xFF;
    ans[1] = raw_status >> 8;

    read_test(ans, 2);

    uint16_t status = UINT16_MAX;

    assert_return_code(isis_antenna_read_deployment_status_code(&status), 0);

    assert_int_equal(raw_status, status);
}

static void isis_antenna_read_deployment_status_test(void **state)
{
    uint8_t cmd = 0xC3;

    write_test(&cmd, 1);

    uint8_t ans[2] = {0};

    uint16_t raw_status = generate_random(0, UINT16_MAX);

    ans[0] = raw_status & 0xFF;
    ans[1] = raw_status >> 8;

    read_test(ans, 2);

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

static void isis_antenna_get_data_test(void **state)
{
    /* Read status */
    uint8_t cmd = 0xC3;

    write_test(&cmd, 1);

    uint8_t ans[2] = {0};

    uint16_t raw_status = generate_random(0, UINT16_MAX);

    ans[0] = raw_status & 0xFF;
    ans[1] = raw_status >> 8;

    read_test(ans, 2);

    /* Read temperature */
    cmd = 0xC0;

    uint16_t raw_temp = generate_random(0, 1023);

    write_test(&cmd, 1);

    ans[0] = raw_temp & 0xFF;
    ans[1] = raw_temp >> 8;

    read_test(ans, 2);

    isis_antenna_data_t data = {0};

    assert_return_code(isis_antenna_get_data(&data), 0);

    assert_int_equal(raw_status, data.status.code);
    assert_int_equal((raw_status >> 15) & 1, data.status.antenna_1.status);
    assert_int_equal((raw_status >> 14) & 1, data.status.antenna_1.timeout);
    assert_int_equal((raw_status >> 13) & 1, data.status.antenna_1.burning);
    assert_int_equal((raw_status >> 11) & 1, data.status.antenna_2.status);
    assert_int_equal((raw_status >> 10) & 1, data.status.antenna_2.timeout);
    assert_int_equal((raw_status >> 9) & 1, data.status.antenna_2.burning);
    assert_int_equal((raw_status >> 8) & 1, data.status.ignoring_switches);
    assert_int_equal((raw_status >> 7) & 1, data.status.antenna_3.status);
    assert_int_equal((raw_status >> 6) & 1, data.status.antenna_3.timeout);
    assert_int_equal((raw_status >> 5) & 1, data.status.antenna_3.burning);
    assert_int_equal((raw_status >> 4) & 1, data.status.independent_burn);
    assert_int_equal((raw_status >> 3) & 1, data.status.antenna_4.status);
    assert_int_equal((raw_status >> 2) & 1, data.status.antenna_4.timeout);
    assert_int_equal((raw_status >> 1) & 1, data.status.antenna_4.burning);
    assert_int_equal((raw_status >> 0) & 1, data.status.armed);

//    if (raw > 130)
    if (raw_temp > 413)
    {
        assert_in_range(data.temperature, -50 + 273, 150 + 273);
    }
    else
    {
        assert_int_equal(data.temperature, UINT16_MAX);
    }
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

        write_test(&cmd, 1);

        uint8_t ans[2] = {0};

        uint16_t raw_status = generate_random(0, UINT16_MAX);

        ans[0] = raw_status & 0xFF;
        ans[1] = raw_status >> 8;

        read_test(ans, 2);

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

        write_test(&cmd, 1);

        uint8_t ans[2] = {0};

        uint16_t raw_status = generate_random(0, UINT16_MAX);

        ans[0] = raw_status & 0xFF;
        ans[1] = raw_status >> 8;

        read_test(ans, 2);

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

        write_test(&cmd, 1);

        uint8_t ans[2] = {0};

        uint16_t raw_status = generate_random(0, UINT16_MAX);

        ans[0] = raw_status & 0xFF;
        ans[1] = raw_status >> 8;

        read_test(ans, 2);

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

    write_test(&cmd, 1);

    uint8_t ans[2] = {0};

    uint16_t raw_status = generate_random(0, UINT16_MAX);

    ans[0] = raw_status & 0xFF;
    ans[1] = raw_status >> 8;

    read_test(ans, 2);

    bool ant_armed = false;

    assert_return_code(isis_antenna_get_arming_status(&ant_armed), 0);
    assert_int_equal((raw_status >> 0) & 1, (int)ant_armed);
}

static void isis_antenna_get_raw_temperature_test(void **state)
{
    uint8_t cmd = 0xC0;

    uint16_t raw = 0;
    for(raw = 0; raw < 1023; raw += 50)
    {
        write_test(&cmd, 1);

        uint8_t ans[2] = {0};

        ans[0] = raw & 0xFF;
        ans[1] = raw >> 8;

        read_test(ans, 2);

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
        write_test(&cmd, 1);

        uint8_t ans[2] = {0};

        ans[0] = raw & 0xFF;
        ans[1] = raw >> 8;

        read_test(ans, 2);

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

static void isis_antenna_get_temperature_k_test(void **state)
{
    uint8_t cmd = 0xC0;

    uint16_t raw = 0;
    for(raw = 0; raw < 1023; raw += 50)
    {
        write_test(&cmd, 1);

        uint8_t ans[2] = {0};

        ans[0] = raw & 0xFF;
        ans[1] = raw >> 8;

        read_test(ans, 2);

        isis_antenna_temp_t temp = UINT16_MAX;

        assert_return_code(isis_antenna_get_temperature_k(&temp), 0);

//        if (raw > 130)
        if (raw > 413)
        {
            assert_in_range(temp, -50 + 273, 150 + 273);
        }
        else
        {
            assert_int_equal(temp, UINT16_MAX);
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
        cmocka_unit_test(isis_antenna_get_data_test),
        cmocka_unit_test(isis_antenna_get_antenna_status_test),
        cmocka_unit_test(isis_antenna_get_antenna_timeout_test),
        cmocka_unit_test(isis_antenna_get_burning_test),
        cmocka_unit_test(isis_antenna_get_arming_status_test),
        cmocka_unit_test(isis_antenna_get_raw_temperature_test),
        cmocka_unit_test(isis_antenna_raw_to_temp_c_test),
        cmocka_unit_test(isis_antenna_get_temperature_c_test),
        cmocka_unit_test(isis_antenna_get_temperature_k_test),
    };

    return cmocka_run_group_tests(isis_antenna_tests, NULL, NULL);
}

unsigned int generate_random(unsigned int l, unsigned int r)
{
    return (rand() % (r - l + 1)) + l;
}

void write_test(uint8_t *wd, uint16_t wd_len)
{
    expect_value(__wrap_tca4311a_enable, config.i2c_port,               ISIS_ANTENNA_IIC_PORT);
    expect_value(__wrap_tca4311a_enable, config.i2c_config.speed_hz,    ISIS_ANTENNA_IIC_CLOCK_HZ);
    expect_value(__wrap_tca4311a_enable, config.en_pin,                 ISIS_ANTENNA_IIC_EN_PIN);
    expect_value(__wrap_tca4311a_enable, config.ready_pin,              ISIS_ANTENNA_IIC_RDY_PIN);

    will_return(__wrap_tca4311a_enable, 0);

    expect_value(__wrap_tca4311a_write, config.i2c_port,                ISIS_ANTENNA_IIC_PORT);
    expect_value(__wrap_tca4311a_write, config.i2c_config.speed_hz,     ISIS_ANTENNA_IIC_CLOCK_HZ);
    expect_value(__wrap_tca4311a_write, config.en_pin,                  ISIS_ANTENNA_IIC_EN_PIN);
    expect_value(__wrap_tca4311a_write, config.ready_pin,               ISIS_ANTENNA_IIC_RDY_PIN);

    expect_value(__wrap_tca4311a_write, adr, ISIS_ANTENNA_IIC_ADR);
    expect_memory(__wrap_tca4311a_write, data, (void*)wd, wd_len);
    expect_value(__wrap_tca4311a_write, len, wd_len);

    will_return(__wrap_tca4311a_write, 0);

    expect_value(__wrap_tca4311a_disable, config.i2c_port,              ISIS_ANTENNA_IIC_PORT);
    expect_value(__wrap_tca4311a_disable, config.i2c_config.speed_hz,   ISIS_ANTENNA_IIC_CLOCK_HZ);
    expect_value(__wrap_tca4311a_disable, config.en_pin,                ISIS_ANTENNA_IIC_EN_PIN);
    expect_value(__wrap_tca4311a_disable, config.ready_pin,             ISIS_ANTENNA_IIC_RDY_PIN);

    will_return(__wrap_tca4311a_disable, 0);
}

void read_test(uint8_t *rd, uint16_t rd_len)
{
    expect_value(__wrap_tca4311a_enable, config.i2c_port,               ISIS_ANTENNA_IIC_PORT);
    expect_value(__wrap_tca4311a_enable, config.i2c_config.speed_hz,    ISIS_ANTENNA_IIC_CLOCK_HZ);
    expect_value(__wrap_tca4311a_enable, config.en_pin,                 ISIS_ANTENNA_IIC_EN_PIN);
    expect_value(__wrap_tca4311a_enable, config.ready_pin,              ISIS_ANTENNA_IIC_RDY_PIN);

    will_return(__wrap_tca4311a_enable, 0);

    expect_value(__wrap_tca4311a_read, config.i2c_port,                 ISIS_ANTENNA_IIC_PORT);
    expect_value(__wrap_tca4311a_read, config.i2c_config.speed_hz,      ISIS_ANTENNA_IIC_CLOCK_HZ);
    expect_value(__wrap_tca4311a_read, config.en_pin,                   ISIS_ANTENNA_IIC_EN_PIN);
    expect_value(__wrap_tca4311a_read, config.ready_pin,                ISIS_ANTENNA_IIC_RDY_PIN);

    expect_value(__wrap_tca4311a_read, adr, ISIS_ANTENNA_IIC_ADR);
    expect_value(__wrap_tca4311a_read, len, rd_len);

    uint16_t i = 0;
    for(i = 0; i < rd_len; i++)
    {
        will_return(__wrap_tca4311a_read, rd[i]);
    }

    will_return(__wrap_tca4311a_read, 0);

    expect_value(__wrap_tca4311a_disable, config.i2c_port,              ISIS_ANTENNA_IIC_PORT);
    expect_value(__wrap_tca4311a_disable, config.i2c_config.speed_hz,   ISIS_ANTENNA_IIC_CLOCK_HZ);
    expect_value(__wrap_tca4311a_disable, config.en_pin,                ISIS_ANTENNA_IIC_EN_PIN);
    expect_value(__wrap_tca4311a_disable, config.ready_pin,             ISIS_ANTENNA_IIC_RDY_PIN);

    will_return(__wrap_tca4311a_disable, 0);
}

/** \} End of isis_antenna_test group */
