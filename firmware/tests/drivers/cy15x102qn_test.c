/*
 * cy15x102qn_test.c
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
 * \brief Unit test of the CY15x102QN driver.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.10.17
 * 
 * \date 2021/08/29
 * 
 * \defgroup cy15x102qn_unit_test CY15x102QN
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

#include <config/config.h>
#include <drivers/spi/spi.h>
#include <drivers/gpio/gpio.h>
#include <drivers/cy15x102qn/cy15x102qn.h>

#define CY15X102QN_SPI_PORT         SPI_PORT_0
#define CY15X102QN_SPI_CS           SPI_CS_5
#define CY15X102QN_SPI_CLOCK_HZ     1000000UL
#define CY15X102QN_SPI_MODE         SPI_MODE_0
#define CY15X102QN_GPIO_WP_PIN      GPIO_PIN_62
#define CY15X102QN_MAX_ADDRESS      0x0003FFFF

cy15x102qn_config_t conf = {0};

unsigned int generate_random(unsigned int l, unsigned int r);

static void cy15x102qn_init_test(void **state)
{
    /* SPI init */
    expect_value(__wrap_spi_init, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_init, config.speed_hz, CY15X102QN_SPI_CLOCK_HZ);
    expect_value(__wrap_spi_init, config.mode, CY15X102QN_SPI_MODE);

    will_return(__wrap_spi_init, 0);

    /* GPIO init */
    expect_value(__wrap_gpio_init, pin, CY15X102QN_GPIO_WP_PIN);
    expect_value(__wrap_gpio_init, config.mode, GPIO_MODE_OUTPUT);

    will_return(__wrap_gpio_init, 0);

    /* GPIO set state */
    expect_value(__wrap_gpio_set_state, pin, CY15X102QN_GPIO_WP_PIN);
    expect_value(__wrap_gpio_set_state, level, true);

    will_return(__wrap_gpio_set_state, 0);

    assert_return_code(cy15x102qn_init(&conf), 0);
}

static void cy15x102qn_set_write_enable_test(void **state)
{
    uint8_t cmd = CY15X102QN_OPCODE_WREN;

    expect_value(__wrap_spi_write, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_write, cs, CY15X102QN_SPI_CS);
    expect_memory(__wrap_spi_write, data, (void*)&cmd, 1);
    expect_value(__wrap_spi_write, len, 1);

    will_return(__wrap_spi_write, 0);

    assert_return_code(cy15x102qn_set_write_enable(&conf), 0);
}

static void cy15x102qn_reset_write_enable_test(void **state)
{
    uint8_t cmd = CY15X102QN_OPCODE_WRDI;

    expect_value(__wrap_spi_write, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_write, cs, CY15X102QN_SPI_CS);
    expect_memory(__wrap_spi_write, data, (void*)&cmd, 1);
    expect_value(__wrap_spi_write, len, 1);

    will_return(__wrap_spi_write, 0);

    assert_return_code(cy15x102qn_reset_write_enable(&conf), 0);
}

static void cy15x102qn_read_status_reg_test(void **state)
{
    uint8_t cmd[2] = {CY15X102QN_OPCODE_RDSR, 0};
    uint8_t ans[2] = {0, 0x12};

    expect_value(__wrap_spi_transfer, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_transfer, cs, CY15X102QN_SPI_CS);
    expect_memory(__wrap_spi_transfer, wd, (void*)cmd, 2);
    expect_value(__wrap_spi_transfer, len, 2);

    will_return(__wrap_spi_transfer, ans[0]);
    will_return(__wrap_spi_transfer, ans[1]);
    will_return(__wrap_spi_transfer, 0);

    cy15x102qn_status_t stat = 0;

    assert_return_code(cy15x102qn_read_status_reg(&conf, &stat), 0);
    assert_int_equal(stat, ans[1]);
}

static void cy15x102qn_write_status_reg_test(void **state)
{
    uint8_t cmd[2] = {CY15X102QN_OPCODE_WRSR, 0x12};

    expect_value(__wrap_spi_write, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_write, cs, CY15X102QN_SPI_CS);
    expect_memory(__wrap_spi_write, data, (void*)cmd, 2);
    expect_value(__wrap_spi_write, len, 2);

    will_return(__wrap_spi_write, 0);

    cy15x102qn_status_t stat = 0x12;

    assert_return_code(cy15x102qn_write_status_reg(&conf, stat), 0);
}

