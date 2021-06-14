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

#include <drivers/spi/spi.h>

#include "cy15x102qn.h"

#define CY15X102QN_SPI_MODE     SPI_MODE_0

int cy15x102qn_spi_init(cy15x102qn_config_t *conf)
{
    spi_config_t spi_conf = {0};

    spi_conf.speed_hz   = conf->clock_hz;
    spi_conf.mode       = CY15X102QN_SPI_MODE;

    return spi_init(conf->port, spi_conf);
}

int cy15x102qn_spi_write(cy15x102qn_config_t *conf, uint8_t *data, uint16_t len)
{
    return spi_write(conf->port, conf->cs_pin, data, len);
}

int cy15x102qn_spi_read(cy15x102qn_config_t *conf, uint8_t *data, uint16_t len)
{
    return spi_read(conf->port, conf->cs_pin, data, len);
}

int cy15x102qn_spi_transfer(cy15x102qn_config_t *conf, uint8_t *wdata, uint8_t *rdata, uint16_t len)
{
    return spi_transfer(conf->port, conf->cs_pin, wdata, rdata, len);
}

int cy15x102qn_spi_select(cy15x102qn_config_t *conf)
{
    return spi_select_slave(conf->port, conf->cs_pin, true);
}

int cy15x102qn_spi_unselect(cy15x102qn_config_t *conf)
{
    return spi_select_slave(conf->port, conf->cs_pin, false);
}

int cy15x102qn_spi_write_only(cy15x102qn_config_t *conf, uint8_t *data, uint16_t len)
{
    return spi_write(conf->port, SPI_CS_NONE, data, len);
}

int cy15x102qn_spi_read_only(cy15x102qn_config_t *conf, uint8_t *data, uint16_t len)
{
    return spi_read(conf->port, SPI_CS_NONE, data, len);
}

int cy15x102qn_spi_transfer_only(cy15x102qn_config_t *conf, uint8_t *wdata, uint8_t *rdata, uint16_t len)
{
    return spi_transfer(conf->port, SPI_CS_NONE, wdata, rdata, len);
}

/** \} End of cy15x102qn group */
