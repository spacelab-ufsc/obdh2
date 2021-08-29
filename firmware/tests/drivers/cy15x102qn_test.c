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
 * \version 0.7.10
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

#include <drivers/spi/spi.h>
#include <drivers/gpio/gpio.h>

static void cy15x102qn_init_test(void **state)
{
}

static void cy15x102qn_set_write_enable_test(void **state)
{
}

static void cy15x102qn_reset_write_enable_test(void **state)
{
}

static void cy15x102qn_read_status_reg_test(void **state)
{
}

static void cy15x102qn_write_status_reg_test(void **state)
{
}

static void cy15x102qn_write_test(void **state)
{
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
}

static void cy15x102qn_spi_write_test(void **state)
{
}

static void cy15x102qn_spi_read_test(void **state)
{
}

static void cy15x102qn_spi_transfer_test(void **state)
{
}

static void cy15x102qn_spi_select_test(void **state)
{
}

static void cy15x102qn_spi_unselect_test(void **state)
{
}

static void cy15x102qn_spi_write_only_test(void **state)
{
}

static void cy15x102qn_spi_read_only_test(void **state)
{
}

static void cy15x102qn_spi_transfer_only_test(void **state)
{
}

static void cy15x102qn_gpio_init_test(void **state)
{
}

static void cy15x102qn_gpio_set_write_protect_test(void **state)
{
}

static void cy15x102qn_gpio_clear_write_protect_test(void **state)
{
}

int main(void)
{
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