static void cy15x102qn_write_test(void **state)
{
    will_return(__wrap_spi_mutex_take, 0);

    /* Select device */
    expect_value(__wrap_spi_select_slave, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_select_slave, cs, CY15X102QN_SPI_CS);
    expect_value(__wrap_spi_select_slave, active, true);

    will_return(__wrap_spi_select_slave, 0);

    /* Write opcode */
    uint8_t cmd = CY15X102QN_OPCODE_WRITE;

    expect_value(__wrap_spi_write, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SPI_CS_NONE);
    expect_memory(__wrap_spi_write, data, (void*)&cmd, 1);
    expect_value(__wrap_spi_write, len, 1);

    will_return(__wrap_spi_write, 0);

    /* Write address */
    uint32_t adr = generate_random(0, CY15X102QN_MAX_ADDRESS);

    uint8_t adr_arr[3] = {0x00};

    adr_arr[0] = (uint8_t)(adr >> 16) & 0xFF;
    adr_arr[1] = (uint8_t)(adr >> 8) & 0xFF;
    adr_arr[2] = (uint8_t)(adr >> 0) & 0xFF;

    expect_value(__wrap_spi_write, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SPI_CS_NONE);
    expect_memory(__wrap_spi_write, data, (void*)adr_arr, 3);
    expect_value(__wrap_spi_write, len, 3);

    will_return(__wrap_spi_write, 0);

    /* Write data */
    uint8_t data[256] = {0xFF};
    uint16_t data_len = generate_random(1, 256);

    uint16_t i = 0;
    for(i=0; i<data_len; i++)
    {
        data[i] = generate_random(0, 255);
    }

    expect_value(__wrap_spi_write, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SPI_CS_NONE);
    expect_memory(__wrap_spi_write, data, (void*)data, data_len);
    expect_value(__wrap_spi_write, len, data_len);

    will_return(__wrap_spi_write, 0);

    /* Unselect device */
    expect_value(__wrap_spi_select_slave, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_select_slave, cs, CY15X102QN_SPI_CS);
    expect_value(__wrap_spi_select_slave, active, false);

    will_return(__wrap_spi_select_slave, 0);

    will_return(__wrap_spi_mutex_give, 0);

    assert_return_code(cy15x102qn_write(&conf, adr, data, data_len), 0);
}

static void cy15x102qn_read_test(void **state)
{
    will_return(__wrap_spi_mutex_take, 0);

    /* Select device */
    expect_value(__wrap_spi_select_slave, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_select_slave, cs, CY15X102QN_SPI_CS);
    expect_value(__wrap_spi_select_slave, active, true);

    will_return(__wrap_spi_select_slave, 0);

    /* Write opcode */
    uint8_t cmd = CY15X102QN_OPCODE_READ;

    expect_value(__wrap_spi_write, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SPI_CS_NONE);
    expect_memory(__wrap_spi_write, data, (void*)&cmd, 1);
    expect_value(__wrap_spi_write, len, 1);

    will_return(__wrap_spi_write, 0);

    /* Write address */
    uint32_t adr = generate_random(0, CY15X102QN_MAX_ADDRESS);

    uint8_t adr_arr[3] = {0x00};

    adr_arr[0] = (uint8_t)(adr >> 16) & 0xFF;
    adr_arr[1] = (uint8_t)(adr >> 8) & 0xFF;
    adr_arr[2] = (uint8_t)(adr >> 0) & 0xFF;

    expect_value(__wrap_spi_write, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SPI_CS_NONE);
    expect_memory(__wrap_spi_write, data, (void*)adr_arr, 3);
    expect_value(__wrap_spi_write, len, 3);

    will_return(__wrap_spi_write, 0);

    /* Read data */
    uint8_t data[256] = {0xFF};
    uint16_t data_len = generate_random(1, 256);

    expect_value(__wrap_spi_read, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_read, cs, SPI_CS_NONE);
    expect_value(__wrap_spi_read, len, data_len);

    uint16_t i = 0;
    for(i=0; i<data_len; i++)
    {
        data[i] = generate_random(0, 255);
        will_return(__wrap_spi_read, data[i]);
    }

    will_return(__wrap_spi_read, 0);

    /* Unselect device */
    expect_value(__wrap_spi_select_slave, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_select_slave, cs, CY15X102QN_SPI_CS);
    expect_value(__wrap_spi_select_slave, active, false);

    will_return(__wrap_spi_select_slave, 0);

    will_return(__wrap_spi_mutex_give, 0);

    uint8_t data_res[256] = {0xFF};

    assert_return_code(cy15x102qn_read(&conf, adr, data_res, data_len), 0);

    for(i=0; i<data_len; i++)
    {
        assert_int_equal(data[i], data_res[i]);
    }
}

