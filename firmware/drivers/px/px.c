/*
 * px.c
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
 * along with OBDH 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Payload X driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.10.8
 * 
 * \date 2020/03/31
 * 
 * \addtogroup px
 * \{
 */

#include "px.h"

int px_init(px_config_t conf)
{
    i2c_config_t i2c_conf = {0};

    i2c_conf.speed_hz = bitrate;

    return i2c_init(conf.port, i2c_conf);
}

int px_write(px_config_t conf, uint8_t *data, uint16_t len)
{
    return i2c_write(conf.port, PX_SLAVE_ADDRESS, data, len);
}

int px_read(px_config_t conf, uint8_t *data, uint16_t len)
{
    return i2c_read(conf.port, PX_SLAVE_ADDRESS, data, len);
}

/** \} End of px group */
