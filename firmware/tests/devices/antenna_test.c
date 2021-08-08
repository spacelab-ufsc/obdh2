/*
 * antenna_test.c
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
 * \brief Unit test of the antenna device.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.2
 * 
 * \date 2021/08/06
 * 
 * \defgroup antenna_unit_test Antenna
 * \ingroup tests
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include <devices/antenna/antenna.h>

static void antenna_init_test(void **state)
{
}

static void antenna_get_status_test(void **state)
{
}

static void antenna_deploy_test(void **state)
{
}

int main(void)
{
    const struct CMUnitTest antenna_tests[] = {
        cmocka_unit_test(antenna_init_test),
        cmocka_unit_test(antenna_get_status_test),
        cmocka_unit_test(antenna_deploy_test),
    };

    return cmocka_run_group_tests(antenna_tests, NULL, NULL);
}

/** \} End of antenna_test group */
