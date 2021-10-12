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
 * \version 0.8.1
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

#include <stdlib.h>

#include <devices/antenna/antenna.h>
#include <drivers/isis_antenna/isis_antenna.h>

#define ANTENNA_INDEPENDENT_DEPLOY_BURN_TIME_SEC        10
#define ANTENNA_SEQUENTIAL_DEPLOY_BURN_TIME_SEC         20

unsigned int generate_random(unsigned int l, unsigned int r);

static void antenna_init_test(void **state)
{
    isis_antenna_status_t status_test = {0};

    status_test.code                = generate_random(0, UINT16_MAX);
    status_test.antenna_1.status    = generate_random(0, UINT8_MAX);
    status_test.antenna_1.timeout   = generate_random(0, UINT8_MAX);
    status_test.antenna_1.burning   = generate_random(0, UINT8_MAX);
    status_test.antenna_2.status    = generate_random(0, UINT8_MAX);
    status_test.antenna_2.timeout   = generate_random(0, UINT8_MAX);
    status_test.antenna_2.burning   = generate_random(0, UINT8_MAX);
    status_test.antenna_3.status    = generate_random(0, UINT8_MAX);
    status_test.antenna_3.timeout   = generate_random(0, UINT8_MAX);
    status_test.antenna_3.burning   = generate_random(0, UINT8_MAX);
    status_test.antenna_4.status    = generate_random(0, UINT8_MAX);
    status_test.antenna_4.timeout   = generate_random(0, UINT8_MAX);
    status_test.antenna_4.burning   = generate_random(0, UINT8_MAX);
    status_test.ignoring_switches   = generate_random(0, UINT8_MAX);
    status_test.independent_burn    = generate_random(0, UINT8_MAX);
    status_test.armed               = generate_random(0, UINT8_MAX);

    int16_t temp_test = generate_random(0, 70);

    int i = 0;
    for(i=-256; i<256; i++)     /* Reduce the test range to avoid a long execution time */
    {
        if (i == 0)
        {
            int j = 0;
            for(j=-256; j<256; j++)
            {
                expect_function_call(__wrap_isis_antenna_init);
                will_return(__wrap_isis_antenna_init, i);

                /* Temperature */
                will_return(__wrap_isis_antenna_get_temperature_c, temp_test);

                will_return(__wrap_isis_antenna_get_temperature_c, 0);

                /* Status */
                will_return(__wrap_isis_antenna_read_deployment_status, status_test.code);
                will_return(__wrap_isis_antenna_read_deployment_status, status_test.antenna_1.status);
                will_return(__wrap_isis_antenna_read_deployment_status, status_test.antenna_1.timeout);
                will_return(__wrap_isis_antenna_read_deployment_status, status_test.antenna_1.burning);
                will_return(__wrap_isis_antenna_read_deployment_status, status_test.antenna_2.status);
                will_return(__wrap_isis_antenna_read_deployment_status, status_test.antenna_2.timeout);
                will_return(__wrap_isis_antenna_read_deployment_status, status_test.antenna_2.burning);
                will_return(__wrap_isis_antenna_read_deployment_status, status_test.antenna_3.status);
                will_return(__wrap_isis_antenna_read_deployment_status, status_test.antenna_3.timeout);
                will_return(__wrap_isis_antenna_read_deployment_status, status_test.antenna_3.burning);
                will_return(__wrap_isis_antenna_read_deployment_status, status_test.antenna_4.status);
                will_return(__wrap_isis_antenna_read_deployment_status, status_test.antenna_4.timeout);
                will_return(__wrap_isis_antenna_read_deployment_status, status_test.antenna_4.burning);
                will_return(__wrap_isis_antenna_read_deployment_status, status_test.ignoring_switches);
                will_return(__wrap_isis_antenna_read_deployment_status, status_test.independent_burn);
                will_return(__wrap_isis_antenna_read_deployment_status, status_test.armed);

                will_return(__wrap_isis_antenna_read_deployment_status, j);

                int result = antenna_init();

                if (j == 0)
                {
                    assert_return_code(result, 0);
                }
                else
                {
                    assert_int_equal(result, -1);
                }
            }
        }
        else
        {
            expect_function_call(__wrap_isis_antenna_init);
            will_return(__wrap_isis_antenna_init, i);

            int result = antenna_init();

            if (i == 0)
            {
                assert_return_code(result, 0);
            }
            else
            {
                assert_int_equal(result, -1);
            }
        }
    }
}

