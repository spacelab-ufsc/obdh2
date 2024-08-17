/*
 * payload_test.c
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
 * \brief Unit test of the payload device.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Bruno Benedetti <brunobenedetti45@gmail.com> 
 * 
 * \version 0.10.11
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

#include <devices/payload/payload.h>
#include <drivers/edc/edc.h>
#include <drivers/px/px.h>
#include <drivers/gpio/gpio.h>
#include <drivers/uart/uart.h>
#include <drivers/i2c/i2c.h>

#define EDC_0_UART_PORT             UART_PORT_1
#define EDC_0_GPIO_EN_PIN           GPIO_PIN_29

#define EDC_1_UART_PORT             UART_PORT_1
#define EDC_1_GPIO_EN_PIN           GPIO_PIN_30

#define PX_I2C_PORT                 I2C_PORT_0
#define PX_I2C_BITRATE              400000UL

static void payload_init_test(void **state)
{
    /* EDC 0 */
    expect_value(__wrap_edc_init, config.interface, EDC_IF_UART);
    expect_value(__wrap_edc_init, config.en_pin, EDC_0_GPIO_EN_PIN);
    expect_value(__wrap_edc_init, config.uart_port, EDC_0_UART_PORT);

    will_return(__wrap_edc_init, 0);

    will_return(__wrap_system_get_time, 946684800);

    expect_value(__wrap_edc_set_rtc_time, config.interface, EDC_IF_UART);
    expect_value(__wrap_edc_set_rtc_time, config.en_pin, EDC_0_GPIO_EN_PIN);
    expect_value(__wrap_edc_set_rtc_time, config.uart_port, EDC_0_UART_PORT);

    expect_value(__wrap_edc_set_rtc_time, time, 0);

    will_return(__wrap_edc_set_rtc_time, 0);

    expect_value(__wrap_edc_get_hk, config.interface, EDC_IF_UART);
    expect_value(__wrap_edc_get_hk, config.en_pin, EDC_0_GPIO_EN_PIN);
    expect_value(__wrap_edc_get_hk, config.uart_port, EDC_0_UART_PORT);

    will_return(__wrap_edc_get_hk, 0);      /* Current time */
    will_return(__wrap_edc_get_hk, 5);      /* Elapsed time */
    will_return(__wrap_edc_get_hk, 55);     /* Current supply digital */
    will_return(__wrap_edc_get_hk, 45);     /* Current supply RF front-end */
    will_return(__wrap_edc_get_hk, 5000);   /* Voltage supply */
    will_return(__wrap_edc_get_hk, 18);     /* Temperature */
    will_return(__wrap_edc_get_hk, 1);      /* PLL sync bit */
    will_return(__wrap_edc_get_hk, 4);      /* ADC RMS */
    will_return(__wrap_edc_get_hk, 1);      /* Number of RX PTT */
    will_return(__wrap_edc_get_hk, 6);      /* Max parl decode */
    will_return(__wrap_edc_get_hk, 0);      /* Memory error count */

    will_return(__wrap_edc_get_hk, 0);

    expect_value(__wrap_edc_resume_ptt_task, config.interface, EDC_IF_UART);
    expect_value(__wrap_edc_resume_ptt_task, config.en_pin, EDC_0_GPIO_EN_PIN);
    expect_value(__wrap_edc_resume_ptt_task, config.uart_port, EDC_0_UART_PORT);

    will_return(__wrap_edc_resume_ptt_task, 0);

    assert_return_code(payload_init(PAYLOAD_EDC_0), 0);

    /* EDC 1 */
    expect_value(__wrap_edc_init, config.interface, EDC_IF_UART);
    expect_value(__wrap_edc_init, config.en_pin, EDC_1_GPIO_EN_PIN);
    expect_value(__wrap_edc_init, config.uart_port, EDC_1_UART_PORT);

    will_return(__wrap_edc_init, 0);

    will_return(__wrap_system_get_time, 946684800);

    expect_value(__wrap_edc_set_rtc_time, config.interface, EDC_IF_UART);
    expect_value(__wrap_edc_set_rtc_time, config.en_pin, EDC_1_GPIO_EN_PIN);
    expect_value(__wrap_edc_set_rtc_time, config.uart_port, EDC_1_UART_PORT);

    expect_value(__wrap_edc_set_rtc_time, time, 0);

    will_return(__wrap_edc_set_rtc_time, 0);

    expect_value(__wrap_edc_get_hk, config.interface, EDC_IF_UART);
    expect_value(__wrap_edc_get_hk, config.en_pin, EDC_1_GPIO_EN_PIN);
    expect_value(__wrap_edc_get_hk, config.uart_port, EDC_1_UART_PORT);

    will_return(__wrap_edc_get_hk, 0);      /* Current time */
    will_return(__wrap_edc_get_hk, 5);      /* Elapsed time */
    will_return(__wrap_edc_get_hk, 55);     /* Current supply digital*/
    will_return(__wrap_edc_get_hk, 45);     /* Current supply RF front-end */
    will_return(__wrap_edc_get_hk, 5000);   /* Voltage supply */
    will_return(__wrap_edc_get_hk, 18);     /* Temperature */
    will_return(__wrap_edc_get_hk, 1);      /* PLL sync bit */
    will_return(__wrap_edc_get_hk, 4);      /* ADC RMS */
    will_return(__wrap_edc_get_hk, 1);      /* Number of RX PTT */
    will_return(__wrap_edc_get_hk, 6);      /* Max parl decode */
    will_return(__wrap_edc_get_hk, 0);      /* Memory error count */

    will_return(__wrap_edc_get_hk, 0);

    expect_value(__wrap_edc_resume_ptt_task, config.interface, EDC_IF_UART);
    expect_value(__wrap_edc_resume_ptt_task, config.en_pin, EDC_1_GPIO_EN_PIN);
    expect_value(__wrap_edc_resume_ptt_task, config.uart_port, EDC_1_UART_PORT);

    will_return(__wrap_edc_resume_ptt_task, 0);

    assert_return_code(payload_init(PAYLOAD_EDC_1), 0);

    /* Payload-X */
    expect_value(__wrap_px_init, conf->port, PX_I2C_PORT);
    expect_value(__wrap_px_init, conf->bitrate, PX_I2C_BITRATE);
    expect_value(__wrap_px_init, conf->en_pin, GPIO_PIN_37);

    will_return(__wrap_px_init, 0);

    assert_return_code(payload_init(PAYLOAD_X), 0);
}

