/*
 * sl_ttc2_test.c
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
 * \brief Unit test of the SpaceLab TTC 2.0 driver.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.15
 * 
 * \date 2021/09/08
 * 
 * \defgroup sl_ttc2_unit_test SpaceLab TTC 2.0
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

#include <drivers/spi/spi.h>
#include <drivers/sl_ttc2/sl_ttc2.h>

#define SL_TTC2_SPI_PORT            SPI_PORT_0
#define SL_TTC2_SPI_CS              SPI_CS_0
#define SL_TTC2_SPI_CLOCK_HZ        1000000UL
#define SL_TTC2_SPI_MODE            SPI_MODE_0

unsigned int generate_random(unsigned int l, unsigned int r);

uint16_t crc16_ccitt(uint8_t *data, uint16_t len);

static void sl_ttc2_init_test(void **state)
{
    /* SPI init */
    expect_value(__wrap_spi_init, port, SL_TTC2_SPI_PORT);
    expect_value(__wrap_spi_init, config.speed_hz, SL_TTC2_SPI_CLOCK_HZ);
    expect_value(__wrap_spi_init, config.mode, SL_TTC2_SPI_MODE);

    will_return(__wrap_spi_init, 0);

    /* SPI transfer */
    uint8_t cmd[8] = {0};
    uint8_t ans[8] = {0};

    cmd[0] = 1;     /* Read reg. command */
    cmd[1] = 0;     /* Read device ID */

    ans[0] = 1;     /* Command */
    ans[1] = 0;     /* Address */
    ans[2] = 0;     /* Dummy byte */
    ans[3] = 0;     /* Dummy byte */
    ans[4] = 0xCC;  /* Radio 0 ID */
    ans[5] = 0x2A;  /* Radio 0 ID */
    ans[6] = 0x93;  /* CRC16 */
    ans[7] = 0xB1;  /* CRC16 */

    expect_value(__wrap_spi_transfer, port, SL_TTC2_SPI_PORT);
    expect_value(__wrap_spi_transfer, cs, SL_TTC2_SPI_CS);
    expect_memory(__wrap_spi_transfer, wd, (void*)cmd, 8);
    expect_value(__wrap_spi_transfer, len, 8);

    uint16_t i = 0;
    for(i=0; i<8; i++)
    {
        will_return(__wrap_spi_transfer, ans[i]);
    }

    will_return(__wrap_spi_transfer, 0);

    sl_ttc2_config_t conf = {0};

    conf.port                   = SL_TTC2_SPI_PORT;
    conf.cs_pin                 = SL_TTC2_SPI_CS;
    conf.port_config.speed_hz   = SL_TTC2_SPI_CLOCK_HZ;
    conf.port_config.mode       = SL_TTC2_SPI_MODE;
    conf.id                     = 0;

    assert_return_code(sl_ttc2_init(conf), 0);
}

static void sl_ttc2_check_device_test(void **state)
{
    uint8_t cmd[8] = {0};
    uint8_t ans[8] = {0};

    cmd[0] = 1;     /* Read reg. command */
    cmd[1] = 0;     /* Read device ID */

    ans[0] = 1;     /* Command */
    ans[1] = 0;     /* Address */
    ans[2] = 0;     /* Dummy byte */
    ans[3] = 0;     /* Dummy byte */
    ans[4] = 0xCC;  /* Radio 0 ID */
    ans[5] = 0x2A;  /* Radio 0 ID */
    ans[6] = 0x93;  /* CRC16 */
    ans[7] = 0xB1;  /* CRC16 */

    expect_value(__wrap_spi_transfer, port, SL_TTC2_SPI_PORT);
    expect_value(__wrap_spi_transfer, cs, SL_TTC2_SPI_CS);
    expect_memory(__wrap_spi_transfer, wd, (void*)cmd, 8);
    expect_value(__wrap_spi_transfer, len, 8);

    uint16_t i = 0;
    for(i=0; i<8; i++)
    {
        will_return(__wrap_spi_transfer, ans[i]);
    }

    will_return(__wrap_spi_transfer, 0);

    sl_ttc2_config_t conf = {0};

    conf.port                   = SL_TTC2_SPI_PORT;
    conf.cs_pin                 = SL_TTC2_SPI_CS;
    conf.port_config.speed_hz   = SL_TTC2_SPI_CLOCK_HZ;
    conf.port_config.mode       = SL_TTC2_SPI_MODE;
    conf.id                     = 0;

    assert_return_code(sl_ttc2_check_device(conf), 0);
}