static void antenna_get_data_test(void **state)
{
    antenna_data_t data_test = {0};

    data_test.status.code               = generate_random(0, UINT16_MAX);
    data_test.status.antenna_1.status   = generate_random(0, UINT8_MAX);
    data_test.status.antenna_1.timeout  = generate_random(0, UINT8_MAX);
    data_test.status.antenna_1.burning  = generate_random(0, UINT8_MAX);
    data_test.status.antenna_2.status   = generate_random(0, UINT8_MAX);
    data_test.status.antenna_2.timeout  = generate_random(0, UINT8_MAX);
    data_test.status.antenna_2.burning  = generate_random(0, UINT8_MAX);
    data_test.status.antenna_3.status   = generate_random(0, UINT8_MAX);
    data_test.status.antenna_3.timeout  = generate_random(0, UINT8_MAX);
    data_test.status.antenna_3.burning  = generate_random(0, UINT8_MAX);
    data_test.status.antenna_4.status   = generate_random(0, UINT8_MAX);
    data_test.status.antenna_4.timeout  = generate_random(0, UINT8_MAX);
    data_test.status.antenna_4.burning  = generate_random(0, UINT8_MAX);
    data_test.status.ignoring_switches  = generate_random(0, UINT8_MAX);
    data_test.status.independent_burn   = generate_random(0, UINT8_MAX);
    data_test.status.armed              = generate_random(0, UINT8_MAX);
    data_test.temperature               = generate_random(0, 70);

    /* Temperature */
    will_return(__wrap_isis_antenna_get_temperature_c, data_test.temperature);

    will_return(__wrap_isis_antenna_get_temperature_c, 0);

    /* Status */
    will_return(__wrap_isis_antenna_read_deployment_status, data_test.status.code);
    will_return(__wrap_isis_antenna_read_deployment_status, data_test.status.antenna_1.status);
    will_return(__wrap_isis_antenna_read_deployment_status, data_test.status.antenna_1.timeout);
    will_return(__wrap_isis_antenna_read_deployment_status, data_test.status.antenna_1.burning);
    will_return(__wrap_isis_antenna_read_deployment_status, data_test.status.antenna_2.status);
    will_return(__wrap_isis_antenna_read_deployment_status, data_test.status.antenna_2.timeout);
    will_return(__wrap_isis_antenna_read_deployment_status, data_test.status.antenna_2.burning);
    will_return(__wrap_isis_antenna_read_deployment_status, data_test.status.antenna_3.status);
    will_return(__wrap_isis_antenna_read_deployment_status, data_test.status.antenna_3.timeout);
    will_return(__wrap_isis_antenna_read_deployment_status, data_test.status.antenna_3.burning);
    will_return(__wrap_isis_antenna_read_deployment_status, data_test.status.antenna_4.status);
    will_return(__wrap_isis_antenna_read_deployment_status, data_test.status.antenna_4.timeout);
    will_return(__wrap_isis_antenna_read_deployment_status, data_test.status.antenna_4.burning);
    will_return(__wrap_isis_antenna_read_deployment_status, data_test.status.ignoring_switches);
    will_return(__wrap_isis_antenna_read_deployment_status, data_test.status.independent_burn);
    will_return(__wrap_isis_antenna_read_deployment_status, data_test.status.armed);

    will_return(__wrap_isis_antenna_read_deployment_status, 0);

    antenna_data_t data_val = {0};

    assert_return_code(antenna_get_data(&data_val), 0);

    assert_memory_equal((void*)&data_test, (void*)&data_val, sizeof(antenna_data_t));
}

