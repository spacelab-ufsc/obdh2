/*
 * edc_test.c
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
 * \brief Unit test of the EDC driver.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Bruno Benedetti <brunobenedetti45@gmail.com> 
 *
 * \version 0.9.2
 * 
 * \date 2021/09/01
 * 
 * \defgroup edc_unit_test EDC
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
#include <math.h>

#include <drivers/i2c/i2c.h>
#include <drivers/gpio/gpio.h>
#include <drivers/edc/edc.h>
#include <drivers/uart/uart.h>

#define EDC_I2C_PORT                I2C_PORT_1
#define EDC_I2C_CLOCK               100000UL
#define EDC_I2C_ADR                 0x13

#define EDC_GPIO_EN_PIN             GPIO_PIN_29

#define EDC_UART_BAUD_RATE          115200
#define EDC_UART_DATA_BITS          8
#define EDC_UART_PARITY             UART_NO_PARITY
#define EDC_UART_STOP_BITS          UART_ONE_STOP_BIT
#define EDC_UART_PORT               UART_PORT_1
#define EDC_UART_CLOCK              1 /* placeholder */


edc_config_t conf = {0};

unsigned int generate_random(unsigned int l, unsigned int r);

static void edc_init_test(void **state)
{
    conf.interface = EDC_IF_I2C;
    /* GPIO init */
    expect_value(__wrap_gpio_init, pin, EDC_GPIO_EN_PIN);
    expect_value(__wrap_gpio_init, config.mode, GPIO_MODE_OUTPUT);

    will_return(__wrap_gpio_init, 0);

    /* Enable */
    expect_value(__wrap_gpio_set_state, pin, EDC_GPIO_EN_PIN);
    expect_value(__wrap_gpio_set_state, level, true);

    will_return(__wrap_gpio_set_state, 0);

    /* I2C init */
    expect_value(__wrap_i2c_init, port, EDC_I2C_PORT);
    expect_value(__wrap_i2c_init, config.speed_hz, EDC_I2C_CLOCK);

    will_return(__wrap_i2c_init, 0);

    /* I2C write */
    uint8_t cmd[8] = {0x30};
    uint16_t cmd_len = 1;

    expect_value(__wrap_i2c_write, port, EDC_I2C_PORT);
    expect_value(__wrap_i2c_write, adr, EDC_I2C_ADR);
    expect_memory(__wrap_i2c_write, data, (void*)cmd, cmd_len);
    expect_value(__wrap_i2c_write, len, cmd_len);

    will_return(__wrap_i2c_write, 0);

    /* I2C read */
    expect_value(__wrap_i2c_read, port, EDC_I2C_PORT);
    expect_value(__wrap_i2c_read, adr, EDC_I2C_ADR);
    expect_value(__wrap_i2c_read, len, 9);

    uint8_t cmd_ans_i2c[16] = {0x11};

    uint16_t i = 0;
    for(i=0; i<9; i++)
    {
        will_return(__wrap_i2c_read, cmd_ans_i2c[i]);
    }

    will_return(__wrap_i2c_read, 0);

    assert_return_code(edc_init(conf), 9);
    
    conf.interface = EDC_IF_UART;

    /* GPIO init */
    expect_value(__wrap_gpio_init, pin, EDC_GPIO_EN_PIN);
    expect_value(__wrap_gpio_init, config.mode, GPIO_MODE_OUTPUT);

    will_return(__wrap_gpio_init, 0);

    /* Enable */
    expect_value(__wrap_gpio_set_state, pin, EDC_GPIO_EN_PIN);
    expect_value(__wrap_gpio_set_state, level, true);

    will_return(__wrap_gpio_set_state, 0);

    /* UART init */
    expect_value(__wrap_uart_init, port, EDC_UART_PORT);
    expect_value(__wrap_uart_init, config.baudrate, EDC_UART_BAUD_RATE);
    expect_value(__wrap_uart_init, config.data_bits, EDC_UART_DATA_BITS);
    expect_value(__wrap_uart_init, config.parity, EDC_UART_PARITY);
    expect_value(__wrap_uart_init, config.stop_bits, EDC_UART_STOP_BITS);

    will_return(__wrap_uart_init, 0);

    /* Uart rx enable */
    expect_value (__wrap_uart_rx_enable, port, EDC_UART_PORT);

    will_return(__wrap_uart_rx_enable, 0);

    /* UART write */
    expect_value (__wrap_uart_write, port, EDC_UART_PORT);
    expect_memory(__wrap_uart_write, data, (void*)cmd, cmd_len);
    expect_value (__wrap_uart_write, len, cmd_len);

    will_return(__wrap_uart_write, 0);

    /* Uart read available */
    expect_value (__wrap_uart_read_available, port, EDC_UART_PORT);

    will_return(__wrap_uart_read_available, 9);

    /* UART read */
    expect_value(__wrap_uart_read, port, EDC_UART_PORT);
    expect_value(__wrap_uart_read, len, 9);

    uint8_t cmd_ans_uart[16] = {0x11};
    i = 0;

    for(i=0; i<9; i++)
    {
        will_return(__wrap_uart_read, cmd_ans_uart[i]);
    }

    will_return(__wrap_uart_read, 0);

    assert_return_code(edc_init(conf), 9);
}

static void edc_enable_test(void **state)
{
    expect_value(__wrap_gpio_set_state, pin, EDC_GPIO_EN_PIN);
    expect_value(__wrap_gpio_set_state, level, true);

    will_return(__wrap_gpio_set_state, 0);

    assert_return_code(edc_enable(conf), 0);
}

static void edc_disable_test(void **state)
{
    expect_value(__wrap_gpio_set_state, pin, EDC_GPIO_EN_PIN);
    expect_value(__wrap_gpio_set_state, level, false);

    will_return(__wrap_gpio_set_state, 0);

    assert_return_code(edc_disable(conf), 0);
}

