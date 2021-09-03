/*
 * radio_test.c
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
 * \brief Unit test of the radio device.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.9
 * 
 * \date 2021/02/21
 * 
 * \defgroup radio_unit_test Radio
 * \ingroup tests
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include <config/config.h>
#include <config/radio_config_Si4463.h>
#include <devices/radio/radio.h>
#include <drivers/si446x/si446x.h>

#define RADIO_ID    0x4463

static void radio_init_test(void **state)
{
    /* SPI init */
    will_return(__wrap_si446x_spi_init, SI446X_SUCCESS);

    /* Reset */
    will_return(__wrap_si446x_reset, SI446X_SUCCESS);

    /* Part info */
    will_return(__wrap_si446x_part_info, 0);
    will_return(__wrap_si446x_part_info, RADIO_ID);
    will_return(__wrap_si446x_part_info, 0);
    will_return(__wrap_si446x_part_info, 0);
    will_return(__wrap_si446x_part_info, 0);
    will_return(__wrap_si446x_part_info, 0);
    will_return(__wrap_si446x_part_info, SI446X_SUCCESS);

    /* Configuration init */
    uint8_t config_par[] = RADIO_CONFIGURATION_DATA_ARRAY;
    uint16_t config_len = sizeof(config_par)-1;

    expect_memory(__wrap_si446x_configuration_init, p_set_prop_cmd, (void*)config_par, config_len);
    expect_value(__wrap_si446x_configuration_init, p_set_prop_cmd_len, config_len);
    will_return(__wrap_si446x_configuration_init, SI446X_SUCCESS);

    /* Set property (frequency calibration) */
    uint8_t prop_param = SI446X_XO_TUNE_REG_VALUE;

    expect_value(__wrap_si446x_set_property, group, SI446X_PROP_GLOBAL_XO_TUNE >> 8);
    expect_value(__wrap_si446x_set_property, num_props, 1);
    expect_value(__wrap_si446x_set_property, start_prop, SI446X_PROP_GLOBAL_XO_TUNE & 0xFF);
    expect_memory(__wrap_si446x_set_property, data, (void*)&prop_param , 1);
    expect_value(__wrap_si446x_set_property, len, 1);

    will_return(__wrap_si446x_set_property, SI446X_SUCCESS);

    /* Set property (FIFO configuration) */
    prop_param = 0x10;

    expect_value(__wrap_si446x_set_property, group, SI446X_PROP_GLOBAL_CONFIG >> 8);
    expect_value(__wrap_si446x_set_property, num_props, 1);
    expect_value(__wrap_si446x_set_property, start_prop, SI446X_PROP_GLOBAL_CONFIG & 0xFF);
    expect_memory(__wrap_si446x_set_property, data, (void*)&prop_param , 1);
    expect_value(__wrap_si446x_set_property, len, 1);

    will_return(__wrap_si446x_set_property, SI446X_SUCCESS);

    /* FIFO info */
    expect_value(__wrap_si446x_fifo_info, rst_rx, true);
    expect_value(__wrap_si446x_fifo_info, rst_tx, true);

    will_return(__wrap_si446x_fifo_info, 0);
    will_return(__wrap_si446x_fifo_info, 129-0);

    will_return(__wrap_si446x_fifo_info, SI446X_SUCCESS);

    assert_return_code(radio_init(), 0);
}

