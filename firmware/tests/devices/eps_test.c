/*
 * eps_test.c
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
 * \brief Unit test of the EPS device.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.2
 * 
 * \date 2021/08/06
 * 
 * \defgroup eps_unit_test EPS
 * \ingroup tests
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include <devices/eps/eps.h>
#include <drivers/sl_eps2/sl_eps2.h>

static void eps_init_test(void **state)
{
}

static void eps_get_bat_voltage_test(void **state)
{
}

static void eps_get_bat_current_test(void **state)
{
}

static void eps_get_bat_charge_test(void **state)
{
}

static void eps_get_data_test(void **state)
{
}

int main(void)
{
    const struct CMUnitTest eps_tests[] = {
        cmocka_unit_test(eps_init_test),
        cmocka_unit_test(eps_get_bat_voltage_test),
        cmocka_unit_test(eps_get_bat_current_test),
        cmocka_unit_test(eps_get_bat_charge_test),
        cmocka_unit_test(eps_get_data_test),
    };

    return cmocka_run_group_tests(eps_tests, NULL, NULL);
}

/** \} End of eps_test group */