static void edc_write_cmd_test(void **state)
{
    uint8_t i = 0;

    uint8_t cmd_arr[6] = {0xFF};
    uint16_t cmd_len = 1;

    for(i=0; i<UINT8_MAX; i++)
    {
        edc_cmd_t cmd = {0};

        cmd.id = i;
        cmd.param = 0x12345678;

        switch(i)
        {
            case 0x01:
                cmd_arr[0] = i;
                cmd_arr[1] = (uint8_t)(cmd.param >> 0);
                cmd_arr[2] = (uint8_t)(cmd.param >> 8);
                cmd_arr[3] = (uint8_t)(cmd.param >> 16);
                cmd_arr[4] = (uint8_t)(cmd.param >> 24);
                cmd_len = 5;

                conf.interface = EDC_IF_I2C;
                /* I2C write */
                expect_value(__wrap_i2c_write, port, EDC_I2C_PORT);
                expect_value(__wrap_i2c_write, adr, EDC_I2C_ADR);
                expect_memory(__wrap_i2c_write, data, (void*)cmd_arr, cmd_len);
                expect_value(__wrap_i2c_write, len, cmd_len);

                will_return(__wrap_i2c_write, 0);

                assert_return_code(edc_write_cmd(conf, cmd), 0);
                
                conf.interface = EDC_IF_UART;
                /* UART write */
                expect_value (__wrap_uart_write, port, EDC_UART_PORT);
                expect_memory(__wrap_uart_write, data, (void*)cmd_arr, cmd_len);
                expect_value (__wrap_uart_write, len, cmd_len);

                will_return(__wrap_uart_write, 0);

                assert_return_code(edc_write_cmd(conf, cmd), 0);

                break;
            case 0x06:
                cmd_arr[0] = i;
                cmd_len = 1;

                conf.interface = EDC_IF_I2C;
                /* I2C write */
                expect_value(__wrap_i2c_write, port, EDC_I2C_PORT);
                expect_value(__wrap_i2c_write, adr, EDC_I2C_ADR);
                expect_memory(__wrap_i2c_write, data, (void*)cmd_arr, cmd_len);
                expect_value(__wrap_i2c_write, len, cmd_len);

                will_return(__wrap_i2c_write, 0);

                assert_return_code(edc_write_cmd(conf, cmd), 0);

                conf.interface = EDC_IF_UART;
                /* UART write */
                expect_value (__wrap_uart_write, port, EDC_UART_PORT);
                expect_memory(__wrap_uart_write, data, (void*)cmd_arr, cmd_len);
                expect_value (__wrap_uart_write, len, cmd_len);

                will_return(__wrap_uart_write, 0);

                assert_return_code(edc_write_cmd(conf, cmd), 0);

                break;
            case 0x08:
                cmd_arr[0] = i;
                cmd_len = 1;

                conf.interface = EDC_IF_I2C;
                /* I2C write */
                expect_value(__wrap_i2c_write, port, EDC_I2C_PORT);
                expect_value(__wrap_i2c_write, adr, EDC_I2C_ADR);
                expect_memory(__wrap_i2c_write, data, (void*)cmd_arr, cmd_len);
                expect_value(__wrap_i2c_write, len, cmd_len);

                will_return(__wrap_i2c_write, 0);

                assert_return_code(edc_write_cmd(conf, cmd), 0);

                conf.interface = EDC_IF_UART;
                /* UART write */
                expect_value (__wrap_uart_write, port, EDC_UART_PORT);
                expect_memory(__wrap_uart_write, data, (void*)cmd_arr, cmd_len);
                expect_value (__wrap_uart_write, len, cmd_len);

                will_return(__wrap_uart_write, 0);

                assert_return_code(edc_write_cmd(conf, cmd), 0);

                break;
            case 0x09:
                cmd_arr[0] = i;
                cmd_len = 1;

                conf.interface = EDC_IF_I2C;
                /* I2C write */
                expect_value(__wrap_i2c_write, port, EDC_I2C_PORT);
                expect_value(__wrap_i2c_write, adr, EDC_I2C_ADR);
                expect_memory(__wrap_i2c_write, data, (void*)cmd_arr, cmd_len);
                expect_value(__wrap_i2c_write, len, cmd_len);

                will_return(__wrap_i2c_write, 0);

                assert_return_code(edc_write_cmd(conf, cmd), 0);

                conf.interface = EDC_IF_UART;
                /* UART write */
                expect_value (__wrap_uart_write, port, EDC_UART_PORT);
                expect_memory(__wrap_uart_write, data, (void*)cmd_arr, cmd_len);
                expect_value (__wrap_uart_write, len, cmd_len);

                will_return(__wrap_uart_write, 0);

                assert_return_code(edc_write_cmd(conf, cmd), 0);

                break;
            case 0x0A:
                cmd_arr[0] = i;
                cmd_len = 1;

                conf.interface = EDC_IF_I2C;
                /* I2C write */
                expect_value(__wrap_i2c_write, port, EDC_I2C_PORT);
                expect_value(__wrap_i2c_write, adr, EDC_I2C_ADR);
                expect_memory(__wrap_i2c_write, data, (void*)cmd_arr, cmd_len);
                expect_value(__wrap_i2c_write, len, cmd_len);

                will_return(__wrap_i2c_write, 0);

                assert_return_code(edc_write_cmd(conf, cmd), 0);

                conf.interface = EDC_IF_UART;
                /* UART write */
                expect_value (__wrap_uart_write, port, EDC_UART_PORT);
                expect_memory(__wrap_uart_write, data, (void*)cmd_arr, cmd_len);
                expect_value (__wrap_uart_write, len, cmd_len);

                will_return(__wrap_uart_write, 0);

                assert_return_code(edc_write_cmd(conf, cmd), 0);

                break;
            case 0x30:
                cmd_arr[0] = i;
                cmd_len = 1;
                
                conf.interface = EDC_IF_I2C;
                /* I2C write */
                expect_value(__wrap_i2c_write, port, EDC_I2C_PORT);
                expect_value(__wrap_i2c_write, adr, EDC_I2C_ADR);
                expect_memory(__wrap_i2c_write, data, (void*)cmd_arr, cmd_len);
                expect_value(__wrap_i2c_write, len, cmd_len);

                will_return(__wrap_i2c_write, 0);

                assert_return_code(edc_write_cmd(conf, cmd), 0);

                conf.interface = EDC_IF_UART;
                /* UART write */
                expect_value (__wrap_uart_write, port, EDC_UART_PORT);
                expect_memory(__wrap_uart_write, data, (void*)cmd_arr, cmd_len);
                expect_value (__wrap_uart_write, len, cmd_len);

                will_return(__wrap_uart_write, 0);

                assert_return_code(edc_write_cmd(conf, cmd), 0);

                break;
            case 0x31:
                cmd_arr[0] = i;
                cmd_arr[1] = 0;
                cmd_len = 2;

                conf.interface = EDC_IF_I2C;
                /* I2C write */
                expect_value(__wrap_i2c_write, port, EDC_I2C_PORT);
                expect_value(__wrap_i2c_write, adr, EDC_I2C_ADR);
                expect_memory(__wrap_i2c_write, data, (void*)cmd_arr, cmd_len);
                expect_value(__wrap_i2c_write, len, cmd_len);

                will_return(__wrap_i2c_write, 0);

                assert_return_code(edc_write_cmd(conf, cmd), 0);

                conf.interface = EDC_IF_UART;
                /* UART write */
                expect_value (__wrap_uart_write, port, EDC_UART_PORT);
                expect_memory(__wrap_uart_write, data, (void*)cmd_arr, cmd_len);
                expect_value (__wrap_uart_write, len, cmd_len);

                will_return(__wrap_uart_write, 0);

                assert_return_code(edc_write_cmd(conf, cmd), 0);

                break;
            case 0x32:
                cmd_arr[0] = i;
                cmd_len = 1;

                conf.interface = EDC_IF_I2C;
                /* I2C write */
                expect_value(__wrap_i2c_write, port, EDC_I2C_PORT);
                expect_value(__wrap_i2c_write, adr, EDC_I2C_ADR);
                expect_memory(__wrap_i2c_write, data, (void*)cmd_arr, cmd_len);
                expect_value(__wrap_i2c_write, len, cmd_len);

                will_return(__wrap_i2c_write, 0);

                assert_return_code(edc_write_cmd(conf, cmd), 0);

                conf.interface = EDC_IF_UART;
                /* UART write */
                expect_value (__wrap_uart_write, port, EDC_UART_PORT);
                expect_memory(__wrap_uart_write, data, (void*)cmd_arr, cmd_len);
                expect_value (__wrap_uart_write, len, cmd_len);

                will_return(__wrap_uart_write, 0);

                assert_return_code(edc_write_cmd(conf, cmd), 0);

                break;
            case 0x34:
                cmd_arr[0] = i;
                cmd_arr[1] = 0;
                cmd_arr[2] = 0;
                cmd_len = 3;

                conf.interface = EDC_IF_I2C;
                /* I2C write */
                expect_value(__wrap_i2c_write, port, EDC_I2C_PORT);
                expect_value(__wrap_i2c_write, adr, EDC_I2C_ADR);
                expect_memory(__wrap_i2c_write, data, (void*)cmd_arr, cmd_len);
                expect_value(__wrap_i2c_write, len, cmd_len);

                will_return(__wrap_i2c_write, 0);

                assert_return_code(edc_write_cmd(conf, cmd), 0);

                conf.interface = EDC_IF_UART;
                /* UART write */
                expect_value (__wrap_uart_write, port, EDC_UART_PORT);
                expect_memory(__wrap_uart_write, data, (void*)cmd_arr, cmd_len);
                expect_value (__wrap_uart_write, len, cmd_len);

                will_return(__wrap_uart_write, 0);

                assert_return_code(edc_write_cmd(conf, cmd), 0);

                break;
            case 0xF0:
                cmd_arr[0] = i;
                cmd_len = 1;

                conf.interface = EDC_IF_I2C;
                /* I2C write */
                expect_value(__wrap_i2c_write, port, EDC_I2C_PORT);
                expect_value(__wrap_i2c_write, adr, EDC_I2C_ADR);
                expect_memory(__wrap_i2c_write, data, (void*)cmd_arr, cmd_len);
                expect_value(__wrap_i2c_write, len, cmd_len);

                will_return(__wrap_i2c_write, 0);

                assert_return_code(edc_write_cmd(conf, cmd), 0);

                conf.interface = EDC_IF_UART;
                /* UART write */
                expect_value (__wrap_uart_write, port, EDC_UART_PORT);
                expect_memory(__wrap_uart_write, data, (void*)cmd_arr, cmd_len);
                expect_value (__wrap_uart_write, len, cmd_len);

                will_return(__wrap_uart_write, 0);

                assert_return_code(edc_write_cmd(conf, cmd), 0);

                break;
            default:
                assert_int_equal(edc_write_cmd(conf, cmd), -1);
        }
    }
}

