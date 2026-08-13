/*
 * sl_ttc2_spi.c
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
 * \brief SpaceLab TTC 2.0 driver SPI implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.8.13
 * 
 * \date 2021/10/13
 * 
 * \addtogroup sl_ttcsb
 * \{
 */

#include <config/config.h>
#include <system/sys_log/sys_log.h>

#include "sl_ttcsb.h"


int sl_ttcsb_i2c_init(sl_ttcsb_config_t config)
{
    return i2c_init(config.port, config.port_config);
}

int sl_ttcsb_i2c_write(sl_ttcsb_config_t config, uint8_t *data, uint16_t len)
{
    return i2c_write(config.port, SL_TTCSB_SLAVE_ADDR, data, len);
}

int sl_ttcsb_i2c_read(sl_ttcsb_config_t config, uint8_t *data, uint16_t len)
{
    return i2c_read(config.port, SL_TTCSB_SLAVE_ADDR, data, len);
}

/** \} End of sl_ttcsb group */
