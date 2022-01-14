/*
 * tca4311a_test.c
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
 * \brief Unit test of the TCA4311A driver.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.8.40
 * 
 * \date 2021/09/01
 * 
 * \defgroup tca4311a_unit_test TCA4311A
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
#include <drivers/tca4311a/tca4311a.h>

#define TCA4311A_I2C_PORT               I2C_PORT_1
#define TCA4311A_I2C_CLOCK_HZ           100000
#define TCA4311A_I2C_ADR                0x52
#define TCA4311A_ENABLE_PIN             GPIO_PIN_17
#define TCA4311A_READY_PIN              GPIO_PIN_20

tca4311a_config_t conf = {0};

unsigned int generate_random(unsigned int l, unsigned int r);

static void tca4311a_init_test(void **state)
{
    /* I2C initialization */
    expect_value(__wrap_i2c_init, port, TCA4311A_I2C_PORT);
    expect_value(__wrap_i2c_init, config.speed_hz, TCA4311A_I2C_CLOCK_HZ);

    will_return(__wrap_i2c_init, 0);

    /* Enable pin initialization */
    expect_value(__wrap_gpio_init, pin, TCA4311A_ENABLE_PIN);
    expect_value(__wrap_gpio_init, config.mode, GPIO_MODE_OUTPUT);

    will_return(__wrap_gpio_init, 0);

    /* Ready pin initialization */
    expect_value(__wrap_gpio_init, pin, TCA4311A_READY_PIN);
    expect_value(__wrap_gpio_init, config.mode, GPIO_MODE_INPUT);

    will_return(__wrap_gpio_init, 0);

    /* Set state */
    expect_value(__wrap_gpio_set_state, pin, TCA4311A_ENABLE_PIN);
    expect_value(__wrap_gpio_set_state, level, true);

    will_return(__wrap_gpio_set_state, 0);

    assert_return_code(tca4311a_init(conf, true), 0);
}

static void tca4311a_enable_test(void **state)
{
    /* Set state */
    expect_value(__wrap_gpio_set_state, pin, TCA4311A_ENABLE_PIN);
    expect_value(__wrap_gpio_set_state, level, true);

    will_return(__wrap_gpio_set_state, 0);

    assert_return_code(tca4311a_enable(conf), 0);
}

static void tca4311a_disable_test(void **state)
{
    /* Set state */
    expect_value(__wrap_gpio_set_state, pin, TCA4311A_ENABLE_PIN);
    expect_value(__wrap_gpio_set_state, level, false);

    will_return(__wrap_gpio_set_state, 0);

    assert_return_code(tca4311a_disable(conf), 0);
}

static void tca4311a_is_ready_test(void **state)
{
    int i = 0;
    for(i=0; i<UINT16_MAX; i++)
    {
        expect_value(__wrap_gpio_get_state, pin, TCA4311A_READY_PIN);

        will_return(__wrap_gpio_get_state, i);

        int result = tca4311a_is_ready(conf);

        if (i == 0)
        {
            assert_return_code(result, TCA4311A_READY);
        }
        else if (i == 1)
        {
            assert_return_code(result, TCA4311A_NOT_READY);
        }
        else
        {
            assert_int_equal(result, TCA4311A_ERROR);
        }
    }
}

static void tca4311a_write_test(void **state)
{
    uint8_t data[256] = {0xFF};
    uint16_t data_len = generate_random(1, 256);

    uint16_t i = 0;
    for(i=0; i<data_len; i++)
    {
        data[i] = generate_random(0, 255);
    }

    /* I2C write */
    expect_value(__wrap_i2c_write, port, TCA4311A_I2C_PORT);
    expect_value(__wrap_i2c_write, adr, TCA4311A_I2C_ADR);
    expect_memory(__wrap_i2c_write, data, (void*)data, data_len);
    expect_value(__wrap_i2c_write, len, data_len);

    will_return(__wrap_i2c_write, 0);

    assert_return_code(tca4311a_write(conf, TCA4311A_I2C_ADR, data, data_len), 0);
}

static void tca4311a_read_test(void **state)
{
    uint8_t data[256] = {0xFF};
    uint16_t data_len = generate_random(1, 256);

    uint16_t i = 0;
    for(i=0; i<data_len; i++)
    {
        data[i] = generate_random(0, 255);
    }

    /* I2C read */
    expect_value(__wrap_i2c_read, port, TCA4311A_I2C_PORT);
    expect_value(__wrap_i2c_read, adr, TCA4311A_I2C_ADR);
    expect_value(__wrap_i2c_read, len, data_len);

    for(i=0; i<data_len; i++)
    {
        will_return(__wrap_i2c_read, data[i]);
    }

    will_return(__wrap_i2c_read, 0);

    uint8_t data_res[256] = {0xFF};

    assert_return_code(tca4311a_read(conf, TCA4311A_I2C_ADR, data_res, data_len), 0);

    for(i=0; i<data_len; i++)
    {
        assert_int_equal(data[i], data_res[i]);
    }
}

static void tca4311a_write_byte_test(void **state)
{
    uint8_t data = generate_random(0, 255);

    /* I2C write */
    expect_value(__wrap_i2c_write, port, TCA4311A_I2C_PORT);
    expect_value(__wrap_i2c_write, adr, TCA4311A_I2C_ADR);
    expect_memory(__wrap_i2c_write, data, (void*)&data, 1);
    expect_value(__wrap_i2c_write, len, 1);

    will_return(__wrap_i2c_write, 0);

    assert_return_code(tca4311a_write_byte(conf, TCA4311A_I2C_ADR, data), 0);
}

static void tca4311a_read_byte_test(void **state)
{
    uint8_t data = generate_random(1, 256);

    /* I2C read */
    expect_value(__wrap_i2c_read, port, TCA4311A_I2C_PORT);
    expect_value(__wrap_i2c_read, adr, TCA4311A_I2C_ADR);
    expect_value(__wrap_i2c_read, len, 1);

    will_return(__wrap_i2c_read, data);

    will_return(__wrap_i2c_read, 0);

    uint8_t data_res = 0xFF;

    assert_return_code(tca4311a_read_byte(conf, TCA4311A_I2C_ADR, &data_res), 0);

    assert_int_equal(data, data_res);
}

int main(void)
{
    conf.i2c_port               = TCA4311A_I2C_PORT;
    conf.i2c_config.speed_hz    = TCA4311A_I2C_CLOCK_HZ;
    conf.en_pin                 = TCA4311A_ENABLE_PIN;
    conf.ready_pin              = TCA4311A_READY_PIN;

    const struct CMUnitTest tca4311a_tests[] = {
        cmocka_unit_test(tca4311a_init_test),
        cmocka_unit_test(tca4311a_enable_test),
        cmocka_unit_test(tca4311a_disable_test),
        cmocka_unit_test(tca4311a_is_ready_test),
        cmocka_unit_test(tca4311a_write_test),
        cmocka_unit_test(tca4311a_read_test),
        cmocka_unit_test(tca4311a_write_byte_test),
        cmocka_unit_test(tca4311a_read_byte_test),
    };

    return cmocka_run_group_tests(tca4311a_tests, NULL, NULL);
}

unsigned int generate_random(unsigned int l, unsigned int r)
{
    return (rand() % (r - l + 1)) + l;
}

/** \} End of tca4311a_test group */