static void edc_read_test(void **state)
{
    uint8_t data[256] = {0xFF};
    uint16_t data_len = generate_random(1, 256);

    conf.interface = EDC_IF_I2C;

    /* I2C read */
    expect_value(__wrap_i2c_read, port, EDC_I2C_PORT);
    expect_value(__wrap_i2c_read, adr, EDC_I2C_ADR);
    expect_value(__wrap_i2c_read, len, data_len);

    uint16_t i = 0;
    for(i=0; i<data_len; i++)
    {
        data[i] = generate_random(0, 255);
        will_return(__wrap_i2c_read, data[i]);
    }

    will_return(__wrap_i2c_read, 0);

    uint8_t ans[256] = {0xFF};

    assert_return_code(edc_read(conf, ans, data_len), 0);

    for(i=0; i<data_len; i++)
    {
        assert_int_equal(data[i], ans[i]);
    }

    conf.interface = EDC_IF_UART;

    /* UART rx available */
    expect_value(__wrap_uart_read_available, port, EDC_UART_PORT);

    will_return(__wrap_uart_read_available, data_len);

    /* UART read */
    expect_value(__wrap_uart_read, port, EDC_UART_PORT);
    expect_value(__wrap_uart_read, len, data_len);

    i = 0;
    for(i=0; i<data_len; i++)
    {
        will_return(__wrap_uart_read, data[i]);
    }
    
    will_return(__wrap_uart_read, 0);

    assert_return_code(edc_read(conf, ans, data_len), 0);

    for(i=0; i<data_len; i++)
    {
        assert_int_equal(data[i], ans[i]);
    }
}

static void edc_check_device_test(void **state)
{
    /* I2C write */
    uint8_t cmd[8] = {0x30};
    uint16_t cmd_len = 1;

    conf.interface = EDC_IF_I2C;

    expect_value(__wrap_i2c_write, port, EDC_I2C_PORT);
    expect_value(__wrap_i2c_write, adr, EDC_I2C_ADR);
    expect_memory(__wrap_i2c_write, data, (void*)cmd, cmd_len);
    expect_value(__wrap_i2c_write, len, cmd_len);

    will_return(__wrap_i2c_write, 0);

    /* I2C read */
    expect_value(__wrap_i2c_read, port, EDC_I2C_PORT);
    expect_value(__wrap_i2c_read, adr, EDC_I2C_ADR);
    expect_value(__wrap_i2c_read, len, 9);

    uint8_t cmd_ans[16] = {0x11};

    uint16_t i = 0;
    for(i=0; i<9; i++)
    {
        will_return(__wrap_i2c_read, cmd_ans[i]);
    }

    will_return(__wrap_i2c_read, 0);

    assert_return_code(edc_check_device(conf), 0);

    conf.interface = EDC_IF_UART;

    /* UART write */
    expect_value (__wrap_uart_write, port, EDC_UART_PORT);
    expect_memory(__wrap_uart_write, data, (void*)cmd, cmd_len);
    expect_value (__wrap_uart_write, len, cmd_len);

    will_return(__wrap_uart_write, 0);

    /* Uart read available */
    expect_value (__wrap_uart_read_available, port, EDC_UART_PORT);

    will_return(__wrap_uart_read_available, 9);

    /* UART read */
    expect_value(__wrap_uart_read, port, EDC_UART_PORT);
    expect_value(__wrap_uart_read, len, 9);

    i = 0;
    for(i=0; i<9; i++)
    {
        will_return(__wrap_uart_read, cmd_ans[i]);
    }

    will_return(__wrap_uart_read, 0);

    assert_return_code(edc_check_device(conf), 0);
}

