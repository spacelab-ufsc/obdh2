/*
 * spi_wrap.h
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
 * \brief SPI driver wrap definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.8
 * 
 * \date 2021/08/25
 * 
 * \defgroup spi_wrap SPI Wrap
 * \ingroup tests
 * \{
 */

#ifndef SPI_WRAP_H_
#define SPI_WRAP_H_

#include <stdint.h>

#include <drivers/spi/spi.h>

int __wrap_spi_init(spi_port_t port, spi_config_t config);

int __wrap_spi_select_slave(spi_port_t port, spi_cs_t cs, bool active);

int __wrap_spi_write(spi_port_t port, spi_cs_t cs, uint8_t *data, uint16_t len);

int __wrap_spi_read(spi_port_t port, spi_cs_t cs, uint8_t *data, uint16_t len);

int __wrap_spi_transfer(spi_port_t port, spi_cs_t cs, uint8_t *wd, uint8_t *rd, uint16_t len);

#endif /* SPI_WRAP_H_ */

/** \} End of spi_wrap group */
