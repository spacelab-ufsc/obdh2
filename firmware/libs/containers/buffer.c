/*
 * buffer.c
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
 * \brief Buffer implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.14
 * 
 * \date 2017/11/09
 * 
 * \addtogroup buffer
 * \{
 */

#include "buffer.h"

void buffer_init(buffer_t *buffer)
{
    buffer->mtu = BUFFER_LENGTH;

    buffer_clear(buffer);
}

uint16_t buffer_length(buffer_t *buffer)
{
    return buffer->mtu;
}

bool buffer_fill(buffer_t *buffer, uint8_t *data, uint16_t len)
{
    bool res = false;

    if (len <= buffer_length(buffer))
    {
        buffer_clear(buffer);

        uint16_t i = 0;
        for(i = 0; i < len; i++)
        {
            buffer->data[i] = data[i];
        }

        buffer->size = len;

        res = true;
    }

    return res;
}

bool buffer_append(buffer_t *buffer, uint8_t *data, uint16_t len)
{
    bool res = false;

    if ((buffer->size + len) <= buffer_length(buffer))
    {
        uint16_t i = 0U;
        for(i = 0U; i < len; i++)
        {
            buffer->data[buffer->size + i] = data[i];
        }

        buffer->size += len;

        res = true;
    }

    return res;
}

void buffer_clear(buffer_t *buffer)
{
    uint16_t i = 0U;
    for(i = 0U; i < buffer_length(buffer); i++)
    {
        buffer->data[i] = BUFFER_DEFAULT_BYTE;
    }
    
    buffer->size = 0U;
}

bool buffer_empty(buffer_t *buffer)
{
    bool res = false;

    if (buffer_size(buffer) == 0U)
    {
        res = true;
    }

    return res;
}

bool buffer_full(buffer_t *buffer)
{
    bool res = false;

    if (buffer_size(buffer) == buffer_length(buffer))
    {
        res = true;
    }

    return res;
}

uint16_t buffer_size(buffer_t *buffer)
{
    return buffer->size;
}

/**< \} End of buffer group */