static void antenna_deploy_test(void **state)
{
    /* Arm */
    expect_function_call(__wrap_isis_antenna_arm);

    will_return(__wrap_isis_antenna_arm, 0);

    /* Independent deploy */
    expect_value(__wrap_isis_antenna_start_independent_deploy, ant, ISIS_ANTENNA_ANT_1);
    expect_value(__wrap_isis_antenna_start_independent_deploy, sec, ANTENNA_INDEPENDENT_DEPLOY_BURN_TIME_SEC);
    expect_value(__wrap_isis_antenna_start_independent_deploy, ovr, ISIS_ANTENNA_INDEPENDENT_DEPLOY_WITH_OVERRIDE);

    will_return(__wrap_isis_antenna_start_independent_deploy, 0);

    expect_function_call(__wrap_isis_antenna_delay_s);

    expect_value(__wrap_isis_antenna_start_independent_deploy, ant, ISIS_ANTENNA_ANT_2);
    expect_value(__wrap_isis_antenna_start_independent_deploy, sec, ANTENNA_INDEPENDENT_DEPLOY_BURN_TIME_SEC);
    expect_value(__wrap_isis_antenna_start_independent_deploy, ovr, ISIS_ANTENNA_INDEPENDENT_DEPLOY_WITH_OVERRIDE);

    will_return(__wrap_isis_antenna_start_independent_deploy, 0);

    expect_function_call(__wrap_isis_antenna_delay_s);

    expect_value(__wrap_isis_antenna_start_independent_deploy, ant, ISIS_ANTENNA_ANT_3);
    expect_value(__wrap_isis_antenna_start_independent_deploy, sec, ANTENNA_INDEPENDENT_DEPLOY_BURN_TIME_SEC);
    expect_value(__wrap_isis_antenna_start_independent_deploy, ovr, ISIS_ANTENNA_INDEPENDENT_DEPLOY_WITH_OVERRIDE);

    will_return(__wrap_isis_antenna_start_independent_deploy, 0);

    expect_function_call(__wrap_isis_antenna_delay_s);

    expect_value(__wrap_isis_antenna_start_independent_deploy, ant, ISIS_ANTENNA_ANT_4);
    expect_value(__wrap_isis_antenna_start_independent_deploy, sec, ANTENNA_INDEPENDENT_DEPLOY_BURN_TIME_SEC);
    expect_value(__wrap_isis_antenna_start_independent_deploy, ovr, ISIS_ANTENNA_INDEPENDENT_DEPLOY_WITH_OVERRIDE);

    will_return(__wrap_isis_antenna_start_independent_deploy, 0);

    expect_function_call(__wrap_isis_antenna_delay_s);

    /* Sequential deploy */
    expect_value(__wrap_isis_antenna_start_sequential_deploy, sec, ANTENNA_SEQUENTIAL_DEPLOY_BURN_TIME_SEC);

    will_return(__wrap_isis_antenna_start_sequential_deploy, 0);

    expect_function_call(__wrap_isis_antenna_delay_s);

    /* Deployment check */
    will_return(__wrap_isis_antenna_read_deployment_status, generate_random(0, UINT16_MAX));
    will_return(__wrap_isis_antenna_read_deployment_status, generate_random(0, 1));
    will_return(__wrap_isis_antenna_read_deployment_status, generate_random(0, 1));
    will_return(__wrap_isis_antenna_read_deployment_status, generate_random(0, 1));
    will_return(__wrap_isis_antenna_read_deployment_status, generate_random(0, 1));
    will_return(__wrap_isis_antenna_read_deployment_status, generate_random(0, 1));
    will_return(__wrap_isis_antenna_read_deployment_status, generate_random(0, 1));
    will_return(__wrap_isis_antenna_read_deployment_status, generate_random(0, 1));
    will_return(__wrap_isis_antenna_read_deployment_status, generate_random(0, 1));
    will_return(__wrap_isis_antenna_read_deployment_status, generate_random(0, 1));
    will_return(__wrap_isis_antenna_read_deployment_status, generate_random(0, 1));
    will_return(__wrap_isis_antenna_read_deployment_status, generate_random(0, 1));
    will_return(__wrap_isis_antenna_read_deployment_status, generate_random(0, 1));
    will_return(__wrap_isis_antenna_read_deployment_status, generate_random(0, 1));
    will_return(__wrap_isis_antenna_read_deployment_status, generate_random(0, 1));
    will_return(__wrap_isis_antenna_read_deployment_status, generate_random(0, 1));

    will_return(__wrap_isis_antenna_read_deployment_status, 0);

    /* Disarm */
    expect_function_call(__wrap_isis_antenna_disarm);

    will_return(__wrap_isis_antenna_disarm, 0);

    assert_return_code(antenna_deploy(generate_random(1, 30000U)), 0);
}

int main(void)
{
    const struct CMUnitTest antenna_tests[] = {
        cmocka_unit_test(antenna_init_test),
        cmocka_unit_test(antenna_get_data_test),
        cmocka_unit_test(antenna_deploy_test),
    };

    return cmocka_run_group_tests(antenna_tests, NULL, NULL);
}

unsigned int generate_random(unsigned int l, unsigned int r)
{
    return (rand() % (r - l + 1)) + l;
}

/** \} End of antenna_test group */
