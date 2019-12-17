/*
 * i2c.h
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
 * \brief I2C driver definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.0
 * 
 * \date 07/12/2019
 * 
 * \defgroup i2c I2C
 * \ingroup drivers
 * \{
 */

#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>

/**
 * \brief I2C ports.
 */
typedef enum
{
    I2C_PORT_0=0,       /**< I2C port 0. */
    I2C_PORT_1,         /**< I2C port 1. */
    I2C_PORT_2          /**< I2C port 2. */
} i2c_port_e;

/**
 * \brief I2C bus configuration parameters.
 */
typedef struct
{
    uint32_t speed;     /**< I2C clock in Hz. */
} i2c_config_t;

/**
 * \brief I2C port.
 */
typedef i2c_port_t uint8_t;

/**
 * \brief I2C slave 7-bit address.
 */
typedef i2c_slave_adr_t uint8_t;

/**
 * \brief I2C interface initialization.
 *
 * \param[in] port is the I2C port to initialize. It can be:
 * \parblock
 *      -\b I2C_PORT_0
 *      -\b I2C_PORT_1
 *      -\b I2C_PORT_2
 * \endparblock
 *
 * \param[in] config is the configuration of the I2C port.
 *
 * \return The status/error code.
 */
int i2c_init(i2c_port_t port, i2c_config_t config);

/**
 * \brief Writes data to a given I2C port and address.
 *
 * \param[in] port is the I2C port to initialize. It can be:
 * \parblock
 *      -\b I2C_PORT_0
 *      -\b I2C_PORT_1
 *      -\b I2C_PORT_2
 * \endparblock
 *
 * \param[in] adr is the 7-bit slave address to write.
 *
 * \param[in] data is the data to write.
 *
 * \param[in] len is the number of bytes to write.
 *
 * \return The status/error code.
 */
int i2c_write(i2c_port_t port, i2c_slave_adr_t adr, uint8_t *data, uint16_t len);

/**
 * \brief Reads data from a given I2C port and address.
 *
 * \param[in] port is the I2C port to initialize. It can be:
 * \parblock
 *      -\b I2C_PORT_0
 *      -\b I2C_PORT_1
 *      -\b I2C_PORT_2
 * \endparblock
 *
 * \param[in] adr is the 7-bit slave address to read.
 *
 * \param[in] data is a pointer to store the read data.
 *
 * \param[in] len is the number of bytes to read.
 *
 * \return The status/error code.
 */
int i2c_read(i2c_port_t port, i2c_slave_adr_t adr, uint8_t *data, uint16_t len);

#endif // I2C_H_

//! \} End of i2c group
