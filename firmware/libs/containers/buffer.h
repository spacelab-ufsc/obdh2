/*
 * buffer.h
 * 
 * Copyright The TTC 2.0 Contributors.
 * 
 * This file is part of TTC 2.0.
 * 
 * TTC 2.0 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * TTC 2.0 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with TTC 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Buffer definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.14
 * 
 * \date 2017/11/09
 * 
 * \defgroup buffer Buffer
 * \ingroup containers
 * \{
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#include <stdint.h>
#include <stdbool.h>

#define BUFFER_LENGTH           300U    /**< Buffer length in bytes. */

#define BUFFER_DEFAULT_BYTE     0xFFU   /**< Buffer length in bytes. */

/**
 * \brief Buffer implementation as a struct.
 */
typedef struct
{
    uint8_t data[BUFFER_LENGTH];        /**< Data of the buffer. */
    uint16_t size;                      /**< Number of elements into the buffer. */
    uint16_t mtu;                       /**< Maximum transmission unit. */
} buffer_t;

/**
 * \brief Buffer initialization.
 * 
 * \param[in,out] buffer is a pointer to a Buffer struct.
 * 
 * \return None.
 */
void buffer_init(buffer_t *buffer);

/**
 * \brief Returns the length (capacity) of a buffer.
 * 
 * \param buffer is a pointer to a Buffer struct.
 * 
 * \return The length of the buffer (or capacity).
 */
uint16_t buffer_length(buffer_t *buffer);

/**
 * \brief Fills the buffer with data.
 * 
 * \param[in,out] buffer is a pointer to a Buffer struct.
 *
 * \param[in,out] data is a pointer to an array with the data to store in the buffer.
 *
 * \param[in] len is the length of data.
 * 
 * \return True/False if the length of the data fits into the buffer.
 */
bool buffer_fill(buffer_t *buffer, uint8_t *data, uint16_t len);

/**
 * \brief Append data to a buffer.
 * 
 * \param[in,out] buffer is a pointer to a Buffer struct.
 *
 * \param[in,out] data is a pointer to an array with the data to append in the buffer.
 *
 * \param[in] len is the length of data.
 * 
 * \return True/False if the length of the data fits into the buffer.
 */
bool buffer_append(buffer_t *buffer, uint8_t *data, uint16_t len);

/**
 * \brief Clear a buffer filling it with the dafult byte (\b BUFFER_DEFAULT_BYTE).
 * 
 * \param[in,out] buffer is a pointer to a Buffer struct.
 * 
 * \return None
 */
void buffer_clear(buffer_t *buffer);

/**
 * \brief Verifies if the a buffer is empty or not.
 * 
 * \param[in,out] buffer is a pointer to a Buffer struct.
 * 
 * \return True/False if the buffer is empty or not.
 */
bool buffer_empty(buffer_t *buffer);

/**
 * \brief Verifies if the a buffer is full or not.
 * 
 * \param[in,out] buffer is a pointer to a Buffer struct.
 * 
 * \return True/False if the buffer is full or not.
 */
bool buffer_full(buffer_t *buffer);

/**
 * \brief Returns the size of a buffer.
 * 
 * \param[in,out] buffer is a pointer to a Buffer struct.
 * 
 * \return The size of the buffer.
 */
uint16_t buffer_size(buffer_t *buffer);

#endif /* BUFFER_H_ */

/**< \} End of buffer group */
