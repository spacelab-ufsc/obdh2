/*
 * cy15x102qn_spi.c
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
 * \brief CY15x102QN driver SPI interface implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.1
 * 
 * \date 2021/06/04
 * 
 * \addtogroup cy15x102qn
 * \{
 */

#include <config/config.h>
#include <drivers/spi/spi.h>

#include "cy15x102qn.h"

#define CY15X102QN_SPI_PORT     SPI_PORT_0
#define CY15X102QN_SPI_MODE     SPI_MODE_0
#define CY15X102QN_SPI_CLK_HZ   CONFIG_SPI_PORT_0_SPEED_BPS
#define CY15X102QN_SPI_CS_PIN   SPI_CS_5

int cy15x102qn_spi_init(void)
{
    spi_config_t conf = {0};

    conf.speed_hz   = CY15X102QN_SPI_CLK_HZ;
    conf.mode       = CY15X102QN_SPI_MODE;

    return spi_init(CY15X102QN_SPI_PORT, conf);
}

int cy15x102qn_spi_write(uint8_t *data, uint16_t len)
{
    return spi_write(CY15X102QN_SPI_PORT, CY15X102QN_SPI_CS_PIN, data, len);
}

int cy15x102qn_spi_read(uint8_t *data, uint16_t len)
{
    return spi_read(CY15X102QN_SPI_PORT, CY15X102QN_SPI_CS_PIN, data, len);
}

int cy15x102qn_spi_transfer(uint8_t *wdata, uint8_t *rdata, uint16_t len)
{
    return spi_transfer(CY15X102QN_SPI_PORT, CY15X102QN_SPI_CS_PIN, wdata, rdata, len);
}

int cy15x102qn_spi_select(void)
{
    return spi_select_slave(CY15X102QN_SPI_PORT, CY15X102QN_SPI_CS_PIN, true);
}

int cy15x102qn_spi_unselect(void)
{
    return spi_select_slave(CY15X102QN_SPI_PORT, CY15X102QN_SPI_CS_PIN, false);
}

int cy15x102qn_spi_write_only(uint8_t *data, uint16_t len)
{
    return spi_write(CY15X102QN_SPI_PORT, SPI_CS_NONE, data, len);
}

int cy15x102qn_spi_read_only(uint8_t *data, uint16_t len)
{
    return spi_read(CY15X102QN_SPI_PORT, SPI_CS_NONE, data, len);
}

int cy15x102qn_spi_transfer_only(uint8_t *wdata, uint8_t *rdata, uint16_t len)
{
    return spi_transfer(CY15X102QN_SPI_PORT, SPI_CS_NONE, wdata, rdata, len);
}

/** \} End of cy15x102qn group */
