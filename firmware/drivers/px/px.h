/*
 * px.h
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
 * \brief Payload X driver definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.44
 * 
 * \date 2020/10/31
 * 
 * \defgroup px Payload X
 * \ingroup drivers
 * \{
 */

#ifndef PX_H_
#define PX_H_

#include <stdint.h>

#include <drivers/i2c/i2c.h>

#define PX_SLAVE_ADDRESS            0x42    /**< 7-bit slave address. */

/**
 * \brief Payload X configuration parameters.
 */
typedef struct
{
    i2c_port_t port;
    uint32_t bitrate;
} px_config_t;

/**
 * \brief Device initialization.
 *
 * \param[in] port is the I2C port which the Payload-X is connected.
 *
 * \param[in] bitrate is the bitrate in bps.
 *
 * \return The status/error code.
 */
int px_init(i2c_port_t port, uint32_t bitrate);

/**
 * \brief Writes data to the Payload X.
 *
 * \param[in] data is an array of bytes to write.
 *
 * \param[in] len is the number of bytes to write.
 *
 * \return The status/error code.
 */
int px_write(uint8_t *data, uint16_t len);

/**
 * \brief Reads data from the Payload X.
 *
 * \param[in,out] data is a pointer to store the read data.
 *
 * \param[in] len is the number of bytes to read.
 *
 * \return The status/error code.
 */
int px_read(uint8_t *data, uint16_t len);

#endif /* PX_H_ */

/** \} End of px group */