static void cy15x102qn_fast_read_test(void **state)
{
    /* Select device */
    expect_value(__wrap_spi_select_slave, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_select_slave, cs, CY15X102QN_SPI_CS);
    expect_value(__wrap_spi_select_slave, active, true);

    will_return(__wrap_spi_select_slave, 0);

    /* Write opcode */
    uint8_t cmd = CY15X102QN_OPCODE_FSTRD;

    expect_value(__wrap_spi_write, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SPI_CS_NONE);
    expect_memory(__wrap_spi_write, data, (void*)&cmd, 1);
    expect_value(__wrap_spi_write, len, 1);

    will_return(__wrap_spi_write, 0);

    /* Write address */
    uint32_t adr = generate_random(0, CY15X102QN_MAX_ADDRESS);

    uint8_t adr_arr[3] = {0x00};

    adr_arr[0] = (uint8_t)(adr >> 16) & 0xFF;
    adr_arr[1] = (uint8_t)(adr >> 8) & 0xFF;
    adr_arr[2] = (uint8_t)(adr >> 0) & 0xFF;

    expect_value(__wrap_spi_write, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SPI_CS_NONE);
    expect_memory(__wrap_spi_write, data, (void*)adr_arr, 3);
    expect_value(__wrap_spi_write, len, 3);

    will_return(__wrap_spi_write, 0);

    /* Write dummy byte */
    uint8_t dummy = 0x00;

    expect_value(__wrap_spi_write, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SPI_CS_NONE);
    expect_memory(__wrap_spi_write, data, (void*)&dummy, 1);
    expect_value(__wrap_spi_write, len, 1);

    will_return(__wrap_spi_write, 0);

    /* Read data */
    uint8_t data[256] = {0xFF};
    uint16_t data_len = generate_random(1, 256);

    expect_value(__wrap_spi_read, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_read, cs, SPI_CS_NONE);
    expect_value(__wrap_spi_read, len, data_len);

    uint16_t i = 0;
    for(i=0; i<data_len; i++)
    {
        data[i] = generate_random(0, 255);
        will_return(__wrap_spi_read, data[i]);
    }

    will_return(__wrap_spi_read, 0);

    /* Unselect device */
    expect_value(__wrap_spi_select_slave, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_select_slave, cs, CY15X102QN_SPI_CS);
    expect_value(__wrap_spi_select_slave, active, false);

    will_return(__wrap_spi_select_slave, 0);

    uint8_t data_res[256] = {0xFF};

    assert_return_code(cy15x102qn_fast_read(&conf, adr, data_res, data_len), 0);

    for(i=0; i<data_len; i++)
    {
        assert_int_equal(data[i], data_res[i]);
    }
}

static void cy15x102qn_special_sector_write_test(void **state)
{
    /* Select device */
    expect_value(__wrap_spi_select_slave, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_select_slave, cs, CY15X102QN_SPI_CS);
    expect_value(__wrap_spi_select_slave, active, true);

    will_return(__wrap_spi_select_slave, 0);

    /* Write opcode */
    uint8_t cmd = CY15X102QN_OPCODE_SSWR;

    expect_value(__wrap_spi_write, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SPI_CS_NONE);
    expect_memory(__wrap_spi_write, data, (void*)&cmd, 1);
    expect_value(__wrap_spi_write, len, 1);

    will_return(__wrap_spi_write, 0);

    /* Write address */
    uint32_t adr = generate_random(0, 255);

    expect_value(__wrap_spi_write, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SPI_CS_NONE);
    expect_memory(__wrap_spi_write, data, (void*)&adr, 1);
    expect_value(__wrap_spi_write, len, 1);

    will_return(__wrap_spi_write, 0);

    /* Write data */
    uint8_t data[256] = {0xFF};
    uint16_t data_len = generate_random(1, 256);

    uint16_t i = 0;
    for(i=0; i<data_len; i++)
    {
        data[i] = generate_random(0, 255);
    }

    expect_value(__wrap_spi_write, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SPI_CS_NONE);
    expect_memory(__wrap_spi_write, data, (void*)data, data_len);
    expect_value(__wrap_spi_write, len, data_len);

    will_return(__wrap_spi_write, 0);

    /* Unselect device */
    expect_value(__wrap_spi_select_slave, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_select_slave, cs, CY15X102QN_SPI_CS);
    expect_value(__wrap_spi_select_slave, active, false);

    will_return(__wrap_spi_select_slave, 0);

    assert_return_code(cy15x102qn_special_sector_write(&conf, adr, data, data_len), 0);
}