static void payload_enable_test(void **state)
{
    /* EDC 0 */
    expect_value(__wrap_edc_enable, config.interface, EDC_IF_UART);
    expect_value(__wrap_edc_enable, config.en_pin, EDC_0_GPIO_EN_PIN);
    expect_value(__wrap_edc_enable, config.uart_port, EDC_0_UART_PORT);

    will_return(__wrap_edc_enable, 0);

    assert_return_code(payload_enable(PAYLOAD_EDC_0), 0);

    /* EDC 1 */
    expect_value(__wrap_edc_enable, config.interface, EDC_IF_UART);
    expect_value(__wrap_edc_enable, config.en_pin, EDC_1_GPIO_EN_PIN);
    expect_value(__wrap_edc_enable, config.uart_port, EDC_1_UART_PORT);

    will_return(__wrap_edc_enable, 0);

    assert_return_code(payload_enable(PAYLOAD_EDC_1), 0);

    expect_value(__wrap_px_enable, conf->en_pin, GPIO_PIN_37);

    will_return(__wrap_px_enable, 0);

    assert_return_code(payload_enable(PAYLOAD_X), 0);
//    assert_return_code(payload_enable(PAYLOAD_PHJ), 0);
}

static void payload_disable_test(void **state)
{
    /* EDC 0 */
    expect_value(__wrap_edc_disable, config.interface, EDC_IF_UART);
    expect_value(__wrap_edc_disable, config.en_pin, EDC_0_GPIO_EN_PIN);
    expect_value(__wrap_edc_disable, config.uart_port, EDC_0_UART_PORT);

    will_return(__wrap_edc_disable, 0);

    assert_return_code(payload_disable(PAYLOAD_EDC_0), 0);

    /* EDC 1 */
    expect_value(__wrap_edc_disable, config.interface, EDC_IF_UART);
    expect_value(__wrap_edc_disable, config.en_pin, EDC_1_GPIO_EN_PIN);
    expect_value(__wrap_edc_disable, config.uart_port, EDC_1_UART_PORT);

    will_return(__wrap_edc_disable, 0);

    assert_return_code(payload_disable(PAYLOAD_EDC_1), 0);

    expect_value(__wrap_px_disable, conf->en_pin, GPIO_PIN_37);

    will_return(__wrap_px_disable, 0);

    assert_return_code(payload_disable(PAYLOAD_X), 0);
//    assert_return_code(payload_disable(PAYLOAD_PHJ), 0);
}

static void payload_write_cmd_test(void **state)
{
//    assert_return_code(payload_write_cmd(PAYLOAD_EDC_0, ), 0);
//    assert_return_code(payload_write_cmd(PAYLOAD_X, ), 0);
//    assert_return_code(payload_write_cmd(PAYLOAD_PHJ, ), 0);
}

static void payload_get_data_test(void **state)
{
//    uint8_t raw_state_data[500] = {0xFF};

//    will_return(__wrap_edc_get_state_pkg, raw_state_data);
//    will_return(__wrap_edc_get_state_pkg, 0);

//    uint32_t raw_state_len = 0;

//    assert_return_code(payload_get_data(PAYLOAD_EDC_0, PAYLOAD_EDC_RAW_STATE, raw_state_data, &raw_state_len), 0);

//    uint8_t raw_hk_data[500] = {0xFF};

//    will_return(__wrap_edc_get_state_pkg, raw_hk_data);
//    will_return(__wrap_edc_get_state_pkg, 0);

//    uint32_t raw_hk_len = 0;

//    assert_return_code(payload_get_data(PAYLOAD_EDC_0, PAYLOAD_EDC_RAW_HK, raw_hk_data, &raw_hk_len), 0);
//    assert_return_code(payload_get_data(PAYLOAD_X, ), 0);
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
