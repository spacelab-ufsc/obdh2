/*
 * si446x_gpio.c
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
 * \brief Si446x GPIO implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.3.23
 * 
 * \date 14/05/2020
 * 
 * \defgroup si446x_gpio GPIO
 * \ingroup si446x
 * \{
 */

#include <drivers/gpio/gpio.h>

#include "si446x.h"

int si446x_gpio_init()
{
    /* SDN pin */
    if (gpio_init(GPIO_PIN_4, (gpio_config_t){.mode=GPIO_MODE_OUTPUT}) != 0)
    {
        return -1;
    }

    /* nIRQ pin */
    return gpio_init(GPIO_PIN_3, (gpio_config_t){.mode=GPIO_MODE_INPUT});
}

int si446x_gpio_write_sdn(bool state)
{
    return gpio_set_state(GPIO_PIN_4, state);
}

int si446x_gpio_read_nirq()
{
    return gpio_get_state(GPIO_PIN_3);
}

/** \} End of si446x_gpio group */
