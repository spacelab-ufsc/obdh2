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
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 0.10.19
 * 
 * \date 2020/03/31
 * 
 * \addtogroup px
 * \{
 */

#include <stdbool.h>

#include "px.h"

int px_init(const px_config_t *conf)
{
    const gpio_config_t gpio_conf = { .mode = GPIO_MODE_OUTPUT };
    const i2c_config_t i2c_conf = { .speed_hz = conf->bitrate };

    (void)gpio_init(conf->en_pin, gpio_conf);

    return i2c_init(conf->port, i2c_conf);
}

int px_write(const px_config_t *conf, uint8_t *data, uint16_t len)
{
    return i2c_write(conf->port, PX_SLAVE_ADDRESS, data, len);
}

int px_read(const px_config_t *conf, uint8_t *data, uint16_t len)
{
    return i2c_read(conf->port, PX_SLAVE_ADDRESS, data, len);
}

int px_enable(const px_config_t *conf)
{
    return gpio_set_state(conf->en_pin, true);
}

int px_disable(const px_config_t *conf)
{
    return gpio_set_state(conf->en_pin, false);
}

/** \} End of px group */