static void edc_set_rtc_time_test(void **state)
{
    conf.interface = EDC_IF_I2C;
    uint32_t timestamp = generate_random(0, UINT32_MAX-1);

    uint8_t cmd_arr[6] = {0xFF};
    uint16_t cmd_len = 5;

    cmd_arr[0] = 0x01;
    cmd_arr[1] = (uint8_t)(timestamp >> 0);
    cmd_arr[2] = (uint8_t)(timestamp >> 8);
    cmd_arr[3] = (uint8_t)(timestamp >> 16);
    cmd_arr[4] = (uint8_t)(timestamp >> 24);

    expect_value(__wrap_i2c_write, port, EDC_I2C_PORT);
    expect_value(__wrap_i2c_write, adr, EDC_I2C_ADR);
    expect_memory(__wrap_i2c_write, data, (void*)cmd_arr, cmd_len);
    expect_value(__wrap_i2c_write, len, cmd_len);

    will_return(__wrap_i2c_write, 0);

    assert_return_code(edc_set_rtc_time(conf, timestamp), 0);

    conf.interface = EDC_IF_UART;

    /* UART write */
    expect_value (__wrap_uart_write, port, EDC_UART_PORT);
    expect_memory(__wrap_uart_write, data, (void*)cmd_arr, cmd_len);
    expect_value (__wrap_uart_write, len, cmd_len);

    will_return(__wrap_uart_write, 0);

    assert_return_code(edc_set_rtc_time(conf, timestamp), 0);
}

static void edc_pop_ptt_pkg_test(void **state)
{
    conf.interface = EDC_IF_I2C;
    uint8_t cmd_arr[6] = {0xFF};
    uint16_t cmd_len = 1;

    cmd_arr[0] = 0x06;

    expect_value(__wrap_i2c_write, port, EDC_I2C_PORT);
    expect_value(__wrap_i2c_write, adr, EDC_I2C_ADR);
    expect_memory(__wrap_i2c_write, data, (void*)cmd_arr, cmd_len);
    expect_value(__wrap_i2c_write, len, cmd_len);

    will_return(__wrap_i2c_write, 0);

    assert_return_code(edc_pop_ptt_pkg(conf), 0);

    conf.interface = EDC_IF_UART;

    /* UART write */
    expect_value (__wrap_uart_write, port, EDC_UART_PORT);
    expect_memory(__wrap_uart_write, data, (void*)cmd_arr, cmd_len);
    expect_value (__wrap_uart_write, len, cmd_len);

    will_return(__wrap_uart_write, 0);

    assert_return_code(edc_pop_ptt_pkg(conf), 0);
}

static void edc_pause_ptt_task_test(void **state)
{
    conf.interface = EDC_IF_I2C;
    uint8_t cmd = 0x08;

    expect_value(__wrap_i2c_write, port, EDC_I2C_PORT);
    expect_value(__wrap_i2c_write, adr, EDC_I2C_ADR);
    expect_memory(__wrap_i2c_write, data, (void*)&cmd, 1);
    expect_value(__wrap_i2c_write, len, 1);

    will_return(__wrap_i2c_write, 0);

    assert_return_code(edc_pause_ptt_task(conf), 0);

    conf.interface = EDC_IF_UART;

    /* UART write */
    expect_value (__wrap_uart_write, port, EDC_UART_PORT);
    expect_memory(__wrap_uart_write, data, (void*)&cmd, 1);
    expect_value (__wrap_uart_write, len, 1);

    will_return(__wrap_uart_write, 0);

    assert_return_code(edc_pause_ptt_task(conf), 0);
}

static void edc_resume_ptt_task_test(void **state)
{
    uint8_t cmd = 0x09;

    conf.interface = EDC_IF_I2C;

    expect_value(__wrap_i2c_write, port, EDC_I2C_PORT);
    expect_value(__wrap_i2c_write, adr, EDC_I2C_ADR);
    expect_memory(__wrap_i2c_write, data, (void*)&cmd, 1);
    expect_value(__wrap_i2c_write, len, 1);

    will_return(__wrap_i2c_write, 0);

    assert_return_code(edc_resume_ptt_task(conf), 0);

    conf.interface = EDC_IF_UART;

    /* UART write */
    expect_value (__wrap_uart_write, port, EDC_UART_PORT);
    expect_memory(__wrap_uart_write, data, (void*)&cmd, 1);
    expect_value (__wrap_uart_write, len, 1);

    will_return(__wrap_uart_write, 0);

    assert_return_code(edc_resume_ptt_task(conf), 0);
}

static void edc_start_adc_task_test(void **state)
{
    uint8_t cmd = 0x0A;

    conf.interface = EDC_IF_I2C;

    expect_value(__wrap_i2c_write, port, EDC_I2C_PORT);
    expect_value(__wrap_i2c_write, adr, EDC_I2C_ADR);
    expect_memory(__wrap_i2c_write, data, (void*)&cmd, 1);
    expect_value(__wrap_i2c_write, len, 1);

    will_return(__wrap_i2c_write, 0);

    assert_return_code(edc_start_adc_task(conf), 0);

    conf.interface = EDC_IF_UART;

    /* UART write */
    expect_value (__wrap_uart_write, port, EDC_UART_PORT);
    expect_memory(__wrap_uart_write, data, (void*)&cmd, 1);
    expect_value (__wrap_uart_write, len, 1);

    will_return(__wrap_uart_write, 0);

    assert_return_code(edc_start_adc_task(conf), 0);
}

static void edc_get_state_pkg_test(void **state)
{
    /* Write command */
    uint8_t cmd = 0x30;

    conf.interface = EDC_IF_I2C;

    expect_value(__wrap_i2c_write, port, EDC_I2C_PORT);
    expect_value(__wrap_i2c_write, adr, EDC_I2C_ADR);
    expect_memory(__wrap_i2c_write, data, (void*)&cmd, 1);
    expect_value(__wrap_i2c_write, len, 1);

    will_return(__wrap_i2c_write, 0);

    /* Read */
    uint8_t data[256] = {0xFF};

    expect_value(__wrap_i2c_read, port, EDC_I2C_PORT);
    expect_value(__wrap_i2c_read, adr, EDC_I2C_ADR);
    expect_value(__wrap_i2c_read, len, 9);

    data[0] = 0x11;
    will_return(__wrap_i2c_read, 0x11);

    uint16_t i = 0;
    for(i=1; i<9; i++)
    {
        data[i] = generate_random(0, 255);
        will_return(__wrap_i2c_read, data[i]);
    }

    will_return(__wrap_i2c_read, 0);

    uint8_t status[256] = {0xFF};

    assert_int_equal(edc_get_state_pkg(conf, status), 9);

    for(i=0; i<9; i++)
    {
        assert_int_equal(status[i], data[i]);
    }

    conf.interface = EDC_IF_UART;

    /* Uart Write */
    expect_value (__wrap_uart_write, port, EDC_UART_PORT);
    expect_memory(__wrap_uart_write, data, (void*)&cmd, 1);
    expect_value (__wrap_uart_write, len, 1);

    will_return(__wrap_uart_write, 0);

    /* Uart read available */
    expect_value (__wrap_uart_read_available, port, EDC_UART_PORT);

    will_return(__wrap_uart_read_available, 9);

    /* UART read */
    expect_value(__wrap_uart_read, port, EDC_UART_PORT);
    expect_value(__wrap_uart_read, len, 9);

    will_return(__wrap_uart_read, 0x11);

    i = 0;
    for(i=1; i<9; i++)
    {
        will_return(__wrap_uart_read, data[i]);
    }

    will_return(__wrap_uart_read, 0);

    assert_int_equal(edc_get_state_pkg(conf, status), 9);

    for(i=0; i<9; i++)
    {
        assert_int_equal(status[i], data[i]);
    }
}

