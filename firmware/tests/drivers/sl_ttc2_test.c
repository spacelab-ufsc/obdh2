/*
 * sl_ttc2_test.c
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
 * \brief Unit test of the SpaceLab TTC 2.0 driver.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 0.10.17
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

sl_ttc2_config_t conf = {0};

unsigned int generate_random(unsigned int l, unsigned int r);

static inline void read_adr(uint8_t adr, uint32_t val);

static void sl_ttc2_init_test(void **state)
{
    /* SPI init */
    expect_value(__wrap_spi_init, port, SL_TTC2_SPI_PORT);
    expect_value(__wrap_spi_init, config.speed_hz, SL_TTC2_SPI_CLOCK_HZ);
    expect_value(__wrap_spi_init, config.mode, SL_TTC2_SPI_MODE);

    will_return(__wrap_spi_init, 0);

    /* SPI transfer */
    uint8_t cmd[7] = {0};
    uint8_t ans[7] = {0};

    cmd[0] = SL_TTC2_PKT_PREAMBLE;     /* Packet Preamble */
    cmd[1] = SL_TTC2_CMD_READ_REG;     /* Read device ID */
    cmd[2] = SL_TTC2_REG_DEVICE_ID;     /* Read device ID */

    ans[0] = SL_TTC2_PKT_PREAMBLE;     /* Packet Preamble */
    ans[1] = SL_TTC2_CMD_READ_REG;     /* Command */
    ans[2] = 0;     /* Address */
    ans[3] = 0xCC;  /* Radio 0 ID */
    ans[4] = 0x2A;  /* Radio 0 ID */

    will_return(__wrap_spi_mutex_take, 0);

    expect_value(__wrap_spi_write, port, SL_TTC2_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SL_TTC2_SPI_CS);
    expect_memory(__wrap_spi_write, data, (void*)cmd, 7U);
    expect_value(__wrap_spi_write, len, 7U);

    will_return(__wrap_spi_write, 0);

    expect_value(__wrap_sl_ttc2_spi_read, config.port, SL_TTC2_SPI_PORT);
    expect_value(__wrap_sl_ttc2_spi_read, config.cs_pin, SL_TTC2_SPI_CS);
    expect_value(__wrap_sl_ttc2_spi_read, config.port_config.speed_hz, SL_TTC2_SPI_CLOCK_HZ);
    expect_value(__wrap_sl_ttc2_spi_read, config.port_config.mode, SL_TTC2_SPI_MODE);
    expect_value(__wrap_sl_ttc2_spi_read, len, 7U);

    uint16_t i = 0;
    for(i = 0; i < 7U; i++)
    {
        will_return(__wrap_sl_ttc2_spi_read, ans[i]);
    }

    will_return(__wrap_sl_ttc2_spi_read, 0);

    will_return(__wrap_spi_mutex_give, 0);

    assert_return_code(sl_ttc2_init(conf), 0);
}

static void sl_ttc2_check_device_test(void **state)
{
    uint32_t adr = 0U;
    uint16_t val1 = 0xCC2AU;
    uint16_t val2 = 0xCC2AU;

    read_adr(adr, (uint32_t)val1);

    assert_return_code(sl_ttc2_check_device(conf), 0);

    read_adr(adr, (uint32_t)val2);

    assert_return_code(sl_ttc2_check_device(conf), 0);
}

