/*
 * media_test.c
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
 * \brief Unit test of the media device.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.7
 * 
 * \date 2021/08/07
 * 
 * \defgroup media_unit_test Media
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

#include <devices/media/media.h>
#include <drivers/flash/flash.h>
#include <drivers/mt25q/mt25q.h>
#include <drivers/cy15x102qn/cy15x102qn.h>

#define MEDIA_FRAM_SPI_PORT         SPI_PORT_0
#define MEDIA_FRAM_SPI_CS_PIN       SPI_CS_5
#define MEDIA_FRAM_SPI_CLOCK_HZ     1000000UL
#define MEDIA_FRAM_WP_PIN           GPIO_PIN_62

unsigned int generate_random(unsigned int l, unsigned int r);

static void media_init_test(void **state)
{
    /* FRAM memory */
    expect_value(__wrap_cy15x102qn_init, conf->port, MEDIA_FRAM_SPI_PORT);
    expect_value(__wrap_cy15x102qn_init, conf->cs_pin, MEDIA_FRAM_SPI_CS_PIN);
    expect_value(__wrap_cy15x102qn_init, conf->clock_hz, MEDIA_FRAM_SPI_CLOCK_HZ);
    expect_value(__wrap_cy15x102qn_init, conf->wp_pin, MEDIA_FRAM_WP_PIN);

    will_return(__wrap_cy15x102qn_init, 0);

    assert_return_code(media_init(MEDIA_FRAM), 0);

    /* NOR memory */
    will_return(__wrap_mt25q_init, 0);

    will_return(__wrap_mt25q_read_device_id, MT25Q_MANUFACTURER_ID);
    will_return(__wrap_mt25q_read_device_id, MT25Q_MEMORY_TYPE_3V);
    will_return(__wrap_mt25q_read_device_id, MT25Q_MEMORY_CAPACITY_1GB);

    will_return(__wrap_mt25q_read_device_id, 0);

    assert_return_code(media_init(MEDIA_NOR), 0);
}

static void media_write_test(void **state)
{
    uint8_t data_val[256] = {0xFF};

    uint32_t adr_val = UINT32_MAX;

    /* FRAM memory */
    for(adr_val=0; adr_val<=UINT16_MAX; adr_val++)
    {
        unsigned int len_val = generate_random(1, 256);

        expect_value(__wrap_cy15x102qn_write, conf->port, MEDIA_FRAM_SPI_PORT);
        expect_value(__wrap_cy15x102qn_write, conf->cs_pin, MEDIA_FRAM_SPI_CS_PIN);
        expect_value(__wrap_cy15x102qn_write, conf->clock_hz, MEDIA_FRAM_SPI_CLOCK_HZ);
        expect_value(__wrap_cy15x102qn_write, conf->wp_pin, MEDIA_FRAM_WP_PIN);

        expect_value(__wrap_cy15x102qn_write, adr, adr_val);
        expect_memory(__wrap_cy15x102qn_write, data, (void*)data_val, len_val);
        expect_value(__wrap_cy15x102qn_write, len, len_val);

        will_return(__wrap_cy15x102qn_write, 0);

        assert_return_code(media_write(MEDIA_FRAM, adr_val, data_val, len_val), 0);
    }

    /* NOR memory */
    for(adr_val=0; adr_val<=UINT16_MAX; adr_val++)
    {
        unsigned int len_val = generate_random(1, 256);

        unsigned int i = 0;
        for(i=0; i<len_val; i++)
        {
            data_val[i] = generate_random(0, 255);
        }

        expect_value(__wrap_mt25q_write, adr, adr_val);
        expect_memory(__wrap_mt25q_write, data, (void*)data_val, len_val);
        expect_value(__wrap_mt25q_write, len, len_val);

        will_return(__wrap_mt25q_write, 0);

        expect_function_call(__wrap_mt25q_delay_ms);

        assert_return_code(media_write(MEDIA_NOR, adr_val, data_val, len_val), 0);
    }
}