static void edc_get_ptt_pkg_test(void **state)
{
    /* Write command */
    uint8_t cmd[2] = {0x31, 0};

    conf.interface = EDC_IF_I2C;

    expect_value(__wrap_i2c_write, port, EDC_I2C_PORT);
    expect_value(__wrap_i2c_write, adr, EDC_I2C_ADR);
    expect_memory(__wrap_i2c_write, data, (void*)cmd, 2);
    expect_value(__wrap_i2c_write, len, 2);

    will_return(__wrap_i2c_write, 0);

    /* Read */
    uint8_t data[256] = {0xFF};

    expect_value(__wrap_i2c_read, port, EDC_I2C_PORT);
    expect_value(__wrap_i2c_read, adr, EDC_I2C_ADR);
    expect_value(__wrap_i2c_read, len, 49);

    data[0] = 0x22;
    will_return(__wrap_i2c_read, 0x22);

    uint16_t i = 0;
    for(i=1; i<49; i++)
    {
        data[i] = generate_random(0, 255);
        will_return(__wrap_i2c_read, data[i]);
    }

    will_return(__wrap_i2c_read, 0);

    uint8_t ptt[256] = {0xFF};

    assert_int_equal(edc_get_ptt_pkg(conf, ptt), 49);

    for(i=0; i<49; i++)
    {
        assert_int_equal(ptt[i], data[i]);
    }

    conf.interface = EDC_IF_UART;

    /* Uart Write */
    expect_value (__wrap_uart_write, port, EDC_UART_PORT);
    expect_memory(__wrap_uart_write, data, (void*)cmd, 2);
    expect_value (__wrap_uart_write, len, 2);

    will_return(__wrap_uart_write, 0);

    /* Uart read available */
    expect_value (__wrap_uart_read_available, port, EDC_UART_PORT);

    will_return(__wrap_uart_read_available, 49);

    /* UART read */
    expect_value(__wrap_uart_read, port, EDC_UART_PORT);
    expect_value(__wrap_uart_read, len, 49);

    will_return(__wrap_uart_read, 0x22);

    i = 0;
    for(i=1; i<49; i++)
    {
        will_return(__wrap_uart_read, data[i]);
    }

    will_return(__wrap_uart_read, 0);

    assert_int_equal(edc_get_ptt_pkg(conf, ptt), 49);

    for(i=0; i<49; i++)
    {
        assert_int_equal(ptt[i], data[i]);
    }
}

static void edc_get_hk_pkg_test(void **state)
{
    /* Write command */
    uint8_t cmd = 0x32;

    conf.interface = EDC_IF_I2C;

    expect_value(__wrap_i2c_write, port, EDC_I2C_PORT);
    expect_value(__wrap_i2c_write, adr, EDC_I2C_ADR);
    expect_memory(__wrap_i2c_write, data, (void*)&cmd, 1);
    expect_value(__wrap_i2c_write, len, 1);

    will_return(__wrap_i2c_write, 0);

    /* Read */
    uint8_t data[256] = {0xFF};

    expect_value(__wrap_i2c_read, port, EDC_I2C_PORT);
    expect_value(__wrap_i2c_read, adr, EDC_I2C_ADR);
    expect_value(__wrap_i2c_read, len, 21);

    data[0] = 0x44;
    will_return(__wrap_i2c_read, 0x44);

    uint16_t i = 0;
    for(i=1; i<21; i++)
    {
        data[i] = generate_random(0, 255);
        will_return(__wrap_i2c_read, data[i]);
    }

    will_return(__wrap_i2c_read, 0);

    uint8_t hk[256] = {0xFF};

    assert_int_equal(edc_get_hk_pkg(conf, hk), 21);

    for(i=0; i<21; i++)
    {
        assert_int_equal(hk[i], data[i]);
    }

    conf.interface = EDC_IF_UART;

    /* Uart Write */
    expect_value (__wrap_uart_write, port, EDC_UART_PORT);
    expect_memory(__wrap_uart_write, data, (void*)&cmd, 1);
    expect_value (__wrap_uart_write, len, 1);

    will_return(__wrap_uart_write, 0);

    /* Uart read available */
    expect_value (__wrap_uart_read_available, port, EDC_UART_PORT);

    will_return(__wrap_uart_read_available, 21);

    /* UART read */
    expect_value(__wrap_uart_read, port, EDC_UART_PORT);
    expect_value(__wrap_uart_read, len, 21);

    will_return(__wrap_uart_read, 0x44);

    i = 0;
    for(i=1; i<21; i++)
    {
        will_return(__wrap_uart_read, data[i]);
    }

    will_return(__wrap_uart_read, 0);

    assert_int_equal(edc_get_hk_pkg(conf, hk), 21);

    for(i=0; i<21; i++)
    {
        assert_int_equal(hk[i], data[i]);
    }
}

static void edc_get_adc_seq_test(void **state)
{
    /* Write command */
    uint8_t cmd[3] = {0x34, 0, 0};

    conf.interface = EDC_IF_I2C;

    expect_value(__wrap_i2c_write, port, EDC_I2C_PORT);
    expect_value(__wrap_i2c_write, adr, EDC_I2C_ADR);
    expect_memory(__wrap_i2c_write, data, (void*)&cmd, 3);
    expect_value(__wrap_i2c_write, len, 3);

    will_return(__wrap_i2c_write, 0);

    /* Read */
    uint8_t data[8199] = {0xFF};

    expect_value(__wrap_i2c_read, port, EDC_I2C_PORT);
    expect_value(__wrap_i2c_read, adr, EDC_I2C_ADR);
    expect_value(__wrap_i2c_read, len, 8199);

    data[0] = 0x33;
    will_return(__wrap_i2c_read, 0x33);

    uint16_t i = 0;
    for(i=1; i<8199; i++)
    {
        data[i] = generate_random(0, 255);
        will_return(__wrap_i2c_read, data[i]);
    }

    will_return(__wrap_i2c_read, 0);

    uint8_t seq[8199] = {0xFF};

    assert_int_equal(edc_get_adc_seq(conf, seq), 8199);

    for(i=0; i<8199; i++)
    {
        assert_int_equal(seq[i], data[i]);
    }

    conf.interface = EDC_IF_UART;

    /* Uart Write */
    expect_value (__wrap_uart_write, port, EDC_UART_PORT);
    expect_memory(__wrap_uart_write, data, (void*)&cmd, 3);
    expect_value (__wrap_uart_write, len, 3);

    will_return(__wrap_uart_write, 0);

    /* Uart read available */
    expect_value (__wrap_uart_read_available, port, EDC_UART_PORT);

    will_return(__wrap_uart_read_available, 8199);

    /* UART read */
    expect_value(__wrap_uart_read, port, EDC_UART_PORT);
    expect_value(__wrap_uart_read, len, 8199);

    will_return(__wrap_uart_read, 0x33);

    i = 0;
    for(i=1; i<8199; i++)
    {
        will_return(__wrap_uart_read, data[i]);
    }

    will_return(__wrap_uart_read, 0);

    assert_int_equal(edc_get_adc_seq(conf, seq), 8199);

    for(i=0; i<8199; i++)
    {
        assert_int_equal(seq[i], data[i]);
    }
}

