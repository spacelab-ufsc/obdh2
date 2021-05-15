/*
 * mt25q_spi.c
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
 * \brief MT25Q driver SPI interface implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.6.6
 * 
 * \date 2019/11/15
 * 
 * \addtogroup mt25q
 * \{
 */

#include <drivers/spi/spi.h>

#include "mt25q.h"

#define MT25Q_SPI_PORT          SPI_PORT_0
#define MT25Q_SPI_MODE          SPI_MODE_0
#define MT25Q_SPI_CLK_HZ        100000
#define MT25Q_SPI_CS_PIN        SPI_CS_2

int mt25q_spi_init(void)
{
    spi_config_t conf = {0};

    conf.speed_hz   = MT25Q_SPI_CLK_HZ;
    conf.mode       = MT25Q_SPI_MODE;

    return spi_init(MT25Q_SPI_PORT, conf);
}

int mt25q_spi_write(uint8_t *data, uint16_t len)
{
    return spi_write(MT25Q_SPI_PORT, MT25Q_SPI_CS_PIN, data, len);
}

int mt25q_spi_read(uint8_t *data, uint16_t len)
{
    return spi_read(MT25Q_SPI_PORT, MT25Q_SPI_CS_PIN, data, len);
}

int mt25q_spi_transfer(uint8_t *wdata, uint8_t *rdata, uint16_t len)
{
    return spi_transfer(MT25Q_SPI_PORT, MT25Q_SPI_CS_PIN, wdata, rdata, len);
}

int mt25q_spi_select(void)
{
    return spi_select_slave(MT25Q_SPI_PORT, MT25Q_SPI_CS_PIN, true);
}

int mt25q_spi_unselect(void)
{
    return spi_select_slave(MT25Q_SPI_PORT, MT25Q_SPI_CS_PIN, false);
}

int mt25q_spi_write_only(uint8_t *data, uint16_t len)
{
    return spi_write(MT25Q_SPI_PORT, SPI_CS_NONE, data, len);
}

int mt25q_spi_read_only(uint8_t *data, uint16_t len)
{
    return spi_read(MT25Q_SPI_PORT, SPI_CS_NONE, data, len);
}

int mt25q_spi_transfer_only(uint8_t *wdata, uint8_t *rdata, uint16_t len)
{
    return spi_transfer(MT25Q_SPI_PORT, SPI_CS_NONE, wdata, rdata, len);
}

/** \} End of mt25q group */