static void sl_ttc2_write_reg_test(void **state)
{
    uint8_t adr = generate_random(0, 255);
    uint32_t val = generate_random(0, UINT32_MAX-1);

    uint8_t cmd[7] = {0};
    uint8_t cmd_len = 7;

    cmd[0] = SL_TTC2_PKT_PREAMBLE;      /* Write reg. command */
    cmd[1] = SL_TTC2_CMD_WRITE_REG;     /* Address */
    cmd[2] = adr;                       /* Address */

    switch(adr)
    {
        case SL_TTC2_REG_DEVICE_ID:
            cmd[3] = (val >> 8) & 0xFFU;
            cmd[4] = (val >> 0) & 0xFFU;
            val &= 0xFFFFU;
            break;
        case SL_TTC2_REG_HARDWARE_VERSION:
            cmd[3] = val & 0xFFU;
            val &= 0xFFU;
            break;
        case SL_TTC2_REG_RESET_COUNTER:
            cmd[3] = (val >> 8) & 0xFFU;
            cmd[4] = (val >> 0) & 0xFFU;
            val &= 0xFFFFU;
            break;
        case SL_TTC2_REG_LAST_RESET_CAUSE:
            cmd[3] = val & 0xFFU;
            val &= 0xFFU;
            break;
        case SL_TTC2_REG_INPUT_VOLTAGE_MCU:
            cmd[3] = (val >> 8) & 0xFFU;
            cmd[4] = (val >> 0) & 0xFFU;
            val &= 0xFFFFU;
            break;
        case SL_TTC2_REG_INPUT_CURRENT_MCU:
            cmd[3] = (val >> 8) & 0xFFU;
            cmd[4] = (val >> 0) & 0xFFU;
            val &= 0xFFFFU;
            break;
        case SL_TTC2_REG_TEMPERATURE_MCU:
            cmd[3] = (val >> 8) & 0xFFU;
            cmd[4] = (val >> 0) & 0xFFU;
            val &= 0xFFFFU;
            break;
        case SL_TTC2_REG_INPUT_VOLTAGE_RADIO:
            cmd[3] = (val >> 8) & 0xFFU;
            cmd[4] = (val >> 0) & 0xFFU;
            val &= 0xFFFFU;
            break;
        case SL_TTC2_REG_INPUT_CURRENT_RADIO:
            cmd[3] = (val >> 8) & 0xFFU;
            cmd[4] = (val >> 0) & 0xFFU;
            val &= 0xFFFFU;
            break;
        case SL_TTC2_REG_TEMPERATURE_RADIO:
            cmd[3] = (val >> 8) & 0xFFU;
            cmd[4] = (val >> 0) & 0xFFU;
            val &= 0xFFFFU;
            break;
        case SL_TTC2_REG_LAST_VALID_TC:
            cmd[3] = val & 0xFFU;
            val &= 0xFFU;
            break;
        case SL_TTC2_REG_RSSI_LAST_VALID_TC:
            cmd[3] = (val >> 8) & 0xFFU;
            cmd[4] = (val >> 0) & 0xFFU;
            val &= 0xFFFFU;
            break;
        case SL_TTC2_REG_TEMPERATURE_ANTENNA:
            cmd[3] = (val >> 8) & 0xFFU;
            cmd[4] = (val >> 0) & 0xFFU;
            val &= 0xFFFFU;
            break;
        case SL_TTC2_REG_ANTENNA_STATUS:
            cmd[3] = (val >> 8) & 0xFFU;
            cmd[4] = (val >> 0) & 0xFFU;
            val &= 0xFFFFU;
            break;
        case SL_TTC2_REG_ANTENNA_DEPLOYMENT_STATUS:
            cmd[3] = val & 0xFFU;
            val &= 0xFFU;
            break;
        case SL_TTC2_REG_ANTENNA_DEP_HIB_STATUS:
            cmd[3] = val & 0xFFU;
            val &= 0xFFU;
            break;
        case SL_TTC2_REG_TX_ENABLE:
            cmd[3] = val & 0xFFU;
            val &= 0xFFU;
            break;
        case SL_TTC2_REG_FIFO_TX_PACKET:
            cmd[3] = val & 0xFFU;
            val &= 0xFFU;
            break;
        case SL_TTC2_REG_FIFO_RX_PACKET:
            cmd[3] = val & 0xFFU;
            val &= 0xFFU;
            break;
        case SL_TTC2_REG_LEN_FIRST_RX_PACKET_IN_FIFO:
            cmd[3] = (val >> 8) & 0xFFU;
            cmd[4] = (val >> 0) & 0xFFU;
            val &= 0xFFFFU;
            break;
        default:
            cmd[3] = (val >> 24) & 0xFFU;
            cmd[4] = (val >> 16) & 0xFFU;
            cmd[5] = (val >> 8)  & 0xFFU;
            cmd[6] = (val >> 0)  & 0xFFU;
            break;
    }

    will_return(__wrap_spi_mutex_take, 0);

    expect_value(__wrap_spi_write, port, SL_TTC2_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SL_TTC2_SPI_CS);
    expect_memory(__wrap_spi_write, data, (void*)cmd, cmd_len);
    expect_value(__wrap_spi_write, len, cmd_len);

    will_return(__wrap_spi_write, 0);

    will_return(__wrap_spi_mutex_give, 0);

    assert_return_code(sl_ttc2_write_reg(conf, adr, val), 0);
}

