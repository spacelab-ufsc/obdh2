/*
 * startup_test.c
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
 * \version 0.9.19
 * 
 * \date 2021/08/14
 * 
 * \defgroup startup_unit_test Startup
 * \ingroup tests
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include <app/tasks/startup.h>
#include <devices/media/media.h>
#include <devices/leds/leds.h>
#include <devices/current_sensor/current_sensor.h>
#include <devices/voltage_sensor/voltage_sensor.h>
#include <devices/temp_sensor/temp_sensor.h>
#include <devices/eps/eps.h>
#include <devices/payload_edc/payload_edc.h>
#include <devices/antenna/antenna.h>

static void vTaskStartup_test(void **state)
{
//    expect_function_call(__wrap_sys_log_init);
//
//    /* Media initialization */
//    expect_value(__wrap_media_init, med, MEDIA_INT_FLASH);
//    will_return(__wrap_media_init, 0);
//
//    expect_value(__wrap_media_init, med, MEDIA_FRAM);
//    will_return(__wrap_media_init, 0);
//
//    expect_value(__wrap_media_init, med, MEDIA_NOR);
//    will_return(__wrap_media_init, 0);
//
//    /* LEDs initialization */
//    will_return(__wrap_leds_init, 0);
//
//    /* Current sensor initialization */
//    will_return(__wrap_current_sensor_init, 0);
//
//    /* Voltage sensor initialization */
//    will_return(__wrap_voltage_sensor_init, 0);
//
//    /* Temperature sensor initialization */
//    will_return(__wrap_temp_sensor_init, 0);
//
//    /* EPS initialization */
//    will_return(__wrap_eps_init, 0);
//
//    /* Antenna initialization */
//    will_return(__wrap_antenna_init, 0);
}

static void startup_init_csp_test(void **state)
{
}

int main(void)
{
    const struct CMUnitTest startup_tests[] = {
        cmocka_unit_test(vTaskStartup_test),
        cmocka_unit_test(startup_init_csp_test),
    };

    return cmocka_run_group_tests(startup_tests, NULL, NULL);
}

/** \} End of startup_test group */
