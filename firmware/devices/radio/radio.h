/*
 * radio.h
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
 * \brief Radio device definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.1
 * 
 * \date 27/10/2019
 * 
 * \defgroup radio Radio
 * \ingroup devices
 * \{
 */

#ifndef RADIO_H_
#define RADIO_H_

#include <stdint.h>

/**
 * \brief Radio device initialization routine.
 *
 * \return The status/error code.
 */
int radio_init();

/**
 * \brief Writes data to the radio device.
 *
 * \param[in] data is an array of bytes.
 *
 * \param[in] len is the number of bytes to write.
 *
 * \return The status/error code.
 */
int radio_send(uint8_t *data, uint16_t len);

/**
 * \brief Reads data from the radio device (if available).
 *
 * \param[in] data is a pointer to store the read data.
 *
 * \param[in] len is the number of bytes to read.
 *
 * \return The number of read bytes (-1 on error).
 */
int radio_recv(uint8_t *data, uint16_t len);

/**
 * \brief Verifies the number of availables bytes to receive.
 *
 * \return The number of available bytes to receive (-1 on error).
 */
int radio_available();

#endif // RADIO_H_

//! \} End of radio group
