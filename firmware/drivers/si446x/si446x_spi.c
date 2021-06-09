/*
 * si446x_spi.c
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
 * \brief Si446x SPI implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.6.34
 * 
 * \date 2017/07/29
 * 
 * \defgroup si446x_spi SPI
 * \ingroup si446x
 * \{
 */

#include <stdint.h>

#include <config/config.h>
#include <drivers/spi/spi.h>
#include <drivers/gpio/gpio.h>

int si446x_spi_init()
{
    return spi_init(SPI_PORT_0, (spi_config_t){.speed_hz=CONFIG_SPI_PORT_0_SPEED_BPS, .mode=SPI_MODE_1});
}

int si446x_spi_transfer(uint8_t *wd, uint8_t *rd, uint16_t len)
{
    return spi_transfer(SPI_PORT_0, GPIO_PIN_5, wd, rd, len);
}

int si446x_spi_write_byte(uint8_t byte)
{
    return spi_write(SPI_PORT_0, GPIO_PIN_5, &byte, 1);
}

int si446x_spi_write(uint8_t *data, uint16_t len)
{
    return spi_write(SPI_PORT_0, GPIO_PIN_5, data, len);
}

int si446x_spi_read(uint8_t *data, uint16_t len)
{
    return spi_read(SPI_PORT_0, GPIO_PIN_5, data, len);
}

/** \} End of si446x group */
