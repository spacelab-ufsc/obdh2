/*
 * rf4463_spi.h
 * 
 * Copyright (C) 2017, Federal University of Santa Catarina
 * 
 * This file is part of FloripaSat-Beacon.
 * 
 * FloripaSat-Beacon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * FloripaSat-Beacon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with FloripaSat-Beacon. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \file rf4463_spi.h
 * 
 * \brief NiceRF RF4463 driver (SPI communication functions).
 * 
 * This library suits for RF4463PRO and RF4463F30 in FIFO mode.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 29/07/2017
 * 
 * \addtogroup rf4463
 * \{
 */

#ifndef RF4463_SPI_H_
#define RF4463_SPI_H_


/**
 * \fn rf4463_spi_write_byte
 * 
 * \brief Transfers a byte through the SPI interface.
 * 
 * \param byte is the byte to be transferes.
 * 
 * \return None
 */
static void rf4463_spi_write_byte(uint8_t byte);

/**
 * \fn rf4463_spi_write
 * 
 * \brief Transfers an array through the SPI interface.
 * 
 * \param data is an array to be  transfered.
 * \param size is the size of the data to be transfered.
 * 
 * \return None
 */
void rf4463_spi_write(uint8_t *data, uint16_t size);

/**
 * \fn rf4463_spi_read_byte
 * 
 * \brief Reads a byte from the SPI interface.
 * 
 * \return The byte read from the SPI interface.
 */
static uint8_t rf4463_spi_read_byte();

/**
 * \fn rf4463_spi_read
 * 
 * \brief Reads data from the SPI interface
 * 
 * \param data is a pointer to where the incoming data will be stored.
 * \param size is how many bytes will be read from the SPI interface.
 * 
 * \return None
 */
void rf4463_spi_read(uint8_t *data, uint16_t size);

/**
 * \fn rf4463_spi_transfer
 * 
 * \brief Makes a byte transfer in the SPI port (Writing and reading).
 * 
 * \param byte is the byte to write in the SPI port.
 * 
 * \return None
 */
uint8_t rf4463_spi_transfer(uint8_t byte);

#endif // RF4463_SPI_H_

//! \} End of rf4463 group
