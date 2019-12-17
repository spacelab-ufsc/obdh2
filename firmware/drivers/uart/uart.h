/*
 * uart.h
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
 * \brief UART driver definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.0
 * 
 * \date 07/12/2019
 * 
 * \defgroup uart UART
 * \ingroup drivers
 * \{
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>

/**
 * \brief UART ports.
 */
typedef enum
{
    UART_PORT_0=0,          /**< UART port 0. */
    UART_PORT_1,            /**< UART port 1. */
    UART_PORT_2             /**< UART port 2. */
};

/**
 * \brief Parity configuration.
 */
typedef enum
{
    UART_NO_PARITY=0,       /**< No parity. */
    UART_ODD_PARITY,        /**< Odd parity. */
    UART_EVEN_PARITY        /**< Even parity. */
} uart_parity_e;

/**
 * \brief Number of stop bits.
 */
typedef enum
{
    UART_ONE_STOP_BIT=1,    /**< One stop bit. */
    UART_TWO_STOP_BITS      /**< Two stop bits. */
};

/**
 * \brief UART port configuration parameters.
 */
typedef struct
{
    uint32_t baudrate;      /**< Baudrate in bps. */
    uint8_t data_bits;      /**< Data bits length. */
    uint8_t parity;         /**< Parity bits. */
    uint8_t stop_bits;      /**< Stop bits. */
} uart_config_t;

/**
 * \brief UART port.
 */
typedef uart_port_t uint8_t;

/**
 * \brief UART interface initialization.
 *
 * \param[in] port is the UART port to initialize. It can be:
 * \parblock
 *      -\b UART_PORT_0
 *      -\b UART_PORT_1
 *      -\b UART_PORT_2
 *      .
 * \endparblock
 *
 * \param[in] config is the UART port configuration.
 *
 * \return The status/error code.
 */
int uart_init(uart_port_t port, uart_config_t config);

/**
 * \brief Writes data to a given UART port.
 *
 * \param[in] port is the UART port to write. It can be:
 * \parblock
 *      -\b UART_PORT_0
 *      -\b UART_PORT_1
 *      -\b UART_PORT_2
 *      .
 * \endparblock
 *
 * \param[in] data is the data to write to the UART port.
 *
 * \param[in] len is the number of bytes to write.
 *
 * \return The status/error code.
 */
int uart_write(uart_port_t port, uint8_t *data, uint16_t len);

/**
 * \brief Reads data from a given UART port.
 *
 * \param[in] port is the UART port to read. It can be:
 * \parblock
 *      -\b UART_PORT_0
 *      -\b UART_PORT_1
 *      -\b UART_PORT_2
 *      .
 * \endparblock
 *
 * \param[in] data is an array to store the read data.
 *
 * \param[in] len is the number of bytes to read.
 *
 * \return The status/error code.
 */
int uart_read(uart_port_t port, uint8_t *data, uint16_t len);

#endif // UART_H_

//! \} End of uart group
