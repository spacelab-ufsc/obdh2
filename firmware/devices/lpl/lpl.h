/*
 * lpl.h
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
 * along with OBDH 2.0. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief LPL device definition.
 * 
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 1.0.0
 * 
 * \date 2025/09/28
 * 
 * \defgroup lpl LPL
 * \ingroup devices
 * \{
 */

#ifndef LPL_H_
#define LPL_H_

#include <stdint.h>

#include <drivers/uart/uart.h>
#include <drivers/gpio/gpio.h>

#define PAYLOAD_MODULE_NAME "LPL"

typedef struct {
    uart_config_t uart_conf;
    uart_port_t uart_port;
    gpio_pin_t en_pin;
} lpl_t;

/**
 * \brief LPL device initialization.
 *
 * \param[in] dev is the LPL device to initialize.
 *
 * \return The status/error code.
 */
int lpl_init(lpl_t *dev);

/**
 * \brief Enables LPL device through GPIO pin.
 *
 * \param[in] dev is the LPL device.
 *
 * \return The status/error code.
 */
int lpl_enable(const lpl_t *dev);

/**
 * \brief Disables LPL device through GPIO pin.
 *
 * \param[in] dev is the LPL device.
 *
 * \return The status/error code.
 */
int lpl_disable(const lpl_t *dev);

/**
 * \brief Checks if there is any data available from LPL device.
 *
 * \param[in] dev is the LPL device.
 *
 * \return The number of bytes available to read or an error code (if negative).
 */
int lpl_available(const lpl_t *dev);

/**
 * \brief Reads data from LPL device.
 *
 * \param[in] dev is the LPL device.
 *
 * \param[out] buf is the buffer to store the data.
 *
 * \param[in] size is the number of bytes to read.
 *
 * \return The status/error code.
 */
int lpl_read(lpl_t *dev, uint8_t *buf, const uint16_t size);

#endif /* PAYLOAD_H_ */

/** \} End of payload group */