static void media_read_test(void **state)
{
    uint8_t data_val[256] = {0xFF};

    uint32_t adr_val = UINT32_MAX;

    /* FRAM memory */
    for(adr_val=0; adr_val<=UINT16_MAX; adr_val++)
    {
        unsigned int len_val = generate_random(1, 256);

        expect_value(__wrap_cy15x102qn_read, conf->port, MEDIA_FRAM_SPI_PORT);
        expect_value(__wrap_cy15x102qn_read, conf->cs_pin, MEDIA_FRAM_SPI_CS_PIN);
        expect_value(__wrap_cy15x102qn_read, conf->clock_hz, MEDIA_FRAM_SPI_CLOCK_HZ);
        expect_value(__wrap_cy15x102qn_read, conf->wp_pin, MEDIA_FRAM_WP_PIN);

        unsigned int i = 0;
        for(i=0; i<len_val; i++)
        {
            data_val[i] = generate_random(0, 255);
        }

        expect_value(__wrap_cy15x102qn_read, adr, adr_val);

        for(i=0; i<len_val; i++)
        {
            will_return(__wrap_cy15x102qn_read, data_val[i]);
        }

        expect_value(__wrap_cy15x102qn_read, len, len_val);

        will_return(__wrap_cy15x102qn_read, 0);

        uint8_t data_buf[256] = {0xFF};

        assert_return_code(media_read(MEDIA_FRAM, adr_val, data_buf, len_val), 0);

        assert_memory_equal((void*)data_buf, (void*)data_val, len_val);
    }

    /* NOR memory */
    for(adr_val=0; adr_val<=UINT16_MAX; adr_val++)
    {
        unsigned int len_val = generate_random(1, 256);

        unsigned int i = 0;
        for(i=0; i<len_val; i++)
        {
            data_val[i] = generate_random(0, 255);
        }

        expect_value(__wrap_mt25q_read, adr, adr_val);

        for(i=0; i<len_val; i++)
        {
            will_return(__wrap_mt25q_read, data_val[i]);
        }

        expect_value(__wrap_mt25q_read, len, len_val);

        will_return(__wrap_mt25q_read, 0);

        uint8_t data_buf[256] = {0xFF};

        assert_return_code(media_read(MEDIA_NOR, adr_val, data_buf, len_val), 0);

        assert_memory_equal((void*)data_buf, (void*)data_val, len_val);
    }
}

static void media_erase_test(void **state)
{
    media_erase_t erase_type = UINT16_MAX;

    /* FRAM memory */
    for(erase_type=0; erase_type<=UINT16_MAX; erase_type++)
    {
        assert_return_code(media_erase(MEDIA_FRAM, erase_type, 0), 0);
    }

    /* NOR memory */
    for(erase_type=0; erase_type<=UINT16_MAX; erase_type++)
    {
        switch(erase_type)
        {
            case MEDIA_ERASE_DIE:
                expect_value(__wrap_mt25q_die_erase, die, 0);

                will_return(__wrap_mt25q_die_erase, 0);

                assert_return_code(media_erase(MEDIA_NOR, erase_type, 0), 0);

                break;
            case MEDIA_ERASE_SECTOR:
                expect_value(__wrap_mt25q_sector_erase, sector, 0);

                will_return(__wrap_mt25q_sector_erase, 0);

                assert_return_code(media_erase(MEDIA_NOR, erase_type, 0), 0);

                break;
            case MEDIA_ERASE_SUB_SECTOR:
                expect_value(__wrap_mt25q_sub_sector_erase, sub, 0);

                will_return(__wrap_mt25q_sub_sector_erase, 0);

                assert_return_code(media_erase(MEDIA_NOR, erase_type, 0), 0);

                break;
            default:
                assert_return_code(media_erase(MEDIA_NOR, erase_type, 0), -1);
        }
    }
}

