/*
 * spi.h
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
 * \brief SPI driver definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.10.17
 * 
 * \date 2019/12/07
 * 
 * \defgroup spi SPI
 * \ingroup drivers
 * \{
 */

#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>
#include <stdbool.h>

#define SPI_MODULE_NAME         "SPI"

#define SPI_MUTEX_WAIT_TIME_MS    10000U

/**
 * \brief SPI ports.
 */
typedef enum
{
    SPI_PORT_0=0,       /**< SPI port 0. */
    SPI_PORT_1,         /**< SPI port 1. */
    SPI_PORT_2,         /**< SPI port 2. */
    SPI_PORT_3,         /**< SPI port 3. */
    SPI_PORT_4,         /**< SPI port 4. */
    SPI_PORT_5          /**< SPI port 5. */
} spi_port_t;

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
    SPI_CS_9,           /**< SPI chip select 9. */
    SPI_CS_NONE         /**< No SPI chip select. */
} spi_cs_t;

/**
 * \brief SPI modes.
 */
typedef enum
{
    SPI_MODE_0=0,       /**< SPI mode 0 (Clock Polarity = 0, Clock Phase = 0). */
    SPI_MODE_1,         /**< SPI mode 1 (Clock Polarity = 0, Clock Phase = 1). */
    SPI_MODE_2,         /**< SPI mode 2 (Clock Polarity = 1, Clock Phase = 0). */
    SPI_MODE_3          /**< SPI mode 3 (Clock Polarity = 1, Clock Phase = 1). */
} spi_mode_t;

/**
 * \brief SPI bus configuration parameters.
 */
typedef struct
{
    uint32_t speed_hz;  /**< Transfer rate in Hertz. */
    spi_mode_t mode;    /**< SPI mode (0, 1, 2 or 3). */
} spi_config_t;

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
 * \brief Selects or unselects an SPI device.
 *
 * \param[in] port is the SPI port of the device to select. It can be:
 * \parblock
 *      -\b SPI_PORT_0
 *      -\b SPI_PORT_1
 *      -\b SPI_PORT_2
 *      .
 * \endparblock
 *
 * \param[in] cs is the chip select pin of the device to select. It can be:
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
 *      -\b SPI_CS_NONE
 *      .
 * \endparblock
 *
 * \param[in] active is TRUE/FALSE to select/unselect the SPI device.
 *
 * \return The status/error code.
 */
int spi_select_slave(spi_port_t port, spi_cs_t cs, bool active);

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
 *      -\b SPI_CS_NONE
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
 *      -\b SPI_CS_NONE
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
 *      -\b SPI_CS_NONE
 *      .
 * \endparblock
 *
 * \param[in] wd is the data to write during the SPI transfer.
 *
 * \param[in] rd is a pointer to store the read data during the SPI transfer.
 *
 * \param[in] len is the number of bytes to transfer.
 *
 * \return The status/error code.
 */
int spi_transfer(spi_port_t port, spi_cs_t cs, uint8_t *wd, uint8_t *rd, uint16_t len);

/**
 * \brief Creates the SPI mutex.
 *
 * \return The status/error code.
 */
int spi_mutex_create(void);

/**
 * \brief Attempts to take the SPI mutex.
 *
 * \return The status/error code.
 */
int spi_mutex_take(void);

/**
 * \brief Gives the SPI mutex.
 *
 * \return The status/error code.
 */
int spi_mutex_give(void);

#endif /* SPI_H_ */

/** \} End of spi group */
