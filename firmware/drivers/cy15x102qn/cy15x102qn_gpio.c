/*
 * cy15x102qn_gpio.c
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
 * \brief CY15x102QN driver GPIO interface implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.29
 * 
 * \date 2021/04/18
 * 
 * \addtogroup cy15x102qn
 * \{
 */

#include <drivers/gpio/gpio.h>

#include "cy15x102qn.h"

int cy15x102qn_gpio_init(cy15x102qn_config_t *conf)
{
    gpio_config_t gpio_conf = {0};

    gpio_conf.mode = GPIO_MODE_OUTPUT;

    int err = -1;

    /* WP pin */
    if (gpio_init(conf->wp_pin, gpio_conf) == 0)
    {
        err = cy15x102qn_gpio_set_write_protect(conf);
    }

    return err;
}

int cy15x102qn_gpio_set_write_protect(cy15x102qn_config_t *conf)
{
    return gpio_set_state(conf->wp_pin, true);
}

int cy15x102qn_gpio_clear_write_protect(cy15x102qn_config_t *conf)
{
    return gpio_set_state(conf->wp_pin, false);
}

/** \} End of cy15x102qn group */
