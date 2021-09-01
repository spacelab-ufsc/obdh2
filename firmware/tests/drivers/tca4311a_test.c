/*
 * tca4311a_test.c
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
 * \brief Unit test of the TCA4311A driver.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.11
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

#include <devices/watchdog/watchdog.h>
#include <drivers/wdt/wdt.h>
#include <drivers/gpio/gpio.h>

#define WATCHDOG_INTERNAL_CLK_SRC       WDT_CLK_SRC_ACLK

static void tca4311a_init_test(void **state)
{
}

static void tca4311a_enable_test(void **state)
{
}

static void tca4311a_disable_test(void **state)
{
}

static void tca4311a_is_ready_test(void **state)
{
}

static void tca4311a_write_test(void **state)
{
}

static void tca4311a_read_test(void **state)
{
}

static void tca4311a_write_byte_test(void **state)
{
}

static void tca4311a_read_byte_test(void **state)
{
}

int main(void)
{
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

/** \} End of tca4311a_test group */
