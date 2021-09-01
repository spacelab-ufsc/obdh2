/*
 * tps382x_test.c
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
 * \brief Unit test of the TPS382x driver.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.11
 * 
 * \date 2021/08/29
 * 
 * \defgroup tps382x_unit_test TPS382x
 * \ingroup tests
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include <drivers/gpio/gpio.h>
#include <drivers/tps382x/tps382x.h>

#define TPS382X_WDI_PIN         GPIO_PIN_66

tps382x_config_t conf = {0};

static void tps382x_init_test(void **state)
{
    expect_value(__wrap_gpio_init, pin, TPS382X_WDI_PIN);
    expect_value(__wrap_gpio_init, config.mode, GPIO_MODE_OUTPUT);

    will_return(__wrap_gpio_init, 0);

    assert_return_code(tps382x_init(conf), 0);
}

static void tps382x_trigger_test(void **state)
{
    expect_value(__wrap_gpio_toggle, pin, TPS382X_WDI_PIN);

    will_return(__wrap_gpio_toggle, 0);

    tps382x_trigger(conf);
}

int main(void)
{
    conf.wdi_pin = TPS382X_WDI_PIN;

    const struct CMUnitTest tps382x_tests[] = {
        cmocka_unit_test(tps382x_init_test),
        cmocka_unit_test(tps382x_trigger_test),
    };

    return cmocka_run_group_tests(tps382x_tests, NULL, NULL);
}

/** \} End of tps382x_test group */