static void edc_echo_test(void **state)
{
    uint8_t cmd = 0xF0;

    conf.interface = EDC_IF_I2C;

    expect_value(__wrap_i2c_write, port, EDC_I2C_PORT);
    expect_value(__wrap_i2c_write, adr, EDC_I2C_ADR);
    expect_memory(__wrap_i2c_write, data, (void*)&cmd, 1);
    expect_value(__wrap_i2c_write, len, 1);

    will_return(__wrap_i2c_write, 0);

    assert_return_code(edc_echo(conf), 0);

    conf.interface = EDC_IF_UART;

    /* Uart Write */
    expect_value (__wrap_uart_write, port, EDC_UART_PORT);
    expect_memory(__wrap_uart_write, data, (void*)&cmd, 1);
    expect_value (__wrap_uart_write, len, 1);

    will_return(__wrap_uart_write, 0);

    assert_return_code(edc_echo(conf), 0);
}

static void edc_calc_checksum_test(void **state)
{
    uint16_t i = 0;
    for(i=0; i<UINT8_MAX; i++)
    {
        uint8_t data[256] = {0xFF};
        uint16_t data_len = generate_random(1, 256);

        uint16_t j = 0;
        for(j=0; j<data_len; j++)
        {
            data[j] = generate_random(0, 255);
        }

        uint16_t checksum = 0;

        for(j=0; j<data_len; j++)
        {
            checksum ^= data[j];
        }

        assert_int_equal(edc_calc_checksum(data, data_len), checksum);
    }
}

static void edc_get_state_test(void **state)
{
    /* Write command */
    uint8_t cmd = 0x30;

    conf.interface = EDC_IF_I2C;

    expect_value(__wrap_i2c_write, port, EDC_I2C_PORT);
    expect_value(__wrap_i2c_write, adr, EDC_I2C_ADR);
    expect_memory(__wrap_i2c_write, data, (void*)&cmd, 1);
    expect_value(__wrap_i2c_write, len, 1);

    will_return(__wrap_i2c_write, 0);

    /* Read */
    uint8_t data[256] = {0xFF};

    expect_value(__wrap_i2c_read, port, EDC_I2C_PORT);
    expect_value(__wrap_i2c_read, adr, EDC_I2C_ADR);
    expect_value(__wrap_i2c_read, len, 9);

    uint16_t i = 0;
    uint8_t checksum = 0;
    for(i=0; i<8; i++)
    {
        if (i == 0)
        {
            data[i] = 0x11;
        }
        else
        {
            data[i] = generate_random(0, 255);
        }

        will_return(__wrap_i2c_read, data[i]);

        checksum ^= data[i];
    }

    data[i] = checksum;
    will_return(__wrap_i2c_read, data[i]);

    will_return(__wrap_i2c_read, 0);

    edc_state_t st = {0};

    assert_return_code(edc_get_state(conf, &st), 0);

    conf.interface = EDC_IF_UART;

    /* Uart Write */
    expect_value (__wrap_uart_write, port, EDC_UART_PORT);
    expect_memory(__wrap_uart_write, data, (void*)&cmd, 1);
    expect_value (__wrap_uart_write, len, 1);

    will_return(__wrap_uart_write, 0);

    /* Uart read available */
    expect_value (__wrap_uart_read_available, port, EDC_UART_PORT);

    will_return(__wrap_uart_read_available, 9);

    /* UART read */
    expect_value(__wrap_uart_read, port, EDC_UART_PORT);
    expect_value(__wrap_uart_read, len, 9);

    will_return(__wrap_uart_read, 0x11);

    i = 0;
    for(i=1; i<8; i++)
    {
        will_return(__wrap_uart_read, data[i]);
    }

    will_return(__wrap_uart_read, data[i]);

    will_return(__wrap_uart_read, 0);

    assert_return_code(edc_get_state(conf, &st), 0);
}

static void edc_get_ptt_test(void **state)
{
    /* Write command */
    uint8_t cmd[2] = {0x31, 0};

    conf.interface = EDC_IF_I2C;

    expect_value(__wrap_i2c_write, port, EDC_I2C_PORT);
    expect_value(__wrap_i2c_write, adr, EDC_I2C_ADR);
    expect_memory(__wrap_i2c_write, data, (void*)cmd, 2);
    expect_value(__wrap_i2c_write, len, 2);

    will_return(__wrap_i2c_write, 0);

    /* Read */
    uint8_t data[256] = {0xFF};

    expect_value(__wrap_i2c_read, port, EDC_I2C_PORT);
    expect_value(__wrap_i2c_read, adr, EDC_I2C_ADR);
    expect_value(__wrap_i2c_read, len, 49);

    uint16_t i = 0;
    uint8_t checksum = 0;
    for(i=0; i<49-1; i++)
    {
        if (i == 0)
        {
            data[i] = 0x22;
        }
        else
        {
            data[i] = generate_random(0, 255);
        }

        will_return(__wrap_i2c_read, data[i]);

        checksum ^= data[i];
    }

    data[i] = checksum;
    will_return(__wrap_i2c_read, data[i]);

    will_return(__wrap_i2c_read, 0);

    edc_ptt_t ptt = {0};

    assert_return_code(edc_get_ptt(conf, &ptt), 0);

    assert_int_equal(ptt.time_tag, ((uint32_t)data[4] << 24) | ((uint32_t)data[3] << 16) | ((uint32_t)data[2] << 8) | ((uint32_t)data[1] << 0));
    assert_int_equal(ptt.error_code, data[5]);
    int32_t carrier_freq = ((uint32_t)data[9] << 24) | ((uint32_t)data[8] << 16) | ((uint32_t)data[7] << 8) | ((uint32_t)data[6] << 0);
    assert_int_equal(ptt.carrier_freq, carrier_freq*128/pow(2, 11) + 401635);
    assert_int_equal(ptt.carrier_abs, ((uint16_t)data[11] << 8) | ((uint16_t)data[10] << 0));
    assert_int_equal(ptt.msg_byte_length, data[12]);

    for(i=13; i<49; i++)
    {
        assert_int_equal(ptt.user_msg[i-13], data[i]);
    }

    conf.interface = EDC_IF_UART;

    /* Uart Write */
    expect_value (__wrap_uart_write, port, EDC_UART_PORT);
    expect_memory(__wrap_uart_write, data, (void*)cmd, 2);
    expect_value (__wrap_uart_write, len, 2);

    will_return(__wrap_uart_write, 0);

    /* Uart read available */
    expect_value (__wrap_uart_read_available, port, EDC_UART_PORT);

    will_return(__wrap_uart_read_available, 49);

    /* UART read */
    expect_value(__wrap_uart_read, port, EDC_UART_PORT);
    expect_value(__wrap_uart_read, len, 49);

    will_return(__wrap_uart_read, 0x22);

    i = 0;
    for(i=1; i<49-1; i++)
    {
        will_return(__wrap_uart_read, data[i]);
    }

    will_return(__wrap_uart_read, data[i]);

    will_return(__wrap_uart_read, 0);

    assert_return_code(edc_get_ptt(conf, &ptt), 0);

    assert_int_equal(ptt.time_tag, ((uint32_t)data[4] << 24) | ((uint32_t)data[3] << 16) | ((uint32_t)data[2] << 8) | ((uint32_t)data[1] << 0));
    assert_int_equal(ptt.error_code, data[5]);
    assert_int_equal(ptt.carrier_freq, carrier_freq*128/pow(2, 11) + 401635);
    assert_int_equal(ptt.carrier_abs, ((uint16_t)data[11] << 8) | ((uint16_t)data[10] << 0));
    assert_int_equal(ptt.msg_byte_length, data[12]);

    for(i=13; i<49; i++)
    {
        assert_int_equal(ptt.user_msg[i-13], data[i]);
    }
}

