/*
 * process_tc_test.c
 * 
 * Copyright The OBDH 2.0 Contributors.
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
 * \brief Unit test of the startup task.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.8.29
 * 
 * \date 2021/11/10
 * 
 * \defgroup process_tc_unit_test Process TC
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
#include <devices/payload_edc/payload_edc.h>

static void process_tc_ping_request_test(void **state)
{
}

static void process_tc_data_request_test(void **state)
{
}

static void process_tc_broadcast_message_test(void **state)
{
}

static void process_tc_enter_hibernation_test(void **state)
{
}

static void process_tc_leave_hibernation_test(void **state)
{
}

static void process_tc_activate_module_test(void **state)
{
}

static void process_tc_deactivate_module_test(void **state)
{
}

static void process_tc_activate_payload_test(void **state)
{
}

static void process_tc_deactivate_payload_test(void **state)
{
}

static void process_tc_erase_memory_test(void **state)
{
}

static void process_tc_force_reset_test(void **state)
{
}

static void process_tc_get_payload_data_test(void **state)
{
}

int main(void)
{
    const struct CMUnitTest process_tc_tests[] = {
        cmocka_unit_test(process_tc_ping_request_test),
        cmocka_unit_test(process_tc_data_request_test),
        cmocka_unit_test(process_tc_broadcast_message_test),
        cmocka_unit_test(process_tc_enter_hibernation_test),
        cmocka_unit_test(process_tc_leave_hibernation_test),
        cmocka_unit_test(process_tc_activate_module_test),
        cmocka_unit_test(process_tc_deactivate_module_test),
        cmocka_unit_test(process_tc_activate_payload_test),
        cmocka_unit_test(process_tc_deactivate_payload_test),
        cmocka_unit_test(process_tc_erase_memory_test),
        cmocka_unit_test(process_tc_force_reset_test),
        cmocka_unit_test(process_tc_get_payload_data_test),
    };

    return cmocka_run_group_tests(process_tc_tests, NULL, NULL);
}

/** \} End of process_tc_test group */
