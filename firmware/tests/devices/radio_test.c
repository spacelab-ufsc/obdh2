/*
 * radio_test.c
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
 * \brief Unit test of the radio device.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.5.17
 * 
 * \date 2021/02/21
 * 
 * \defgroup radio_unit_test Radio
 * \ingroup tests
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include <devices/radio/radio.h>
#include <tests/mockups/sys_log_wrap.h>

static void radio_init_test(void **state)
{
    int result = radio_init();

    assert_return_code(result, 0);
}

static void radio_send_test(void **state)
{
    uint8_t data[50] = {0};

    int result = radio_send(data, 50, 100);

    assert_return_code(result, 0);
}

static void radio_recv_test(void **state)
{
    uint8_t data[50] = {0};

    int result = radio_recv(data, 10, 1000);

    assert_return_code(result, 0);
}

static void radio_available_test(void **state)
{
    int result = radio_available();

//    assert_return_code(result, 0);
}

static void radio_sleep_test(void **state)
{
    int result = radio_sleep();

    assert_return_code(result, 0);
}

int main(void)
{
    const struct CMUnitTest radio_tests[] = {
        cmocka_unit_test(radio_init_test),
        cmocka_unit_test(radio_send_test),
        cmocka_unit_test(radio_recv_test),
        cmocka_unit_test(radio_available_test),
        cmocka_unit_test(radio_sleep_test),
    };

    return cmocka_run_group_tests(radio_tests, NULL, NULL);
}

/** \} End of radio_unit_test group */
