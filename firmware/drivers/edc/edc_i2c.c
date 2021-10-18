/*
 * edc_i2c.c
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
 * along with OBDH 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief EDC driver I2C routines implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.8.14
 * 
 * \date 2021/10/16
 * 
 * \addtogroup edc
 * \{
 */

#include <drivers/i2c/i2c.h>

#include "edc.h"

int edc_i2c_init(edc_config_t config)
{
    i2c_config_t i2c_conf = {0};

    i2c_conf.speed_hz = config.bitrate;

    return i2c_init(config.port, i2c_conf);
}

int edc_i2c_write(edc_config_t config, uint8_t *data, uint16_t len)
{
    return i2c_write(config.port, EDC_SLAVE_ADDRESS, data, len);
}

int edc_i2c_read(edc_config_t config, uint8_t *data, uint16_t len)
{
    return i2c_read(config.port, EDC_SLAVE_ADDRESS, data, len);
}

/** \} End of edc group */
