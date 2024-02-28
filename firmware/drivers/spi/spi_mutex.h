/*
 * spi_mutex.h
 *
 * Copyright The OBDH 2.0 Contributors.
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
 * along with OBDH 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 *
 */

/**
 * \brief SPI mutex definition.
 *
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 *
 * \version 0.10.10
 *
 * \date 2024/02/27
 *
 * \defgroup spi_mutex SPI Mutex
 * \ingroup spi
 * \{
 */

#ifndef DRIVERS_SPI_SPI_MUTEX_H_
#define DRIVERS_SPI_SPI_MUTEX_H_

#define SPI_MUTEX_WAIT_TIME_MS    50

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

#endif /* DRIVERS_SPI_SPI_MUTEX_H_ */

/** \} End of spi_mutex group */
