/*
 * queue.c
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
 * \brief Queue implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 0.1.16
 * 
 * \date 2017/11/08
 * 
 * \addtogroup queue
 * \{
 */

#include "queue.h"

void queue_init(queue_t *queue)
{
    queue->head = 0U;
    queue->tail = 0U;
    queue->size = 0U;
    queue->mtu = QUEUE_LENGTH;
    
    uint16_t i = 0U;
    for(i = 0U; i < queue_length(queue); i++)
    {
        queue->data[i] = QUEUE_DEFAULT_BYTE;
    }
}

uint16_t queue_length(queue_t *queue)
{
    return queue->mtu;
}

bool queue_push_back(queue_t *queue, uint8_t byte)
{
    bool res = false;

    if (!queue_full(queue))
    {
        queue->data[queue->tail] = byte;
        queue->tail++;
        queue->size++;

        if (queue->tail == queue_length(queue))
        {
            queue->tail = 0U;
        }

        res = true;
    }

    return res;
}

uint8_t queue_pop_front(queue_t *queue)
{
    uint8_t res = 0U;

    if (queue_empty(queue))
    {
        res = QUEUE_DEFAULT_BYTE;
    }
    else
    {
        uint8_t byte = queue->data[queue->head];
        queue->head++;

        if (queue->head == queue_length(queue))
        {
            queue->head = 0U;
        }

        res = byte;
        queue->size--;
    }

    return res;
}

bool queue_empty(queue_t *queue)
{
    bool res = false;

    if (queue->head == queue->tail)
    {
        res = true;
    }

    return res;
}

bool queue_full(queue_t *queue)
{
    bool res = false;

    if (((queue->tail + 1U) == queue->head) || (((queue->tail + 1U) == queue_length(queue)) && (queue->head == 0U)))
    {
        res = true;
    }

    return res;
}

uint16_t queue_size(queue_t *queue)
{
    return queue->size;
}

void queue_clear(queue_t *queue)
{
    queue_init(queue);
}

/**< \} End of queue group */