static void radio_send_test(void **state)
{
    uint8_t data[50] = {0};
    uint8_t data_len = 50;
    uint32_t timeout_ms = 100;

    /* Set property */
    uint8_t prop_param = data_len;

    expect_value(__wrap_si446x_set_property, group, SI446X_PROP_PKT_FIELD_1_LENGTH_7_0 >> 8);
    expect_value(__wrap_si446x_set_property, num_props, 1);
    expect_value(__wrap_si446x_set_property, start_prop, SI446X_PROP_PKT_FIELD_1_LENGTH_7_0 & 0xFF);
    expect_memory(__wrap_si446x_set_property, data, (void*)&prop_param , 1);
    expect_value(__wrap_si446x_set_property, len, 1);

    will_return(__wrap_si446x_set_property, SI446X_SUCCESS);

    /* FIFO reset */
    expect_value(__wrap_si446x_fifo_info, rst_rx, true);
    expect_value(__wrap_si446x_fifo_info, rst_tx, true);

    will_return(__wrap_si446x_fifo_info, 0);
    will_return(__wrap_si446x_fifo_info, 129);

    will_return(__wrap_si446x_fifo_info, SI446X_SUCCESS);

    /* Write TX FIFO */
    expect_value(__wrap_si446x_write_tx_fifo, num_bytes, data_len);
    expect_memory(__wrap_si446x_write_tx_fifo, p_tx_data, (void*)data, data_len);

    will_return(__wrap_si446x_write_tx_fifo, SI446X_SUCCESS);

    /* Get INT status */
    expect_value(__wrap_si446x_get_int_status, ph_clr_pend, 0);
    expect_value(__wrap_si446x_get_int_status, modem_clr_pend, 0);
    expect_value(__wrap_si446x_get_int_status, chip_clr_pend, 0);

    will_return(__wrap_si446x_get_int_status, 0);
    will_return(__wrap_si446x_get_int_status, 0);
    will_return(__wrap_si446x_get_int_status, 0);
    will_return(__wrap_si446x_get_int_status, 0);
    will_return(__wrap_si446x_get_int_status, 0);
    will_return(__wrap_si446x_get_int_status, 0);
    will_return(__wrap_si446x_get_int_status, 0);
    will_return(__wrap_si446x_get_int_status, 0);

    will_return(__wrap_si446x_get_int_status, SI446X_SUCCESS);

    /* Start TX */
    expect_value(__wrap_si446x_start_tx, channel, 0);
    expect_value(__wrap_si446x_start_tx, condition, (SI446X_RX_STATE << 4) | SI446X_START_TX_DO_NOT_RETRANSMIT | SI446X_START_TX_START_TX_IMMEDIATELY);
    expect_value(__wrap_si446x_start_tx, tx_len, 0);

    will_return(__wrap_si446x_start_tx, SI446X_SUCCESS);

    timeout_ms /= 10;

    while(timeout_ms--)
    {
        /* Get INT status */
        expect_value(__wrap_si446x_get_int_status, ph_clr_pend, 0);
        expect_value(__wrap_si446x_get_int_status, modem_clr_pend, 0);
        expect_value(__wrap_si446x_get_int_status, chip_clr_pend, 0);

        will_return(__wrap_si446x_get_int_status, 0);
        will_return(__wrap_si446x_get_int_status, 0);
        will_return(__wrap_si446x_get_int_status, 0);

        if (timeout_ms > 5)
        {
            will_return(__wrap_si446x_get_int_status, 0);
        }
        else
        {
            will_return(__wrap_si446x_get_int_status, SI446X_INT_STATUS_PACKET_SENT);
        }

        will_return(__wrap_si446x_get_int_status, 0);
        will_return(__wrap_si446x_get_int_status, 0);
        will_return(__wrap_si446x_get_int_status, 0);
        will_return(__wrap_si446x_get_int_status, 0);

        will_return(__wrap_si446x_get_int_status, SI446X_SUCCESS);

        if (timeout_ms > 5)
        {
            expect_function_call(__wrap_si446x_delay_ms);
        }
        else
        {
            break;
        }
    }

    assert_return_code(radio_send(data, data_len, 100), 0);
}

