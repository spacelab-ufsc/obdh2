/*
 * spi.c
 * 
 * Copyright (C) 2019, SpaceLab.
 * 
 * This file is part of OBDH 2.0.
 * 
 * OBDH 2.0 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * OBDH 2.0 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with OBDH 2.0. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief SPI driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.0
 * 
 * \date 07/12/2019
 * 
 * \addtogroup spi
 * \{
 */

#include "spi.h"

int spi_init(spi_port_t port, spi_config_t config)
{
    return -1;
}

int spi_transfer(spi_port_t port, spi_cs_t cs, uint8_t *data_w, uint8_t *data_r, uint16_t len)
{
    return -1;
}

int spi_write(spi_port_t port, spi_cs_t cs, uint8_t *data, uint16_t len)
{
    return -1;
}

int spi_read(spi_port_t port, spi_cs_t cs, uint8_t *data, uint16_t len)
{
    return -1;
}

//! \} End of spi group