static void sl_ttc2_write_reg_test(void **state)
{
    uint8_t adr = generate_random(0, 255);
    uint32_t val = generate_random(0, UINT32_MAX-1);

    uint8_t cmd[8] = {0};

    cmd[0] = 2;     /* Write reg. command */
    cmd[1] = adr;   /* Address */
    cmd[2] = (val >> 24) & 0xFF;
    cmd[3] = (val >> 16) & 0xFF;
    cmd[4] = (val >> 8) & 0xFF;
    cmd[5] = val & 0xFF;

    uint16_t checksum = crc16_ccitt(cmd, 6);

    cmd[6] = (checksum >> 8) & 0xFF;
    cmd[7] = checksum & 0xFF;

    expect_value(__wrap_spi_write, port, SL_TTC2_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SL_TTC2_SPI_CS);
    expect_memory(__wrap_spi_write, data, (void*)cmd, 8);
    expect_value(__wrap_spi_write, len, 8);

    will_return(__wrap_spi_write, 0);

    sl_ttc2_config_t conf = {0};

    conf.port                   = SL_TTC2_SPI_PORT;
    conf.cs_pin                 = SL_TTC2_SPI_CS;
    conf.port_config.speed_hz   = SL_TTC2_SPI_CLOCK_HZ;
    conf.port_config.mode       = SL_TTC2_SPI_MODE;
    conf.id                     = 0;

    assert_return_code(sl_ttc2_write_reg(conf, adr, val), 0);
}

static void sl_ttc2_read_reg_test(void **state)
{
    uint8_t adr = generate_random(0, 255);
    uint32_t val = generate_random(0, UINT32_MAX-1);

    uint8_t cmd[8] = {0};
    uint8_t ans[8] = {0};

    cmd[0] = 1;     /* Write reg. command */
    cmd[1] = adr;   /* Address */

    ans[0] = 1;
    ans[1] = adr;
    ans[2] = (val >> 24) & 0xFF;
    ans[3] = (val >> 16) & 0xFF;
    ans[4] = (val >> 8) & 0xFF;
    ans[5] = val & 0xFF;

    uint16_t checksum = crc16_ccitt(ans, 6);

    ans[6] = (checksum >> 8) & 0xFF;
    ans[7] = checksum & 0xFF;

    expect_value(__wrap_spi_transfer, port, SL_TTC2_SPI_PORT);
    expect_value(__wrap_spi_transfer, cs, SL_TTC2_SPI_CS);
    expect_memory(__wrap_spi_transfer, wd, (void*)cmd, 8);
    expect_value(__wrap_spi_transfer, len, 8);

    uint16_t i = 0;
    for(i=0; i<8; i++)
    {
        will_return(__wrap_spi_transfer, ans[i]);
    }

    will_return(__wrap_spi_transfer, 0);

    sl_ttc2_config_t conf = {0};

    conf.port                   = SL_TTC2_SPI_PORT;
    conf.cs_pin                 = SL_TTC2_SPI_CS;
    conf.port_config.speed_hz   = SL_TTC2_SPI_CLOCK_HZ;
    conf.port_config.mode       = SL_TTC2_SPI_MODE;
    conf.id                     = 0;

    uint32_t res = UINT32_MAX;

    assert_return_code(sl_ttc2_read_reg(conf, adr, &res), 0);

    assert_int_equal(res, val);
}

static void sl_ttc2_read_hk_data_test(void **state)
{
}

static void sl_ttc2_read_device_id_test(void **state)
{
}

static void sl_ttc2_read_hardware_version_test(void **state)
{
}

static void sl_ttc2_read_firmware_version_test(void **state)
{
}

static void sl_ttc2_read_time_counter_test(void **state)
{
}

