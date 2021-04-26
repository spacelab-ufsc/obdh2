/*
 * current_sensor_test.c
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
 * \brief Unit test of the current sensor device.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.6.3
 * 
 * \date 2021/02/11
 * 
 * \defgroup current_sensor_unit_test Current Sensor
 * \ingroup tests
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include <devices/current_sensor/current_sensor.h>
#include <tests/mockups/sys_log_wrap.h>
#include <tests/mockups/adc_wrap.h>

#define CURRENT_SENSOR_ADC_PORT         ADC_PORT_8
#define CURRENT_SENSOR_ADC_MIN_VAL      0
#define CURRENT_SENSOR_ADC_MAX_VAL      0xFFF       /* 12-bit precision = 0xFFF */
#define CURRENT_SENSOR_MIN_VAL_MA       0
#define CURRENT_SENSOR_MAX_VAL_MA       (uint16_t)(1000*CURRENT_SENSOR_ADC_MAX_VAL*(ADC_VREF_MV/(ADC_RANGE*CURRENT_SENSOR_RL_VALUE_OHM*CURRENT_SENSOR_GAIN_A_V*CURRENT_SENSOR_RSENSE_VALUE_OHM)))

static void current_sensor_init_test(void **state)
{
    expect_value(__wrap_adc_init, port, CURRENT_SENSOR_ADC_PORT);
    expect_value(__wrap_adc_read, port, CURRENT_SENSOR_ADC_PORT);

    will_return(__wrap_adc_read, 1024);

    int result = current_sensor_init();

    assert_return_code(result, 0);
}

static void current_sensor_read_raw_test(void **state)
{
    uint16_t i = 0;

    for(i=CURRENT_SENSOR_ADC_MIN_VAL; i<=CURRENT_SENSOR_ADC_MAX_VAL; i++)
    {
        expect_value(__wrap_adc_read, port, CURRENT_SENSOR_ADC_PORT);

        will_return(__wrap_adc_read, i);

        uint16_t raw_current = UINT16_MAX;

        int result = current_sensor_read_raw(&raw_current);

        assert_return_code(result, 0);
        assert_int_equal(raw_current, i);
    }
}

static void current_sensor_raw_to_ma_test(void **state)
{
    uint16_t i = 0;

    for(i=CURRENT_SENSOR_ADC_MIN_VAL; i<=CURRENT_SENSOR_ADC_MAX_VAL; i++)
    {
        uint16_t current_ma = current_sensor_raw_to_ma(i);

        assert_in_range(current_ma, CURRENT_SENSOR_MIN_VAL_MA, CURRENT_SENSOR_MAX_VAL_MA);
    }
}

static void current_sensor_read_ma_test(void **state)
{
    uint16_t i = 0;

    for(i=CURRENT_SENSOR_ADC_MIN_VAL; i<=CURRENT_SENSOR_ADC_MAX_VAL; i++)
    {
        expect_value(__wrap_adc_read, port, CURRENT_SENSOR_ADC_PORT);

        will_return(__wrap_adc_read, i);

        uint16_t current_ma = UINT16_MAX;

        int result = current_sensor_read_ma(&current_ma);

        assert_return_code(result, 0);
        assert_in_range(current_ma, CURRENT_SENSOR_MIN_VAL_MA, CURRENT_SENSOR_MAX_VAL_MA);
    }
}

int main(void)
{
    const struct CMUnitTest current_sensor_tests[] = {
        cmocka_unit_test(current_sensor_init_test),
        cmocka_unit_test(current_sensor_read_raw_test),
        cmocka_unit_test(current_sensor_raw_to_ma_test),
        cmocka_unit_test(current_sensor_read_ma_test),
    };

    return cmocka_run_group_tests(current_sensor_tests, NULL, NULL);
}

/** \} End of current_sensor_unit_test group */
