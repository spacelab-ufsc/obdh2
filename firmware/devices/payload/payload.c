/*
 * payload.c
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
 * \brief Payload device implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.3
 * 
 * \date 2021/08/15
 * 
 * \addtogroup payload
 * \{
 */

#include <system/sys_log/sys_log.h>

#include "payload.h"

int payload_init(payload_t pl)
{
    return -1;
}

int payload_enable(payload_t pl)
{
    return -1;
}

int payload_disable(payload_t pl)
{
    return -1;
}

int payload_write_cmd(payload_t pl, payload_cmd_t cmd)
{
    return -1;
}

int payload_get_data(payload_t pl, uint8_t *data, uint32_t *len)
{
    return -1;
}

/** \} End of payload group */