static void radio_recv_test(void **state)
{
    /* Change state */
    expect_value(__wrap_si446x_change_state, next_state, SI446X_RX_STATE);

    will_return(__wrap_si446x_change_state, SI446X_SUCCESS);

    /* Set property */
    uint8_t prop_param = 0x03;

    expect_value(__wrap_si446x_set_property, group, SI446X_PROP_INT_CTL_ENABLE >> 8);
    expect_value(__wrap_si446x_set_property, num_props, 1);
    expect_value(__wrap_si446x_set_property, start_prop, SI446X_PROP_INT_CTL_ENABLE & 0xFF);
    expect_memory(__wrap_si446x_set_property, data, (void*)&prop_param , 1);
    expect_value(__wrap_si446x_set_property, len, 1);

    will_return(__wrap_si446x_set_property, SI446X_SUCCESS);

    /* Set property */
    prop_param = 0x18;

    expect_value(__wrap_si446x_set_property, group, SI446X_PROP_INT_CTL_PH_ENABLE >> 8);
    expect_value(__wrap_si446x_set_property, num_props, 1);
    expect_value(__wrap_si446x_set_property, start_prop, SI446X_PROP_INT_CTL_PH_ENABLE & 0xFF);
    expect_memory(__wrap_si446x_set_property, data, (void*)&prop_param , 1);
    expect_value(__wrap_si446x_set_property, len, 1);

    will_return(__wrap_si446x_set_property, SI446X_SUCCESS);

    /* Set property */
    prop_param = 0x00;

    expect_value(__wrap_si446x_set_property, group, SI446X_PROP_INT_CTL_MODEM_ENABLE >> 8);
    expect_value(__wrap_si446x_set_property, num_props, 1);
    expect_value(__wrap_si446x_set_property, start_prop, SI446X_PROP_INT_CTL_MODEM_ENABLE & 0xFF);
    expect_memory(__wrap_si446x_set_property, data, (void*)&prop_param , 1);
    expect_value(__wrap_si446x_set_property, len, 1);

    will_return(__wrap_si446x_set_property, SI446X_SUCCESS);

    /* Start RX */
    expect_value(__wrap_si446x_start_rx, channel, 0);
    expect_value(__wrap_si446x_start_rx, condition, SI446X_START_RX_START_RX_IMMEDIATELY);
    expect_value(__wrap_si446x_start_rx, rx_len, 0);
    expect_value(__wrap_si446x_start_rx, next_state1, SI446X_RX_STATE);
    expect_value(__wrap_si446x_start_rx, next_state2, SI446X_RX_STATE);
    expect_value(__wrap_si446x_start_rx, next_state3, SI446X_RX_STATE);

    will_return(__wrap_si446x_start_rx, SI446X_SUCCESS);

    uint8_t data[50] = {0};

    assert_return_code(radio_recv(data, 10, 1000), 0);
}

static void radio_available_test(void **state)
{
    /* INT status */
    expect_value(__wrap_si446x_get_int_status, ph_clr_pend, 0);
    expect_value(__wrap_si446x_get_int_status, modem_clr_pend, 0);
    expect_value(__wrap_si446x_get_int_status, chip_clr_pend, 0);

    will_return(__wrap_si446x_get_int_status, 0);
    will_return(__wrap_si446x_get_int_status, 0);
    will_return(__wrap_si446x_get_int_status, 0);
    will_return(__wrap_si446x_get_int_status, 0x10);
    will_return(__wrap_si446x_get_int_status, 0);
    will_return(__wrap_si446x_get_int_status, 0);
    will_return(__wrap_si446x_get_int_status, 0);
    will_return(__wrap_si446x_get_int_status, 0);

    will_return(__wrap_si446x_get_int_status, SI446X_SUCCESS);

    /* FIFO info */
    expect_value(__wrap_si446x_fifo_info, rst_rx, false);
    expect_value(__wrap_si446x_fifo_info, rst_tx, false);

    will_return(__wrap_si446x_fifo_info, 50);
    will_return(__wrap_si446x_fifo_info, 129-50);

    will_return(__wrap_si446x_fifo_info, SI446X_SUCCESS);

    assert_return_code(radio_available(), 0);
}

static void radio_sleep_test(void **state)
{
    expect_value(__wrap_si446x_change_state, next_state, SI446X_SLEEP_STATE);

    will_return(__wrap_si446x_change_state, SI446X_SUCCESS);

    assert_return_code(radio_sleep(), 0);
}

int main(void)
{
    const struct CMUnitTest radio_tests[] = {
        cmocka_unit_test(radio_init_test),
        cmocka_unit_test(radio_send_test),
        cmocka_unit_test(radio_recv_test),
        cmocka_unit_test(radio_available_test),
        cmocka_unit_test(radio_sleep_test),
    };

    return cmocka_run_group_tests(radio_tests, NULL, NULL);
}

/** \} End of radio_unit_test group */
