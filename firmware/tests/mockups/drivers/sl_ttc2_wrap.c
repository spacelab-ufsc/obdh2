/*
 * sl_ttc2_wrap.c
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
 * \brief SpaceLab TTC 2.0 wrap implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 0.10.14
 * 
 * \date 2021/08/06
 * 
 * \addtogroup sl_ttc2_wrap
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include "sl_ttc2_wrap.h"

int __wrap_sl_ttc2_init(sl_ttc2_config_t config)
{
    check_expected(config.port);
    check_expected(config.cs_pin);
    check_expected(config.port_config.speed_hz);
    check_expected(config.port_config.mode);
    check_expected(config.id);

    return mock_type(int);
}

int __wrap_sl_ttc2_check_device(sl_ttc2_config_t config)
{
    check_expected(config.port);
    check_expected(config.cs_pin);
    check_expected(config.port_config.speed_hz);
    check_expected(config.port_config.mode);
    check_expected(config.id);

    return mock_type(int);
}

int __wrap_sl_ttc2_write_reg(sl_ttc2_config_t config, uint8_t adr, uint32_t val)
{
    check_expected(config.port);
    check_expected(config.cs_pin);
    check_expected(config.port_config.speed_hz);
    check_expected(config.port_config.mode);
    check_expected(config.id);

    check_expected(adr);
    check_expected(val);

    return mock_type(int);
}

int __wrap_sl_ttc2_read_reg(sl_ttc2_config_t config, uint8_t adr, uint32_t *val)
{
    check_expected(config.port);
    check_expected(config.cs_pin);
    check_expected(config.port_config.speed_hz);
    check_expected(config.port_config.mode);
    check_expected(config.id);

    check_expected(adr);

    if (val != NULL)
    {
        *val = mock_type(uint32_t);
    }

    return mock_type(int);
}

int __wrap_sl_ttc2_read_hk_data(sl_ttc2_config_t config, sl_ttc2_hk_data_t *data)
{
    check_expected(config.port);
    check_expected(config.cs_pin);
    check_expected(config.port_config.speed_hz);
    check_expected(config.port_config.mode);
    check_expected(config.id);

    if (data != NULL)
    {
        data->time_counter          = mock_type(uint32_t);
        data->reset_counter         = mock_type(uint16_t);
        data->last_reset_cause      = mock_type(uint8_t);
        data->voltage_mcu           = mock_type(sl_ttc2_voltage_t);
        data->current_mcu           = mock_type(sl_ttc2_current_t);
        data->temperature_mcu       = mock_type(sl_ttc2_temp_t);
        data->voltage_radio         = mock_type(sl_ttc2_voltage_t);
        data->current_radio         = mock_type(sl_ttc2_current_t);
        data->temperature_radio     = mock_type(sl_ttc2_temp_t);
        data->last_valid_tc         = mock_type(uint8_t);
        data->rssi_last_valid_tc    = mock_type(sl_ttc2_rssi_t);
        data->temperature_antenna   = mock_type(sl_ttc2_temp_t);
        data->antenna_status        = mock_type(uint16_t);
        data->deployment_status     = mock_type(uint8_t);
        data->hibernation_status    = mock_type(uint8_t);
        data->tx_packet_counter     = mock_type(uint32_t);
        data->rx_packet_counter     = mock_type(uint32_t);
    }

    return mock_type(int);
}

int __wrap_sl_ttc2_read_device_id(sl_ttc2_config_t config, uint16_t *val)
{
    check_expected(config.port);
    check_expected(config.cs_pin);
    check_expected(config.port_config.speed_hz);
    check_expected(config.port_config.mode);
    check_expected(config.id);

    if (val != NULL)
    {
        *val = mock_type(uint32_t);
    }

    return mock_type(int);
}

int __wrap_sl_ttc2_read_hardware_version(sl_ttc2_config_t config, uint8_t *val)
{
    check_expected(config.port);
    check_expected(config.cs_pin);
    check_expected(config.port_config.speed_hz);
    check_expected(config.port_config.mode);
    check_expected(config.id);

    if (val != NULL)
    {
        *val = mock_type(uint8_t);
    }

    return mock_type(int);
}

int __wrap_sl_ttc2_read_firmware_version(sl_ttc2_config_t config, uint32_t *val)
{
    check_expected(config.port);
    check_expected(config.cs_pin);
    check_expected(config.port_config.speed_hz);
    check_expected(config.port_config.mode);
    check_expected(config.id);

    if (val != NULL)
    {
        *val = mock_type(uint32_t);
    }

    return mock_type(int);
}

int __wrap_sl_ttc2_read_time_counter(sl_ttc2_config_t config, uint32_t *val)
{
    check_expected(config.port);
    check_expected(config.cs_pin);
    check_expected(config.port_config.speed_hz);
    check_expected(config.port_config.mode);
    check_expected(config.id);

    if (val != NULL)
    {
        *val = mock_type(uint32_t);
    }

    return mock_type(int);
}

int __wrap_sl_ttc2_read_reset_counter(sl_ttc2_config_t config, uint16_t *val)
{
    check_expected(config.port);
    check_expected(config.cs_pin);
    check_expected(config.port_config.speed_hz);
    check_expected(config.port_config.mode);
    check_expected(config.id);

    if (val != NULL)
    {
        *val = mock_type(uint16_t);
    }

    return mock_type(int);
}

int __wrap_sl_ttc2_read_reset_cause(sl_ttc2_config_t config, uint8_t *val)
{
    check_expected(config.port);
    check_expected(config.cs_pin);
    check_expected(config.port_config.speed_hz);
    check_expected(config.port_config.mode);
    check_expected(config.id);

    if (val != NULL)
    {
        *val = mock_type(uint8_t);
    }

    return mock_type(int);
}

int __wrap_sl_ttc2_read_voltage(sl_ttc2_config_t config, uint8_t volt, sl_ttc2_voltage_t *val)
{
    check_expected(config.port);
    check_expected(config.cs_pin);
    check_expected(config.port_config.speed_hz);
    check_expected(config.port_config.mode);
    check_expected(config.id);

    check_expected(volt);

    if (val != NULL)
    {
        *val = mock_type(sl_ttc2_voltage_t);
    }

    return mock_type(int);
}

int __wrap_sl_ttc2_read_current(sl_ttc2_config_t config, uint8_t cur, sl_ttc2_current_t *val)
{
    check_expected(config.port);
    check_expected(config.cs_pin);
    check_expected(config.port_config.speed_hz);
    check_expected(config.port_config.mode);
    check_expected(config.id);

    check_expected(cur);

    if (val != NULL)
    {
        *val = mock_type(sl_ttc2_current_t);
    }

    return mock_type(int);
}

int __wrap_sl_ttc2_read_temp(sl_ttc2_config_t config, uint8_t temp, sl_ttc2_temp_t *val)
{
    check_expected(config.port);
    check_expected(config.cs_pin);
    check_expected(config.port_config.speed_hz);
    check_expected(config.port_config.mode);
    check_expected(config.id);

    check_expected(temp);

    if (val != NULL)
    {
        *val = mock_type(sl_ttc2_temp_t);
    }

    return mock_type(int);
}

int __wrap_sl_ttc2_read_last_valid_tc(sl_ttc2_config_t config, uint8_t *val)
{
    check_expected(config.port);
    check_expected(config.cs_pin);
    check_expected(config.port_config.speed_hz);
    check_expected(config.port_config.mode);
    check_expected(config.id);

    if (val != NULL)
    {
        *val = mock_type(uint8_t);
    }

    return mock_type(int);
}

int __wrap_sl_ttc2_read_rssi(sl_ttc2_config_t config, sl_ttc2_rssi_t *val)
{
    check_expected(config.port);
    check_expected(config.cs_pin);
    check_expected(config.port_config.speed_hz);
    check_expected(config.port_config.mode);
    check_expected(config.id);

    if (val != NULL)
    {
        *val = mock_type(sl_ttc2_rssi_t);
    }

    return mock_type(int);
}

int __wrap_sl_ttc2_read_antenna_status(sl_ttc2_config_t config, uint16_t *val)
{
    check_expected(config.port);
    check_expected(config.cs_pin);
    check_expected(config.port_config.speed_hz);
    check_expected(config.port_config.mode);
    check_expected(config.id);

    if (val != NULL)
    {
        *val = mock_type(uint16_t);
    }

    return mock_type(int);
}

int __wrap_sl_ttc2_read_antenna_deployment_status(sl_ttc2_config_t config, uint8_t *val)
{
    check_expected(config.port);
    check_expected(config.cs_pin);
    check_expected(config.port_config.speed_hz);
    check_expected(config.port_config.mode);
    check_expected(config.id);

    if (val != NULL)
    {
        *val = mock_type(uint8_t);
    }

    return mock_type(int);
}

int __wrap_sl_ttc2_read_antenna_deployment_hibernation_status(sl_ttc2_config_t config, uint8_t *val)
{
    check_expected(config.port);
    check_expected(config.cs_pin);
    check_expected(config.port_config.speed_hz);
    check_expected(config.port_config.mode);
    check_expected(config.id);

    if (val != NULL)
    {
        *val = mock_type(uint8_t);
    }

    return mock_type(int);
}

int __wrap_sl_ttc2_read_tx_enable(sl_ttc2_config_t config, uint8_t *val)
{
    check_expected(config.port);
    check_expected(config.cs_pin);
    check_expected(config.port_config.speed_hz);
    check_expected(config.port_config.mode);
    check_expected(config.id);

    if (val != NULL)
    {
        *val = mock_type(uint8_t);
    }

    return mock_type(int);
}

int __wrap_sl_ttc2_set_tx_enable(sl_ttc2_config_t config, bool en)
{
    check_expected(config.port);
    check_expected(config.cs_pin);
    check_expected(config.port_config.speed_hz);
    check_expected(config.port_config.mode);
    check_expected(config.id);

    check_expected(en);

    return mock_type(int);
}

int __wrap_sl_ttc2_read_pkt_counter(sl_ttc2_config_t config, uint8_t pkt, uint32_t *val)
{
    check_expected(config.port);
    check_expected(config.cs_pin);
    check_expected(config.port_config.speed_hz);
    check_expected(config.port_config.mode);
    check_expected(config.id);

    check_expected(pkt);

    if (val != NULL)
    {
        *val = mock_type(uint32_t);
    }

    return mock_type(int);
}

int __wrap_sl_ttc2_read_fifo_pkts(sl_ttc2_config_t config, uint8_t pkt, uint8_t *val)
{
    check_expected(config.port);
    check_expected(config.cs_pin);
    check_expected(config.port_config.speed_hz);
    check_expected(config.port_config.mode);
    check_expected(config.id);

    check_expected(pkt);

    if (val != NULL)
    {
        *val = mock_type(uint8_t);
    }

    return mock_type(int);
}

int __wrap_sl_ttc2_read_len_rx_pkt_in_fifo(sl_ttc2_config_t config, uint16_t *val)
{
    check_expected(config.port);
    check_expected(config.cs_pin);
    check_expected(config.port_config.speed_hz);
    check_expected(config.port_config.mode);
    check_expected(config.id);

    if (val != NULL)
    {
        *val = mock_type(uint16_t);
    }

    return mock_type(int);
}

int __wrap_sl_ttc2_check_pkt_avail(sl_ttc2_config_t config)
{
    check_expected(config.port);
    check_expected(config.cs_pin);
    check_expected(config.port_config.speed_hz);
    check_expected(config.port_config.mode);
    check_expected(config.id);

    return mock_type(int);
}

int __wrap_sl_ttc2_transmit_packet(sl_ttc2_config_t config, uint8_t *data, uint16_t len)
{
    check_expected(config.port);
    check_expected(config.cs_pin);
    check_expected(config.port_config.speed_hz);
    check_expected(config.port_config.mode);
    check_expected(config.id);

    check_expected_ptr(data);
    check_expected(len);

    return mock_type(int);
}

int __wrap_sl_ttc2_read_packet(sl_ttc2_config_t config, uint8_t *data, uint16_t *len)
{
    check_expected(config.port);
    check_expected(config.cs_pin);
    check_expected(config.port_config.speed_hz);
    check_expected(config.port_config.mode);
    check_expected(config.id);

    if (len != NULL)
    {
        *len = mock_type(uint16_t);
    }

    if (data != NULL)
    {
        uint16_t i = 0;
        for(i=0; i<*len; i++)
        {
            data[i] = mock_type(uint8_t);
        }
    }

    return mock_type(int);
}

int __wrap_sl_ttc2_mutex_create(void)
{
    return mock_type(int);
}

int __wrap_sl_ttc2_mutex_take(void)
{
    return mock_type(int);
}

int __wrap_sl_ttc2_mutex_give(void)
{
    return mock_type(int);
}

void __wrap_sl_ttc2_delay_ms(uint32_t ms)
{
    function_called();

    return;
}

int __wrap_sl_ttc2_spi_read(sl_ttc2_config_t config, uint8_t *data, uint16_t len)
{
    check_expected(config.port);
    check_expected(config.cs_pin);
    check_expected(config.port_config.speed_hz);
    check_expected(config.port_config.mode);
    check_expected(len);

    if (data != NULL)
    {
        uint16_t i = 0;
        for(i=0; i<len; i++)
        {
            data[i] = mock_type(uint8_t);
        }
    }

    return mock_type(int);
}

/** \} End of sl_ttc2_wrap group */
