/*
 * edc_test.c
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
 * \brief Unit test of the EDC driver.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.11
 * 
 * \date 2021/09/01
 * 
 * \defgroup edc_unit_test EDC
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

static void edc_init_test(void **state)
{
}

static void edc_write_cmd_test(void **state)
{
}

static void edc_read_test(void **state)
{
}

static void edc_check_device_test(void **state)
{
}

static void edc_set_rtc_time_test(void **state)
{
}

static void edc_pop_ptt_pkg_test(void **state)
{
}

static void edc_pause_ptt_task_test(void **state)
{
}

static void edc_resume_ptt_task_test(void **state)
{
}

static void edc_start_adc_task_test(void **state)
{
}

static void edc_get_state_pkg_test(void **state)
{
}

static void edc_get_ptt_pkg_test(void **state)
{
}

static void edc_get_hk_pkg_test(void **state)
{
}

static void edc_get_adc_seq_test(void **state)
{
}

static void edc_echo_test(void **state)
{
}

static void edc_calc_checksum_test(void **state)
{
}

static void edc_get_state_test(void **state)
{
}

static void edc_get_hk_test(void **state)
{
}

static void edc_delay_ms_test(void **state)
{
}

int main(void)
{
    const struct CMUnitTest edc_tests[] = {
        cmocka_unit_test(edc_init_test),
        cmocka_unit_test(edc_write_cmd_test),
        cmocka_unit_test(edc_read_test),
        cmocka_unit_test(edc_check_device_test),
        cmocka_unit_test(edc_set_rtc_time_test),
        cmocka_unit_test(edc_pop_ptt_pkg_test),
        cmocka_unit_test(edc_pause_ptt_task_test),
        cmocka_unit_test(edc_resume_ptt_task_test),
        cmocka_unit_test(edc_start_adc_task_test),
        cmocka_unit_test(edc_get_state_pkg_test),
        cmocka_unit_test(edc_get_ptt_pkg_test),
        cmocka_unit_test(edc_get_hk_pkg_test),
        cmocka_unit_test(edc_get_adc_seq_test),
        cmocka_unit_test(edc_echo_test),
        cmocka_unit_test(edc_calc_checksum_test),
        cmocka_unit_test(edc_get_state_test),
        cmocka_unit_test(edc_get_hk_test),
        cmocka_unit_test(edc_delay_ms_test),
    };

    return cmocka_run_group_tests(edc_tests, NULL, NULL);
}

/** \} End of edc_test group */
