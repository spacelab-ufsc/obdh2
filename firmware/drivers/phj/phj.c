/*
 * phj.c
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

/*
 * \brief PHJ driver implementation.
 * 
 * \author João Cláudio Elsen Barcellos <joaoclaudiobarcellos@gmail.com>
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.8.9
 *
 * \date 2020/03/30
 *
 * \addtogroup phj
 * \{
 */

#include "phj.h"

static i2c_port_t phj_i2c_port;
static gpio_pin_t phj_gpio_pin;

/* I2C configuration */
int phj_init_i2c(phj_config_i2c_t config)
{
    phj_i2c_port = config.port;

    i2c_config_t i2c_conf = {0};

    i2c_conf.speed_hz = config.bitrate;

    return i2c_init(config.port, i2c_conf);
}

/* GPIO configuration */
int phj_init_gpio(phj_config_gpio_t config)
{
    phj_gpio_pin = config.pin;

    gpio_config_t gpio_conf = {0};

    gpio_conf.mode = config.mode;

    return gpio_init(config.pin, gpio_conf);
}

/* Read data from Payload PHJ */
int phj_read(uint8_t *data, uint16_t len)
{
    return i2c_read(phj_i2c_port, PHJ_SLAVE_ADDRESS, data, len);
}

/* Check the converter's health */
int phj_check_converter(void)
{
    return gpio_get_state(phj_gpio_pin);
}

/* Check for comunication errors */
int phj_check_message(void)
{
    int err = 0;

    uint8_t status[PHJ_FRAME_STATE_LEN] = {0};

    if (phj_read(status, PHJ_FRAME_STATE_LEN) != 0)
    {
        err = -1;
    }

    if (status[0] != PHJ_FRAME_ID_STATE)
    {
        err = -2;
    }

    return err;
}

/** \} End of phj group */
