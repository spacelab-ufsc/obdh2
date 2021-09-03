/*
 * radio.h
 * 
 * Copyright (C) 2020, SpaceLab.
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
 * \brief Radio device definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.4
 * 
 * \date 2019/10/27
 * 
 * \defgroup radio Radio
 * \ingroup devices
 * \{
 */

#ifndef RADIO_H_
#define RADIO_H_

#include <stdint.h>

#define RADIO_MODULE_NAME           "Radio"

/**
 * \brief Radio device initialization routine.
 *
 * \return The status/error code.
 */
int radio_init(void);

/**
 * \brief Writes data to the radio device.
 *
 * \param[in] data is an array of bytes.
 *
 * \param[in] len is the number of bytes to write.
 *
 * \param[in] timeout_ms is the send timeout in milliseconds.
 *
 * \return The status/error code.
 */
int radio_send(uint8_t *data, uint16_t len, uint32_t timeout_ms);

/**
 * \brief Reads data from the radio device (if available) with timeout.
 *
 * \param[in] data is a pointer to store the read data.
 *
 * \param[in] len is the number of bytes to read.
 *
 * \param[in] timeout_ms is the timeout to received a packet in milliseconds.
 *
 * \return The number of read bytes (-1 on error).
 */
int radio_recv(uint8_t *data, uint16_t len, uint32_t timeout_ms);

/**
 * \brief Verifies the number of availables bytes to receive.
 *
 * \return The number of available bytes to receive (-1 on error).
 */
int radio_available(void);

/**
 * \brief Puts the radio in sleep mode.
 *
 * \return Tge status/error code.
 */
int radio_sleep(void);

#endif /* RADIO_H_ */

/** \} End of radio group */