static void sl_ttc2_read_reg_test(void **state)
{
    uint8_t adr = generate_random(0, 255) % 23U;
    uint32_t val = generate_random(0, UINT32_MAX-1);

    uint32_t res = UINT32_MAX;

    uint8_t cmd[7] = {0};
    uint8_t ans[7] = {0};

    cmd[0] = SL_TTC2_PKT_PREAMBLE;     
    cmd[1] = SL_TTC2_CMD_READ_REG;     /* Read reg. command */
    cmd[2] = adr;                       /* Address */

    ans[0] = SL_TTC2_PKT_PREAMBLE;
    ans[1] = SL_TTC2_CMD_READ_REG;
    ans[2] = adr;

    switch(adr)
    {
        case SL_TTC2_REG_DEVICE_ID:
            ans[3] = (val >> 8) & 0xFFU;
            ans[4] = (val >> 0) & 0xFFU;
            val &= 0xFFFFU;
            break;
        case SL_TTC2_REG_HARDWARE_VERSION:
            ans[3] = val & 0xFFU;
            val &= 0xFFU;
            break;
        case SL_TTC2_REG_RESET_COUNTER:
            ans[3] = (val >> 8) & 0xFFU;
            ans[4] = (val >> 0) & 0xFFU;
            val &= 0xFFFFU;
            break;
        case SL_TTC2_REG_LAST_RESET_CAUSE:
            ans[3] = val & 0xFFU;
            val &= 0xFFU;
            break;
        case SL_TTC2_REG_INPUT_VOLTAGE_MCU:
            ans[3] = (val >> 8) & 0xFFU;
            ans[4] = (val >> 0) & 0xFFU;
            val &= 0xFFFFU;
            break;
        case SL_TTC2_REG_INPUT_CURRENT_MCU:
            ans[3] = (val >> 8) & 0xFFU;
            ans[4] = (val >> 0) & 0xFFU;
            val &= 0xFFFFU;
            break;
        case SL_TTC2_REG_TEMPERATURE_MCU:
            ans[3] = (val >> 8) & 0xFFU;
            ans[4] = (val >> 0) & 0xFFU;
            val &= 0xFFFFU;
            break;
        case SL_TTC2_REG_INPUT_VOLTAGE_RADIO:
            ans[3] = (val >> 8) & 0xFFU;
            ans[4] = (val >> 0) & 0xFFU;
            val &= 0xFFFFU;
            break;
        case SL_TTC2_REG_INPUT_CURRENT_RADIO:
            ans[3] = (val >> 8) & 0xFFU;
            ans[4] = (val >> 0) & 0xFFU;
            val &= 0xFFFFU;
            break;
        case SL_TTC2_REG_TEMPERATURE_RADIO:
            ans[3] = (val >> 8) & 0xFFU;
            ans[4] = (val >> 0) & 0xFFU;
            val &= 0xFFFFU;
            break;
        case SL_TTC2_REG_LAST_VALID_TC:
            ans[3] = val & 0xFFU;
            val &= 0xFFU;
            break;
        case SL_TTC2_REG_RSSI_LAST_VALID_TC:
            ans[3] = (val >> 8) & 0xFFU;
            ans[4] = (val >> 0) & 0xFFU;
            val &= 0xFFFFU;
            break;
        case SL_TTC2_REG_TEMPERATURE_ANTENNA:
            ans[3] = (val >> 8) & 0xFFU;
            ans[4] = (val >> 0) & 0xFFU;
            val &= 0xFFFFU;
            break;
        case SL_TTC2_REG_ANTENNA_STATUS:
            ans[3] = (val >> 8) & 0xFFU;
            ans[4] = (val >> 0) & 0xFFU;
            val &= 0xFFFFU;
            break;
        case SL_TTC2_REG_ANTENNA_DEPLOYMENT_STATUS:
            ans[3] = val & 0xFFU;
            val &= 0xFFU;
            break;
        case SL_TTC2_REG_ANTENNA_DEP_HIB_STATUS:
            ans[3] = val & 0xFFU;
            val &= 0xFFU;
            break;
        case SL_TTC2_REG_TX_ENABLE:
            ans[3] = val & 0xFFU;
            val &= 0xFFU;
            break;
        case SL_TTC2_REG_FIFO_TX_PACKET:
            ans[3] = val & 0xFFU;
            val &= 0xFFU;
            break;
        case SL_TTC2_REG_FIFO_RX_PACKET:
            ans[3] = val & 0xFFU;
            val &= 0xFFU;
            break;
        case SL_TTC2_REG_LEN_FIRST_RX_PACKET_IN_FIFO:
            ans[3] = (val >> 8) & 0xFFU;
            ans[4] = (val >> 0) & 0xFFU;
            val &= 0xFFFFU;
            break;
        default:
            ans[3] = (val >> 24) & 0xFFU;
            ans[4] = (val >> 16) & 0xFFU;
            ans[5] = (val >> 8)  & 0xFFU;
            ans[6] = (val >> 0)  & 0xFFU;
            break;
    }

    will_return(__wrap_spi_mutex_take, 0);

    expect_value(__wrap_spi_write, port, SL_TTC2_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SL_TTC2_SPI_CS);
    expect_memory(__wrap_spi_write, data, (void*)cmd, 7U);
    expect_value(__wrap_spi_write, len, 7U);

    will_return(__wrap_spi_write, 0);

    expect_value(__wrap_sl_ttc2_spi_read, config.port, SL_TTC2_SPI_PORT);
    expect_value(__wrap_sl_ttc2_spi_read, config.cs_pin, SL_TTC2_SPI_CS);
    expect_value(__wrap_sl_ttc2_spi_read, config.port_config.speed_hz, SL_TTC2_SPI_CLOCK_HZ);
    expect_value(__wrap_sl_ttc2_spi_read, config.port_config.mode, SL_TTC2_SPI_MODE);
    expect_value(__wrap_sl_ttc2_spi_read, len, 7U);

    uint16_t i = 0;
    for(i = 0; i < 7U; i++)
    {
        will_return(__wrap_sl_ttc2_spi_read, ans[i]);
    }

    will_return(__wrap_sl_ttc2_spi_read, 0);

    will_return(__wrap_spi_mutex_give, 0);

    assert_return_code(sl_ttc2_read_reg(conf, adr, &res), 0);

    assert_int_equal(res, val);
}

