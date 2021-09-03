/*
 * watchdog_test.c
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
 * \brief Unit test of the watchdog device.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.9
 * 
 * \date 2021/02/16
 * 
 * \defgroup watchdog_unit_test Watchdog
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
#define WATCHDOG_INTERNAL_CLK_DIV       WDT_CLK_DIV_32K
#define WATCHDOG_EXTERNAL_PIN           GPIO_PIN_66

static void watchdog_init_test(void **state)
{
    expect_value(__wrap_wdt_init, config.clk_src, WATCHDOG_INTERNAL_CLK_SRC);
    expect_value(__wrap_wdt_init, config.clk_div, WATCHDOG_INTERNAL_CLK_DIV);

    expect_value(__wrap_tps382x_init, config.wdi_pin, WATCHDOG_EXTERNAL_PIN);
    will_return(__wrap_tps382x_init, 0);

    will_return(__wrap_wdt_init, 0);

    assert_return_code(watchdog_init(), 0);
}

static void watchdog_reset_test(void **state)
{
    expect_function_call(__wrap_wdt_reset);

    expect_value(__wrap_tps382x_trigger, config.wdi_pin, WATCHDOG_EXTERNAL_PIN);

    watchdog_reset();
}

int main(void)
{
    const struct CMUnitTest watchdog_tests[] = {
        cmocka_unit_test(watchdog_init_test),
        cmocka_unit_test(watchdog_reset_test),
    };

    return cmocka_run_group_tests(watchdog_tests, NULL, NULL);
}

/** \} End of watchdog_test group */