static void cy15x102qn_special_sector_read_test(void **state)
{
    /* Select device */
    expect_value(__wrap_spi_select_slave, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_select_slave, cs, CY15X102QN_SPI_CS);
    expect_value(__wrap_spi_select_slave, active, true);

    will_return(__wrap_spi_select_slave, 0);

    /* Write opcode */
    uint8_t cmd = CY15X102QN_OPCODE_SSRD;

    expect_value(__wrap_spi_write, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SPI_CS_NONE);
    expect_memory(__wrap_spi_write, data, (void*)&cmd, 1);
    expect_value(__wrap_spi_write, len, 1);

    will_return(__wrap_spi_write, 0);

    /* Write address */
    uint32_t adr = generate_random(0, 255);

    expect_value(__wrap_spi_write, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SPI_CS_NONE);
    expect_memory(__wrap_spi_write, data, (void*)&adr, 1);
    expect_value(__wrap_spi_write, len, 1);

    will_return(__wrap_spi_write, 0);

    /* Read data */
    uint8_t data[256] = {0xFF};
    uint16_t data_len = generate_random(1, 256);

    expect_value(__wrap_spi_read, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_read, cs, SPI_CS_NONE);
    expect_value(__wrap_spi_read, len, data_len);

    uint16_t i = 0;
    for(i=0; i<data_len; i++)
    {
        data[i] = generate_random(0, 255);
        will_return(__wrap_spi_read, data[i]);
    }

    will_return(__wrap_spi_read, 0);

    /* Unselect device */
    expect_value(__wrap_spi_select_slave, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_select_slave, cs, CY15X102QN_SPI_CS);
    expect_value(__wrap_spi_select_slave, active, false);

    will_return(__wrap_spi_select_slave, 0);

    uint8_t data_res[256] = {0xFF};

    assert_return_code(cy15x102qn_special_sector_read(&conf, adr, data_res, data_len), 0);

    for(i=0; i<data_len; i++)
    {
        assert_int_equal(data[i], data_res[i]);
    }
}

static void cy15x102qn_read_device_id_test(void **state)
{
    /* Select device */
    expect_value(__wrap_spi_select_slave, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_select_slave, cs, CY15X102QN_SPI_CS);
    expect_value(__wrap_spi_select_slave, active, true);

    will_return(__wrap_spi_select_slave, 0);

    /* Write opcode */
    uint8_t cmd = CY15X102QN_OPCODE_RDID;

    expect_value(__wrap_spi_write, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SPI_CS_NONE);
    expect_memory(__wrap_spi_write, data, (void*)&cmd, 1);
    expect_value(__wrap_spi_write, len, 1);

    will_return(__wrap_spi_write, 0);

    /* Read data */
    uint8_t data[9] = {0xFF};

    expect_value(__wrap_spi_read, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_read, cs, SPI_CS_NONE);
    expect_value(__wrap_spi_read, len, 9);

    uint16_t i = 0;
    for(i=0; i<9; i++)
    {
        data[i] = generate_random(0, 255);
        will_return(__wrap_spi_read, data[i]);
    }

    will_return(__wrap_spi_read, 0);

    /* Unselect device */
    expect_value(__wrap_spi_select_slave, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_select_slave, cs, CY15X102QN_SPI_CS);
    expect_value(__wrap_spi_select_slave, active, false);

    will_return(__wrap_spi_select_slave, 0);

    cy15x102qn_device_id_t dev_id = {0};

    assert_return_code(cy15x102qn_read_device_id(&conf, &dev_id), 0);

    uint64_t manufacturer_id = ((uint64_t)data[8] << 48) |
                               ((uint64_t)data[7] << 40) |
                               ((uint64_t)data[6] << 32) |
                               ((uint64_t)data[5] << 24) |
                               ((uint64_t)data[4] << 16) |
                               ((uint64_t)data[3] << 8) |
                                (uint64_t)data[2];

    uint8_t family          = (data[1] & 0xE0) >> 5;
    uint8_t density         = (data[1] & 0x1E) >> 1;
    uint8_t inrush          = data[1] & 0x01;
    uint8_t sub_type        = (data[0] & 0xE0) >> 5;
    uint8_t revision        = (data[0] & 0x18) >> 3;
    uint8_t voltage         = (data[0] & 0x04) >> 2;
    uint8_t frequency       = data[0] & 0x03;

    assert_int_equal(dev_id.manufacturer_id,    manufacturer_id);
    assert_int_equal(dev_id.family,             family);
    assert_int_equal(dev_id.density,            density);
    assert_int_equal(dev_id.inrush,             inrush);
    assert_int_equal(dev_id.sub_type,           sub_type);
    assert_int_equal(dev_id.revision,           revision);
    assert_int_equal(dev_id.voltage,            voltage);
    assert_int_equal(dev_id.frequency,          frequency);
}