static void sl_ttc2_read_hk_data_test(void **state)
{
    sl_ttc2_hk_data_t hk_val = {0};

    hk_val.time_counter = generate_random(0, UINT32_MAX-1);
    read_adr(3, hk_val.time_counter);

    hk_val.reset_counter = generate_random(0, UINT16_MAX);
    read_adr(4, (uint32_t)hk_val.reset_counter);

    hk_val.last_reset_cause = generate_random(0, UINT8_MAX);
    read_adr(5, (uint32_t)hk_val.last_reset_cause);

    hk_val.voltage_mcu = generate_random(0, UINT16_MAX);
    read_adr(6, (uint32_t)hk_val.voltage_mcu);

    hk_val.current_mcu = generate_random(0, UINT16_MAX);
    read_adr(7, (uint32_t)hk_val.current_mcu);

    hk_val.temperature_mcu = generate_random(0, UINT16_MAX);
    read_adr(8, (uint32_t)hk_val.temperature_mcu);

    hk_val.voltage_radio = generate_random(0, UINT16_MAX);
    read_adr(9, (uint32_t)hk_val.voltage_radio);

    hk_val.current_radio = generate_random(0, UINT16_MAX);
    read_adr(10, (uint32_t)hk_val.current_radio);

    hk_val.temperature_radio = generate_random(0, UINT16_MAX);
    read_adr(11, (uint32_t)hk_val.temperature_radio);

    hk_val.last_valid_tc = generate_random(0, UINT8_MAX);
    read_adr(12, (uint32_t)hk_val.last_valid_tc);

    hk_val.rssi_last_valid_tc = generate_random(0, UINT16_MAX);
    read_adr(13, (uint32_t)hk_val.rssi_last_valid_tc);

    hk_val.temperature_antenna = generate_random(0, UINT16_MAX);
    read_adr(14, (uint32_t)hk_val.temperature_antenna);

    hk_val.antenna_status = generate_random(0, UINT16_MAX);
    read_adr(15, (uint32_t)hk_val.antenna_status);

    hk_val.deployment_status = generate_random(0, UINT8_MAX);
    read_adr(16, (uint32_t)hk_val.deployment_status);

    hk_val.hibernation_status = generate_random(0, UINT8_MAX);
    read_adr(17, (uint32_t)hk_val.hibernation_status);

    hk_val.tx_packet_counter = generate_random(0, UINT32_MAX-1);
    read_adr(19, (uint32_t)hk_val.tx_packet_counter);

    hk_val.rx_packet_counter = generate_random(0, UINT32_MAX-1);
    read_adr(20, (uint32_t)hk_val.rx_packet_counter);

    sl_ttc2_hk_data_t hk_res = {0};

    assert_return_code(sl_ttc2_read_hk_data(conf, &hk_res), 0);

    assert_int_equal(hk_val.time_counter,           hk_res.time_counter);
    assert_int_equal(hk_val.reset_counter,          hk_res.reset_counter);
    assert_int_equal(hk_val.last_reset_cause,       hk_res.last_reset_cause);
    assert_int_equal(hk_val.voltage_mcu,            hk_res.voltage_mcu);
    assert_int_equal(hk_val.current_mcu,            hk_res.current_mcu);
    assert_int_equal(hk_val.temperature_mcu,        hk_res.temperature_mcu);
    assert_int_equal(hk_val.voltage_radio,          hk_res.voltage_radio);
    assert_int_equal(hk_val.current_radio,          hk_res.current_radio);
    assert_int_equal(hk_val.temperature_radio,      hk_res.temperature_radio);
    assert_int_equal(hk_val.last_valid_tc,          hk_res.last_valid_tc);
    assert_int_equal(hk_val.rssi_last_valid_tc,     hk_res.rssi_last_valid_tc);
    assert_int_equal(hk_val.temperature_antenna,    hk_res.temperature_antenna);
    assert_int_equal(hk_val.antenna_status,         hk_res.antenna_status);
    assert_int_equal(hk_val.deployment_status,      hk_res.deployment_status);
    assert_int_equal(hk_val.hibernation_status,     hk_res.hibernation_status);
    assert_int_equal(hk_val.tx_packet_counter,      hk_res.tx_packet_counter);
    assert_int_equal(hk_val.rx_packet_counter,      hk_res.rx_packet_counter);
}

static void sl_ttc2_read_device_id_test(void **state)
{
    uint8_t adr = 0;    /* Device ID register */
    uint16_t val = generate_random(0, UINT16_MAX);

    read_adr(adr, (uint32_t)val);

    uint16_t res = UINT16_MAX;

    assert_return_code(sl_ttc2_read_device_id(conf, &res), 0);

    assert_int_equal(res, val);
}

static void sl_ttc2_read_hardware_version_test(void **state)
{
    uint8_t adr = 1;    /* Hardware version register */
    uint8_t val = generate_random(0, UINT8_MAX);

    read_adr(adr, (uint32_t)val);

    uint8_t res = UINT8_MAX;

    assert_return_code(sl_ttc2_read_hardware_version(conf, &res), 0);

    assert_int_equal(res, val);
}

static void sl_ttc2_read_firmware_version_test(void **state)
{
    uint8_t adr = 2;    /* Firmware version register */
    uint32_t val = generate_random(0, UINT32_MAX-1);

    read_adr(adr, val);

    uint32_t res = UINT32_MAX;

    assert_return_code(sl_ttc2_read_firmware_version(conf, &res), 0);

    assert_int_equal(res, val);
}

static void sl_ttc2_read_time_counter_test(void **state)
{
    uint8_t adr = 3;    /* Time counter register */
    uint32_t val = generate_random(0, UINT32_MAX-1);

    read_adr(adr, val);

    uint32_t res = UINT32_MAX;

    assert_return_code(sl_ttc2_read_time_counter(conf, &res), 0);

    assert_int_equal(res, val);
}

