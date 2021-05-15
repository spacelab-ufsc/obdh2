/*
 * sl_ttc2.c
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
 * \brief SpaceLab TTC 2.0 driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.6.21
 * 
 * \date 2021/05/12
 * 
 * \addtogroup sl_ttc2
 * \{
 */

#include <string.h>

#include <config/config.h>
#include <system/sys_log/sys_log.h>

#include "sl_ttc2.h"

/**
 * \brief Computes the CRC-16 of a sequence of bytes.
 *
 * \param[in] data is an array of data to compute the CRC-16.
 *
 * \param[in] len is the number of bytes of the given array.
 *
 * \return The computed CRC-16 value of the given data.
 */
uint16_t sl_ttc2_crc16(uint8_t *data, uint16_t len);

/**
 * \brief Checks the CRC value of a given sequence of bytes.
 *
 * \param[in] data is the data to check the CRC.
 *
 * \param[in] len is the number of bytes to check the CRC value.
 *
 * \param[in] crc is the CRC-16 value to check.
 *
 * \return TRUE/FALSE if the given CRC value is correct or not.
 */
bool sl_ttc2_check_crc(uint8_t *data, uint16_t len, uint16_t crc);

int sl_ttc2_init(sl_ttc2_config_t config)
{
    if (spi_init(config.port, config.port_config) != 0)
    {
    #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
        sys_log_print_event_from_module(SYS_LOG_ERROR, SL_TTC2_MODULE_NAME, "Error initializing the SPI port!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        return -1;
    }

    sl_ttc2_delay_ms(10);

    if (sl_ttc2_check_device(config) != 0)
    {
        return -1;
    }

    return 0;
}

int sl_ttc2_check_device(sl_ttc2_config_t config)
{
    uint16_t id = UINT16_MAX;

    if (sl_ttc2_read_device_id(config, &id) != 0)
    {
        return -1;
    }

    uint16_t ref_id = 0;

    if (config.id == SL_TTC2_RADIO_0)
    {
        ref_id = SL_TTC2_DEVICE_ID_RADIO_0;
    }
    else if (config.id == SL_TTC2_RADIO_1)
    {
        ref_id = SL_TTC2_DEVICE_ID_RADIO_1;
    }
    else
    {
    #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
        sys_log_print_event_from_module(SYS_LOG_ERROR, SL_TTC2_MODULE_NAME, "Error checking the device! Invalid radio index!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        return -1;
    }

    if (id != ref_id)
    {
    #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
        sys_log_print_event_from_module(SYS_LOG_ERROR, SL_TTC2_MODULE_NAME, "Error checking the device! (read=");
        sys_log_print_hex(id);
        sys_log_print_msg(", expected=");
        sys_log_print_hex(ref_id);
        sys_log_print_msg(")");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        return -1;
    }

    return 0;
}

int sl_ttc2_write_reg(sl_ttc2_config_t config, uint8_t adr, uint32_t val)
{
    uint8_t buf[8] = {0};

    /* Command ID */
    buf[0] = SL_TTC2_CMD_WRITE_REG;

    /* Register address */
    buf[1] = adr;

    /* Register data */
    buf[2] = (val >> 24) & 0xFF;
    buf[3] = (val >> 16) & 0xFF;
    buf[4] = (val >> 8)  & 0xFF;
    buf[5] = (val >> 0)  & 0xFF;

    /* Checksum (CRC16) */
    uint16_t crc = sl_ttc2_crc16(buf, 5);
    buf[6] = (crc >> 8) & 0xFF;
    buf[7] = (crc >> 0) & 0xFF;

    return spi_write(config.port, config.cs_pin, buf, 8);
}

int sl_ttc2_read_reg(sl_ttc2_config_t config, uint8_t adr, uint32_t *val)
{
    uint8_t wbuf[8] = {0};
    uint8_t rbuf[8] = {0};

    /* Command ID */
    wbuf[0] = SL_TTC2_CMD_READ_REG;
    rbuf[0] = SL_TTC2_CMD_READ_REG;

    /* Register address */
    wbuf[1] = adr;
    rbuf[1] = adr;

    /* Register data + Checksum */
    if (spi_transfer(config.port, config.cs_pin, wbuf, rbuf, 8) != 0)
    {
    #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
        sys_log_print_event_from_module(SYS_LOG_ERROR, SL_TTC2_MODULE_NAME, "Error reading the register ");
        sys_log_print_hex(adr);
        sys_log_print_msg("! Error during SPI transfer!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        return -1;
    }

    if (!sl_ttc2_check_crc(rbuf, 6, ((uint16_t)rbuf[6] << 8) | (uint16_t)rbuf[7]))
    {
    #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
        sys_log_print_event_from_module(SYS_LOG_ERROR, SL_TTC2_MODULE_NAME, "Error reading the register ");
        sys_log_print_hex(adr);
        sys_log_print_msg("! Invalid data!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        return -1;
    }

    *val = ((uint32_t)rbuf[2] << 24) |
           ((uint32_t)rbuf[3] << 16) |
           ((uint32_t)rbuf[4] << 8)  |
           ((uint32_t)rbuf[5] << 0);

    return 0;
}

int sl_ttc2_read_hk_data(sl_ttc2_config_t config, sl_ttc2_hk_data_t *data)
{
    /* Time counter */
    if (sl_ttc2_read_time_counter(config, &(data->time_counter)) != 0)
    {
        return -1;
    }

    /* Reset counter */
    if (sl_ttc2_read_reset_counter(config, &(data->reset_counter)) != 0)
    {
        return -1;
    }

    /* Last reset cause */
    if (sl_ttc2_read_reset_cause(config, &(data->last_reset_cause)) != 0)
    {
        return -1;
    }

    sl_ttc2_delay_ms(5);

    /* MCU power */
    if (sl_ttc2_read_voltage(config, SL_TTC2_VOLTAGE_MCU, &(data->voltage_mcu)) != 0)
    {
        return -1;
    }

    if (sl_ttc2_read_current(config, SL_TTC2_CURRENT_MCU, &(data->current_mcu)) != 0)
    {
        return -1;
    }

    /* MCU temperature */
    if (sl_ttc2_read_temp(config, SL_TTC2_TEMP_MCU, &(data->temperature_mcu)) != 0)
    {
        return -1;
    }

    sl_ttc2_delay_ms(5);

    /* Radio power */
    if (sl_ttc2_read_voltage(config, SL_TTC2_VOLTAGE_RADIO, &(data->voltage_radio)) != 0)
    {
        return -1;
    }

    if (sl_ttc2_read_current(config, SL_TTC2_CURRENT_RADIO, &(data->current_radio)) != 0)
    {
        return -1;
    }

    /* Radio temperature */
    if (sl_ttc2_read_temp(config, SL_TTC2_TEMP_RADIO, &(data->temperature_radio)) != 0)
    {
        return -1;
    }

    sl_ttc2_delay_ms(5);

    /* Last valid telecommand */
    if (sl_ttc2_read_last_valid_tc(config, &(data->last_valid_tc)) != 0)
    {
        return -1;
    }

    /* RSSI */
    if (sl_ttc2_read_rssi(config, &(data->rssi_last_valid_tc)) != 0)
    {
        return -1;
    }

    /* Antenna data */
    if (sl_ttc2_read_temp(config, SL_TTC2_TEMP_ANTENNA, &(data->temperature_antenna)) != 0)
    {
        return -1;
    }

    sl_ttc2_delay_ms(5);

    if (sl_ttc2_read_antenna_status(config, &(data->antenna_status)) != 0)
    {
        return -1;
    }

    if (sl_ttc2_read_antenna_deployment_status(config, &(data->deployment_status)) != 0)
    {
        return -1;
    }

    if (sl_ttc2_read_antenna_deployment_hibernation_status(config, &(data->hibernation_status)) != 0)
    {
        return -1;
    }

    sl_ttc2_delay_ms(5);

    /* Packet counter */
    if (sl_ttc2_read_pkt_counter(config, SL_TTC2_TX_PKT, &(data->tx_packet_counter)) != 0)
    {
        return -1;
    }

    if (sl_ttc2_read_pkt_counter(config, SL_TTC2_RX_PKT, &(data->rx_packet_counter)) != 0)
    {
        return -1;
    }

    return 0;
}

int sl_ttc2_read_device_id(sl_ttc2_config_t config, uint16_t *val)
{
    return sl_ttc2_read_reg(config, SL_TTC2_REG_DEVICE_ID, (uint32_t*)val);
}

int sl_ttc2_read_hardware_version(sl_ttc2_config_t config, uint8_t *val)
{
    return sl_ttc2_read_reg(config, SL_TTC2_REG_HARDWARE_VERSION, (uint32_t*)val);
}

int sl_ttc2_read_firmware_version(sl_ttc2_config_t config, uint32_t *val)
{
    return sl_ttc2_read_reg(config, SL_TTC2_REG_FIRMWARE_VERSION, val);
}

int sl_ttc2_read_time_counter(sl_ttc2_config_t config, uint32_t *val)
{
    return sl_ttc2_read_reg(config, SL_TTC2_REG_TIME_COUNTER, val);
}

int sl_ttc2_read_reset_counter(sl_ttc2_config_t config, uint16_t *val)
{
    return sl_ttc2_read_reg(config, SL_TTC2_REG_RESET_COUNTER, (uint32_t*)val);
}

int sl_ttc2_read_reset_cause(sl_ttc2_config_t config, uint8_t *val)
{
    return sl_ttc2_read_reg(config, SL_TTC2_REG_LAST_RESET_CAUSE, (uint32_t*)val);
}

int sl_ttc2_read_voltage(sl_ttc2_config_t config, uint8_t volt, sl_ttc2_voltage_t *val)
{
    switch(volt)
    {
        case SL_TTC2_VOLTAGE_MCU:       return sl_ttc2_read_reg(config, SL_TTC2_REG_INPUT_VOLTAGE_MCU, (uint32_t*)val);
        case SL_TTC2_VOLTAGE_RADIO:     return sl_ttc2_read_reg(config, SL_TTC2_REG_INPUT_VOLTAGE_RADIO, (uint32_t*)val);
        default:
        #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
            sys_log_print_event_from_module(SYS_LOG_ERROR, SL_TTC2_MODULE_NAME, "Error reading the voltage! Invalid voltage type!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            return -1;
    }
}

int sl_ttc2_read_current(sl_ttc2_config_t config, uint8_t cur, sl_ttc2_current_t *val)
{
    switch(cur)
    {
        case SL_TTC2_CURRENT_MCU:       return sl_ttc2_read_reg(config, SL_TTC2_CURRENT_MCU, (uint32_t*)val);
        case SL_TTC2_CURRENT_RADIO:     return sl_ttc2_read_reg(config, SL_TTC2_CURRENT_RADIO, (uint32_t*)val);
        default:
        #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
            sys_log_print_event_from_module(SYS_LOG_ERROR, SL_TTC2_MODULE_NAME, "Error reading the current! Invalid current type!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            return -1;
    }
}

int sl_ttc2_read_temp(sl_ttc2_config_t config, uint8_t temp, sl_ttc2_temp_t *val)
{
    switch(temp)
    {
        case SL_TTC2_TEMP_MCU:          return sl_ttc2_read_reg(config, SL_TTC2_REG_TEMPERATURE_MCU, (uint32_t*)val);
        case SL_TTC2_TEMP_RADIO:        return sl_ttc2_read_reg(config, SL_TTC2_REG_TEMPERATURE_RADIO, (uint32_t*)val);
        case SL_TTC2_TEMP_ANTENNA:      return sl_ttc2_read_reg(config, SL_TTC2_REG_TEMPERATURE_ANTENNA, (uint32_t*)val);
        default:
        #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
            sys_log_print_event_from_module(SYS_LOG_ERROR, SL_TTC2_MODULE_NAME, "Error reading the temperature! Invalid temperature type!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            return -1;
    }
}

int sl_ttc2_read_last_valid_tc(sl_ttc2_config_t config, uint8_t *val)
{
    return sl_ttc2_read_reg(config, SL_TTC2_REG_LAST_VALID_TC, (uint32_t*)val);
}

int sl_ttc2_read_rssi(sl_ttc2_config_t config, sl_ttc2_rssi_t *val)
{
    return sl_ttc2_read_reg(config, SL_TTC2_REG_RSSI_LAST_VALID_TC, (uint32_t*)val);
}

int sl_ttc2_read_antenna_status(sl_ttc2_config_t config, uint16_t *val)
{
    return sl_ttc2_read_reg(config, SL_TTC2_REG_ANTENNA_STATUS, (uint32_t*)val);
}

int sl_ttc2_read_antenna_deployment_status(sl_ttc2_config_t config, uint8_t *val)
{
    return sl_ttc2_read_reg(config, SL_TTC2_REG_ANTENNA_DEPLOYMENT_STATUS, (uint32_t*)val);
}

int sl_ttc2_read_antenna_deployment_hibernation_status(sl_ttc2_config_t config, uint8_t *val)
{
    return sl_ttc2_read_reg(config, SL_TTC2_REG_ANTENNA_DEP_HIB_STATUS, (uint32_t*)val);
}

int sl_ttc2_read_tx_enable(sl_ttc2_config_t config, uint8_t *val)
{
    return sl_ttc2_read_reg(config, SL_TTC2_REG_TX_ENABLE, (uint32_t*)val);
}

int sl_ttc2_set_tx_enable(sl_ttc2_config_t config, bool en)
{
    return sl_ttc2_write_reg(config, SL_TTC2_REG_TX_ENABLE, (uint32_t)en);
}

int sl_ttc2_read_pkt_counter(sl_ttc2_config_t config, uint8_t pkt, uint32_t *val)
{
    switch(pkt)
    {
        case SL_TTC2_TX_PKT:    return sl_ttc2_read_reg(config, SL_TTC2_REG_TX_PACKET_COUNTER, val);
        case SL_TTC2_RX_PKT:    return sl_ttc2_read_reg(config, SL_TTC2_REG_RX_PACKET_COUNTER, val);
        default:
        #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
            sys_log_print_event_from_module(SYS_LOG_ERROR, SL_TTC2_MODULE_NAME, "Error reading the packet counter! Invalid packet type!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            return -1;
    }
}

int sl_ttc2_read_fifo_pkts(sl_ttc2_config_t config, uint8_t pkt, uint8_t *val)
{
    switch(pkt)
    {
        case SL_TTC2_TX_PKT:    return sl_ttc2_read_reg(config, SL_TTC2_REG_FIFO_TX_PACKET, (uint32_t*)val);
        case SL_TTC2_RX_PKT:    return sl_ttc2_read_reg(config, SL_TTC2_REG_FIFO_RX_PACKET, (uint32_t*)val);
        default:
        #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
            sys_log_print_event_from_module(SYS_LOG_ERROR, SL_TTC2_MODULE_NAME, "Error reading the FIFO buffer! Invalid packet type!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            return -1;
    }
}

int sl_ttc2_read_len_rx_pkt_in_fifo(sl_ttc2_config_t config, uint16_t *val)
{
    return sl_ttc2_read_reg(config, SL_TTC2_REG_LEN_FIRST_RX_PACKET_IN_FIFO, (uint32_t*)val);
}

int sl_ttc2_check_pkt_avail(sl_ttc2_config_t config)
{
    uint8_t pkts = 0;

    if (sl_ttc2_read_fifo_pkts(config, SL_TTC2_REG_FIFO_RX_PACKET, &pkts) != 0)
    {
    #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
        sys_log_print_event_from_module(SYS_LOG_ERROR, SL_TTC2_MODULE_NAME, "Error reading the available RX packets!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        return -1;
    }

    return pkts;
}

int sl_ttc2_transmit_packet(sl_ttc2_config_t config, uint8_t *data, uint16_t len)
{
    uint8_t buf[1+220+2] = {0};

    buf[0] = SL_TTC2_CMD_TRANSMIT_PKT;

    memcpy(buf+1, data, len);

    uint16_t crc = sl_ttc2_crc16(buf, 1+len);

    buf[1+len] = (crc >> 8) & 0xFF;
    buf[1+len] = (crc >> 0) & 0xFF;

    return spi_write(config.port, config.cs_pin, buf, 1+len+2);
}

int sl_ttc2_read_packet(sl_ttc2_config_t config, uint8_t *data, uint16_t *len)
{
    uint8_t wbuf[2] = {0};

    wbuf[0] = SL_TTC2_CMD_RECEIVE_PKT;
    data[0] = SL_TTC2_CMD_RECEIVE_PKT;

    if (sl_ttc2_read_len_rx_pkt_in_fifo(config, len) != 0)
    {
        return -1;
    }

    if (spi_transfer(config.port, config.cs_pin, wbuf, data, 1+(*len)+2) != 0)
    {
        return -1;
    }

    if (!sl_ttc2_check_crc(data, 1+(*len), sl_ttc2_crc16(data, 1+(*len))))
    {
        *len = 0;
        return -1;
    }

    memcpy(data, data+1, *len);

    return 0;
}

uint16_t sl_ttc2_crc16(uint8_t *data, uint16_t len)
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

bool sl_ttc2_check_crc(uint8_t *data, uint16_t len, uint16_t crc)
{
    if (crc != sl_ttc2_crc16(data, len))
    {
        return false;
    }

    return true;
}

/** \} End of sl_ttc2 group */
