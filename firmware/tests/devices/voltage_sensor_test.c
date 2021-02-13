/*
 * voltage_sensor_test.c
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
 * \brief Unit test of the voltage sensor device.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.5.7
 * 
 * \date 2021/02/13
 * 
 * \defgroup voltage_sensor_unit_test Voltage Sensor
 * \ingroup tests
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include <devices/voltage_sensor/voltage_sensor.h>
#include <tests/mockups/sys_log_wrap.h>
#include <tests/mockups/adc_wrap.h>

#define VOLTAGE_SENSOR_ADC_PORT         ADC_PORT_4
#define VOLTAGE_SENSOR_ADC_MIN_VAL      0
#define VOLTAGE_SENSOR_ADC_MAX_VAL      0xFFF       /* 12-bit precision = 0xFFF */
#define VOLTAGE_SENSOR_MIN_VAL_MV       0
#define VOLTAGE_SENSOR_MAX_VAL_MV       (uint16_t)(1000.0*VOLTAGE_SENSOR_ADC_MAX_VAL*ADC_AVCC*VOLTAGE_SENSOR_DIV/ADC_RANGE)

static void voltage_sensor_init_test(void **state)
{
    expect_value(__wrap_adc_init, port, VOLTAGE_SENSOR_ADC_PORT);
    expect_value(__wrap_adc_read, port, VOLTAGE_SENSOR_ADC_PORT);

    will_return(__wrap_adc_read, 1024);

    int result = voltage_sensor_init();

    assert_return_code(result, 0);
}

static void voltage_sensor_read_raw_test(void **state)
{
    uint16_t i = 0;

    for(i=VOLTAGE_SENSOR_ADC_MIN_VAL; i<=VOLTAGE_SENSOR_ADC_MAX_VAL; i++)
    {
        expect_value(__wrap_adc_read, port, VOLTAGE_SENSOR_ADC_PORT);

        will_return(__wrap_adc_read, i);

        uint16_t raw_voltage = UINT16_MAX;

        int result = voltage_sensor_read_raw(&raw_voltage);

        assert_return_code(result, 0);
        assert_int_equal(raw_voltage, i);
    }
}

static void voltage_sensor_raw_to_mv_test(void **state)
{
    uint16_t i = 0;

    for(i=VOLTAGE_SENSOR_ADC_MIN_VAL; i<=VOLTAGE_SENSOR_ADC_MAX_VAL; i++)
    {
        uint16_t voltage_mv = voltage_sensor_raw_to_mv(i);

        assert_in_range(voltage_mv, VOLTAGE_SENSOR_MIN_VAL_MV, VOLTAGE_SENSOR_MAX_VAL_MV);
    }
}

static void voltage_sensor_read_mv_test(void **state)
{
    uint16_t i = 0;

    for(i=VOLTAGE_SENSOR_ADC_MIN_VAL; i<=VOLTAGE_SENSOR_ADC_MAX_VAL; i++)
    {
        expect_value(__wrap_adc_read, port, VOLTAGE_SENSOR_ADC_PORT);

        will_return(__wrap_adc_read, i);

        uint16_t voltage_mv = UINT16_MAX;

        int result = voltage_sensor_read_mv(&voltage_mv);

        assert_return_code(result, 0);
        assert_in_range(voltage_mv, VOLTAGE_SENSOR_MIN_VAL_MV, VOLTAGE_SENSOR_MAX_VAL_MV);
    }
}

int main(void)
{
    const struct CMUnitTest voltage_sensor_tests[] = {
        cmocka_unit_test(voltage_sensor_init_test),
        cmocka_unit_test(voltage_sensor_read_raw_test),
        cmocka_unit_test(voltage_sensor_raw_to_mv_test),
        cmocka_unit_test(voltage_sensor_read_mv_test),
    };

    return cmocka_run_group_tests(voltage_sensor_tests, NULL, NULL);
}

/** \} End of voltage_sensor_unit_test group */