static void cy15x102qn_read_unique_id_test(void **state)
{
    /* Select device */
    expect_value(__wrap_spi_select_slave, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_select_slave, cs, CY15X102QN_SPI_CS);
    expect_value(__wrap_spi_select_slave, active, true);

    will_return(__wrap_spi_select_slave, 0);

    /* Write opcode */
    uint8_t cmd = CY15X102QN_OPCODE_RUID;

    expect_value(__wrap_spi_write, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SPI_CS_NONE);
    expect_memory(__wrap_spi_write, data, (void*)&cmd, 1);
    expect_value(__wrap_spi_write, len, 1);

    will_return(__wrap_spi_write, 0);

    /* Read data */
    uint8_t data[8] = {0xFF};

    expect_value(__wrap_spi_read, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_read, cs, SPI_CS_NONE);
    expect_value(__wrap_spi_read, len, 8);

    uint16_t i = 0;
    for(i=0; i<8; i++)
    {
        data[i] = generate_random(0, 255);
        will_return(__wrap_spi_read, data[i]);
    }

    will_return(__wrap_spi_read, 0);

    /* Unselect device */
    expect_value(__wrap_spi_select_slave, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_select_slave, cs, CY15X102QN_SPI_CS);
    expect_value(__wrap_spi_select_slave, active, false);

    will_return(__wrap_spi_select_slave, 0);

    cy15x102qn_uid_t uid = 0;

    assert_return_code(cy15x102qn_read_unique_id(&conf, &uid), 0);

    uint64_t uid_res = ((uint64_t)data[7] << 56) |
                       ((uint64_t)data[6] << 48) |
                       ((uint64_t)data[5] << 40) |
                       ((uint64_t)data[4] << 32) |
                       ((uint64_t)data[3] << 24) |
                       ((uint64_t)data[2] << 16) |
                       ((uint64_t)data[1] << 8) |
                        (uint64_t)data[0];

    assert_int_equal(uid, uid_res);
}

static void cy15x102qn_write_serial_number_test(void **state)
{
    /* Select device */
    expect_value(__wrap_spi_select_slave, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_select_slave, cs, CY15X102QN_SPI_CS);
    expect_value(__wrap_spi_select_slave, active, true);

    will_return(__wrap_spi_select_slave, 0);

    /* Write opcode */
    uint8_t cmd = CY15X102QN_OPCODE_WRSN;

    expect_value(__wrap_spi_write, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SPI_CS_NONE);
    expect_memory(__wrap_spi_write, data, (void*)&cmd, 1);
    expect_value(__wrap_spi_write, len, 1);

    will_return(__wrap_spi_write, 0);

    /* Write serial number */
    uint8_t data[8] = {0xFF};

    uint16_t i = 0;
    for(i=0; i<8; i++)
    {
        data[i] = generate_random(0, 255);
    }

    expect_value(__wrap_spi_write, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SPI_CS_NONE);
    expect_memory(__wrap_spi_write, data, (void*)data, 8);
    expect_value(__wrap_spi_write, len, 8);

    will_return(__wrap_spi_write, 0);

    /* Unselect device */
    expect_value(__wrap_spi_select_slave, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_select_slave, cs, CY15X102QN_SPI_CS);
    expect_value(__wrap_spi_select_slave, active, false);

    will_return(__wrap_spi_select_slave, 0);

    cy15x102qn_serial_number_t ser_num = {0};

    ser_num.customer_id     = ((uint16_t)data[0] << 8) | data[1];
    ser_num.unique_number   = ((uint64_t)data[2] << 32) | ((uint64_t)data[3] << 24) | ((uint64_t)data[4] << 16) |
                              ((uint64_t)data[5] << 8) | data[6];
    ser_num.crc             = data[7];

    assert_return_code(cy15x102qn_write_serial_number(&conf, ser_num), 0);
}