static void sl_ttc2_read_reset_counter_test(void **state)
{
    uint8_t adr = 4;    /* Reset counter register */
    uint16_t val = generate_random(0, UINT16_MAX);

    read_adr(adr, (uint32_t)val);

    uint16_t res = UINT16_MAX;

    assert_return_code(sl_ttc2_read_reset_counter(conf, &res), 0);

    assert_int_equal(res, val);
}

static void sl_ttc2_read_reset_cause_test(void **state)
{
    uint8_t adr = 5;    /* Reset cause register */
    uint8_t val = generate_random(0, UINT8_MAX);

    read_adr(adr, (uint32_t)val);

    uint8_t res = UINT8_MAX;

    assert_return_code(sl_ttc2_read_reset_cause(conf, &res), 0);

    assert_int_equal(res, val);
}

static void sl_ttc2_read_voltage_test(void **state)
{
    uint8_t i = 0;
    for(i=0; i<UINT8_MAX; i++)
    {
        uint8_t adr = UINT8_MAX;
        sl_ttc2_voltage_t res = UINT16_MAX;

        switch(i)
        {
            case SL_TTC2_VOLTAGE_MCU:
                adr = 6;    /* MCU voltage register */
                break;
            case SL_TTC2_VOLTAGE_RADIO:
                adr = 9;    /* Radio voltage register */
                break;
            default:
                assert_int_equal(sl_ttc2_read_voltage(conf, i, &res), -1);

                continue;
        }

        sl_ttc2_voltage_t val = generate_random(0, UINT16_MAX);

        read_adr(adr, (uint32_t)val);

        assert_return_code(sl_ttc2_read_voltage(conf, i, &res), 0);

        assert_int_equal(res, val);
    }
}

static void sl_ttc2_read_current_test(void **state)
{
    uint8_t i = 0;
    for(i=0; i<UINT8_MAX; i++)
    {
        uint8_t adr = UINT8_MAX;
        sl_ttc2_current_t res = UINT16_MAX;

        switch(i)
        {
            case SL_TTC2_CURRENT_MCU:
                adr = 7;    /* MCU current register */
                break;
            case SL_TTC2_CURRENT_RADIO:
                adr = 10;   /* Radio current register */
                break;
            default:
                assert_int_equal(sl_ttc2_read_current(conf, i, &res), -1);

                continue;
        }

        sl_ttc2_current_t val = generate_random(0, UINT16_MAX);

        read_adr(adr, (uint32_t)val);

        assert_return_code(sl_ttc2_read_current(conf, i, &res), 0);

        assert_int_equal(res, val);
    }
}

static void sl_ttc2_read_temp_test(void **state)
{
    uint8_t i = 0;
    for(i=0; i<UINT8_MAX; i++)
    {
        uint8_t adr = UINT8_MAX;
        sl_ttc2_temp_t res = UINT16_MAX;

        switch(i)
        {
            case SL_TTC2_TEMP_MCU:
                adr = 8;    /* MCU temperature register */
                break;
            case SL_TTC2_TEMP_RADIO:
                adr = 11;   /* Radio temperature register */
                break;
            case SL_TTC2_TEMP_ANTENNA:
                adr = 14;   /* Antenna temperature register */
                break;
            default:
                assert_int_equal(sl_ttc2_read_temp(conf, i, &res), -1);

                continue;
        }

        sl_ttc2_temp_t val = generate_random(0, UINT16_MAX);

        read_adr(adr, (uint32_t)val);

        assert_return_code(sl_ttc2_read_temp(conf, i, &res), 0);

        assert_int_equal(res, val);
    }
}

static void sl_ttc2_read_last_valid_tc_test(void **state)
{
    uint8_t adr = 12;   /* Last valid TC register */
    uint8_t val = generate_random(0, UINT8_MAX);

    read_adr(adr, (uint32_t)val);

    uint8_t res = UINT8_MAX;

    assert_return_code(sl_ttc2_read_last_valid_tc(conf, &res), 0);

    assert_int_equal(res, val);
}

static void sl_ttc2_read_rssi_test(void **state)
{
    uint8_t adr = 13;   /* RSSI register */
    sl_ttc2_rssi_t val = generate_random(0, UINT16_MAX);

    read_adr(adr, (uint32_t)val);

    sl_ttc2_rssi_t res = UINT16_MAX;

    assert_return_code(sl_ttc2_read_rssi(conf, &res), 0);

    assert_int_equal(res, val);
}

static void sl_ttc2_read_antenna_status_test(void **state)
{
    uint8_t adr = 15;   /* Antenna status register */
    uint16_t val = generate_random(0, UINT16_MAX);

    read_adr(adr, (uint32_t)val);

    uint16_t res = UINT16_MAX;

    assert_return_code(sl_ttc2_read_antenna_status(conf, &res), 0);

    assert_int_equal(res, val);
}

static void sl_ttc2_read_antenna_deployment_status_test(void **state)
{
    uint8_t adr = 16;   /* Antenna deploymenbt status register */
    uint8_t val = generate_random(0, UINT8_MAX);

    read_adr(adr, (uint32_t)val);

    uint8_t res = UINT8_MAX;

    assert_return_code(sl_ttc2_read_antenna_deployment_status(conf, &res), 0);

    assert_int_equal(res, val);
}

