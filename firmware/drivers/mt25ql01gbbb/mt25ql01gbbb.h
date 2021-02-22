/*
 * mt25ql01gbbb.h
 * 
 * Copyright (C) 2019, SpaceLab.
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
 * \brief MT25QL01GBBB driver definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.0
 * 
 * \date 15/11/2019
 * 
 * \defgroup mt25ql01gbbb MT25QL01GBBB
 * \ingroup drivers
 * \{
 */

#ifndef MT25QL01GBBB_H_
#define MT25QL01GBBB_H_

#include <stdint.h>

/* Registers */
#define MT25QL01GBBB_REG_   0x00

/* Commands */
#define MT25QL01GBBB_CMD_   0x00

/**
 * \brief SPI interface configuration.
 */
typedef struct
{
    uint32_t clock_hz;          /**< Clock rate of the SPI communication in Hertz. */
    uint8_t clock_phase;        /**< SPI clock phase. */
    uint8_t clock_polarity;     /**< SPI clock polarity. */
} mt25ql01gbbb_spi_config_t;

/**
 * \brief Driver initialization.
 *
 * \return Error/status code.
 */
int mt25ql01gbbb_init();

/**
 * \brief Writes data to a given address.
 *
 * \param[in] adr is the address to write.
 * \param[in] data is an array of bytes to write.
 * \param[in] len is the number of bytes to write.
 *
 * \return Error/status code.
 */
int mt25ql01gbbb_write(uint32_t adr, uint8_t *data, uint32_t len);

/**
 * \brief Reads data from a given address.
 *
 * \param[in] adr is the address to write.
 * \param[in] data is an array of bytes to write.
 * \param[in] len is the number of bytes to write.
 *
 * \return Error/status code.
 */
int mt25ql01gbbb_read(uint32_t adr, uint8_t *data, uint32_t len);

/**
 * \brief SPI interface initialization.
 *
 * \param[in] spi_config
 *
 * \return Error/status code.
 */
int mt25ql01gbbb_spi_init(mt25ql01gbbb_spi_config_t spi_config);

/**
 * \brief Writes the device using the SPI interface.
 *
 * \param[in] data is an array of bytes to write.
 * \param[in] len is the number of bytes to write.
 *
 * \return Error/status code.
 */
int mt25ql01gbbb_spi_write(uint8_t *data, uint16_t len);

/**
 * \brief Reads the device using the SPI interface.
 *
 * \param[in] data is an array to store the read bytes.
 * \param[in] len is the number of bytes to write.
 *
 * \return Error/status code.
 */
int mt25ql01gbbb_spi_read(uint8_t *data, uint16_t len);

/**
 * \brief SPI transfer operation (write and/or read).
 *
 * \return Error/status code.
 */
int mt25ql01gbbb_spi_transfer();

#endif /* MT25QL01GBBB_H_ */

/** \} End of mt25ql01gbbb group */