static void cy15x102qn_read_serial_number_test(void **state)
{
    /* Select device */
    expect_value(__wrap_spi_select_slave, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_select_slave, cs, CY15X102QN_SPI_CS);
    expect_value(__wrap_spi_select_slave, active, true);

    will_return(__wrap_spi_select_slave, 0);

    /* Write opcode */
    uint8_t cmd = CY15X102QN_OPCODE_SNR;

    expect_value(__wrap_spi_write, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SPI_CS_NONE);
    expect_memory(__wrap_spi_write, data, (void*)&cmd, 1);
    expect_value(__wrap_spi_write, len, 1);

    will_return(__wrap_spi_write, 0);

    /* Read serial number */
    expect_value(__wrap_spi_read, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_read, cs, SPI_CS_NONE);
    expect_value(__wrap_spi_read, len, 8);

    uint8_t ser_num_data[8] = {0xFF};

    uint16_t i = 0;
    for(i=0; i<8; i++)
    {
        ser_num_data[i] = generate_random(1, 256);

        will_return(__wrap_spi_read, ser_num_data[i]);
    }

    will_return(__wrap_spi_read, 0);

    /* Unselect device */
    expect_value(__wrap_spi_select_slave, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_select_slave, cs, CY15X102QN_SPI_CS);
    expect_value(__wrap_spi_select_slave, active, false);

    will_return(__wrap_spi_select_slave, 0);

    cy15x102qn_serial_number_t ser_num = {0};

    assert_return_code(cy15x102qn_read_serial_number(&conf, &ser_num), 0);

    assert_int_equal(ser_num.customer_id, ((uint16_t)ser_num_data[0] << 8) | (uint16_t)ser_num_data[1]);
    assert_int_equal(ser_num.unique_number, ((uint64_t)ser_num_data[2] << 32) | ((uint64_t)ser_num_data[3] << 24) |
                                            ((uint64_t)ser_num_data[4] << 16) | ((uint64_t)ser_num_data[5] << 8) |
                                            (uint64_t)ser_num_data[6]);
    assert_int_equal(ser_num.crc, ser_num_data[7]);
}

static void cy15x102qn_deep_power_down_mode_test(void **state)
{
    uint8_t cmd = CY15X102QN_OPCODE_DPD;

    expect_value(__wrap_spi_write, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_write, cs, CY15X102QN_SPI_CS);
    expect_memory(__wrap_spi_write, data, (void*)&cmd, 1);
    expect_value(__wrap_spi_write, len, 1);

    will_return(__wrap_spi_write, 0);

    assert_return_code(cy15x102qn_deep_power_down_mode(&conf), 0);
}

static void cy15x102qn_hibernate_mode_test(void **state)
{
    uint8_t cmd = CY15X102QN_OPCODE_HBN;

    expect_value(__wrap_spi_write, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_write, cs, CY15X102QN_SPI_CS);
    expect_memory(__wrap_spi_write, data, (void*)&cmd, 1);
    expect_value(__wrap_spi_write, len, 1);

    will_return(__wrap_spi_write, 0);

    assert_return_code(cy15x102qn_hibernate_mode(&conf), 0);
}

static void cy15x102qn_spi_init_test(void **state)
{
    expect_value(__wrap_spi_init, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_init, config.speed_hz, CY15X102QN_SPI_CLOCK_HZ);
    expect_value(__wrap_spi_init, config.mode, CY15X102QN_SPI_MODE);

    will_return(__wrap_spi_init, 0);

    assert_return_code(cy15x102qn_spi_init(&conf), 0);
}

static void cy15x102qn_spi_write_test(void **state)
{
    uint8_t data_test[256] = {0xFF};
    uint16_t len_test = generate_random(1, 256);

    uint16_t i = 0;
    for(i=0; i<len_test; i++)
    {
        data_test[i] = generate_random(0, 255);
    }

    expect_value(__wrap_spi_write, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_write, cs, CY15X102QN_SPI_CS);
    expect_memory(__wrap_spi_write, data, (void*)data_test, len_test);
    expect_value(__wrap_spi_write, len, len_test);

    will_return(__wrap_spi_write, 0);

    assert_return_code(cy15x102qn_spi_write(&conf, data_test, len_test), 0);
}