static void sl_ttc2_read_antenna_deployment_hibernation_status_test(void **state)
{
    uint8_t adr = 17;   /* Antenna deloyment hibernation status register */
    uint8_t val = generate_random(0, UINT8_MAX);

    read_adr(adr, (uint32_t)val);

    uint8_t res = UINT8_MAX;

    assert_return_code(sl_ttc2_read_antenna_deployment_hibernation_status(conf, &res), 0);

    assert_int_equal(res, val);
}

static void sl_ttc2_read_tx_enable_test(void **state)
{
    uint8_t adr = 18;   /* TX enable register */
    uint8_t val = generate_random(0, UINT8_MAX);

    read_adr(adr, (uint32_t)val);

    uint8_t res = UINT8_MAX;

    assert_return_code(sl_ttc2_read_tx_enable(conf, &res), 0);

    assert_int_equal(res, val);
}

static void sl_ttc2_set_tx_enable_test(void **state)
{
    uint8_t adr = SL_TTC2_REG_TX_ENABLE;   /* TX enable register */
    uint8_t val = generate_random(0, 1);

    uint8_t cmd[7] = {0};

    cmd[0] = SL_TTC2_PKT_PREAMBLE;     /* Write reg. command */
    cmd[1] = SL_TTC2_CMD_WRITE_REG;   /* Address */
    cmd[2] = adr;   /* Address */
    cmd[3] = val;

    will_return(__wrap_spi_mutex_take, 0);

    expect_value(__wrap_spi_write, port, SL_TTC2_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SL_TTC2_SPI_CS);
    expect_memory(__wrap_spi_write, data, (void*)cmd, 7U);
    expect_value(__wrap_spi_write, len, 7U);

    will_return(__wrap_spi_write, 0);

    will_return(__wrap_spi_mutex_give, 0);

    assert_return_code(sl_ttc2_set_tx_enable(conf, (bool)val), 0);
}

static void sl_ttc2_read_pkt_counter_test(void **state)
{
    uint8_t i = 0;
    for(i=0; i<UINT8_MAX; i++)
    {
        uint8_t adr = UINT8_MAX;
        uint32_t res = UINT8_MAX;

        switch(i)
        {
            case SL_TTC2_TX_PKT:
                adr = 19;   /* TX packet counter register */
                break;
            case SL_TTC2_RX_PKT:
                adr = 20;   /* RX packet counter register */
                break;
            default:
                assert_int_equal(sl_ttc2_read_pkt_counter(conf, i, &res), -1);

                continue;
        }

        uint32_t val = generate_random(0, UINT32_MAX-1);

        read_adr(adr, (uint32_t)val);

        assert_return_code(sl_ttc2_read_pkt_counter(conf, i, &res), 0);

        assert_int_equal(res, val);
    }
}

static void sl_ttc2_read_fifo_pkts_test(void **state)
{
    uint8_t i = 0;
    for(i=0; i<UINT8_MAX; i++)
    {
        uint8_t adr = UINT8_MAX;
        uint8_t res = UINT8_MAX;

        switch(i)
        {
            case SL_TTC2_TX_PKT:
                adr = 21;   /* FIFO TX packet register */
                break;
            case SL_TTC2_RX_PKT:
                adr = 22;   /* FIFO RX packet register */
                break;
            default:
                assert_int_equal(sl_ttc2_read_fifo_pkts(conf, i, &res), -1);

                continue;
        }

        uint8_t val = generate_random(0, UINT8_MAX);

        read_adr(adr, (uint32_t)val);

        assert_return_code(sl_ttc2_read_fifo_pkts(conf, i, &res), 0);

        assert_int_equal(res, val);
    }
}

static void sl_ttc2_read_len_rx_pkt_in_fifo_test(void **state)
{
    uint8_t adr = SL_TTC2_REG_LEN_FIRST_RX_PACKET_IN_FIFO;   /* Length of the first available RX packet register */
    uint16_t val = generate_random(0, UINT16_MAX);

    uint16_t res = UINT16_MAX;

    read_adr(adr, (uint32_t)val);

    assert_return_code(sl_ttc2_read_len_rx_pkt_in_fifo(conf, &res), 0);

    assert_int_equal(res, val);
}

static void sl_ttc2_check_pkt_avail_test(void **state)
{
    uint8_t adr = 22;   /* FIFO RX packets register */
    uint8_t val = generate_random(0, UINT8_MAX);

    read_adr(adr, (uint32_t)val);

    assert_int_equal((uint8_t)(sl_ttc2_check_pkt_avail(conf)), val);
}

