/*
 * mt25ql01gbbb_spi.c
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
 * \brief MT25QL01GBBB driver SPI interface implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.5.23
 * 
 * \date 2019/11/15
 * 
 * \addtogroup mt25ql01gbbb
 * \{
 */

#include <drivers/spi/spi.h>

#include "mt25ql01gbbb.h"

#define MT25QL01GBBB_SPI_PORT           SPI_PORT_0
#define MT25QL01GBBB_SPI_MODE           SPI_MODE_0
#define MT25QL01GBBB_SPI_CLK_HZ         100000
#define MT25QL01GBBB_SPI_CS_PIN         SPI_CS_2

int mt25ql01gbbb_spi_init(void)
{
    spi_config_t conf = {0};

    conf.speed_hz   = MT25QL01GBBB_SPI_CLK_HZ;
    conf.mode       = MT25QL01GBBB_SPI_MODE;

    return spi_init(MT25QL01GBBB_SPI_PORT, conf);
}

int mt25ql01gbbb_spi_write(uint8_t *data, uint16_t len)
{
    return spi_write(MT25QL01GBBB_SPI_PORT, MT25QL01GBBB_SPI_CS_PIN, data, len);
}

int mt25ql01gbbb_spi_read(uint8_t *data, uint16_t len)
{
    return spi_read(MT25QL01GBBB_SPI_PORT, MT25QL01GBBB_SPI_CS_PIN, data, len);
}

int mt25ql01gbbb_spi_transfer(uint8_t *wdata, uint8_t *rdata, uint16_t len)
{
    return spi_transfer(MT25QL01GBBB_SPI_PORT, MT25QL01GBBB_SPI_CS_PIN, wdata, rdata, len);
}

/** \} End of mt25ql01gbbb group */