static void sl_ttc2_read_reset_counter_test(void **state)
{
}

static void sl_ttc2_read_reset_cause_test(void **state)
{
}

static void sl_ttc2_read_voltage_test(void **state)
{
}

static void sl_ttc2_read_current_test(void **state)
{
}

static void sl_ttc2_read_temp_test(void **state)
{
}

static void sl_ttc2_read_last_valid_tc_test(void **state)
{
}

static void sl_ttc2_read_rssi_test(void **state)
{
}

static void sl_ttc2_read_antenna_status_test(void **state)
{
}

static void sl_ttc2_read_antenna_deployment_status_test(void **state)
{
}

static void sl_ttc2_read_antenna_deployment_hibernation_status_test(void **state)
{
}

static void sl_ttc2_read_tx_enable_test(void **state)
{
}

static void sl_ttc2_set_tx_enable_test(void **state)
{
}

static void sl_ttc2_read_pkt_counter_test(void **state)
{
}

static void sl_ttc2_read_fifo_pkts_test(void **state)
{
}

static void sl_ttc2_read_len_rx_pkt_in_fifo_test(void **state)
{
}

static void sl_ttc2_check_pkt_avail_test(void **state)
{
}

static void sl_ttc2_transmit_packet_test(void **state)
{
}

static void sl_ttc2_read_packet_test(void **state)
{
}

int main(void)
{
    const struct CMUnitTest sl_ttc2_tests[] = {
        cmocka_unit_test(sl_ttc2_init_test),
        cmocka_unit_test(sl_ttc2_check_device_test),
        cmocka_unit_test(sl_ttc2_write_reg_test),
        cmocka_unit_test(sl_ttc2_read_reg_test),
        cmocka_unit_test(sl_ttc2_read_hk_data_test),
        cmocka_unit_test(sl_ttc2_read_device_id_test),
        cmocka_unit_test(sl_ttc2_read_hardware_version_test),
        cmocka_unit_test(sl_ttc2_read_firmware_version_test),
        cmocka_unit_test(sl_ttc2_read_time_counter_test),
        cmocka_unit_test(sl_ttc2_read_reset_counter_test),
        cmocka_unit_test(sl_ttc2_read_reset_cause_test),
        cmocka_unit_test(sl_ttc2_read_voltage_test),
        cmocka_unit_test(sl_ttc2_read_current_test),
        cmocka_unit_test(sl_ttc2_read_temp_test),
        cmocka_unit_test(sl_ttc2_read_last_valid_tc_test),
        cmocka_unit_test(sl_ttc2_read_rssi_test),
        cmocka_unit_test(sl_ttc2_read_antenna_status_test),
        cmocka_unit_test(sl_ttc2_read_antenna_deployment_status_test),
        cmocka_unit_test(sl_ttc2_read_antenna_deployment_hibernation_status_test),
        cmocka_unit_test(sl_ttc2_read_tx_enable_test),
        cmocka_unit_test(sl_ttc2_set_tx_enable_test),
        cmocka_unit_test(sl_ttc2_read_pkt_counter_test),
        cmocka_unit_test(sl_ttc2_read_fifo_pkts_test),
        cmocka_unit_test(sl_ttc2_read_len_rx_pkt_in_fifo_test),
        cmocka_unit_test(sl_ttc2_check_pkt_avail_test),
        cmocka_unit_test(sl_ttc2_transmit_packet_test),
        cmocka_unit_test(sl_ttc2_read_packet_test),
    };

    return cmocka_run_group_tests(sl_ttc2_tests, NULL, NULL);
}

unsigned int generate_random(unsigned int l, unsigned int r)
{
    return (rand() % (r - l + 1)) + l;
}

uint16_t crc16_ccitt(uint8_t *data, uint16_t len)
{
    uint8_t x;
    uint16_t crc = 0;   /* Initial value */

    while(len--)
    {
        x = crc >> 8 ^ *data++;
        x ^= x >> 4;
        crc = (crc << 8) ^ ((uint16_t)(x << 12)) ^ ((uint16_t)(x << 5)) ^ ((uint16_t)x);
    }

    return crc;
}

/** \} End of sl_ttc2_test group */
