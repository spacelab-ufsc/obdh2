/*
 * mt25q_test.c
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
 * \brief Unit test of the MT25Q driver.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.20
 * 
 * \date 2021/09/05
 * 
 * \defgroup mt25q_unit_test MT25Q
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

#include <drivers/gpio/gpio.h>
#include <drivers/spi/spi.h>
#include <drivers/mt25q/mt25q.h>

#define MT25Q_SPI_PORT              SPI_PORT_0
#define MT25Q_SPI_CS_PIN            SPI_CS_2
#define MT25Q_SPI_CLOCK_HZ          1000000UL
#define MT25Q_SPI_MODE              SPI_MODE_0
#define MT25Q_GPIO_HOLD_PIN         GPIO_PIN_26
#define MT25Q_GPIO_RESET_PIN        GPIO_PIN_27

unsigned int generate_random(unsigned int l, unsigned int r);

static void mt25q_init_test(void **state)
{
    /* SPI init */
//    expect_value(__wrap_spi_init, port, MT25Q_SPI_PORT);
//    expect_value(__wrap_spi_init, config.speed_hz, MT25Q_SPI_CLOCK_HZ);
//    expect_value(__wrap_spi_init, config.mode, MT25Q_SPI_MODE);
//
//    will_return(__wrap_spi_init, 0);

    /* GPIO init */
//    expect_value(__wrap_gpio_init, pin, MT25Q_GPIO_HOLD_PIN);
//    expect_value(__wrap_gpio_init, config.mode, GPIO_MODE_OUTPUT);
//
//    will_return(__wrap_gpio_init, 0);
//
//    expect_value(__wrap_gpio_init, pin, MT25Q_GPIO_RESET_PIN);
//    expect_value(__wrap_gpio_init, config.mode, GPIO_MODE_OUTPUT);
//
//    will_return(__wrap_gpio_init, 0);

    /* Read device ID */
//    uint8_t cmd[350] = {0x00};
//    uint8_t ans[350] = {0x00};
//
//    cmd[0] = 0x9E;
//
//    ans[1] = 0x20;
//    ans[2] = 0xBA;
//    ans[3] = 0x21;
//
//    expect_value(__wrap_spi_transfer, port, MT25Q_SPI_PORT);
//    expect_value(__wrap_spi_transfer, cs, MT25Q_SPI_CS_PIN);
//    expect_memory(__wrap_spi_transfer, wd, (void*)cmd, 4);
//    expect_value(__wrap_spi_transfer, len, 4);
//
//    uint16_t i = 0;
//    for(i=0; i<4; i++)
//    {
//        will_return(__wrap_spi_transfer, ans[i]);
//    }
//
//    will_return(__wrap_spi_transfer, 0);

    /* Read the discovery parameter signature */
//    cmd[0] = 0x5A;
//
//    ans[1] = 0x00;
//    ans[2] = 0x00;
//    ans[3] = 0x00;
//    ans[4] = 0x00;
//    ans[5] = 'S';
//    ans[6] = 'F';
//    ans[7] = 'D';
//    ans[8] = 'P';
//    ans[] = ;

//    expect_value(__wrap_spi_transfer, port, MT25Q_SPI_PORT);
//    expect_value(__wrap_spi_transfer, cs, MT25Q_SPI_CS_PIN);
//    expect_memory(__wrap_spi_transfer, wd, (void*)cmd, 350);
//    expect_value(__wrap_spi_transfer, len, 350);
//
//    i = 0;
//    for(i=0; i<350; i++)
//    {
//        will_return(__wrap_spi_transfer, ans[i]);
//    }
//
//    will_return(__wrap_spi_transfer, 0);
//
//    assert_return_code(mt25q_init(), 0);
}

static void mt25q_read_device_id_test(void **state)
{
    uint8_t cmd[256] = {0x00};
    uint8_t ans[256] = {0x00};

    cmd[0] = 0x9E;

    ans[1] = 0x20;
    ans[2] = 0xBA;
    ans[3] = 0x21;

    expect_value(__wrap_spi_transfer, port, MT25Q_SPI_PORT);
    expect_value(__wrap_spi_transfer, cs, MT25Q_SPI_CS_PIN);
    expect_memory(__wrap_spi_transfer, wd, (void*)cmd, 4);
    expect_value(__wrap_spi_transfer, len, 4);

    uint16_t i = 0;
    for(i=0; i<4; i++)
    {
        will_return(__wrap_spi_transfer, ans[i]);
    }

    will_return(__wrap_spi_transfer, 0);

    mt25q_dev_id_t dev_id = {0};

    assert_return_code(mt25q_read_device_id(&dev_id), 0);

    assert_int_equal(ans[1], dev_id.manufacturer_id);
    assert_int_equal(ans[2], dev_id.memory_type);
    assert_int_equal(ans[3], dev_id.memory_capacity);
}

static void mt25q_read_flash_description_test(void **state)
{
}

static void mt25q_read_status_test(void **state)
{
}