static void media_get_info_test(void **state)
{
    /* FRAM memory */
    media_info_t info = media_get_info(MEDIA_FRAM);

    assert_true(info.id                     == 0);
    assert_true(info.type                   == 0);
    assert_true(info.starting_address       == 0);
    assert_true(info.address_mask           == 0);
    assert_true(info.size                   == 0);
    assert_true(info.otp_size               == 0);
    assert_true(info.die_count              == 0);
    assert_true(info.die_size               == 0);
    assert_true(info.die_size_bit           == 0);
    assert_true(info.sector_size            == 0);
    assert_true(info.sector_size_bit        == 0);
    assert_true(info.sector_count           == 0);
    assert_true(info.sector_erase_cmd       == 0);
    assert_true(info.sub_sector_size        == 0);
    assert_true(info.sub_sector_size_bit    == 0);
    assert_true(info.sub_sector_count       == 0);
    assert_true(info.sub_sector_erase_cmd   == 0);
    assert_true(info.page_size              == 0);
    assert_true(info.page_count             == 0);
    assert_true(info.buffer_size            == 0);
    assert_true(info.data_width             == 0);
    assert_true(info.num_adr_byte           == 0);

    /* NOR memory */
    will_return(__wrap_mt25q_get_flash_description, 0);
    will_return(__wrap_mt25q_get_flash_description, 1);
    will_return(__wrap_mt25q_get_flash_description, 2);
    will_return(__wrap_mt25q_get_flash_description, 3);
    will_return(__wrap_mt25q_get_flash_description, 4);
    will_return(__wrap_mt25q_get_flash_description, 5);
    will_return(__wrap_mt25q_get_flash_description, 6);
    will_return(__wrap_mt25q_get_flash_description, 7);
    will_return(__wrap_mt25q_get_flash_description, 8);
    will_return(__wrap_mt25q_get_flash_description, 9);
    will_return(__wrap_mt25q_get_flash_description, 10);
    will_return(__wrap_mt25q_get_flash_description, 11);
    will_return(__wrap_mt25q_get_flash_description, 12);
    will_return(__wrap_mt25q_get_flash_description, 13);
    will_return(__wrap_mt25q_get_flash_description, 14);
    will_return(__wrap_mt25q_get_flash_description, 15);
    will_return(__wrap_mt25q_get_flash_description, 16);
    will_return(__wrap_mt25q_get_flash_description, 17);
    will_return(__wrap_mt25q_get_flash_description, 18);
    will_return(__wrap_mt25q_get_flash_description, 19);
    will_return(__wrap_mt25q_get_flash_description, 20);
    will_return(__wrap_mt25q_get_flash_description, 21);

    info = media_get_info(MEDIA_NOR);

    assert_true(info.id                     == 0);
    assert_true(info.type                   == 1);
    assert_true(info.starting_address       == 2);
    assert_true(info.address_mask           == 3);
    assert_true(info.size                   == 4);
    assert_true(info.otp_size               == 5);
    assert_true(info.die_count              == 6);
    assert_true(info.die_size               == 7);
    assert_true(info.die_size_bit           == 8);
    assert_true(info.sector_size            == 9);
    assert_true(info.sector_size_bit        == 10);
    assert_true(info.sector_count           == 11);
    assert_true(info.sector_erase_cmd       == 12);
    assert_true(info.sub_sector_size        == 13);
    assert_true(info.sub_sector_size_bit    == 14);
    assert_true(info.sub_sector_count       == 15);
    assert_true(info.sub_sector_erase_cmd   == 16);
    assert_true(info.page_size              == 17);
    assert_true(info.page_count             == 18);
    assert_true(info.buffer_size            == 19);
    assert_true(info.data_width             == 20);
    assert_true(info.num_adr_byte           == 21);
}

int main(void)
{
    const struct CMUnitTest media_tests[] = {
        cmocka_unit_test(media_init_test),
        cmocka_unit_test(media_write_test),
        cmocka_unit_test(media_read_test),
        cmocka_unit_test(media_erase_test),
        cmocka_unit_test(media_get_info_test),
    };

    return cmocka_run_group_tests(media_tests, NULL, NULL);
}

unsigned int generate_random(unsigned int l, unsigned int r)
{
    return (rand() % (r - l + 1)) + l;
}

/** \} End of media_test group */
