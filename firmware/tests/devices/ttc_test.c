/*
 * ttc_test.c
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
 * \brief Unit test of the TTC device.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.2
 * 
 * \date 2021/08/06
 * 
 * \defgroup ttc_unit_test TTC
 * \ingroup tests
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include <devices/ttc/ttc.h>
#include <drivers/sl_ttc2/sl_ttc2.h>

static void ttc_init_test(void **state)
{
}

static void ttc_get_data_test(void **state)
{
}

static void ttc_send_test(void **state)
{
}

static void ttc_recv_test(void **state)
{
}

static void ttc_avail_test(void **state)
{
}

static void ttc_enter_hibernation_test(void **state)
{
}

static void ttc_leave_hibernation_test(void **state)
{
}

int main(void)
{
    const struct CMUnitTest ttc_tests[] = {
        cmocka_unit_test(ttc_init_test),
        cmocka_unit_test(ttc_get_data_test),
        cmocka_unit_test(ttc_send_test),
        cmocka_unit_test(ttc_recv_test),
        cmocka_unit_test(ttc_avail_test),
        cmocka_unit_test(ttc_enter_hibernation_test),
        cmocka_unit_test(ttc_leave_hibernation_test),
    };

    return cmocka_run_group_tests(ttc_tests, NULL, NULL);
}

/** \} End of ttc_test group */