static void mt25q_reset_test(void **state)
{
    uint8_t cmd = 0x66;

    expect_value(__wrap_spi_write, port, MT25Q_SPI_PORT);
    expect_value(__wrap_spi_write, cs, MT25Q_SPI_CS_PIN);
    expect_memory(__wrap_spi_write, data, (void*)&cmd, 1);
    expect_value(__wrap_spi_write, len, 1);

    will_return(__wrap_spi_write, 0);

    cmd = 0x99;

    expect_value(__wrap_spi_write, port, MT25Q_SPI_PORT);
    expect_value(__wrap_spi_write, cs, MT25Q_SPI_CS_PIN);
    expect_memory(__wrap_spi_write, data, (void*)&cmd, 1);
    expect_value(__wrap_spi_write, len, 1);

    will_return(__wrap_spi_write, 0);

    assert_return_code(mt25q_reset(), 0);
}

static void mt25q_enter_deep_power_down_test(void **state)
{
    uint8_t cmd = 0xB9;

    expect_value(__wrap_spi_write, port, MT25Q_SPI_PORT);
    expect_value(__wrap_spi_write, cs, MT25Q_SPI_CS_PIN);
    expect_memory(__wrap_spi_write, data, (void*)&cmd, 1);
    expect_value(__wrap_spi_write, len, 1);

    will_return(__wrap_spi_write, 0);

    assert_return_code(mt25q_enter_deep_power_down(), 0);
}

static void mt25q_release_from_deep_power_down_test(void **state)
{
    uint8_t cmd = 0xAB;

    expect_value(__wrap_spi_write, port, MT25Q_SPI_PORT);
    expect_value(__wrap_spi_write, cs, MT25Q_SPI_CS_PIN);
    expect_memory(__wrap_spi_write, data, (void*)&cmd, 1);
    expect_value(__wrap_spi_write, len, 1);

    will_return(__wrap_spi_write, 0);

    assert_return_code(mt25q_release_from_deep_power_down(), 0);
}

static void mt25q_write_enable_test(void **state)
{
}

static void mt25q_write_disable_test(void **state)
{
}

static void mt25q_is_busy_test(void **state)
{
}

static void mt25q_die_erase_test(void **state)
{
}

static void mt25q_sector_erase_test(void **state)
{
}

static void mt25q_sub_sector_erase_test(void **state)
{
}

static void mt25q_write_test(void **state)
{
}

static void mt25q_read_test(void **state)
{
}

static void mt25q_get_max_address_test(void **state)
{
}

static void mt25q_enter_4_byte_address_mode_test(void **state)
{
}

static void mt25q_read_flag_status_register_test(void **state)
{
    uint8_t cmd[5] = {0x70, 0x00};
    uint8_t ans[5] = {0x00};

    ans[1] = generate_random(0, UINT8_MAX);

    expect_value(__wrap_spi_transfer, port, MT25Q_SPI_PORT);
    expect_value(__wrap_spi_transfer, cs, MT25Q_SPI_CS_PIN);
    expect_memory(__wrap_spi_transfer, wd, (void*)cmd, 2);
    expect_value(__wrap_spi_transfer, len, 2);

    will_return(__wrap_spi_transfer, ans[0]);
    will_return(__wrap_spi_transfer, ans[1]);

    will_return(__wrap_spi_transfer, 0);

    uint8_t flag = UINT8_MAX;

    assert_return_code(mt25q_read_flag_status_register(&flag), 0);

    assert_int_equal(ans[1], flag);
}

static void mt25q_clear_flag_status_register_test(void **state)
{
    uint8_t cmd = 0x50;

    expect_value(__wrap_spi_write, port, MT25Q_SPI_PORT);
    expect_value(__wrap_spi_write, cs, MT25Q_SPI_CS_PIN);
    expect_memory(__wrap_spi_write, data, (void*)&cmd, 1);
    expect_value(__wrap_spi_write, len, 1);

    will_return(__wrap_spi_write, 0);

    assert_return_code(mt25q_clear_flag_status_register(), 0);
}

static void mt25q_get_flash_description_test(void **state)
{
}

static void mt25q_gen_program_test(void **state)
{
}

int main(void)
{
    const struct CMUnitTest mt25q_tests[] = {
        cmocka_unit_test(mt25q_init_test),
        cmocka_unit_test(mt25q_read_device_id_test),
        cmocka_unit_test(mt25q_read_flash_description_test),
        cmocka_unit_test(mt25q_read_status_test),
        cmocka_unit_test(mt25q_reset_test),
        cmocka_unit_test(mt25q_enter_deep_power_down_test),
        cmocka_unit_test(mt25q_release_from_deep_power_down_test),
        cmocka_unit_test(mt25q_write_enable_test),
        cmocka_unit_test(mt25q_write_disable_test),
        cmocka_unit_test(mt25q_is_busy_test),
        cmocka_unit_test(mt25q_die_erase_test),
        cmocka_unit_test(mt25q_sector_erase_test),
        cmocka_unit_test(mt25q_sub_sector_erase_test),
        cmocka_unit_test(mt25q_write_test),
        cmocka_unit_test(mt25q_read_test),
        cmocka_unit_test(mt25q_get_max_address_test),
        cmocka_unit_test(mt25q_enter_4_byte_address_mode_test),
        cmocka_unit_test(mt25q_read_flag_status_register_test),
        cmocka_unit_test(mt25q_clear_flag_status_register_test),
        cmocka_unit_test(mt25q_get_flash_description_test),
        cmocka_unit_test(mt25q_gen_program_test),
    };

    return cmocka_run_group_tests(mt25q_tests, NULL, NULL);
}

unsigned int generate_random(unsigned int l, unsigned int r)
{
    return (rand() % (r - l + 1)) + l;
}

/** \} End of mt25q_test group */