static void cy15x102qn_spi_read_test(void **state)
{
    uint8_t data_test[256] = {0xFF};
    uint16_t len_test = generate_random(1, 256);

    uint16_t i = 0;
    for(i=0; i<len_test; i++)
    {
        data_test[i] = generate_random(0, 255);
    }

    expect_value(__wrap_spi_read, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_read, cs, CY15X102QN_SPI_CS);
    expect_value(__wrap_spi_read, len, len_test);

    for(i=0; i<len_test; i++)
    {
        will_return(__wrap_spi_read, data_test[i]);
    }

    will_return(__wrap_spi_read, 0);

    uint8_t data_res[256] = {0xFF};

    assert_return_code(cy15x102qn_spi_read(&conf, data_res, len_test), 0);

    for(i=0; i<len_test; i++)
    {
        assert_int_equal(data_test[i], data_res[i]);
    }
}

static void cy15x102qn_spi_transfer_test(void **state)
{
    uint8_t wd_test[256] = {0xFF};
    uint8_t rd_test[256] = {0xFF};
    uint16_t len_test = generate_random(1, 256);

    uint16_t i = 0;
    for(i=0; i<len_test; i++)
    {
        wd_test[i] = generate_random(0, 255);
        rd_test[i] = generate_random(0, 255);
    }

    expect_value(__wrap_spi_transfer, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_transfer, cs, CY15X102QN_SPI_CS);
    expect_memory(__wrap_spi_transfer, wd, (void*)wd_test, len_test);
    expect_value(__wrap_spi_transfer, len, len_test);

    for(i=0; i<len_test; i++)
    {
        will_return(__wrap_spi_transfer, rd_test[i]);
    }

    will_return(__wrap_spi_transfer, 0);

    uint8_t rd_res[256] = {0xFF};

    assert_return_code(cy15x102qn_spi_transfer(&conf, wd_test, rd_res, len_test), 0);

    for(i=0; i<len_test; i++)
    {
        assert_int_equal(rd_test[i], rd_res[i]);
    }
}

static void cy15x102qn_spi_select_test(void **state)
{
    expect_value(__wrap_spi_select_slave, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_select_slave, cs, CY15X102QN_SPI_CS);
    expect_value(__wrap_spi_select_slave, active, true);

    will_return(__wrap_spi_select_slave, 0);

    assert_return_code(cy15x102qn_spi_select(&conf), 0);
}

static void cy15x102qn_spi_unselect_test(void **state)
{
    expect_value(__wrap_spi_select_slave, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_select_slave, cs, CY15X102QN_SPI_CS);
    expect_value(__wrap_spi_select_slave, active, false);

    will_return(__wrap_spi_select_slave, 0);

    assert_return_code(cy15x102qn_spi_unselect(&conf), 0);
}

static void cy15x102qn_spi_write_only_test(void **state)
{
    uint8_t data_test[256] = {0xFF};
    uint16_t len_test = generate_random(1, 256);

    uint16_t i = 0;
    for(i=0; i<len_test; i++)
    {
        data_test[i] = generate_random(0, 255);
    }

    expect_value(__wrap_spi_write, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SPI_CS_NONE);
    expect_memory(__wrap_spi_write, data, (void*)data_test, len_test);
    expect_value(__wrap_spi_write, len, len_test);

    will_return(__wrap_spi_write, 0);

    assert_return_code(cy15x102qn_spi_write_only(&conf, data_test, len_test), 0);
}

static void cy15x102qn_spi_read_only_test(void **state)
{
    uint8_t data_test[256] = {0xFF};
    uint16_t len_test = generate_random(1, 256);

    uint16_t i = 0;
    for(i=0; i<len_test; i++)
    {
        data_test[i] = generate_random(0, 255);
    }

    expect_value(__wrap_spi_read, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_read, cs, SPI_CS_NONE);
    expect_value(__wrap_spi_read, len, len_test);

    for(i=0; i<len_test; i++)
    {
        will_return(__wrap_spi_read, data_test[i]);
    }

    will_return(__wrap_spi_read, 0);

    assert_return_code(cy15x102qn_spi_read_only(&conf, data_test, len_test), 0);
}

static void cy15x102qn_spi_transfer_only_test(void **state)
{
    uint8_t wd_test[256] = {0xFF};
    uint8_t rd_test[256] = {0xFF};
    uint16_t len_test = generate_random(1, 256);

    uint16_t i = 0;
    for(i=0; i<len_test; i++)
    {
        wd_test[i] = generate_random(0, 255);
        rd_test[i] = generate_random(0, 255);
    }

    expect_value(__wrap_spi_transfer, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_transfer, cs, SPI_CS_NONE);
    expect_memory(__wrap_spi_transfer, wd, (void*)wd_test, len_test);
    expect_value(__wrap_spi_transfer, len, len_test);

    for(i=0; i<len_test; i++)
    {
        will_return(__wrap_spi_transfer, rd_test[i]);
    }

    will_return(__wrap_spi_transfer, 0);

    uint8_t rd_res[256] = {0xFF};

    assert_return_code(cy15x102qn_spi_transfer_only(&conf, wd_test, rd_res, len_test), 0);

    for(i=0; i<len_test; i++)
    {
        assert_int_equal(rd_test[i], rd_res[i]);
    }
}

