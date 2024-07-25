/*
 * sl_ttc2.c
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
 * along with OBDH 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief SpaceLab TTC 2.0 driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 0.10.17
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


/* TTC 2.0 registers lenght in bytes */
#define SL_TTC2_REG_DEVICE_ID_NUM_BYTES                     2
#define SL_TTC2_REG_HARDWARE_VERSION_NUM_BYTES              1
#define SL_TTC2_REG_FIRMWARE_VERSION_NUM_BYTES              4
#define SL_TTC2_REG_TIME_COUNTER_NUM_BYTES                  4
#define SL_TTC2_REG_RESET_COUNTER_NUM_BYTES                 2
#define SL_TTC2_REG_LAST_RESET_CAUSE_NUM_BYTES              1
#define SL_TTC2_REG_INPUT_VOLTAGE_MCU_NUM_BYTES             2
#define SL_TTC2_REG_INPUT_CURRENT_MCU_NUM_BYTES             2
#define SL_TTC2_REG_TEMPERATURE_MCU_NUM_BYTES               2
#define SL_TTC2_REG_INPUT_VOLTAGE_RADIO_NUM_BYTES           2
#define SL_TTC2_REG_INPUT_CURRENT_RADIO_NUM_BYTES           2
#define SL_TTC2_REG_TEMPERATURE_RADIO_NUM_BYTES             2
#define SL_TTC2_REG_LAST_VALID_TC_NUM_BYTES                 1
#define SL_TTC2_REG_RSSI_LAST_VALID_TC_NUM_BYTES            2
#define SL_TTC2_REG_TEMPERATURE_ANTENNA_NUM_BYTES           2
#define SL_TTC2_REG_ANTENNA_STATUS_NUM_BYTES                2
#define SL_TTC2_REG_ANTENNA_DEPLOYMENT_STATUS_NUM_BYTES     1
#define SL_TTC2_REG_ANTENNA_DEP_HIB_STATUS_NUM_BYTES        1
#define SL_TTC2_REG_TX_ENABLE_NUM_BYTES                     1
#define SL_TTC2_REG_TX_PACKET_COUNTER_NUM_BYTES             4
#define SL_TTC2_REG_RX_PACKET_COUNTER_NUM_BYTES             4
#define SL_TTC2_REG_FIFO_TX_PACKET_NUM_BYTES                1
#define SL_TTC2_REG_FIFO_RX_PACKET_NUM_BYTES                1
#define SL_TTC2_REG_LEN_FIRST_RX_PACKET_IN_FIFO_NUM_BYTES   2

