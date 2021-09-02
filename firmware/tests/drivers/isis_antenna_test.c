/*
 * isis_antenna_test.c
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
 * \brief Unit test of the ISIS Antenna driver.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.11
 * 
 * \date 2021/09/01
 * 
 * \defgroup isis_antenna_unit_test ISIS Antenna
 * \ingroup tests
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include <drivers/i2c/i2c.h>

static void isis_antenna_init_test(void **state)
{
}

static void isis_antenna_arm_test(void **state)
{
}

static void isis_antenna_disarm_test(void **state)
{
}

static void isis_antenna_start_sequential_deploy_test(void **state)
{
}

static void isis_antenna_start_independent_deploy_test(void **state)
{
}

static void isis_antenna_read_deployment_status_code_test(void **state)
{
}

static void isis_antenna_read_deployment_status_test(void **state)
{
}

static void isis_antenna_get_antenna_status_test(void **state)
{
}

static void isis_antenna_get_antenna_timeout_test(void **state)
{
}

static void isis_antenna_get_burning_test(void **state)
{
}

static void isis_antenna_get_arming_status_test(void **state)
{
}

static void isis_antenna_get_raw_temperature_test(void **state)
{
}

static void isis_antenna_raw_to_temp_c_test(void **state)
{
}

static void isis_antenna_get_temperature_c_test(void **state)
{
}

static void isis_antenna_delay_s_test(void **state)
{
}

static void isis_antenna_delay_ms_test(void **state)
{
}

int main(void)
{
    const struct CMUnitTest isis_antenna_tests[] = {
        cmocka_unit_test(isis_antenna_init_test),
        cmocka_unit_test(isis_antenna_arm_test),
        cmocka_unit_test(isis_antenna_disarm_test),
        cmocka_unit_test(isis_antenna_start_sequential_deploy_test),
        cmocka_unit_test(isis_antenna_start_independent_deploy_test),
        cmocka_unit_test(isis_antenna_read_deployment_status_code_test),
        cmocka_unit_test(isis_antenna_read_deployment_status_test),
        cmocka_unit_test(isis_antenna_get_antenna_status_test),
        cmocka_unit_test(isis_antenna_get_antenna_timeout_test),
        cmocka_unit_test(isis_antenna_get_burning_test),
        cmocka_unit_test(isis_antenna_get_arming_status_test),
        cmocka_unit_test(isis_antenna_get_raw_temperature_test),
        cmocka_unit_test(isis_antenna_raw_to_temp_c_test),
        cmocka_unit_test(isis_antenna_get_temperature_c_test),
        cmocka_unit_test(isis_antenna_delay_s_test),
        cmocka_unit_test(isis_antenna_delay_ms_test),
    };

    return cmocka_run_group_tests(isis_antenna_tests, NULL, NULL);
}

/** \} End of isis_antenna_test group */
