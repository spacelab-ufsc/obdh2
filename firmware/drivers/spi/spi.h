/*
 * spi.h
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
 * \brief SPI driver definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.0
 * 
 * \date 07/12/2019
 * 
 * \defgroup spi SPI
 * \ingroup drivers
 * \{
 */

#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>

/**
 * \brief SPI ports.
 */
typedef enum
{
    SPI_PORT_0=0,       /**< SPI port 0. */
    SPI_PORT_1,         /**< SPI port 1. */
    SPI_PORT_2          /**< SPI port 2. */
} spi_ports_e;

/**
 * \brief SPI chips select.
 */
typedef enum
{
    SPI_CS_0=0,         /**< SPI chip select 0. */
    SPI_CS_1,           /**< SPI chip select 1. */
    SPI_CS_2,           /**< SPI chip select 2. */
    SPI_CS_3,           /**< SPI chip select 3. */
    SPI_CS_4,           /**< SPI chip select 4. */
    SPI_CS_5,           /**< SPI chip select 5. */
    SPI_CS_6,           /**< SPI chip select 6. */
    SPI_CS_7,           /**< SPI chip select 7. */
    SPI_CS_8,           /**< SPI chip select 8. */
    SPI_CS_9            /**< SPI chip select 9. */
} spi_cs_e;

/**
 * \brief SPI bus configuration parameters.
 */
typedef struct
{
    uint32_t speed;
} spi_config_t;

/**
 * \brief SPI port type.
 */
typedef uint8_t spi_port_t;

/**
 * \brief SPI chip select type.
 */
typedef uint8_t spi_cs_t;

/**
 * \brief SPI port initialization.
 *
 * \param[in] port is the SPI port to initialize. It can be:
 * \parblock
 *      -\b SPI_PORT_0
 *      -\b SPI_PORT_1
 *      -\b SPI_PORT_2
 *      .
 * \endparblock
 *
 * \param[in] config is the configuration of the SPI port.
 *
 * \return The status/error code.
 */
int spi_init(spi_port_t port, spi_config_t config);

/**
 * \brief Transfer data over a SPI port (full-duplex operation).
 *
 * \param[in] port is the SPI port to transfer data. It can be:
 * \parblock
 *      -\b SPI_PORT_0
 *      -\b SPI_PORT_1
 *      -\b SPI_PORT_2
 *      .
 * \endparblock
 *
 * \param[in] cs is the chip select pin. It can be:
 * \parblock
 *      -\b SPI_CS_0
 *      -\b SPI_CS_1
 *      -\b SPI_CS_2
 *      -\b SPI_CS_3
 *      -\b SPI_CS_4
 *      -\b SPI_CS_5
 *      -\b SPI_CS_6
 *      -\b SPI_CS_7
 *      -\b SPI_CS_8
 *      -\b SPI_CS_9
 *      .
 * \endparblock
 *
 * \param[in] data_w is the data to write during the SPI transfer.
 *
 * \param[in] data_r is a pointer to store the read data during the SPI transfer.
 *
 * \param[in] len is the number of bytes to transfer.
 *
 * \return The status/error code.
 */
int spi_transfer(spi_port_t port, spi_cs_t cs, uint8_t *data_w, uint8_t *data_r, uint16_t len);

/**
 * \brief Writes data to a given SPI port.
 *
 * \param[in] port is the SPI port to write data. It can be:
 * \parblock
 *      -\b SPI_PORT_0
 *      -\b SPI_PORT_1
 *      -\b SPI_PORT_2
 *      .
 * \endparblock
 *
 * \param[in] cs is the chip select pin. It can be:
 * \parblock
 *      -\b SPI_CS_0
 *      -\b SPI_CS_1
 *      -\b SPI_CS_2
 *      -\b SPI_CS_3
 *      -\b SPI_CS_4
 *      -\b SPI_CS_5
 *      -\b SPI_CS_6
 *      -\b SPI_CS_7
 *      -\b SPI_CS_8
 *      -\b SPI_CS_9
 *      .
 * \endparblock
 *
 * \param[in] data is the data to write.
 *
 * \param[in] len is the number of bytes to write.
 *
 * \return The status/error code.
 */
int spi_write(spi_port_t port, spi_cs_t cs, uint8_t *data, uint16_t len);

/**
 * \brief Reads data from a given SPI port.
 *
 * \param[in] port is the SPI port to read data. It can be:
 * \parblock
 *      -\b SPI_PORT_0
 *      -\b SPI_PORT_1
 *      -\b SPI_PORT_2
 *      .
 * \endparblock
 *
 * \param[in] cs is the chip select pin. It can be:
 * \parblock
 *      -\b SPI_CS_0
 *      -\b SPI_CS_1
 *      -\b SPI_CS_2
 *      -\b SPI_CS_3
 *      -\b SPI_CS_4
 *      -\b SPI_CS_5
 *      -\b SPI_CS_6
 *      -\b SPI_CS_7
 *      -\b SPI_CS_8
 *      -\b SPI_CS_9
 *      .
 * \endparblock
 *
 * \param[in] data is a pointer to store the read data.
 *
 * \param[in] len is the number of bytes to read.
 *
 * \return The status/error code.
 */
int spi_read(spi_port_t port, spi_cs_t cs, uint8_t *data, uint16_t len);

#endif // SPI_H_

//! \} End of spi group
