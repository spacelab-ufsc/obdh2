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
#include <drivers/isis_antenna/isis_antenna.h>

static void antenna_init_test(void **state)
{
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

                will_return(__wrap_isis_antenna_read_deployment_status, 0);                                 /* Status code */
                will_return(__wrap_isis_antenna_read_deployment_status, ISIS_ANTENNA_STATUS_NOT_DEPLOYED);  /* Antenna 1 status */
                will_return(__wrap_isis_antenna_read_deployment_status, 0);                                 /* Antenna 1 timeout */
                will_return(__wrap_isis_antenna_read_deployment_status, ISIS_ANTENNA_BURN_INACTIVE);        /* Antenna 1 burning */
                will_return(__wrap_isis_antenna_read_deployment_status, ISIS_ANTENNA_STATUS_NOT_DEPLOYED);  /* Antenna 2 status */
                will_return(__wrap_isis_antenna_read_deployment_status, 0);                                 /* Antenna 2 timeout */
                will_return(__wrap_isis_antenna_read_deployment_status, ISIS_ANTENNA_BURN_INACTIVE);        /* Antenna 2 burning */
                will_return(__wrap_isis_antenna_read_deployment_status, ISIS_ANTENNA_STATUS_NOT_DEPLOYED);  /* Antenna 3 status */
                will_return(__wrap_isis_antenna_read_deployment_status, 0);                                 /* Antenna 3 timeout */
                will_return(__wrap_isis_antenna_read_deployment_status, ISIS_ANTENNA_BURN_INACTIVE);        /* Antenna 3 burning */
                will_return(__wrap_isis_antenna_read_deployment_status, ISIS_ANTENNA_STATUS_NOT_DEPLOYED);  /* Antenna 4 status */
                will_return(__wrap_isis_antenna_read_deployment_status, 0);                                 /* Antenna 4 timeout */
                will_return(__wrap_isis_antenna_read_deployment_status, ISIS_ANTENNA_BURN_INACTIVE);        /* Antenna 4 burning */
                will_return(__wrap_isis_antenna_read_deployment_status, 0);                                 /* Ignoring switches */
                will_return(__wrap_isis_antenna_read_deployment_status, 0);                                 /* Independent burn */
                will_return(__wrap_isis_antenna_read_deployment_status, 0);                                 /* Armed */

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
