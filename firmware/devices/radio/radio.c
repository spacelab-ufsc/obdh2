/*
 * radio.c
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
 * \brief Radio device implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.2.12
 * 
 * \date 27/10/2019
 * 
 * \addtogroup radio
 * \{
 */

#include <drivers/si446x/si446x.h>

#include <devices/logger/logger.h>

#include "radio.h"

int radio_init()
{
    logger_print_event_from_module(LOGGER_INFO, RADIO_MODULE_NAME, "Initializing radio device...");
    logger_new_line();

    return si446x_init();
}

int radio_send(uint8_t *data, uint16_t len)
{
    return -1;
}

int radio_recv(uint8_t *data, uint16_t len)
{
    return -1;
}

int radio_available()
{
    return -1;
}

/** \} End of radio group */