static void sl_ttc2_transmit_packet_test(void **state)
{
    uint8_t cmd[223] = {0U};
    uint16_t data_len = generate_random(1, 220);

    cmd[0] = SL_TTC2_PKT_PREAMBLE;     /* Packet Preamble */
    cmd[1] = SL_TTC2_CMD_TRANSMIT_PKT;     /* Transmit packet command */
    cmd[2] = data_len;     /* Data lenght */

    will_return(__wrap_spi_mutex_take, 0);

    expect_value(__wrap_spi_write, port, SL_TTC2_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SL_TTC2_SPI_CS);
    expect_memory(__wrap_spi_write, data, (void*)cmd, 7U);
    expect_value(__wrap_spi_write, len, 7U);

    will_return(__wrap_spi_write, 0);

    uint16_t i = 0;
    for(i = 0; i < data_len; i++)
    {
        cmd[i+3] = generate_random(0, UINT8_MAX);
    }

    expect_value(__wrap_spi_write, port, SL_TTC2_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SL_TTC2_SPI_CS);
    expect_memory(__wrap_spi_write, data, (void*)cmd, 3U + data_len);
    expect_value(__wrap_spi_write, len, 3U + data_len);

    will_return(__wrap_spi_write, 0);

    will_return(__wrap_spi_mutex_give, 0);

    assert_return_code(sl_ttc2_transmit_packet(conf, &cmd[3], data_len), 0);
}

static void sl_ttc2_read_packet_test(void **state)
{
    uint8_t adr = SL_TTC2_REG_LEN_FIRST_RX_PACKET_IN_FIFO;   /* Length of the first available RX packet register */
    uint8_t cmd[7U] = {0};
    uint8_t pkt[UINT16_MAX] = {UINT8_MAX};
    uint16_t pkt_len = generate_random(1, UINT16_MAX);
    uint8_t res_pkt[220] = {UINT8_MAX};
    uint16_t res_pkt_len = UINT16_MAX;

    cmd[0] = SL_TTC2_PKT_PREAMBLE;     /* Packet Preamble */
    cmd[1] = SL_TTC2_CMD_RECEIVE_PKT;     /* Read packet command */

    if ((pkt_len < 0) || (pkt_len > 220)) 
    {
        read_adr(adr, (uint32_t)pkt_len); /* Needed to mock behavior of the function */

        /* Packet lenght is greater than the maximum allowed, in this case the function should return -1 */
        assert_int_equal(sl_ttc2_read_packet(conf, res_pkt, &res_pkt_len), -1); 
        
        pkt_len = generate_random(1, 220); /* Generating a valid packet lenght */
    }

    read_adr(adr, (uint32_t)pkt_len); 

    will_return(__wrap_spi_mutex_take, 0);

    expect_value(__wrap_spi_write, port, SL_TTC2_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SL_TTC2_SPI_CS);
    expect_memory(__wrap_spi_write, data, (void*)cmd, 7U);
    expect_value(__wrap_spi_write, len, 7U);

    will_return(__wrap_spi_write, 0);

    expect_value(__wrap_sl_ttc2_spi_read, config.port, SL_TTC2_SPI_PORT);
    expect_value(__wrap_sl_ttc2_spi_read, config.cs_pin, SL_TTC2_SPI_CS);
    expect_value(__wrap_sl_ttc2_spi_read, config.port_config.speed_hz, SL_TTC2_SPI_CLOCK_HZ);
    expect_value(__wrap_sl_ttc2_spi_read, config.port_config.mode, SL_TTC2_SPI_MODE);
    expect_value(__wrap_sl_ttc2_spi_read, len, 2U + pkt_len);


    will_return(__wrap_sl_ttc2_spi_read, SL_TTC2_PKT_PREAMBLE);
    will_return(__wrap_sl_ttc2_spi_read, SL_TTC2_CMD_RECEIVE_PKT);

    uint16_t i = 0;
    for(i = 0; i < pkt_len; i++)
    {
        will_return(__wrap_sl_ttc2_spi_read, pkt[i]);
    }

    will_return(__wrap_sl_ttc2_spi_read, 0);

    will_return(__wrap_spi_mutex_give, 0);

    assert_return_code(sl_ttc2_read_packet(conf, res_pkt, &res_pkt_len), 0);

    assert_memory_equal((void*)res_pkt, (void*)pkt, pkt_len);
    assert_int_equal(res_pkt_len, pkt_len);
}

