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
 * \addtogroup sl_ttc2
 * \{
 */

#include <config/config.h>
#include <system/sys_log/sys_log.h>

#include "sl_ttc2.h"

int sl_ttc2_spi_init(sl_ttc2_config_t config)
{
    return spi_init(config.port, config.port_config);
}

int sl_ttc2_spi_write(sl_ttc2_config_t config, uint8_t *data, uint16_t len)
{
    return spi_write(config.port, config.cs_pin, data, len);
}

int sl_ttc2_spi_read(sl_ttc2_config_t config, uint8_t *data, uint16_t len)
{
    return spi_read(config.port, config.cs_pin, data, len);
}

int sl_ttc2_spi_transfer(sl_ttc2_config_t config, uint8_t *wdata, uint8_t *rdata, uint16_t len)
{
    return spi_transfer(config.port, config.cs_pin, wdata, rdata, len);
}

/** \} End of sl_ttc2 group */
