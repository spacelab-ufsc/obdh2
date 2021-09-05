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
 * \version 0.7.12
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

unsigned int generate_random(unsigned int l, unsigned int r);

static void mt25q_init_test(void **state)
{
}

static void mt25q_read_device_id_test(void **state)
{
}

static void mt25q_read_flash_description_test(void **state)
{
}

static void mt25q_read_status_test(void **state)
{
}

static void mt25q_reset_test(void **state)
{
}

static void mt25q_enter_deep_power_down_test(void **state)
{
}

static void mt25q_release_from_deep_power_down_test(void **state)
{
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
}

static void mt25q_clear_flag_status_register_test(void **state)
{
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