int main(void)
{
    conf.port                   = SL_TTC2_SPI_PORT;
    conf.cs_pin                 = SL_TTC2_SPI_CS;
    conf.port_config.speed_hz   = SL_TTC2_SPI_CLOCK_HZ;
    conf.port_config.mode       = SL_TTC2_SPI_MODE;
    conf.id                     = 0;

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

static inline void read_adr(uint8_t adr, uint32_t val)
{
    uint8_t cmd[7] = {0};
    uint8_t ans[7] = {0};

    cmd[0] = SL_TTC2_PKT_PREAMBLE;     
    cmd[1] = SL_TTC2_CMD_READ_REG;     /* Read reg. command */
    cmd[2] = adr;                       /* Address */

    ans[0] = SL_TTC2_PKT_PREAMBLE;
    ans[1] = SL_TTC2_CMD_READ_REG;
    ans[2] = adr;

    switch(adr)
    {
        case SL_TTC2_REG_DEVICE_ID:
            ans[3] = (val >> 8) & 0xFFU;
            ans[4] = (val >> 0) & 0xFFU;
            val &= 0xFFFFU;
            break;
        case SL_TTC2_REG_HARDWARE_VERSION:
            ans[3] = val & 0xFFU;
            val &= 0xFFU;
            break;
        case SL_TTC2_REG_RESET_COUNTER:
            ans[3] = (val >> 8) & 0xFFU;
            ans[4] = (val >> 0) & 0xFFU;
            val &= 0xFFFFU;
            break;
        case SL_TTC2_REG_LAST_RESET_CAUSE:
            ans[3] = val & 0xFFU;
            val &= 0xFFU;
            break;
        case SL_TTC2_REG_INPUT_VOLTAGE_MCU:
            ans[3] = (val >> 8) & 0xFFU;
            ans[4] = (val >> 0) & 0xFFU;
            val &= 0xFFFFU;
            break;
        case SL_TTC2_REG_INPUT_CURRENT_MCU:
            ans[3] = (val >> 8) & 0xFFU;
            ans[4] = (val >> 0) & 0xFFU;
            val &= 0xFFFFU;
            break;
        case SL_TTC2_REG_TEMPERATURE_MCU:
            ans[3] = (val >> 8) & 0xFFU;
            ans[4] = (val >> 0) & 0xFFU;
            val &= 0xFFFFU;
            break;
        case SL_TTC2_REG_INPUT_VOLTAGE_RADIO:
            ans[3] = (val >> 8) & 0xFFU;
            ans[4] = (val >> 0) & 0xFFU;
            val &= 0xFFFFU;
            break;
        case SL_TTC2_REG_INPUT_CURRENT_RADIO:
            ans[3] = (val >> 8) & 0xFFU;
            ans[4] = (val >> 0) & 0xFFU;
            val &= 0xFFFFU;
            break;
        case SL_TTC2_REG_TEMPERATURE_RADIO:
            ans[3] = (val >> 8) & 0xFFU;
            ans[4] = (val >> 0) & 0xFFU;
            val &= 0xFFFFU;
            break;
        case SL_TTC2_REG_LAST_VALID_TC:
            ans[3] = val & 0xFFU;
            val &= 0xFFU;
            break;
        case SL_TTC2_REG_RSSI_LAST_VALID_TC:
            ans[3] = (val >> 8) & 0xFFU;
            ans[4] = (val >> 0) & 0xFFU;
            val &= 0xFFFFU;
            break;
        case SL_TTC2_REG_TEMPERATURE_ANTENNA:
            ans[3] = (val >> 8) & 0xFFU;
            ans[4] = (val >> 0) & 0xFFU;
            val &= 0xFFFFU;
            break;
        case SL_TTC2_REG_ANTENNA_STATUS:
            ans[3] = (val >> 8) & 0xFFU;
            ans[4] = (val >> 0) & 0xFFU;
            val &= 0xFFFFU;
            break;
        case SL_TTC2_REG_ANTENNA_DEPLOYMENT_STATUS:
            ans[3] = val & 0xFFU;
            val &= 0xFFU;
            break;
        case SL_TTC2_REG_ANTENNA_DEP_HIB_STATUS:
            ans[3] = val & 0xFFU;
            val &= 0xFFU;
            break;
        case SL_TTC2_REG_TX_ENABLE:
            ans[3] = val & 0xFFU;
            val &= 0xFFU;
            break;
        case SL_TTC2_REG_FIFO_TX_PACKET:
            ans[3] = val & 0xFFU;
            val &= 0xFFU;
            break;
        case SL_TTC2_REG_FIFO_RX_PACKET:
            ans[3] = val & 0xFFU;
            val &= 0xFFU;
            break;
        case SL_TTC2_REG_LEN_FIRST_RX_PACKET_IN_FIFO:
            ans[3] = (val >> 8) & 0xFFU;
            ans[4] = (val >> 0) & 0xFFU;
            val &= 0xFFFFU;
            break;
        default:
            ans[3] = (val >> 24) & 0xFFU;
            ans[4] = (val >> 16) & 0xFFU;
            ans[5] = (val >> 8)  & 0xFFU;
            ans[6] = (val >> 0)  & 0xFFU;
            break;
    }

    will_return(__wrap_spi_mutex_take, 0);

    expect_value(__wrap_spi_write, port, SL_TTC2_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SL_TTC2_SPI_CS);
    expect_memory(__wrap_spi_write, data, (void*)cmd, 7U);
    expect_value(__wrap_spi_write, len, 7U);

    will_return(__wrap_spi_write, 0);

    expect_value(__wrap_sl_ttc2_spi_read, config.port, SL_TTC2_SPI_PORT);
    expect_value(__wrap_sl_ttc2_spi_read, config.cs_pin, SL_TTC2_SPI_CS);
    expect_value(__wrap_sl_ttc2_spi_read, config.port_config.speed_hz, SL_TTC2_SPI_CLOCK_HZ);
    expect_value(__wrap_sl_ttc2_spi_read, config.port_config.mode, SL_TTC2_SPI_MODE);
    expect_value(__wrap_sl_ttc2_spi_read, len, 7U);

    uint16_t i = 0;
    for(i = 0; i < 7U; i++)
    {
        will_return(__wrap_sl_ttc2_spi_read, ans[i]);
    }

    will_return(__wrap_sl_ttc2_spi_read, 0);

    will_return(__wrap_spi_mutex_give, 0);
}

/** \} End of sl_ttc2_test group */
