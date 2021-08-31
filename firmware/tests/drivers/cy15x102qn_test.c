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
 * \version 0.7.11
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

#include <config/config.h>
#include <drivers/spi/spi.h>
#include <drivers/gpio/gpio.h>
#include <drivers/cy15x102qn/cy15x102qn.h>

#define CY15X102QN_SPI_PORT         SPI_PORT_0
#define CY15X102QN_SPI_CS           SPI_CS_5
#define CY15X102QN_SPI_CLOCK_HZ     1000000UL
#define CY15X102QN_SPI_MODE         SPI_MODE_0
#define CY15X102QN_GPIO_WP_PIN      GPIO_PIN_62

cy15x102qn_config_t conf = {0};

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
    uint8_t adr[3] = {0x01, 0x02, 0x03};

    expect_value(__wrap_spi_write, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SPI_CS_NONE);
    expect_memory(__wrap_spi_write, data, (void*)adr, 3);
    expect_value(__wrap_spi_write, len, 3);

    will_return(__wrap_spi_write, 0);

    /* Write data */
    uint8_t data[5] = {0x21, 0x8F, 0x96, 0xFC, 0x32};

    expect_value(__wrap_spi_write, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SPI_CS_NONE);
    expect_memory(__wrap_spi_write, data, (void*)data, 5);
    expect_value(__wrap_spi_write, len, 5);

    will_return(__wrap_spi_write, 0);

    /* Unselect device */
    expect_value(__wrap_spi_select_slave, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_select_slave, cs, CY15X102QN_SPI_CS);
    expect_value(__wrap_spi_select_slave, active, false);

    will_return(__wrap_spi_select_slave, 0);

    uint32_t adr32 = ((uint32_t)adr[0] << 16) | ((uint32_t)adr[1] << 8) | adr[2];

    assert_return_code(cy15x102qn_write(&conf, adr32, data, 5), 0);
}

static void cy15x102qn_read_test(void **state)
{
}

static void cy15x102qn_fast_read_test(void **state)
{
}

static void cy15x102qn_special_sector_write_test(void **state)
{
}

static void cy15x102qn_special_sector_read_test(void **state)
{
}

static void cy15x102qn_read_device_id_test(void **state)
{
}

static void cy15x102qn_read_unique_id_test(void **state)
{
}

static void cy15x102qn_write_serial_number_test(void **state)
{
}

static void cy15x102qn_read_serial_number_test(void **state)
{
}

static void cy15x102qn_deep_power_down_mode_test(void **state)
{
}

static void cy15x102qn_hibernate_mode_test(void **state)
{
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
    uint8_t data_test[10] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
    uint16_t len_test = 10;

    expect_value(__wrap_spi_write, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_write, cs, CY15X102QN_SPI_CS);
    expect_memory(__wrap_spi_write, data, (void*)data_test, len_test);
    expect_value(__wrap_spi_write, len, len_test);

    will_return(__wrap_spi_write, 0);

    assert_return_code(cy15x102qn_spi_write(&conf, data_test, len_test), 0);
}

static void cy15x102qn_spi_read_test(void **state)
{
    uint8_t data_test[10] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
    uint16_t len_test = 10;

    expect_value(__wrap_spi_read, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_read, cs, CY15X102QN_SPI_CS);
    expect_value(__wrap_spi_read, len, len_test);

    uint16_t i = 0;
    for(i=0; i<len_test; i++)
    {
        will_return(__wrap_spi_read, data_test[i]);
    }

    will_return(__wrap_spi_read, 0);

    assert_return_code(cy15x102qn_spi_read(&conf, data_test, len_test), 0);
}

static void cy15x102qn_spi_transfer_test(void **state)
{
    uint8_t wd_test[10] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
    uint8_t rd_test[10] = {0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13};
    uint16_t len_test = 10;

    expect_value(__wrap_spi_transfer, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_transfer, cs, CY15X102QN_SPI_CS);
    expect_memory(__wrap_spi_transfer, wd, (void*)wd_test, len_test);
    expect_value(__wrap_spi_transfer, len, len_test);

    uint16_t i = 0;
    for(i=0; i<len_test; i++)
    {
        will_return(__wrap_spi_transfer, rd_test[i]);
    }

    will_return(__wrap_spi_transfer, 0);

    uint8_t rd_res[10] = {0xFF};

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
    uint8_t data_test[10] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
    uint16_t len_test = 10;

    expect_value(__wrap_spi_write, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SPI_CS_NONE);
    expect_memory(__wrap_spi_write, data, (void*)data_test, len_test);
    expect_value(__wrap_spi_write, len, len_test);

    will_return(__wrap_spi_write, 0);

    assert_return_code(cy15x102qn_spi_write_only(&conf, data_test, len_test), 0);
}

static void cy15x102qn_spi_read_only_test(void **state)
{
    uint8_t data_test[10] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
    uint16_t len_test = 10;

    expect_value(__wrap_spi_read, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_read, cs, SPI_CS_NONE);
    expect_value(__wrap_spi_read, len, len_test);

    uint16_t i = 0;
    for(i=0; i<len_test; i++)
    {
        will_return(__wrap_spi_read, data_test[i]);
    }

    will_return(__wrap_spi_read, 0);

    assert_return_code(cy15x102qn_spi_read_only(&conf, data_test, len_test), 0);
}

static void cy15x102qn_spi_transfer_only_test(void **state)
{
    uint8_t wd_test[10] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
    uint8_t rd_test[10] = {0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13};
    uint16_t len_test = 10;

    expect_value(__wrap_spi_transfer, port, CY15X102QN_SPI_PORT);
    expect_value(__wrap_spi_transfer, cs, SPI_CS_NONE);
    expect_memory(__wrap_spi_transfer, wd, (void*)wd_test, len_test);
    expect_value(__wrap_spi_transfer, len, len_test);

    uint16_t i = 0;
    for(i=0; i<len_test; i++)
    {
        will_return(__wrap_spi_transfer, rd_test[i]);
    }

    will_return(__wrap_spi_transfer, 0);

    uint8_t rd_res[10] = {0xFF};

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

/** \} End of cy15x102qn_test group */
