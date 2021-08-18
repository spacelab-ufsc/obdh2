/*
 * payload_test.c
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
 * \brief Unit test of the payload device.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.6
 * 
 * \date 2021/08/16
 * 
 * \defgroup payload_unit_test Payload
 * \ingroup tests
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include <drivers/gpio/gpio.h>
#include <devices/payload/payload.h>
#include <tests/mockups/edc_wrap.h>
#include <tests/mockups/phj_wrap.h>

static void payload_init_test(void **state)
{
    expect_value(__wrap_edc_init, config.port, I2C_PORT_0);
    expect_value(__wrap_edc_init, config.bitrate, 400000);

    will_return(__wrap_edc_init, 0);

    will_return(__wrap_edc_pause_ptt_task, 0);

    will_return(__wrap_edc_get_hk, 0);      /* Current time */
    will_return(__wrap_edc_get_hk, 5);      /* Elapsed time */
    will_return(__wrap_edc_get_hk, 100);    /* Current supply */
    will_return(__wrap_edc_get_hk, 5000);   /* Voltage supply */
    will_return(__wrap_edc_get_hk, 18);     /* Temperature */
    will_return(__wrap_edc_get_hk, 1);      /* PLL sync bit */
    will_return(__wrap_edc_get_hk, 4);      /* ADC RMS */
    will_return(__wrap_edc_get_hk, 1);      /* Number of RX PTT */
    will_return(__wrap_edc_get_hk, 6);      /* Max parl decode */
    will_return(__wrap_edc_get_hk, 0);      /* Memory error count */

    will_return(__wrap_edc_get_hk, 0);

    assert_return_code(payload_init(PAYLOAD_EDC), 0);
//    assert_return_code(payload_init(PAYLOAD_X), 0);

    expect_value(__wrap_phj_init_i2c, config.port, I2C_PORT_0);
    expect_value(__wrap_phj_init_i2c, config.bitrate, 400000);

    will_return(__wrap_phj_init_i2c, 0);

    expect_value(__wrap_phj_init_gpio, config.pin, GPIO_PIN_0);
    expect_value(__wrap_phj_init_gpio, config.mode, GPIO_MODE_INPUT);

    will_return(__wrap_phj_init_gpio, 0);

    assert_return_code(payload_init(PAYLOAD_PHJ), 0);
//    assert_return_code(payload_init(PAYLOAD_HARSH), 0);
}

static void payload_enable_test(void **state)
{
    will_return(__wrap_edc_resume_ptt_task, 0);

    assert_return_code(payload_enable(PAYLOAD_EDC), 0);
//    assert_return_code(payload_enable(PAYLOAD_X), 0);
//    assert_return_code(payload_enable(PAYLOAD_PHJ), 0);
//    assert_return_code(payload_enable(PAYLOAD_HARSH), 0);
}

static void payload_disable_test(void **state)
{
    will_return(__wrap_edc_pause_ptt_task, 0);

    assert_return_code(payload_disable(PAYLOAD_EDC), 0);
//    assert_return_code(payload_disable(PAYLOAD_X), 0);
//    assert_return_code(payload_disable(PAYLOAD_PHJ), 0);
//    assert_return_code(payload_disable(PAYLOAD_HARSH), 0);
}

static void payload_write_cmd_test(void **state)
{
//    assert_return_code(payload_write_cmd(PAYLOAD_EDC, ), 0);
//    assert_return_code(payload_write_cmd(PAYLOAD_X, ), 0);
//    assert_return_code(payload_write_cmd(PAYLOAD_PHJ, ), 0);
//    assert_return_code(payload_write_cmd(PAYLOAD_HARSH, ), 0);
}

static void payload_get_data_test(void **state)
{
//    uint8_t raw_state_data[500] = {0xFF};

//    will_return(__wrap_edc_get_state_pkg, raw_state_data);
//    will_return(__wrap_edc_get_state_pkg, 0);

//    uint32_t raw_state_len = 0;

//    assert_return_code(payload_get_data(PAYLOAD_EDC, PAYLOAD_EDC_RAW_STATE, raw_state_data, &raw_state_len), 0);

//    uint8_t raw_hk_data[500] = {0xFF};

//    will_return(__wrap_edc_get_state_pkg, raw_hk_data);
//    will_return(__wrap_edc_get_state_pkg, 0);

//    uint32_t raw_hk_len = 0;

//    assert_return_code(payload_get_data(PAYLOAD_EDC, PAYLOAD_EDC_RAW_HK, raw_hk_data, &raw_hk_len), 0);
//    assert_return_code(payload_get_data(PAYLOAD_X, ), 0);
//    assert_return_code(payload_get_data(PAYLOAD_PHJ, ), 0);
//    assert_return_code(payload_get_data(PAYLOAD_HARSH, ), 0);
}

int main(void)
{
    const struct CMUnitTest payload_tests[] = {
        cmocka_unit_test(payload_init_test),
        cmocka_unit_test(payload_enable_test),
        cmocka_unit_test(payload_disable_test),
        cmocka_unit_test(payload_write_cmd_test),
        cmocka_unit_test(payload_get_data_test),
    };

    return cmocka_run_group_tests(payload_tests, NULL, NULL);
}

/** \} End of payload_unit_test group */