static void edc_get_hk_test(void **state)
{
    /* Write command */
    uint8_t cmd = 0x32;

    conf.interface = EDC_IF_I2C;

    expect_value(__wrap_i2c_write, port, EDC_I2C_PORT);
    expect_value(__wrap_i2c_write, adr, EDC_I2C_ADR);
    expect_memory(__wrap_i2c_write, data, (void*)&cmd, 1);
    expect_value(__wrap_i2c_write, len, 1);

    will_return(__wrap_i2c_write, 0);

    /* Read */
    uint8_t data[256] = {0xFF};

    expect_value(__wrap_i2c_read, port, EDC_I2C_PORT);
    expect_value(__wrap_i2c_read, adr, EDC_I2C_ADR);
    expect_value(__wrap_i2c_read, len, 21);

    uint16_t i = 0;
    uint8_t checksum = 0;
    for(i=0; i<20; i++)
    {
        if (i == 0)
        {
            data[i] = 0x44;
        }
        else
        {
            data[i] = generate_random(0, 255);
        }

        will_return(__wrap_i2c_read, data[i]);

        checksum ^= data[i];
    }

    data[i] = checksum;
    will_return(__wrap_i2c_read, data[i]);

    will_return(__wrap_i2c_read, 0);

    edc_hk_t hk_data = {0};

    assert_return_code(edc_get_hk(conf, &hk_data), 0);

    assert_int_equal(hk_data.current_time, ((uint32_t)data[4] << 24) | ((uint32_t)data[3] << 16) | ((uint32_t)data[2] << 8) | ((uint32_t)data[1] << 0));
    assert_int_equal(hk_data.elapsed_time, ((uint32_t)data[8] << 24) | ((uint32_t)data[7] << 16) | ((uint32_t)data[6] << 8) | ((uint32_t)data[5] << 0));
    assert_int_equal(hk_data.current_supply, ((uint32_t)data[10] << 8) | ((uint32_t)data[9] << 0));
    assert_int_equal(hk_data.voltage_supply, ((uint32_t)data[12] << 8) | ((uint32_t)data[11] << 0));
    assert_int_equal(hk_data.temp, (int8_t)data[13] - 40);
    assert_int_equal(hk_data.pll_sync_bit, data[14]);
    assert_int_equal(hk_data.adc_rms, (int16_t)(((uint32_t)data[16] << 8) | ((uint32_t)data[15] << 0)));
    assert_int_equal(hk_data.num_rx_ptt, data[17]);
    assert_int_equal(hk_data.max_parl_decod, data[18]);
    assert_int_equal(hk_data.mem_err_count, data[19]);

    conf.interface = EDC_IF_UART;

    /* Uart Write */
    expect_value (__wrap_uart_write, port, EDC_UART_PORT);
    expect_memory(__wrap_uart_write, data, (void*)&cmd, 1);
    expect_value (__wrap_uart_write, len, 1);

    will_return(__wrap_uart_write, 0);

    /* Uart read available */
    expect_value (__wrap_uart_read_available, port, EDC_UART_PORT);

    will_return(__wrap_uart_read_available, 21);

    /* UART read */
    expect_value(__wrap_uart_read, port, EDC_UART_PORT);
    expect_value(__wrap_uart_read, len, 21);

    will_return(__wrap_uart_read, 0x44);

    i = 0;
    for(i=1; i<20; i++)
    {
        will_return(__wrap_uart_read, data[i]);
    }

    will_return(__wrap_uart_read, data[i]);

    will_return(__wrap_uart_read, 0);

    assert_return_code(edc_get_hk(conf, &hk_data), 0);

    assert_int_equal(hk_data.current_time, ((uint32_t)data[4] << 24) | ((uint32_t)data[3] << 16) | ((uint32_t)data[2] << 8) | ((uint32_t)data[1] << 0));
    assert_int_equal(hk_data.elapsed_time, ((uint32_t)data[8] << 24) | ((uint32_t)data[7] << 16) | ((uint32_t)data[6] << 8) | ((uint32_t)data[5] << 0));
    assert_int_equal(hk_data.current_supply, ((uint32_t)data[10] << 8) | ((uint32_t)data[9] << 0));
    assert_int_equal(hk_data.voltage_supply, ((uint32_t)data[12] << 8) | ((uint32_t)data[11] << 0));
    assert_int_equal(hk_data.temp, (int8_t)data[13] - 40);
    assert_int_equal(hk_data.pll_sync_bit, data[14]);
    assert_int_equal(hk_data.adc_rms, (int16_t)(((uint32_t)data[16] << 8) | ((uint32_t)data[15] << 0)));
    assert_int_equal(hk_data.num_rx_ptt, data[17]);
    assert_int_equal(hk_data.max_parl_decod, data[18]);
    assert_int_equal(hk_data.mem_err_count, data[19]);
}

static void edc_i2c_init_test(void **state)
{
    expect_value(__wrap_i2c_init, port, EDC_I2C_PORT);
    expect_value(__wrap_i2c_init, config.speed_hz, EDC_I2C_CLOCK);

    will_return(__wrap_i2c_init, 0);

    assert_return_code(edc_i2c_init(conf), 0);
}