static void cy15x102qn_gpio_init_test(void **state)
{
    expect_value(__wrap_gpio_init, pin, CY15X102QN_GPIO_WP_PIN);
    expect_value(__wrap_gpio_init, config.mode, GPIO_MODE_OUTPUT);

    will_return(__wrap_gpio_init, 0);

    expect_value(__wrap_gpio_set_state, pin, CY15X102QN_GPIO_WP_PIN);
    expect_value(__wrap_gpio_set_state, level, true);

    will_return(__wrap_gpio_set_state, 0);

    assert_return_code(cy15x102qn_gpio_init(&conf), 0);
}

static void cy15x102qn_gpio_set_write_protect_test(void **state)
{
    expect_value(__wrap_gpio_set_state, pin, CY15X102QN_GPIO_WP_PIN);
    expect_value(__wrap_gpio_set_state, level, true);

    will_return(__wrap_gpio_set_state, 0);

    assert_return_code(cy15x102qn_gpio_set_write_protect(&conf), 0);
}

static void cy15x102qn_gpio_clear_write_protect_test(void **state)
{
    expect_value(__wrap_gpio_set_state, pin, CY15X102QN_GPIO_WP_PIN);
    expect_value(__wrap_gpio_set_state, level, false);

    will_return(__wrap_gpio_set_state, 0);

    assert_return_code(cy15x102qn_gpio_clear_write_protect(&conf), 0);
}

int main(void)
{
    conf.port       = CY15X102QN_SPI_PORT;
    conf.cs_pin     = CY15X102QN_SPI_CS;
    conf.clock_hz   = CY15X102QN_SPI_CLOCK_HZ;
    conf.wp_pin     = CY15X102QN_GPIO_WP_PIN;

    const struct CMUnitTest cy15x102qn_tests[] = {
        cmocka_unit_test(cy15x102qn_init_test),
        cmocka_unit_test(cy15x102qn_set_write_enable_test),
        cmocka_unit_test(cy15x102qn_reset_write_enable_test),
        cmocka_unit_test(cy15x102qn_read_status_reg_test),
        cmocka_unit_test(cy15x102qn_write_status_reg_test),
        cmocka_unit_test(cy15x102qn_write_test),
        cmocka_unit_test(cy15x102qn_read_test),
        cmocka_unit_test(cy15x102qn_fast_read_test),
        cmocka_unit_test(cy15x102qn_special_sector_write_test),
        cmocka_unit_test(cy15x102qn_special_sector_read_test),
        cmocka_unit_test(cy15x102qn_read_device_id_test),
        cmocka_unit_test(cy15x102qn_read_unique_id_test),
        cmocka_unit_test(cy15x102qn_write_serial_number_test),
        cmocka_unit_test(cy15x102qn_read_serial_number_test),
        cmocka_unit_test(cy15x102qn_deep_power_down_mode_test),
        cmocka_unit_test(cy15x102qn_hibernate_mode_test),
        cmocka_unit_test(cy15x102qn_spi_init_test),
        cmocka_unit_test(cy15x102qn_spi_write_test),
        cmocka_unit_test(cy15x102qn_spi_read_test),
        cmocka_unit_test(cy15x102qn_spi_transfer_test),
        cmocka_unit_test(cy15x102qn_spi_select_test),
        cmocka_unit_test(cy15x102qn_spi_unselect_test),
        cmocka_unit_test(cy15x102qn_spi_write_only_test),
        cmocka_unit_test(cy15x102qn_spi_read_only_test),
        cmocka_unit_test(cy15x102qn_spi_transfer_only_test),
        cmocka_unit_test(cy15x102qn_gpio_init_test),
        cmocka_unit_test(cy15x102qn_gpio_set_write_protect_test),
        cmocka_unit_test(cy15x102qn_gpio_clear_write_protect_test),
    };

    return cmocka_run_group_tests(cy15x102qn_tests, NULL, NULL);
}

unsigned int generate_random(unsigned int l, unsigned int r)
{
    return (rand() % (r - l + 1)) + l;
}

/** \} End of cy15x102qn_test group */