int sl_ttc2_init(sl_ttc2_config_t config)
{
    int err = -1;

    if (sl_ttc2_spi_init(config) == 0)
    {
        sl_ttc2_delay_ms(10);

        if (sl_ttc2_check_device(config) == 0)
        {
            err = 0;
        }
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, SL_TTC2_MODULE_NAME, "Error initializing the SPI port!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return err;
}

int sl_ttc2_check_device(sl_ttc2_config_t config)
{
    int err = 0;

    uint16_t id = UINT16_MAX;

    if (sl_ttc2_read_device_id(config, &id) == 0)
    {
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

int sl_ttc2_write_reg(sl_ttc2_config_t config, uint8_t adr, uint32_t val)
{
    int err = -1;

    uint8_t buf[8] = {0};

    /* Adding preamble byte */
    buf[0] = SL_TTC2_PKT_PREAMBLE;

    /* Command ID */
    buf[1] = SL_TTC2_CMD_WRITE_REG;

    /* Register address */
    buf[2] = adr;

    /* Register data */
    switch(adr)
    {
        case SL_TTC2_REG_DEVICE_ID:
            buf[3] = (val >> 8) & 0xFFU;
            buf[4] = (val >> 0) & 0xFFU;
            break;
        case SL_TTC2_REG_HARDWARE_VERSION:
            buf[3] = val & 0xFFU;
            break;
        case SL_TTC2_REG_RESET_COUNTER:
            buf[3] = (val >> 8) & 0xFFU;
            buf[4] = (val >> 0) & 0xFFU;
            break;
        case SL_TTC2_REG_LAST_RESET_CAUSE:
            buf[3] = val & 0xFFU;
            break;
        case SL_TTC2_REG_INPUT_VOLTAGE_MCU:
            buf[3] = (val >> 8) & 0xFFU;
            buf[4] = (val >> 0) & 0xFFU;
            break;
        case SL_TTC2_REG_INPUT_CURRENT_MCU:
            buf[3] = (val >> 8) & 0xFFU;
            buf[4] = (val >> 0) & 0xFFU;
            break;
        case SL_TTC2_REG_TEMPERATURE_MCU:
            buf[3] = (val >> 8) & 0xFFU;
            buf[4] = (val >> 0) & 0xFFU;
            break;
        case SL_TTC2_REG_INPUT_VOLTAGE_RADIO:
            buf[3] = (val >> 8) & 0xFFU;
            buf[4] = (val >> 0) & 0xFFU;
            break;
        case SL_TTC2_REG_INPUT_CURRENT_RADIO:
            buf[3] = (val >> 8) & 0xFFU;
            buf[4] = (val >> 0) & 0xFFU;
            break;
        case SL_TTC2_REG_TEMPERATURE_RADIO:
            buf[3] = (val >> 8) & 0xFFU;
            buf[4] = (val >> 0) & 0xFFU;
            break;
        case SL_TTC2_REG_LAST_VALID_TC:
            buf[3] = val & 0xFFU;
            break;
        case SL_TTC2_REG_RSSI_LAST_VALID_TC:
            buf[3] = (val >> 8) & 0xFFU;
            buf[4] = (val >> 0) & 0xFFU;
            break;
        case SL_TTC2_REG_TEMPERATURE_ANTENNA:
            buf[3] = (val >> 8) & 0xFFU;
            buf[4] = (val >> 0) & 0xFFU;
            break;
        case SL_TTC2_REG_ANTENNA_STATUS:
            buf[3] = (val >> 8) & 0xFFU;
            buf[4] = (val >> 0) & 0xFFU;
            break;
        case SL_TTC2_REG_ANTENNA_DEPLOYMENT_STATUS:
            buf[3] = val & 0xFFU;
            break;
        case SL_TTC2_REG_ANTENNA_DEP_HIB_STATUS:
            buf[3] = val & 0xFFU;
            break;
        case SL_TTC2_REG_TX_ENABLE:
            buf[3] = val & 0xFFU;
            break;
        case SL_TTC2_REG_FIFO_TX_PACKET:
            buf[3] = val & 0xFFU;
            break;
        case SL_TTC2_REG_FIFO_RX_PACKET:
            buf[3] = val & 0xFFU;
            break;
        case SL_TTC2_REG_LEN_FIRST_RX_PACKET_IN_FIFO:
            buf[3] = (val >> 8) & 0xFFU;
            buf[4] = (val >> 0) & 0xFFU;
            break;
        default:
            buf[3] = (val >> 24) & 0xFFU;
            buf[4] = (val >> 16) & 0xFFU;
            buf[5] = (val >> 8)  & 0xFFU;
            buf[6] = (val >> 0)  & 0xFFU;
            break;
    }

    if (sl_ttc2_mutex_take() == 0)
    {
        err = sl_ttc2_spi_write(config, buf, 7U);

        sl_ttc2_delay_ms(110);

        (void)sl_ttc2_mutex_give();
    }
    else 
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, SL_TTC2_MODULE_NAME, "Failed to take the mutex in time!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return err;
}

int sl_ttc2_read_reg(sl_ttc2_config_t config, uint8_t adr, uint32_t *val)
{
    int err = -1;

    uint8_t wbuf[7] = {0};
    uint8_t rbuf[7] = {0};

    /* Adding preamble byte */
    wbuf[0] = SL_TTC2_PKT_PREAMBLE;

    /* Command ID */
    wbuf[1] = SL_TTC2_CMD_READ_REG;

    /* Register address */
    wbuf[2] = adr;

    if (sl_ttc2_mutex_take() == 0)
    {
        /* Register data */
        if (sl_ttc2_spi_write(config, wbuf, 7U) == 0)
        {
            sl_ttc2_delay_ms(110);

            if (sl_ttc2_spi_read(config, rbuf, 7U) == 0)
            {
                if ((rbuf[0] == SL_TTC2_PKT_PREAMBLE) && (rbuf[1] == SL_TTC2_CMD_READ_REG) && (rbuf[2] == adr))
                {
                    uint32_t val_buf = ((uint32_t)rbuf[3] << 24) |
                                       ((uint32_t)rbuf[4] << 16) |
                                       ((uint32_t)rbuf[5] << 8)  |
                                       ((uint32_t)rbuf[6] << 0);

                    switch(adr)
                    {
                        case SL_TTC2_REG_DEVICE_ID:                     *val = val_buf >> 16;   break;
                        case SL_TTC2_REG_HARDWARE_VERSION:              *val = val_buf >> 24;   break;
                        case SL_TTC2_REG_RESET_COUNTER:                 *val = val_buf >> 16;   break;
                        case SL_TTC2_REG_LAST_RESET_CAUSE:              *val = val_buf >> 24;   break;
                        case SL_TTC2_REG_INPUT_VOLTAGE_MCU:             *val = val_buf >> 16;   break;
                        case SL_TTC2_REG_INPUT_CURRENT_MCU:             *val = val_buf >> 16;   break;
                        case SL_TTC2_REG_TEMPERATURE_MCU:               *val = val_buf >> 16;   break;
                        case SL_TTC2_REG_INPUT_VOLTAGE_RADIO:           *val = val_buf >> 16;   break;
                        case SL_TTC2_REG_INPUT_CURRENT_RADIO:           *val = val_buf >> 16;   break;
                        case SL_TTC2_REG_TEMPERATURE_RADIO:             *val = val_buf >> 16;   break;
                        case SL_TTC2_REG_LAST_VALID_TC:                 *val = val_buf >> 24;   break;
                        case SL_TTC2_REG_RSSI_LAST_VALID_TC:            *val = val_buf >> 16;   break;
                        case SL_TTC2_REG_TEMPERATURE_ANTENNA:           *val = val_buf >> 16;   break;
                        case SL_TTC2_REG_ANTENNA_STATUS:                *val = val_buf >> 16;   break;
                        case SL_TTC2_REG_ANTENNA_DEPLOYMENT_STATUS:     *val = val_buf >> 24;   break;
                        case SL_TTC2_REG_ANTENNA_DEP_HIB_STATUS:        *val = val_buf >> 24;   break;
                        case SL_TTC2_REG_TX_ENABLE:                     *val = val_buf >> 24;   break;
                        case SL_TTC2_REG_FIFO_TX_PACKET:                *val = val_buf >> 24;   break;
                        case SL_TTC2_REG_FIFO_RX_PACKET:                *val = val_buf >> 24;   break;
                        case SL_TTC2_REG_LEN_FIRST_RX_PACKET_IN_FIFO:   *val = val_buf >> 16;   break;
                        default:                                        *val = val_buf;         break;
                    }

                    err = 0;
                }
                else
                {
                #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                    sys_log_print_event_from_module(SYS_LOG_ERROR, SL_TTC2_MODULE_NAME, "Error reading the register ");
                    sys_log_print_hex(adr);
                    sys_log_print_msg("! Invalid response!");
                    sys_log_new_line();
                #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                }
            }
            else
            {
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_ERROR, SL_TTC2_MODULE_NAME, "Error reading the register ");
                sys_log_print_hex(adr);
                sys_log_print_msg("! Error during SPI reading!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            }
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, SL_TTC2_MODULE_NAME, "Error reading the register ");
            sys_log_print_hex(adr);
            sys_log_print_msg("! Error during SPI writing!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        }

        sl_ttc2_delay_ms(110);

        (void)sl_ttc2_mutex_give();
    }
    else 
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, SL_TTC2_MODULE_NAME, "Failed to take the mutex in time!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return err;
}

int sl_ttc2_read_hk_data(sl_ttc2_config_t config, sl_ttc2_hk_data_t *data)
{
    int err_counter = 0;

    /* Time counter */
    if (sl_ttc2_read_time_counter(config, &(data->time_counter)) != 0)
    {
        err_counter++;
    }

    /* Reset counter */
    if (sl_ttc2_read_reset_counter(config, &(data->reset_counter)) != 0)
    {
        err_counter++;
    }

    /* Last reset cause */
    if (sl_ttc2_read_reset_cause(config, &(data->last_reset_cause)) != 0)
    {
        err_counter++;
    }

    sl_ttc2_delay_ms(5);

    /* MCU power */
    if (sl_ttc2_read_voltage(config, SL_TTC2_VOLTAGE_MCU, &(data->voltage_mcu)) != 0)
    {
        err_counter++;
    }

    if (sl_ttc2_read_current(config, SL_TTC2_CURRENT_MCU, &(data->current_mcu)) != 0)
    {
        err_counter++;
    }

    /* MCU temperature */
    if (sl_ttc2_read_temp(config, SL_TTC2_TEMP_MCU, &(data->temperature_mcu)) != 0)
    {
        err_counter++;
    }

    sl_ttc2_delay_ms(5);

    /* Radio power */
    if (sl_ttc2_read_voltage(config, SL_TTC2_VOLTAGE_RADIO, &(data->voltage_radio)) != 0)
    {
        err_counter++;
    }

    if (sl_ttc2_read_current(config, SL_TTC2_CURRENT_RADIO, &(data->current_radio)) != 0)
    {
        err_counter++;
    }

    /* Radio temperature */
    if (sl_ttc2_read_temp(config, SL_TTC2_TEMP_RADIO, &(data->temperature_radio)) != 0)
    {
        err_counter++;
    }

    sl_ttc2_delay_ms(5);

    /* Last valid telecommand */
    if (sl_ttc2_read_last_valid_tc(config, &(data->last_valid_tc)) != 0)
    {
        err_counter++;
    }

    /* RSSI */
    if (sl_ttc2_read_rssi(config, &(data->rssi_last_valid_tc)) != 0)
    {
        err_counter++;
    }

    /* Antenna data */
    if (sl_ttc2_read_temp(config, SL_TTC2_TEMP_ANTENNA, &(data->temperature_antenna)) != 0)
    {
        err_counter++;
    }

    sl_ttc2_delay_ms(5);

    if (sl_ttc2_read_antenna_status(config, &(data->antenna_status)) != 0)
    {
        err_counter++;
    }

    if (sl_ttc2_read_antenna_deployment_status(config, &(data->deployment_status)) != 0)
    {
        err_counter++;
    }

    if (sl_ttc2_read_antenna_deployment_hibernation_status(config, &(data->hibernation_status)) != 0)
    {
        err_counter++;
    }

    sl_ttc2_delay_ms(5);

    /* Packet counter */
    if (sl_ttc2_read_pkt_counter(config, SL_TTC2_TX_PKT, &(data->tx_packet_counter)) != 0)
    {
        err_counter++;
    }

    if (sl_ttc2_read_pkt_counter(config, SL_TTC2_RX_PKT, &(data->rx_packet_counter)) != 0)
    {
        err_counter++;
    }

    return err_counter;

}

int sl_ttc2_read_device_id(sl_ttc2_config_t config, uint16_t *val)
{
    uint32_t buf = UINT32_MAX;

    int res = sl_ttc2_read_reg(config, SL_TTC2_REG_DEVICE_ID, &buf);

    *val = (uint16_t)buf;

    return res;
}

int sl_ttc2_read_hardware_version(sl_ttc2_config_t config, uint8_t *val)
{
    uint32_t buf = UINT32_MAX;

    int res = sl_ttc2_read_reg(config, SL_TTC2_REG_HARDWARE_VERSION, &buf);

    *val = (uint8_t)buf;

    return res;
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
    uint32_t buf = UINT32_MAX;

    int res = sl_ttc2_read_reg(config, SL_TTC2_REG_RESET_COUNTER, &buf);

    *val = (uint16_t)buf;

    return res;
}

int sl_ttc2_read_reset_cause(sl_ttc2_config_t config, uint8_t *val)
{
    uint32_t buf = UINT32_MAX;

    int res = sl_ttc2_read_reg(config, SL_TTC2_REG_LAST_RESET_CAUSE, &buf);

    *val = (uint8_t)buf;

    return res;
}

int sl_ttc2_read_voltage(sl_ttc2_config_t config, uint8_t volt, sl_ttc2_voltage_t *val)
{
    int res = -1;

    uint32_t buf = UINT32_MAX;

    switch(volt)
    {
        case SL_TTC2_VOLTAGE_MCU:
            res = sl_ttc2_read_reg(config, SL_TTC2_REG_INPUT_VOLTAGE_MCU, &buf);

            *val = (sl_ttc2_voltage_t)buf;

            break;
        case SL_TTC2_VOLTAGE_RADIO:
            res = sl_ttc2_read_reg(config, SL_TTC2_REG_INPUT_VOLTAGE_RADIO, &buf);

            *val = (sl_ttc2_voltage_t)buf;

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

int sl_ttc2_read_current(sl_ttc2_config_t config, uint8_t cur, sl_ttc2_current_t *val)
{
    int res = -1;

    uint32_t buf = UINT32_MAX;

    switch(cur)
    {
        case SL_TTC2_CURRENT_MCU:
            res = sl_ttc2_read_reg(config, SL_TTC2_REG_INPUT_CURRENT_MCU, &buf);

            *val = (sl_ttc2_current_t)buf;

            break;
        case SL_TTC2_CURRENT_RADIO:
            res = sl_ttc2_read_reg(config, SL_TTC2_REG_INPUT_CURRENT_RADIO, &buf);

            *val = (sl_ttc2_current_t)buf;

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

int sl_ttc2_read_temp(sl_ttc2_config_t config, uint8_t temp, sl_ttc2_temp_t *val)
{
    int res = -1;

    uint32_t buf = UINT32_MAX;

    switch(temp)
    {
        case SL_TTC2_TEMP_MCU:
            res = sl_ttc2_read_reg(config, SL_TTC2_REG_TEMPERATURE_MCU, &buf);

            *val = (sl_ttc2_temp_t)buf;

            break;
        case SL_TTC2_TEMP_RADIO:
            res = sl_ttc2_read_reg(config, SL_TTC2_REG_TEMPERATURE_RADIO, &buf);

            *val = (sl_ttc2_temp_t)buf;

            break;
        case SL_TTC2_TEMP_ANTENNA:
            res = sl_ttc2_read_reg(config, SL_TTC2_REG_TEMPERATURE_ANTENNA, &buf);

            *val = (sl_ttc2_temp_t)buf;

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

int sl_ttc2_read_last_valid_tc(sl_ttc2_config_t config, uint8_t *val)
{
    uint32_t buf = UINT32_MAX;

    int res = sl_ttc2_read_reg(config, SL_TTC2_REG_LAST_VALID_TC, &buf);

    *val = (uint8_t)buf;

    return res;
}

int sl_ttc2_read_rssi(sl_ttc2_config_t config, sl_ttc2_rssi_t *val)
{
    uint32_t buf = UINT32_MAX;

    int res = sl_ttc2_read_reg(config, SL_TTC2_REG_RSSI_LAST_VALID_TC, &buf);

    *val = (sl_ttc2_rssi_t)buf;

    return res;
}

int sl_ttc2_read_antenna_status(sl_ttc2_config_t config, uint16_t *val)
{
    uint32_t buf = UINT32_MAX;

    int res = sl_ttc2_read_reg(config, SL_TTC2_REG_ANTENNA_STATUS, &buf);

    *val = (uint16_t)buf;

    return res;
}

int sl_ttc2_read_antenna_deployment_status(sl_ttc2_config_t config, uint8_t *val)
{
    uint32_t buf = UINT32_MAX;

    int res = sl_ttc2_read_reg(config, SL_TTC2_REG_ANTENNA_DEPLOYMENT_STATUS, &buf);

    *val = (uint8_t)buf;

    return res;
}

int sl_ttc2_read_antenna_deployment_hibernation_status(sl_ttc2_config_t config, uint8_t *val)
{
    uint32_t buf = UINT32_MAX;

    int res = sl_ttc2_read_reg(config, SL_TTC2_REG_ANTENNA_DEP_HIB_STATUS, &buf);

    *val = (uint8_t)buf;

    return res;
}

int sl_ttc2_read_tx_enable(sl_ttc2_config_t config, uint8_t *val)
{
    uint32_t buf = UINT32_MAX;

    int res = sl_ttc2_read_reg(config, SL_TTC2_REG_TX_ENABLE, &buf);

    *val = (uint8_t)buf;

    return res;
}

int sl_ttc2_set_tx_enable(sl_ttc2_config_t config, bool en)
{
    return sl_ttc2_write_reg(config, SL_TTC2_REG_TX_ENABLE, (en? 1UL : 0UL));
}

int sl_ttc2_read_pkt_counter(sl_ttc2_config_t config, uint8_t pkt, uint32_t *val)
{
    int err = -1;

    switch(pkt)
    {
        case SL_TTC2_TX_PKT:    err = sl_ttc2_read_reg(config, SL_TTC2_REG_TX_PACKET_COUNTER, val);     break;
        case SL_TTC2_RX_PKT:    err = sl_ttc2_read_reg(config, SL_TTC2_REG_RX_PACKET_COUNTER, val);     break;
        default:
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, SL_TTC2_MODULE_NAME, "Error reading the packet counter! Invalid packet type!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            break;
    }

    return err;
}

int sl_ttc2_read_fifo_pkts(sl_ttc2_config_t config, uint8_t pkt, uint8_t *val)
{
    int res = -1;

    uint32_t buf = UINT32_MAX;

    switch(pkt)
    {
        case SL_TTC2_TX_PKT:
            res = sl_ttc2_read_reg(config, SL_TTC2_REG_FIFO_TX_PACKET, &buf);

            *val = (uint8_t)buf;

            break;
        case SL_TTC2_RX_PKT:
            res = sl_ttc2_read_reg(config, SL_TTC2_REG_FIFO_RX_PACKET, &buf);

            *val = (uint8_t)buf;

            break;
        default:
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, SL_TTC2_MODULE_NAME, "Error reading the FIFO buffer! Invalid packet type!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            break;
    }

    return res;
}

int sl_ttc2_read_len_rx_pkt_in_fifo(sl_ttc2_config_t config, uint16_t *val)
{
    uint32_t buf = UINT32_MAX;

    int res = sl_ttc2_read_reg(config, SL_TTC2_REG_LEN_FIRST_RX_PACKET_IN_FIFO, &buf);

    *val = (uint16_t)buf;

    return res;
}

int sl_ttc2_check_pkt_avail(sl_ttc2_config_t config)
{
    int res = -1;

    uint8_t pkts = 0;

    if (sl_ttc2_read_fifo_pkts(config, SL_TTC2_RX_PKT, &pkts) == 0)
    {
        res = pkts;
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, SL_TTC2_MODULE_NAME, "Error reading the available RX packets!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return res;
}

int sl_ttc2_transmit_packet(sl_ttc2_config_t config, uint8_t *data, uint16_t len)
{
    int err = -1;

    uint8_t buf[3 + 220] = {0};

    /* Adding preamble byte */
    buf[0] = SL_TTC2_PKT_PREAMBLE;

    /* Command byte */
    buf[1] = SL_TTC2_CMD_TRANSMIT_PKT;

    /* Packet lenght */
    buf[2] = len;

    if (sl_ttc2_mutex_take() == 0)
    {
        if (sl_ttc2_spi_write(config, buf, 7U) == 0)
        {
            sl_ttc2_delay_ms(110);

            if (memcpy(&buf[3], data, len) == &buf[3])
            {
                err = sl_ttc2_spi_write(config, buf, 3U + len);
            }
        }

        sl_ttc2_delay_ms(110);

        (void)sl_ttc2_mutex_give();
    }
    else 
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, SL_TTC2_MODULE_NAME, "Failed to take the mutex in time!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return err;
}

int sl_ttc2_read_packet(sl_ttc2_config_t config, uint8_t *data, uint16_t *len)
{
    int err = -1;

    uint8_t buf[7] = {0};

    /* Adding preamble byte */
    buf[0] = SL_TTC2_PKT_PREAMBLE;

    /* Command byte */
    buf[1] = SL_TTC2_CMD_RECEIVE_PKT;

    if (sl_ttc2_read_len_rx_pkt_in_fifo(config, len) == 0)
    {
        if ((*len > 0) && (*len <= 220))
        {
            if (sl_ttc2_mutex_take() == 0)
            {
                if (sl_ttc2_spi_write(config, buf, 7U) == 0)
                {
                    sl_ttc2_delay_ms(110);

                    if (sl_ttc2_spi_read(config, data, 1U + 1U + (*len)) == 0)
                    {
                        if (memcpy(data, &data[2], *len) == data)
                        {
                            err = 0;
                        }
                    }
                }

                sl_ttc2_delay_ms(110);

                (void)sl_ttc2_mutex_give();
            }
            else 
            {
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_ERROR, SL_TTC2_MODULE_NAME, "Failed to take the mutex in time!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            }
        }
    }

    return err;
}

/** \} End of sl_ttc2 group */
