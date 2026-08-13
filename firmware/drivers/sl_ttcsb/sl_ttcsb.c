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
 * along with OBDH 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief SpaceLab TTC SBAND driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.8.13
 * 
 * \date 2021/05/12
 * 
 * \addtogroup sl_ttcsb
 * \{
 */

#include <string.h>

#include <config/config.h>
#include <system/sys_log/sys_log.h>

#include "sl_ttcsb.h"

#define SL_TTCSB_CRC8_INITIAL_VALUE          0U      /**< CRC8-CCITT initial value. USING SAME AS EPS */ 
#define SL_TTCSB_CRC8_POLYNOMIAL             0x07U   /**< CRC8-CCITT polynomial. USING SAME AS EPS */

/**
 * \brief Computes the CRC-8 of a sequence of bytes.
 *
 * \param[in] data is an array of data to compute the CRC-8.
 *
 * \param[in] len is the number of bytes of the given array.
 *
 * \return The computed CRC-8 value of the given data.
 */
static uint8_t sl_ttcsb_crc8(uint8_t *data, uint8_t len);

/**
 * \brief Checks the CRC value of a given sequence of bytes.
 *
 * \param[in] data is the data to check the CRC.
 *
 * \param[in] len is the number of bytes to check the CRC value.
 *
 * \param[in] crc is the CRC-8 value to check.
 *
 * \return TRUE/FALSE if the given CRC value is correct or not.
 */
static bool sl_ttcsb_check_crc(uint8_t *data, uint8_t len, uint8_t crc);

int sl_ttcsb_init(sl_ttcsb_config_t config)
{
    int err = -1;

    if (sl_ttcsb_i2c_init(config) == 0)
    {
        sl_ttcsb_delay_ms(10);

        if (sl_ttcsb_check_device(config) == 0)
        {
            err = 0;
        }
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, SL_TTC2_MODULE_NAME, "Error initializing the I2C port!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return err;
}

int sl_ttcsb_check_device(sl_ttcsb_config_t config)
{
    int err = 0;

    uint16_t id = UINT16_MAX;

    if (sl_ttcsb_read_device_id(config, &id) == 0) 
    {
        uint16_t ref_id = 0;
        if (config.id == SL_TTCSB_RADIO_0)
        {
            ref_id = SL_TTCSB_DEVICE_ID_RADIO_0;
        }
        else if (config.id == SL_TTCSB_RADIO_1)
        {
            ref_id = SL_TTCSB_DEVICE_ID_RADIO_1;
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, SL_TTC2_MODULE_NAME, "Error checking the device! Invalid radio index!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;
        }

        if (err == 0)
        {
            if (id != ref_id)
            {
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_ERROR, SL_TTC2_MODULE_NAME, "Error checking the device! (read=");
                sys_log_print_hex(id);
                sys_log_print_msg(", expected=");
                sys_log_print_hex(ref_id);
                sys_log_print_msg(")");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                err = -1;
            }
        }
    }

    return err;
}

int sl_ttcsb_write_reg(sl_ttcsb_config_t config, uint8_t adr, uint32_t val)
{
    int err = 0;

    uint8_t buf[1 + 4 + 1] = {0};

    buf[0] = adr;
    buf[1] = (val >> 24) & 0xFFU;
    buf[2] = (val >> 16) & 0xFFU;
    buf[3] = (val >> 8)  & 0xFFU;
    buf[4] = (val >> 0)  & 0xFFU;
    buf[5] = sl_ttcsb_crc8(buf, 5);

    if (sl_ttcsb_i2c_write(config, buf, 6U) != I2C_READY)
    {
        err = -1;
    }

    return err;
}

int sl_ttcsb_read_reg(sl_ttcsb_config_t config, uint8_t adr, uint32_t *val)
{
   
    int err = 0;

    uint8_t buf[1 + 4 + 1] = {0};

    buf[0] = adr;
    buf[1] = sl_ttcsb_crc8(buf, 1);

    if (sl_ttcsb_i2c_write(config, buf, 2U) != I2C_READY)
    {
        err = -1;
    }

    sl_ttcsb_delay_ms(50);

    if (sl_ttcsb_i2c_read(config, buf, 6U) != I2C_READY)
    {
        err = -1;
    }

    if (!sl_ttcsb_check_crc(buf, 5U, buf[5]))
    {
        err = -1;
    }

    *val = ((uint32_t)buf[1] << 24) |
           ((uint32_t)buf[2] << 16) |
           ((uint32_t)buf[3] << 8)  |
           ((uint32_t)buf[4] << 0);

    return err;
}

int sl_ttcsb_read_hk_data(sl_ttcsb_config_t config, sl_ttcsb_hk_data_t *data, uint64_t *err_id)
{
    int err_counter = 0;

    /* Time counter */
    if (sl_ttcsb_read_time_counter(config, &(data->time_counter)) != 0)
    {
        err_counter++;
        *err_id |= (1ULL << 0);
    }

    /* Reset counter */
    if (sl_ttcsb_read_reset_counter(config, &(data->reset_counter)) != 0)
    {
        err_counter++;
        *err_id |= (1ULL << 1);
    }

    /* Last reset cause */
    if (sl_ttcsb_read_reset_cause(config, &(data->last_reset_cause)) != 0)
    {
        err_counter++;
        *err_id |= (1ULL << 2);
    }

    sl_ttc2_delay_ms(5);

    /* MCU power */
    if (sl_ttcsb_read_voltage(config, SL_TTCSB_VOLTAGE_MCU, &(data->voltage_mcu)) != 0)
    {
        err_counter++;
        *err_id |= (1ULL << 3);
    }

    if (sl_ttcsb_read_current(config, SL_TTCSB_CURRENT_MCU, &(data->current_mcu)) != 0)
    {
        err_counter++;
        *err_id |= (1ULL << 4);
    }

    /* MCU temperature */
    if (sl_ttcsb_read_temp(config, SL_TTCSB_TEMP_MCU, &(data->temperature_mcu)) != 0)
    {
        err_counter++;
        *err_id |= (1ULL << 5);
    }

    sl_ttc2_delay_ms(5);

    /* Radio power */
    if (sl_ttcsb_read_voltage(config, SL_TTCSB_VOLTAGE_RADIO, &(data->voltage_radio)) != 0)
    {
        err_counter++;
        *err_id |= (1ULL << 6);
    }

    if (sl_ttcsb_read_current(config, SL_TTCSB_CURRENT_RADIO, &(data->current_radio)) != 0)
    {
        err_counter++;
        *err_id |= (1ULL << 7);
    }

    /* Radio temperature */
    if (sl_ttcsb_read_temp(config, SL_TTCSB_TEMP_RADIO, &(data->temperature_radio)) != 0)
    {
        err_counter++;
        *err_id |= (1ULL << 8);
    }

    sl_ttc2_delay_ms(5);

    /* Last valid telecommand */
    if (sl_ttcsb_read_last_valid_tc(config, &(data->last_valid_tc)) != 0)
    {
        err_counter++;
        *err_id |= (1ULL << 9);
    }

    /* RSSI */
    if (sl_ttcsb_read_rssi(config, &(data->rssi_last_valid_tc)) != 0)
    {
        err_counter++;
        *err_id |= (1ULL << 10);
    }

    /* Antenna data */
    if (sl_ttcsb_read_temp(config, SL_TTCSB_TEMP_ANTENNA, &(data->temperature_antenna)) != 0)
    {
        err_counter++;
        *err_id |= (1ULL << 11);
    }

    sl_ttc2_delay_ms(5);

    if (sl_ttcsb_read_antenna_status(config, &(data->antenna_status)) != 0)
    {
        err_counter++;
        *err_id |= (1ULL << 12);
    }

    if (sl_ttcsb_read_antenna_deployment_status(config, &(data->deployment_status)) != 0)
    {
        err_counter++;
        *err_id |= (1ULL << 13);
    }

    if (sl_ttcsb_read_antenna_deployment_hibernation_status(config, &(data->hibernation_status)) != 0)
    {
        err_counter++;
        *err_id |= (1ULL << 14);
    }

    return err_counter;
}

int sl_ttcsb_read_device_id(sl_ttcsb_config_t config, uint16_t *val)
{
    uint32_t buf = UINT32_MAX;

    int res = sl_ttcsb_read_reg(config, SL_TTCSB_REG_DEVICE_ID, &buf);

    *val = (uint16_t)buf;

    return res;
}

int sl_ttcsb_read_hardware_version(sl_ttcsb_config_t config, uint8_t *val)
{
    uint32_t buf = UINT32_MAX;

    int res = sl_ttcsb_read_reg(config, SL_TTCSB_REG_HARDWARE_VERSION, &buf);

    *val = (uint8_t)buf;

    return res;
}

int sl_ttcsb_read_firmware_version(sl_ttcsb_config_t config, uint32_t *val)
{
    return sl_ttcsb_read_reg(config, SL_TTCSB_REG_FIRMWARE_VERSION, val);
}

int sl_ttcsb_read_time_counter(sl_ttcsb_config_t config, uint32_t *val)
{
    return sl_ttcsb_read_reg(config, SL_TTCSB_REG_TIME_COUNTER, val);
}

int sl_ttcsb_read_reset_counter(sl_ttcsb_config_t config, uint16_t *val)
{
    uint32_t buf = UINT32_MAX;

    int res = sl_ttcsb_read_reg(config, SL_TTCSB_REG_RESET_COUNTER, &buf);

    *val = (uint16_t)buf;

    return res;
}

int sl_ttcsb_read_reset_cause(sl_ttcsb_config_t config, uint8_t *val)
{
    uint32_t buf = UINT32_MAX;

    int res = sl_ttcsb_read_reg(config, SL_TTCSB_REG_LAST_RESET_CAUSE, &buf);

    *val = (uint8_t)buf;

    return res;
}

int sl_ttcsb_read_voltage(sl_ttcsb_config_t config, uint8_t volt, sl_ttcsb_voltage_t *val)
{
    int res = -1;

    uint32_t buf = UINT32_MAX;

    switch(volt)
    {
        case SL_TTCSB_VOLTAGE_MCU:
            res = sl_ttcsb_read_reg(config, SL_TTCSB_REG_INPUT_VOLTAGE_MCU, &buf);

            *val = (sl_ttcsb_voltage_t)buf;

            break;
        case SL_TTCSB_VOLTAGE_RADIO:
            res = sl_ttcsb_read_reg(config, SL_TTCSB_REG_INPUT_VOLTAGE_RADIO, &buf);

            *val = (sl_ttcsb_voltage_t)buf;

            break;
        default:
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, SL_TTC2_MODULE_NAME, "Error reading the voltage! Invalid voltage type!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            break;
    }

    return res;
}

int sl_ttcsb_read_current(sl_ttcsb_config_t config, uint8_t cur, sl_ttcsb_current_t *val)
{
    int res = -1;

    uint32_t buf = UINT32_MAX;

    switch(cur)
    {
        case SL_TTCSB_CURRENT_MCU:
            res = sl_ttcsb_read_reg(config, SL_TTCSB_REG_INPUT_CURRENT_MCU, &buf);

            *val = (sl_ttcsb_current_t)buf;

            break;
        case SL_TTCSB_CURRENT_RADIO:
            res = sl_ttcsb_read_reg(config, SL_TTCSB_REG_INPUT_CURRENT_RADIO, &buf);

            *val = (sl_ttcsb_current_t)buf;

            break;
        default:
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, SL_TTC2_MODULE_NAME, "Error reading the current! Invalid current type!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            break;
    }

    return res;
}

int sl_ttcsb_read_temp(sl_ttcsb_config_t config, uint8_t temp, sl_ttcsb_temp_t *val)
{
    int res = -1;

    uint32_t buf = UINT32_MAX;

    switch(temp)
    {
        case SL_TTCSB_TEMP_MCU:
            res = sl_ttcsb_read_reg(config, SL_TTCSB_REG_TEMPERATURE_MCU, &buf);

            *val = (sl_ttcsb_temp_t)buf;

            break;
        case SL_TTCSB_TEMP_RADIO:
            res = sl_ttcsb_read_reg(config, SL_TTCSB_REG_TEMPERATURE_RADIO, &buf);

            *val = (sl_ttcsb_temp_t)buf;

            break;
        case SL_TTCSB_TEMP_ANTENNA:
            res = sl_ttcsb_read_reg(config, SL_TTCSB_REG_TEMPERATURE_ANTENNA, &buf);

            *val = (sl_ttcsb_temp_t)buf;

            break;
        default:
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, SL_TTC2_MODULE_NAME, "Error reading the temperature! Invalid temperature type!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            break;
    }

    return res;
}

int sl_ttcsb_read_last_valid_tc(sl_ttcsb_config_t config, uint8_t *val)
{
    uint32_t buf = UINT32_MAX;

    int res = sl_ttcsb_read_reg(config, SL_TTCSB_REG_LAST_VALID_TC, &buf);

    *val = (uint8_t)buf;

    return res;
}

int sl_ttcsb_read_rssi(sl_ttcsb_config_t config, sl_ttcsb_rssi_t *val)
{
    uint32_t buf = UINT32_MAX;

    int res = sl_ttcsb_read_reg(config, SL_TTCSB_REG_RSSI_LAST_VALID_TC, &buf);

    *val = (sl_ttcsb_rssi_t)buf;

    return res;
}

int sl_ttcsb_read_antenna_status(sl_ttcsb_config_t config, uint16_t *val)
{
    uint32_t buf = UINT32_MAX;

    int res = sl_ttcsb_read_reg(config, SL_TTCSB_REG_ANTENNA_STATUS, &buf);

    *val = (uint16_t)buf;

    return res;
}

int sl_ttcsb_read_antenna_deployment_status(sl_ttcsb_config_t config, uint8_t *val)
{
    uint32_t buf = UINT32_MAX;

    int res = sl_ttcsb_read_reg(config, SL_TTCSB_REG_ANTENNA_DEPLOYMENT_STATUS, &buf);

    *val = (uint8_t)buf;

    return res;
}

int sl_ttcsb_read_antenna_deployment_hibernation_status(sl_ttcsb_config_t config, uint8_t *val)
{
    uint32_t buf = UINT32_MAX;

    int res = sl_ttcsb_read_reg(config, SL_TTCSB_REG_ANTENNA_DEP_HIB_STATUS, &buf);

    *val = (uint8_t)buf;

    return res;
}

int sl_ttcsb_read_tx_enable(sl_ttcsb_config_t config, uint8_t *val)
{
    uint32_t buf = UINT32_MAX;

    int res = sl_ttcsb_read_reg(config, SL_TTCSB_REG_TX_ENABLE, &buf);

    *val = (uint8_t)buf;

    return res;
}

int sl_ttcsb_set_tx_enable(sl_ttcsb_config_t config, bool en)
{
    return sl_ttcsb_write_reg(config, SL_TTCSB_REG_TX_ENABLE, (en? 1UL : 0UL));
}

int sl_ttcsb_read_pkt_counter(sl_ttcsb_config_t config, uint8_t pkt, uint32_t *val)
{
    int err = -1;

    switch(pkt)
    {
        case SL_TTCSB_TX_PKT:    err = sl_ttcsb_read_reg(config, SL_TTCSB_REG_TX_PACKET_COUNTER, val);     break;
        case SL_TTCSB_RX_PKT:    err = sl_ttcsb_read_reg(config, SL_TTCSB_REG_RX_PACKET_COUNTER, val);     break;
        default:
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, SL_TTC2_MODULE_NAME, "Error reading the packet counter! Invalid packet type!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            break;
    }

    return err;
}

static uint8_t sl_ttcsb_crc8(uint8_t *data, uint8_t len)
{
    uint8_t crc = SL_TTCSB_CRC8_INITIAL_VALUE;

    uint8_t i = 0U;
    for(i = 0; i < len; i++)
    {
        crc ^= data[i];

        uint8_t j = 0U;
        for (j = 0U; j < 8U; j++)
        {
            crc = (crc << 1) ^ ((crc & 0x80U) ? SL_TTCSB_CRC8_POLYNOMIAL : 0U);
        }

        crc &= 0xFFU;
    }

    return crc;
}

static bool sl_ttcsb_check_crc(uint8_t *data, uint8_t len, uint8_t crc)
{
    return (crc == sl_ttcsb_crc8(data, len));
}


/** \} End of sl_ttcsb group */