static void edc_i2c_write_test(void **state)
{
    uint16_t cmd_len = generate_random(1, 256);
    uint8_t cmd[256] = {0};

    uint16_t i = 0;
    for(i = 0; i < cmd_len; i++)
    {
        cmd[i] = generate_random(0, UINT8_MAX);
    }

    expect_value(__wrap_i2c_write, port, EDC_I2C_PORT);
    expect_value(__wrap_i2c_write, adr, EDC_I2C_ADR);
    expect_memory(__wrap_i2c_write, data, (void*)cmd, cmd_len);
    expect_value(__wrap_i2c_write, len, cmd_len);

    will_return(__wrap_i2c_write, 0);

    assert_return_code(edc_i2c_write(conf, cmd, cmd_len), 0);
}

static void edc_i2c_read_test(void **state)
{
    uint16_t ans_len = generate_random(1, UINT8_MAX);

    expect_value(__wrap_i2c_read, port, EDC_I2C_PORT);
    expect_value(__wrap_i2c_read, adr, EDC_I2C_ADR);
    expect_value(__wrap_i2c_read, len, ans_len);

    uint8_t ans[256] = {0};

    uint16_t i = 0;
    for(i = 0; i < ans_len; i++)
    {
        ans[i] = generate_random(0, UINT8_MAX);
        will_return(__wrap_i2c_read, ans[i]);
    }

    will_return(__wrap_i2c_read, 0);

    uint8_t data[256] = {0};

    assert_return_code(edc_i2c_read(conf, data, ans_len), 0);

    assert_memory_equal((void*)ans, (void*)data, ans_len);
}

static void edc_gpio_init_test(void **state)
{
    expect_value(__wrap_gpio_init, pin, EDC_GPIO_EN_PIN);
    expect_value(__wrap_gpio_init, config.mode, GPIO_MODE_OUTPUT);

    will_return(__wrap_gpio_init, 0);

    assert_return_code(edc_gpio_init(conf), 0);
}

static void edc_gpio_set_test(void **state)
{
    expect_value(__wrap_gpio_set_state, pin, EDC_GPIO_EN_PIN);
    expect_value(__wrap_gpio_set_state, level, true);

    will_return(__wrap_gpio_set_state, 0);

    assert_return_code(edc_gpio_set(conf), 0);
}

static void edc_gpio_clear_test(void **state)
{
    expect_value(__wrap_gpio_set_state, pin, EDC_GPIO_EN_PIN);
    expect_value(__wrap_gpio_set_state, level, false);

    will_return(__wrap_gpio_set_state, 0);

    assert_return_code(edc_gpio_clear(conf), 0);
}

static void edc_uart_init_test(void **state)
{
    expect_value(__wrap_uart_init, port, EDC_UART_PORT);
    expect_value(__wrap_uart_init, config.baudrate, EDC_UART_BAUD_RATE);
    expect_value(__wrap_uart_init, config.data_bits, EDC_UART_DATA_BITS);
    expect_value(__wrap_uart_init, config.parity, EDC_UART_PARITY);
    expect_value(__wrap_uart_init, config.stop_bits, EDC_UART_STOP_BITS);

    will_return(__wrap_uart_init, 0);

    /* Uart rx enable */
    expect_value (__wrap_uart_rx_enable, port, EDC_UART_PORT);

    will_return(__wrap_uart_rx_enable, 0);

    assert_return_code(edc_uart_init(conf), 0);
}

static void edc_uart_write_test(void **state)
{
    uint16_t cmd_len = generate_random(1, 256);
    uint8_t cmd[256] = {0};

    uint16_t i = 0;
    for(i = 0; i < cmd_len; i++)
    {
        cmd[i] = generate_random(0, UINT8_MAX);
    }

    expect_value (__wrap_uart_write, port, EDC_UART_PORT);
    expect_memory(__wrap_uart_write, data, (void*)cmd, cmd_len);
    expect_value (__wrap_uart_write, len, cmd_len);

    will_return(__wrap_uart_write, 0);

    assert_return_code(edc_uart_write(conf, cmd, cmd_len), 0);
}

static void edc_uart_read_test(void **state)
{
    uint16_t ans_len = generate_random(1, UINT8_MAX);

    expect_value(__wrap_uart_read, port, EDC_UART_PORT);
    expect_value(__wrap_uart_read, len, ans_len);

    uint8_t ans[256] = {0};

    uint16_t i = 0;
    for(i = 0; i < ans_len; i++)
    {
        ans[i] = generate_random(0, UINT8_MAX);
        will_return(__wrap_uart_read, ans[i]);
    }

    will_return(__wrap_uart_read, 0);

    uint8_t data[256] = {0};

    assert_return_code(edc_uart_read(conf, data, ans_len), 0);

    assert_memory_equal((void*)ans, (void*)data, ans_len);
}

int main(void)
{
    conf.i2c_port     = EDC_I2C_PORT;
    conf.i2c_bitrate  = EDC_I2C_CLOCK;
    conf.en_pin       = EDC_GPIO_EN_PIN;
    conf.uart_port    = EDC_UART_PORT;

    const struct CMUnitTest edc_tests[] = {
        cmocka_unit_test(edc_init_test),
        cmocka_unit_test(edc_enable_test),
        cmocka_unit_test(edc_disable_test),
        cmocka_unit_test(edc_write_cmd_test),
        cmocka_unit_test(edc_read_test),
        cmocka_unit_test(edc_check_device_test),
        cmocka_unit_test(edc_set_rtc_time_test),
        cmocka_unit_test(edc_pop_ptt_pkg_test),
        cmocka_unit_test(edc_pause_ptt_task_test),
        cmocka_unit_test(edc_resume_ptt_task_test),
        cmocka_unit_test(edc_start_adc_task_test),
        cmocka_unit_test(edc_get_state_pkg_test),
        cmocka_unit_test(edc_get_ptt_pkg_test),
        cmocka_unit_test(edc_get_hk_pkg_test),
        cmocka_unit_test(edc_get_adc_seq_test),
        cmocka_unit_test(edc_echo_test),
        cmocka_unit_test(edc_calc_checksum_test),
        cmocka_unit_test(edc_get_state_test),
        cmocka_unit_test(edc_get_ptt_test),
        cmocka_unit_test(edc_get_hk_test),
        cmocka_unit_test(edc_i2c_init_test),
        cmocka_unit_test(edc_i2c_write_test),
        cmocka_unit_test(edc_i2c_read_test),
        cmocka_unit_test(edc_gpio_init_test),
        cmocka_unit_test(edc_gpio_set_test),
        cmocka_unit_test(edc_gpio_clear_test),
        cmocka_unit_test(edc_uart_init_test),
        cmocka_unit_test(edc_uart_write_test),
        cmocka_unit_test(edc_uart_read_test),
    };

    return cmocka_run_group_tests(edc_tests, NULL, NULL);
}

unsigned int generate_random(unsigned int l, unsigned int r)
{
    return (rand() % (r - l + 1)) + l;
}

/** \} End of edc_test group */
