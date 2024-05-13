/*
 * ttc_test.c
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
 * \brief Unit test of the TTC device.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.10.14
 * 
 * \date 2021/08/06
 * 
 * \defgroup ttc_unit_test TTC
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

#include <devices/ttc/ttc.h>
#include <drivers/sl_ttc2/sl_ttc2.h>

#define TTC_0_SPI_PORT              SPI_PORT_0
#define TTC_0_SPI_CS_PIN            SPI_CS_0
#define TTC_0_SPI_MODE              SPI_MODE_0
#define TTC_0_SPI_CLOCK_HZ          1000000UL
#define TTC_0_ID                    SL_TTC2_RADIO_0

#define TTC_1_SPI_PORT              SPI_PORT_0
#define TTC_1_SPI_CS_PIN            SPI_CS_1
#define TTC_1_SPI_MODE              SPI_MODE_0
#define TTC_1_SPI_CLOCK_HZ          1000000UL
#define TTC_1_ID                    SL_TTC2_RADIO_1

unsigned int generate_random(unsigned int l, unsigned int r);

static void ttc_init_test(void **state)
{
    ttc_e ttc_dev = 0;
    for(ttc_dev=0; ttc_dev<UINT8_MAX; ttc_dev++)
    {
        if (ttc_dev == TTC_0)
        {
            expect_value(__wrap_sl_ttc2_init, config.port, TTC_0_SPI_PORT);
            expect_value(__wrap_sl_ttc2_init, config.cs_pin, TTC_0_SPI_CS_PIN);
            expect_value(__wrap_sl_ttc2_init, config.port_config.speed_hz, TTC_0_SPI_CLOCK_HZ);
            expect_value(__wrap_sl_ttc2_init, config.port_config.mode, TTC_0_SPI_MODE);
            expect_value(__wrap_sl_ttc2_init, config.id, TTC_0_ID);

            will_return(__wrap_sl_ttc2_init, 0);

            expect_value(__wrap_sl_ttc2_read_hardware_version, config.port, TTC_0_SPI_PORT);
            expect_value(__wrap_sl_ttc2_read_hardware_version, config.cs_pin, TTC_0_SPI_CS_PIN);
            expect_value(__wrap_sl_ttc2_read_hardware_version, config.port_config.speed_hz, TTC_0_SPI_CLOCK_HZ);
            expect_value(__wrap_sl_ttc2_read_hardware_version, config.port_config.mode, TTC_0_SPI_MODE);
            expect_value(__wrap_sl_ttc2_read_hardware_version, config.id, TTC_0_ID);

            will_return(__wrap_sl_ttc2_read_hardware_version, generate_random(0, UINT8_MAX));

            will_return(__wrap_sl_ttc2_read_hardware_version, 0);

            expect_value(__wrap_sl_ttc2_read_firmware_version, config.port, TTC_0_SPI_PORT);
            expect_value(__wrap_sl_ttc2_read_firmware_version, config.cs_pin, TTC_0_SPI_CS_PIN);
            expect_value(__wrap_sl_ttc2_read_firmware_version, config.port_config.speed_hz, TTC_0_SPI_CLOCK_HZ);
            expect_value(__wrap_sl_ttc2_read_firmware_version, config.port_config.mode, TTC_0_SPI_MODE);
            expect_value(__wrap_sl_ttc2_read_firmware_version, config.id, TTC_0_ID);

            will_return(__wrap_sl_ttc2_read_firmware_version, generate_random(0, UINT32_MAX-1));

            will_return(__wrap_sl_ttc2_read_firmware_version, 0);
        }
        else if (ttc_dev == TTC_1)
        {
            expect_value(__wrap_sl_ttc2_init, config.port, TTC_1_SPI_PORT);
            expect_value(__wrap_sl_ttc2_init, config.cs_pin, TTC_1_SPI_CS_PIN);
            expect_value(__wrap_sl_ttc2_init, config.port_config.speed_hz, TTC_1_SPI_CLOCK_HZ);
            expect_value(__wrap_sl_ttc2_init, config.port_config.mode, TTC_1_SPI_MODE);
            expect_value(__wrap_sl_ttc2_init, config.id, TTC_1_ID);

            will_return(__wrap_sl_ttc2_init, 0);

            expect_value(__wrap_sl_ttc2_read_hardware_version, config.port, TTC_1_SPI_PORT);
            expect_value(__wrap_sl_ttc2_read_hardware_version, config.cs_pin, TTC_1_SPI_CS_PIN);
            expect_value(__wrap_sl_ttc2_read_hardware_version, config.port_config.speed_hz, TTC_1_SPI_CLOCK_HZ);
            expect_value(__wrap_sl_ttc2_read_hardware_version, config.port_config.mode, TTC_1_SPI_MODE);
            expect_value(__wrap_sl_ttc2_read_hardware_version, config.id, TTC_1_ID);

            will_return(__wrap_sl_ttc2_read_hardware_version, generate_random(0, UINT8_MAX));

            will_return(__wrap_sl_ttc2_read_hardware_version, 0);

            expect_value(__wrap_sl_ttc2_read_firmware_version, config.port, TTC_1_SPI_PORT);
            expect_value(__wrap_sl_ttc2_read_firmware_version, config.cs_pin, TTC_1_SPI_CS_PIN);
            expect_value(__wrap_sl_ttc2_read_firmware_version, config.port_config.speed_hz, TTC_1_SPI_CLOCK_HZ);
            expect_value(__wrap_sl_ttc2_read_firmware_version, config.port_config.mode, TTC_1_SPI_MODE);
            expect_value(__wrap_sl_ttc2_read_firmware_version, config.id, TTC_1_ID);

            will_return(__wrap_sl_ttc2_read_firmware_version, generate_random(0, UINT32_MAX-1));

            will_return(__wrap_sl_ttc2_read_firmware_version, 0);
        }
        else
        {
            assert_int_equal(ttc_init(ttc_dev), -1);

            continue;
        }

        assert_return_code(ttc_init(ttc_dev), 0);
    }
}

static void ttc_set_param_test(void **state)
{
    ttc_e ttc_dev = 0;

    for(ttc_dev = 0; ttc_dev < UINT8_MAX; ttc_dev++)
    {
        uint8_t i = 0;

        uint32_t par_val = generate_random(0, UINT32_MAX-1);

        if (ttc_dev == TTC_0)
        {
            for(i = 0; i < UINT8_MAX; i++)
            {
                expect_value(__wrap_sl_ttc2_write_reg, config.port, TTC_0_SPI_PORT);
                expect_value(__wrap_sl_ttc2_write_reg, config.cs_pin, TTC_0_SPI_CS_PIN);
                expect_value(__wrap_sl_ttc2_write_reg, config.port_config.speed_hz, TTC_0_SPI_CLOCK_HZ);
                expect_value(__wrap_sl_ttc2_write_reg, config.port_config.mode, TTC_0_SPI_MODE);
                expect_value(__wrap_sl_ttc2_write_reg, config.id, TTC_0_ID);

                expect_value(__wrap_sl_ttc2_write_reg, adr, i);

                expect_value(__wrap_sl_ttc2_write_reg, val, par_val);

                will_return(__wrap_sl_ttc2_write_reg, 0);

                assert_return_code(ttc_set_param(ttc_dev, i, par_val), 0);
            }
        }
        else if (ttc_dev == TTC_1)
        {
            for(i = 0; i < UINT8_MAX; i++)
            {
                expect_value(__wrap_sl_ttc2_write_reg, config.port, TTC_1_SPI_PORT);
                expect_value(__wrap_sl_ttc2_write_reg, config.cs_pin, TTC_1_SPI_CS_PIN);
                expect_value(__wrap_sl_ttc2_write_reg, config.port_config.speed_hz, TTC_1_SPI_CLOCK_HZ);
                expect_value(__wrap_sl_ttc2_write_reg, config.port_config.mode, TTC_1_SPI_MODE);
                expect_value(__wrap_sl_ttc2_write_reg, config.id, TTC_1_ID);

                expect_value(__wrap_sl_ttc2_write_reg, adr, i);

                expect_value(__wrap_sl_ttc2_write_reg, val, par_val);

                will_return(__wrap_sl_ttc2_write_reg, 0);

                assert_return_code(ttc_set_param(ttc_dev, i, par_val), 0);
            }
        }
        else
        {
            assert_int_equal(ttc_set_param(ttc_dev, i, par_val), -1);

            continue;
        }
    }
}

static void ttc_get_param_test(void **state)
{
    ttc_e ttc_dev = 0;

    for(ttc_dev = 0; ttc_dev < UINT8_MAX; ttc_dev++)
    {
        uint8_t i = 0;

        uint32_t par_val = generate_random(0, UINT32_MAX-1);

        uint32_t par_buf = UINT32_MAX;

        if (ttc_dev == TTC_0)
        {
            for(i = 0; i < UINT8_MAX; i++)
            {
                expect_value(__wrap_sl_ttc2_read_reg, config.port, TTC_0_SPI_PORT);
                expect_value(__wrap_sl_ttc2_read_reg, config.cs_pin, TTC_0_SPI_CS_PIN);
                expect_value(__wrap_sl_ttc2_read_reg, config.port_config.speed_hz, TTC_0_SPI_CLOCK_HZ);
                expect_value(__wrap_sl_ttc2_read_reg, config.port_config.mode, TTC_0_SPI_MODE);
                expect_value(__wrap_sl_ttc2_read_reg, config.id, TTC_0_ID);

                expect_value(__wrap_sl_ttc2_read_reg, adr, i);

                will_return(__wrap_sl_ttc2_read_reg, par_val);

                will_return(__wrap_sl_ttc2_read_reg, 0);

                assert_return_code(ttc_get_param(ttc_dev, i, &par_buf), 0);

                assert_int_equal(par_val, par_buf);
            }
        }
        else if (ttc_dev == TTC_1)
        {
            for(i = 0; i < UINT8_MAX; i++)
            {
                expect_value(__wrap_sl_ttc2_read_reg, config.port, TTC_1_SPI_PORT);
                expect_value(__wrap_sl_ttc2_read_reg, config.cs_pin, TTC_1_SPI_CS_PIN);
                expect_value(__wrap_sl_ttc2_read_reg, config.port_config.speed_hz, TTC_1_SPI_CLOCK_HZ);
                expect_value(__wrap_sl_ttc2_read_reg, config.port_config.mode, TTC_1_SPI_MODE);
                expect_value(__wrap_sl_ttc2_read_reg, config.id, TTC_1_ID);

                expect_value(__wrap_sl_ttc2_read_reg, adr, i);

                will_return(__wrap_sl_ttc2_read_reg, par_val);

                will_return(__wrap_sl_ttc2_read_reg, 0);

                assert_return_code(ttc_get_param(ttc_dev, i, &par_buf), 0);

                assert_int_equal(par_val, par_buf);
            }
        }
        else
        {
            assert_int_equal(ttc_get_param(ttc_dev, i, &par_buf), -1);

            continue;
        }
    }
}

static void ttc_get_data_test(void **state)
{
    ttc_data_t test_data = {0};

    test_data.time_counter          = generate_random(0, UINT32_MAX-1);
    test_data.reset_counter         = generate_random(0, UINT16_MAX);
    test_data.last_reset_cause      = generate_random(0, UINT8_MAX);
    test_data.voltage_mcu           = generate_random(0, UINT16_MAX);
    test_data.current_mcu           = generate_random(0, UINT16_MAX);
    test_data.temperature_mcu       = generate_random(0, UINT16_MAX);
    test_data.voltage_radio         = generate_random(0, UINT16_MAX);
    test_data.current_radio         = generate_random(0, UINT16_MAX);
    test_data.temperature_radio     = generate_random(0, UINT16_MAX);
    test_data.last_valid_tc         = generate_random(0, UINT8_MAX);
    test_data.rssi_last_valid_tc    = generate_random(0, UINT16_MAX);
    test_data.temperature_antenna   = generate_random(0, UINT16_MAX);
    test_data.antenna_status        = generate_random(0, UINT16_MAX);
    test_data.deployment_status     = generate_random(0, UINT8_MAX);
    test_data.hibernation_status    = generate_random(0, UINT8_MAX);
    test_data.tx_packet_counter     = generate_random(0, UINT32_MAX-1);
    test_data.rx_packet_counter     = generate_random(0, UINT32_MAX-1);

    ttc_e ttc_dev = 0;
    for(ttc_dev=0; ttc_dev<UINT8_MAX; ttc_dev++)
    {
        ttc_data_t read_data = {0};

        if (ttc_dev == TTC_0)
        {
            expect_value(__wrap_sl_ttc2_check_device, config.port, TTC_0_SPI_PORT);
            expect_value(__wrap_sl_ttc2_check_device, config.cs_pin, TTC_0_SPI_CS_PIN);
            expect_value(__wrap_sl_ttc2_check_device, config.port_config.speed_hz, TTC_0_SPI_CLOCK_HZ);
            expect_value(__wrap_sl_ttc2_check_device, config.port_config.mode, TTC_0_SPI_MODE);
            expect_value(__wrap_sl_ttc2_check_device, config.id, TTC_0_ID);

            will_return(__wrap_sl_ttc2_check_device, 0);

            expect_value(__wrap_sl_ttc2_read_hk_data, config.port, TTC_0_SPI_PORT);
            expect_value(__wrap_sl_ttc2_read_hk_data, config.cs_pin, TTC_0_SPI_CS_PIN);
            expect_value(__wrap_sl_ttc2_read_hk_data, config.port_config.speed_hz, TTC_0_SPI_CLOCK_HZ);
            expect_value(__wrap_sl_ttc2_read_hk_data, config.port_config.mode, TTC_0_SPI_MODE);
            expect_value(__wrap_sl_ttc2_read_hk_data, config.id, TTC_0_ID);
        }
        else if (ttc_dev == TTC_1)
        {
            expect_value(__wrap_sl_ttc2_check_device, config.port, TTC_1_SPI_PORT);
            expect_value(__wrap_sl_ttc2_check_device, config.cs_pin, TTC_1_SPI_CS_PIN);
            expect_value(__wrap_sl_ttc2_check_device, config.port_config.speed_hz, TTC_1_SPI_CLOCK_HZ);
            expect_value(__wrap_sl_ttc2_check_device, config.port_config.mode, TTC_1_SPI_MODE);
            expect_value(__wrap_sl_ttc2_check_device, config.id, TTC_1_ID);

            will_return(__wrap_sl_ttc2_check_device, 0);

            expect_value(__wrap_sl_ttc2_read_hk_data, config.port, TTC_1_SPI_PORT);
            expect_value(__wrap_sl_ttc2_read_hk_data, config.cs_pin, TTC_1_SPI_CS_PIN);
            expect_value(__wrap_sl_ttc2_read_hk_data, config.port_config.speed_hz, TTC_1_SPI_CLOCK_HZ);
            expect_value(__wrap_sl_ttc2_read_hk_data, config.port_config.mode, TTC_1_SPI_MODE);
            expect_value(__wrap_sl_ttc2_read_hk_data, config.id, TTC_1_ID);
        }
        else
        {
            assert_int_equal(ttc_get_data(ttc_dev, &read_data), -1);

            continue;
        }

        will_return(__wrap_sl_ttc2_read_hk_data, test_data.time_counter);
        will_return(__wrap_sl_ttc2_read_hk_data, test_data.reset_counter);
        will_return(__wrap_sl_ttc2_read_hk_data, test_data.last_reset_cause);
        will_return(__wrap_sl_ttc2_read_hk_data, test_data.voltage_mcu);
        will_return(__wrap_sl_ttc2_read_hk_data, test_data.current_mcu);
        will_return(__wrap_sl_ttc2_read_hk_data, test_data.temperature_mcu);
        will_return(__wrap_sl_ttc2_read_hk_data, test_data.voltage_radio);
        will_return(__wrap_sl_ttc2_read_hk_data, test_data.current_radio);
        will_return(__wrap_sl_ttc2_read_hk_data, test_data.temperature_radio);
        will_return(__wrap_sl_ttc2_read_hk_data, test_data.last_valid_tc);
        will_return(__wrap_sl_ttc2_read_hk_data, test_data.rssi_last_valid_tc);
        will_return(__wrap_sl_ttc2_read_hk_data, test_data.temperature_antenna);
        will_return(__wrap_sl_ttc2_read_hk_data, test_data.antenna_status);
        will_return(__wrap_sl_ttc2_read_hk_data, test_data.deployment_status);
        will_return(__wrap_sl_ttc2_read_hk_data, test_data.hibernation_status);
        will_return(__wrap_sl_ttc2_read_hk_data, test_data.tx_packet_counter);
        will_return(__wrap_sl_ttc2_read_hk_data, test_data.rx_packet_counter);

        will_return(__wrap_sl_ttc2_read_hk_data, 0);

        assert_return_code(ttc_get_data(ttc_dev, &read_data), 0);

        assert_memory_equal((void*)&test_data, (void*)&read_data, sizeof(ttc_data_t));
    }
}

static void ttc_send_test(void **state)
{
    ttc_e ttc_dev = 0;
    for(ttc_dev=0; ttc_dev<UINT8_MAX; ttc_dev++)
    {
        uint8_t test_data[256] = {UINT8_MAX};
        uint16_t test_len = generate_random(0, UINT8_MAX);

        if (ttc_dev == TTC_0)
        {
            expect_value(__wrap_sl_ttc2_check_device, config.port, TTC_0_SPI_PORT);
            expect_value(__wrap_sl_ttc2_check_device, config.cs_pin, TTC_0_SPI_CS_PIN);
            expect_value(__wrap_sl_ttc2_check_device, config.port_config.speed_hz, TTC_0_SPI_CLOCK_HZ);
            expect_value(__wrap_sl_ttc2_check_device, config.port_config.mode, TTC_0_SPI_MODE);
            expect_value(__wrap_sl_ttc2_check_device, config.id, TTC_0_ID);

            will_return(__wrap_sl_ttc2_check_device, 0);

            expect_value(__wrap_sl_ttc2_transmit_packet, config.port, TTC_0_SPI_PORT);
            expect_value(__wrap_sl_ttc2_transmit_packet, config.cs_pin, TTC_0_SPI_CS_PIN);
            expect_value(__wrap_sl_ttc2_transmit_packet, config.port_config.speed_hz, TTC_0_SPI_CLOCK_HZ);
            expect_value(__wrap_sl_ttc2_transmit_packet, config.port_config.mode, TTC_0_SPI_MODE);
            expect_value(__wrap_sl_ttc2_transmit_packet, config.id, TTC_0_ID);

        }
        else if (ttc_dev == TTC_1)
        {
            expect_value(__wrap_sl_ttc2_check_device, config.port, TTC_1_SPI_PORT);
            expect_value(__wrap_sl_ttc2_check_device, config.cs_pin, TTC_1_SPI_CS_PIN);
            expect_value(__wrap_sl_ttc2_check_device, config.port_config.speed_hz, TTC_1_SPI_CLOCK_HZ);
            expect_value(__wrap_sl_ttc2_check_device, config.port_config.mode, TTC_1_SPI_MODE);
            expect_value(__wrap_sl_ttc2_check_device, config.id, TTC_1_ID);

            will_return(__wrap_sl_ttc2_check_device, 0);

            expect_value(__wrap_sl_ttc2_transmit_packet, config.port, TTC_1_SPI_PORT);
            expect_value(__wrap_sl_ttc2_transmit_packet, config.cs_pin, TTC_1_SPI_CS_PIN);
            expect_value(__wrap_sl_ttc2_transmit_packet, config.port_config.speed_hz, TTC_1_SPI_CLOCK_HZ);
            expect_value(__wrap_sl_ttc2_transmit_packet, config.port_config.mode, TTC_1_SPI_MODE);
            expect_value(__wrap_sl_ttc2_transmit_packet, config.id, TTC_1_ID);

        }
        else
        {
            assert_int_equal(ttc_send(ttc_dev, test_data, test_len), -1);

            continue;
        }

        expect_memory(__wrap_sl_ttc2_transmit_packet, data, (void*)test_data, test_len);
        expect_value(__wrap_sl_ttc2_transmit_packet, len, test_len);

        will_return(__wrap_sl_ttc2_transmit_packet, 0);

        assert_return_code(ttc_send(ttc_dev, test_data, test_len), 0);
    }
}

static void ttc_recv_test(void **state)
{
    ttc_e ttc_dev = 0;
    for(ttc_dev=0; ttc_dev<UINT8_MAX; ttc_dev++)
    {
        uint8_t test_data[256] = {UINT8_MAX};
        uint16_t test_len = generate_random(0, UINT8_MAX);

        uint16_t i = 0;
        for(i=0; i<test_len; i++)
        {
            test_data[i] = generate_random(0, UINT8_MAX);
        }

        if (ttc_dev == TTC_0)
        {
            expect_value(__wrap_sl_ttc2_check_pkt_avail, config.port, TTC_0_SPI_PORT);
            expect_value(__wrap_sl_ttc2_check_pkt_avail, config.cs_pin, TTC_0_SPI_CS_PIN);
            expect_value(__wrap_sl_ttc2_check_pkt_avail, config.port_config.speed_hz, TTC_0_SPI_CLOCK_HZ);
            expect_value(__wrap_sl_ttc2_check_pkt_avail, config.port_config.mode, TTC_0_SPI_MODE);
            expect_value(__wrap_sl_ttc2_check_pkt_avail, config.id, TTC_0_ID);

            will_return(__wrap_sl_ttc2_check_pkt_avail, test_len);

            expect_value(__wrap_sl_ttc2_read_packet, config.port, TTC_0_SPI_PORT);
            expect_value(__wrap_sl_ttc2_read_packet, config.cs_pin, TTC_0_SPI_CS_PIN);
            expect_value(__wrap_sl_ttc2_read_packet, config.port_config.speed_hz, TTC_0_SPI_CLOCK_HZ);
            expect_value(__wrap_sl_ttc2_read_packet, config.port_config.mode, TTC_0_SPI_MODE);
            expect_value(__wrap_sl_ttc2_read_packet, config.id, TTC_0_ID);
        }
        else if (ttc_dev == TTC_1)
        {
            expect_value(__wrap_sl_ttc2_check_pkt_avail, config.port, TTC_1_SPI_PORT);
            expect_value(__wrap_sl_ttc2_check_pkt_avail, config.cs_pin, TTC_1_SPI_CS_PIN);
            expect_value(__wrap_sl_ttc2_check_pkt_avail, config.port_config.speed_hz, TTC_1_SPI_CLOCK_HZ);
            expect_value(__wrap_sl_ttc2_check_pkt_avail, config.port_config.mode, TTC_1_SPI_MODE);
            expect_value(__wrap_sl_ttc2_check_pkt_avail, config.id, TTC_1_ID);

            will_return(__wrap_sl_ttc2_check_pkt_avail, test_len);

            expect_value(__wrap_sl_ttc2_read_packet, config.port, TTC_1_SPI_PORT);
            expect_value(__wrap_sl_ttc2_read_packet, config.cs_pin, TTC_1_SPI_CS_PIN);
            expect_value(__wrap_sl_ttc2_read_packet, config.port_config.speed_hz, TTC_1_SPI_CLOCK_HZ);
            expect_value(__wrap_sl_ttc2_read_packet, config.port_config.mode, TTC_1_SPI_MODE);
            expect_value(__wrap_sl_ttc2_read_packet, config.id, TTC_1_ID);
        }
        else
        {
            assert_int_equal(ttc_recv(ttc_dev, test_data, &test_len), -1);

            continue;
        }

        will_return(__wrap_sl_ttc2_read_packet, test_len);

        for(i=0; i<test_len; i++)
        {
            will_return(__wrap_sl_ttc2_read_packet, test_data[i]);
        }

        will_return(__wrap_sl_ttc2_read_packet, 0);

        uint8_t read_data[256] = {UINT8_MAX};
        uint16_t read_len = UINT16_MAX;

        assert_return_code(ttc_recv(ttc_dev, read_data, &read_len), 0);

        assert_memory_equal((void*)&test_data, (void*)&read_data, test_len);
    }
}

static void ttc_avail_test(void **state)
{
    ttc_e ttc_dev = 0;
    for(ttc_dev=0; ttc_dev<UINT8_MAX; ttc_dev++)
    {
        if (ttc_dev == TTC_0)
        {
            expect_value(__wrap_sl_ttc2_check_pkt_avail, config.port, TTC_0_SPI_PORT);
            expect_value(__wrap_sl_ttc2_check_pkt_avail, config.cs_pin, TTC_0_SPI_CS_PIN);
            expect_value(__wrap_sl_ttc2_check_pkt_avail, config.port_config.speed_hz, TTC_0_SPI_CLOCK_HZ);
            expect_value(__wrap_sl_ttc2_check_pkt_avail, config.port_config.mode, TTC_0_SPI_MODE);
            expect_value(__wrap_sl_ttc2_check_pkt_avail, config.id, TTC_0_ID);
        }
        else if (ttc_dev == TTC_1)
        {
            expect_value(__wrap_sl_ttc2_check_pkt_avail, config.port, TTC_1_SPI_PORT);
            expect_value(__wrap_sl_ttc2_check_pkt_avail, config.cs_pin, TTC_1_SPI_CS_PIN);
            expect_value(__wrap_sl_ttc2_check_pkt_avail, config.port_config.speed_hz, TTC_1_SPI_CLOCK_HZ);
            expect_value(__wrap_sl_ttc2_check_pkt_avail, config.port_config.mode, TTC_1_SPI_MODE);
            expect_value(__wrap_sl_ttc2_check_pkt_avail, config.id, TTC_1_ID);
        }
        else
        {
            assert_int_equal(ttc_avail(ttc_dev), -1);

            continue;
        }

        int pkt_avail = generate_random(0, UINT16_MAX);

        will_return(__wrap_sl_ttc2_check_pkt_avail, pkt_avail);

        assert_int_equal(ttc_avail(ttc_dev), pkt_avail);
    }
}

static void ttc_enter_hibernation_test(void **state)
{
    ttc_e ttc_dev = 0;
    for(ttc_dev=0; ttc_dev<UINT8_MAX; ttc_dev++)
    {
        if (ttc_dev == TTC_0)
        {
            expect_value(__wrap_sl_ttc2_set_tx_enable, config.port, TTC_0_SPI_PORT);
            expect_value(__wrap_sl_ttc2_set_tx_enable, config.cs_pin, TTC_0_SPI_CS_PIN);
            expect_value(__wrap_sl_ttc2_set_tx_enable, config.port_config.speed_hz, TTC_0_SPI_CLOCK_HZ);
            expect_value(__wrap_sl_ttc2_set_tx_enable, config.port_config.mode, TTC_0_SPI_MODE);
            expect_value(__wrap_sl_ttc2_set_tx_enable, config.id, TTC_0_ID);
        }
        else if (ttc_dev == TTC_1)
        {
            expect_value(__wrap_sl_ttc2_set_tx_enable, config.port, TTC_1_SPI_PORT);
            expect_value(__wrap_sl_ttc2_set_tx_enable, config.cs_pin, TTC_1_SPI_CS_PIN);
            expect_value(__wrap_sl_ttc2_set_tx_enable, config.port_config.speed_hz, TTC_1_SPI_CLOCK_HZ);
            expect_value(__wrap_sl_ttc2_set_tx_enable, config.port_config.mode, TTC_1_SPI_MODE);
            expect_value(__wrap_sl_ttc2_set_tx_enable, config.id, TTC_1_ID);
        }
        else
        {
            assert_int_equal(ttc_leave_hibernation(ttc_dev), -1);

            continue;
        }

        expect_value(__wrap_sl_ttc2_set_tx_enable, en, 0);

        will_return(__wrap_sl_ttc2_set_tx_enable, 0);

        assert_return_code(ttc_enter_hibernation(ttc_dev), 0);
    }
}

static void ttc_leave_hibernation_test(void **state)
{
    ttc_e ttc_dev = 0;
    for(ttc_dev=0; ttc_dev<UINT8_MAX; ttc_dev++)
    {
        if (ttc_dev == TTC_0)
        {
            expect_value(__wrap_sl_ttc2_set_tx_enable, config.port, TTC_0_SPI_PORT);
            expect_value(__wrap_sl_ttc2_set_tx_enable, config.cs_pin, TTC_0_SPI_CS_PIN);
            expect_value(__wrap_sl_ttc2_set_tx_enable, config.port_config.speed_hz, TTC_0_SPI_CLOCK_HZ);
            expect_value(__wrap_sl_ttc2_set_tx_enable, config.port_config.mode, TTC_0_SPI_MODE);
            expect_value(__wrap_sl_ttc2_set_tx_enable, config.id, TTC_0_ID);
        }
        else if (ttc_dev == TTC_1)
        {
            expect_value(__wrap_sl_ttc2_set_tx_enable, config.port, TTC_1_SPI_PORT);
            expect_value(__wrap_sl_ttc2_set_tx_enable, config.cs_pin, TTC_1_SPI_CS_PIN);
            expect_value(__wrap_sl_ttc2_set_tx_enable, config.port_config.speed_hz, TTC_1_SPI_CLOCK_HZ);
            expect_value(__wrap_sl_ttc2_set_tx_enable, config.port_config.mode, TTC_1_SPI_MODE);
            expect_value(__wrap_sl_ttc2_set_tx_enable, config.id, TTC_1_ID);
        }
        else
        {
            assert_int_equal(ttc_leave_hibernation(ttc_dev), -1);

            continue;
        }

        expect_value(__wrap_sl_ttc2_set_tx_enable, en, 1);

        will_return(__wrap_sl_ttc2_set_tx_enable, 0);

        assert_return_code(ttc_leave_hibernation(ttc_dev), 0);
    }
}

int main(void)
{
    const struct CMUnitTest ttc_tests[] = {
        cmocka_unit_test(ttc_init_test),
        cmocka_unit_test(ttc_set_param_test),
        cmocka_unit_test(ttc_get_param_test),
        cmocka_unit_test(ttc_get_data_test),
        cmocka_unit_test(ttc_send_test),
        cmocka_unit_test(ttc_recv_test),
        cmocka_unit_test(ttc_avail_test),
        cmocka_unit_test(ttc_enter_hibernation_test),
        cmocka_unit_test(ttc_leave_hibernation_test),
    };

    return cmocka_run_group_tests(ttc_tests, NULL, NULL);
}

unsigned int generate_random(unsigned int l, unsigned int r)
{
    return (rand() % (r - l + 1)) + l;
}

/** \} End of ttc_test group */
