/*
 * sl_ttc2_wrap.c
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
 * \brief SpaceLab TTC 2.0 wrap implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.2
 * 
 * \date 2021/08/06
 * 
 * \addtogroup sl_ttc2_wrap
 * \{
 */

#include "sl_ttc2_wrap.h"

int __wrap_sl_ttc2_init(sl_ttc2_config_t config)
{
    return 0;
}

int __wrap_sl_ttc2_check_device(sl_ttc2_config_t config)
{
    return 0;
}

int __wrap_sl_ttc2_write_reg(sl_ttc2_config_t config, uint8_t adr, uint32_t val)
{
    return 0;
}

int __wrap_sl_ttc2_read_reg(sl_ttc2_config_t config, uint8_t adr, uint32_t *val)
{
    return 0;
}

int __wrap_sl_ttc2_read_hk_data(sl_ttc2_config_t config, sl_ttc2_hk_data_t *data)
{
    return 0;
}

int __wrap_sl_ttc2_read_device_id(sl_ttc2_config_t config, uint16_t *val)
{
    return 0;
}

int __wrap_sl_ttc2_read_hardware_version(sl_ttc2_config_t config, uint8_t *val)
{
    return 0;
}

int __wrap_sl_ttc2_read_firmware_version(sl_ttc2_config_t config, uint32_t *val)
{
    return 0;
}

int __wrap_sl_ttc2_read_time_counter(sl_ttc2_config_t config, uint32_t *val)
{
    return 0;
}

int __wrap_sl_ttc2_read_reset_counter(sl_ttc2_config_t config, uint16_t *val)
{
    return 0;
}

int __wrap_sl_ttc2_read_reset_cause(sl_ttc2_config_t config, uint8_t *val)
{
    return 0;
}

int __wrap_sl_ttc2_read_voltage(sl_ttc2_config_t config, uint8_t volt, sl_ttc2_voltage_t *val)
{
    return 0;
}

int __wrap_sl_ttc2_read_current(sl_ttc2_config_t config, uint8_t cur, sl_ttc2_current_t *val)
{
    return 0;
}

int __wrap_sl_ttc2_read_temp(sl_ttc2_config_t config, uint8_t temp, sl_ttc2_temp_t *val)
{
    return 0;
}

int __wrap_sl_ttc2_read_last_valid_tc(sl_ttc2_config_t config, uint8_t *val)
{
    return 0;
}

int __wrap_sl_ttc2_read_rssi(sl_ttc2_config_t config, sl_ttc2_rssi_t *val)
{
    return 0;
}

int __wrap_sl_ttc2_read_antenna_status(sl_ttc2_config_t config, uint16_t *val)
{
    return 0;
}

int __wrap_sl_ttc2_read_antenna_deployment_status(sl_ttc2_config_t config, uint8_t *val)
{
    return 0;
}

int __wrap_sl_ttc2_read_antenna_deployment_hibernation_status(sl_ttc2_config_t config, uint8_t *val)
{
    return 0;
}

int __wrap_sl_ttc2_read_tx_enable(sl_ttc2_config_t config, uint8_t *val)
{
    return 0;
}

int __wrap_sl_ttc2_set_tx_enable(sl_ttc2_config_t config, bool en)
{
    return 0;
}

int __wrap_sl_ttc2_read_pkt_counter(sl_ttc2_config_t config, uint8_t pkt, uint32_t *val)
{
    return 0;
}

int __wrap_sl_ttc2_read_fifo_pkts(sl_ttc2_config_t config, uint8_t pkt, uint8_t *val)
{
    return 0;
}

int __wrap_sl_ttc2_read_len_rx_pkt_in_fifo(sl_ttc2_config_t config, uint16_t *val)
{
    return 0;
}

int __wrap_sl_ttc2_check_pkt_avail(sl_ttc2_config_t config)
{
    return 0;
}

int __wrap_sl_ttc2_transmit_packet(sl_ttc2_config_t config, uint8_t *data, uint16_t len)
{
    return 0;
}

int __wrap_sl_ttc2_read_packet(sl_ttc2_config_t config, uint8_t *data, uint16_t *len)
{
    return 0;
}

void __wrap_sl_ttc2_delay_ms(uint32_t ms)
{
    return;
}

/** \} End of sl_ttc2_wrap group */
