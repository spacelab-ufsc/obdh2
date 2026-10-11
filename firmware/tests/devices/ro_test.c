/*
 * ro_test.c
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
 * \brief Unit test of the RO device.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Bruno Benedetti <brunobenedetti45@gmail.com> 
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * \author Renato Augusto Schenkel Meneghin Marchiori <renato.meneghin@posgrad.ufsc.br>
 * \version 0.0.01
 * 
 * \date 2026/10/06
 * 
 * \defgroup ro_unit_test RO
 * \ingroup tests
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include <devices/ro/ro.h>
#include <drivers/ro/ro.h>
#include <drivers/gpio/gpio.h>
#include <drivers/uart/uart.h>
#include <drivers/i2c/i2c.h>

#define RO_0_UART_PORT             UART_PORT_1
#define RO_0_GPIO_EN_PIN           GPIO_PIN_29

#define RO_1_UART_PORT             UART_PORT_1
#define RO_1_GPIO_EN_PIN           GPIO_PIN_30

#define RO_I2C_PORT                 I2C_PORT_0
#define RO_I2C_BITRATE              400000UL

static void ro_init_test(void **state)
{
    /* RO 0 */
    expect_value(__wrap_ro_init, config.interface, RO_IF_UART);
    expect_value(__wrap_ro_init, config.en_pin, RO_0_GPIO_EN_PIN);
    expect_value(__wrap_ro_init, config.uart_port, RO_0_UART_PORT);

    will_return(__wrap_ro_init, 0);

    will_return(__wrap_system_get_time, 946684800);

    expect_value(__wrap_ro_set_rtc_time, config.interface, RO_IF_UART);
    expect_value(__wrap_ro_set_rtc_time, config.en_pin, RO_0_GPIO_EN_PIN);
    expect_value(__wrap_ro_set_rtc_time, config.uart_port, RO_0_UART_PORT);

    expect_value(__wrap_ro_set_rtc_time, time, 0);

    will_return(__wrap_ro_set_rtc_time, 0);

    expect_value(__wrap_ro_get_hk, config.interface, RO_IF_UART);
    expect_value(__wrap_ro_get_hk, config.en_pin, RO_0_GPIO_EN_PIN);
    expect_value(__wrap_ro_get_hk, config.uart_port, RO_0_UART_PORT);

    will_return(__wrap_ro_get_hk, 0);      /* Current time */
    will_return(__wrap_ro_get_hk, 5);      /* Elapsed time */
    will_return(__wrap_ro_get_hk, 55);     /* Current supply digital */
    will_return(__wrap_ro_get_hk, 45);     /* Current supply RF front-end */
    will_return(__wrap_ro_get_hk, 5000);   /* Voltage supply */
    will_return(__wrap_ro_get_hk, 18);     /* Temperature */
    will_return(__wrap_ro_get_hk, 1);      /* PLL sync bit */
    will_return(__wrap_ro_get_hk, 4);      /* ADC RMS */
    will_return(__wrap_ro_get_hk, 1);      /* Number of RX PTT */
    will_return(__wrap_ro_get_hk, 6);      /* Max parl decode */
    will_return(__wrap_ro_get_hk, 0);      /* Memory error count */

    will_return(__wrap_ro_get_hk, 0);

    expect_value(__wrap_ro_resume_ptt_task, config.interface, RO_IF_UART);
    expect_value(__wrap_ro_resume_ptt_task, config.en_pin, RO_0_GPIO_EN_PIN);
    expect_value(__wrap_ro_resume_ptt_task, config.uart_port, RO_0_UART_PORT);

    will_return(__wrap_ro_resume_ptt_task, 0);

    assert_return_code(ro_init(PAYLOAD_RO_0), 0);
}

static void ro_enable_test(void **state)
{
    /* RO 0 */
    expect_value(__wrap_ro_enable, config.interface, RO_IF_UART);
    expect_value(__wrap_ro_enable, config.en_pin, RO_0_GPIO_EN_PIN);
    expect_value(__wrap_ro_enable, config.uart_port, RO_0_UART_PORT);

    will_return(__wrap_ro_enable, 0);

    assert_return_code(ro_enable(PAYLOAD_RO_0), 0);
}

static void ro_disable_test(void **state)
{
    /* RO 0 */
    expect_value(__wrap_ro_disable, config.interface, RO_IF_UART);
    expect_value(__wrap_ro_disable, config.en_pin, RO_0_GPIO_EN_PIN);
    expect_value(__wrap_ro_disable, config.uart_port, RO_0_UART_PORT);

    will_return(__wrap_ro_disable, 0);

    assert_return_code(ro_disable(PAYLOAD_RO_0), 0);
}

static void ro_write_cmd_test(void **state)
{
//    assert_return_code(ro_write_cmd(PAYLOAD_RO_0, ), 0);
}

static void ro_get_data_test(void **state)
{
//    uint8_t raw_state_data[500] = {0xFF};

//    will_return(__wrap_ro_get_state_pkg, raw_state_data);
//    will_return(__wrap_ro_get_state_pkg, 0);

//    uint32_t raw_state_len = 0;

//    assert_return_code(ro_get_data(PAYLOAD_RO_0, PAYLOAD_RO_RAW_STATE, raw_state_data, &raw_state_len), 0);
}

int main(void)
{
    const struct CMUnitTest ro_tests[] = {
        cmocka_unit_test(ro_init_test),
        cmocka_unit_test(ro_enable_test),
        cmocka_unit_test(ro_disable_test),
        cmocka_unit_test(ro_write_cmd_test),
        cmocka_unit_test(ro_get_data_test),
    };

    return cmocka_run_group_tests(ro_tests, NULL, NULL);
}

/** \} End of ro_unit_test group */
