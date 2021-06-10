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
 * along with OBDH 2.0. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief CY15x102QN driver GPIO interface implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.1
 * 
 * \date 2021/04/18
 * 
 * \addtogroup cy15x102qn
 * \{
 */

#include <drivers/gpio/gpio.h>

#include "cy15x102qn.h"

#define CY15X102QN_GPIO_WP_PIN      GPIO_PIN_62

int cy15x102qn_gpio_init(void)
{
    gpio_config_t conf = {0};

    conf.mode = GPIO_MODE_OUTPUT;

    /* WP pin */
    if (gpio_init(CY15X102QN_GPIO_WP_PIN, conf) != 0)
    {
        return -1;
    }

    return cy15x102qn_gpio_set_write_protect();
}

int cy15x102qn_gpio_set_write_protect(void)
{
    return gpio_set_state(CY15X102QN_GPIO_WP_PIN, true);
}

int cy15x102qn_gpio_clear_write_protect(void)
{
    return gpio_set_state(CY15X102QN_GPIO_WP_PIN, false);
}

/** \} End of cy15x102qn group */
