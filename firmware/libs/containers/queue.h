/*
 * queue.h
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
 * \brief Queue definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.16
 * 
 * \date 2017/11/08
 * 
 * \defgroup queue Queue
 * \ingroup containers
 * \{
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdint.h>
#include <stdbool.h>

#define QUEUE_LENGTH            300U    /**< Queue length in bytes. */

#define QUEUE_DEFAULT_BYTE      0xFFU   /**< Queue default byte (empty position). */

/**
 * \brief Queue representation as a struct.
 */
typedef struct
{
    uint8_t data[QUEUE_LENGTH];         /**< Data buffer. */
    uint16_t head;                      /**< Head position of the data array. */
    uint16_t tail;                      /**< Tail position of the data array. */
    uint16_t size;                      /**< Length, in bytes, of the data. */
    uint16_t mtu;                       /**< Maximum transmission unit. */
} queue_t;

/**
 * \brief Queue initialization.
 * 
 * \param[in,out] queue is a pointer to a queue_t struct.
 * 
 * \return None.
 */
void queue_init(queue_t *queue);

/**
 * \brief Returns the length (capacity) of a queue.
 * 
 * \param[in,out] queue is a pointer to a queue_t struct.
 * 
 * \return The length of the queue (or capacity).
 */
uint16_t queue_length(queue_t *queue);

/**
 * \brief Puts an element into the back position of an queue.
 * 
 * \param[in,out] queue is a pointer to a queue_t struct.
 *
 * \param[in] byte is the byte to be pushed to the queue.
 * 
 * \return True/False if the element was pushed or not.
 */
bool queue_push_back(queue_t *queue, uint8_t byte);

/**
 * \brief Grabs an element from the front position of an queue.
 * 
 * \param[in,out] queue is a pointer to a queue_t struct.
 * 
 * \return The byte grabbed from the queue.
 */
uint8_t queue_pop_front(queue_t *queue);

/**
 * \brief Verifies if the a queue is empty or not.
 * 
 * \param[in,out] queue is a pointer to a queue_t struct.
 * 
 * \return True/False if the queue is empty or not.
 */
bool queue_empty(queue_t *queue);

/**
 * \brief Verifies if the a queue is full or not.
 * 
 * \param[in,out] queue is a pointer to a queue_t struct.
 * 
 * \return True/False if the queue is full or not.
 */
bool queue_full(queue_t *queue);

/**
 * \brief Returns the size of a queue.
 * 
 * \param[in,out] queue is a pointer to a queue_t struct.
 * 
 * \return The size of the queue.
 */
uint16_t queue_size(queue_t *queue);

/**
 * \brief Resets queue size to zero.
 *
 * \param[in,out] queue is a pointer to a queue_t struct.
 *
 * \return None.
 */
void queue_clear(queue_t *queue);

#endif /* QUEUE_H_ */

/**< \} End of queue group */
