/*
 * px.c
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
 * \brief Payload X driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.3.5
 * 
 * \date 31/03/2020
 * 
 * \addtogroup px
 * \{
 */

#include "px.h"

/**
 * \brief Payload X I2C port.
 */
i2c_port_t px_i2c_port;

int px_init(px_config_t config)
{
    return -1;
}

int px_write(uint8_t *data, uint16_t len)
{
    return -1;
}

int px_read(uint8_t *data, uint16_t len)
{
    return -1;
}

/** \} End of px group */
