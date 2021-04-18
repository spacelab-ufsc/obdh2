/*
 * mt25ql01gbbb_gpio.c
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
 * \brief MT25QL01GBBB driver GPIO interface implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.5.23
 * 
 * \date 2021/04/18
 * 
 * \addtogroup mt25ql01gbbb
 * \{
 */

#include <drivers/gpio/gpio.h>

#include "mt25ql01gbbb.h"

#define MT25QL01GBBB_GPIO_HOLD_PIN      GPIO_PIN_26
#define MT25QL01GBBB_GPIO_RESET_PIN     GPIO_PIN_27

int mt25ql01gbbb_gpio_init(void)
{
    gpio_config_t conf = {0};

    conf.mode = GPIO_MODE_OUTPUT;

    if (gpio_init(MT25QL01GBBB_GPIO_HOLD_PIN, conf) != 0)
    {
        return -1;
    }

    mt25ql01gbbb_gpio_set_hold(false);

    if (gpio_init(MT25QL01GBBB_GPIO_RESET_PIN, conf) != 0)
    {
        return -1;
    }

    mt25ql01gbbb_gpio_set_reset(true);

    return 0;
}

int mt25ql01gbbb_gpio_set_hold(bool state)
{
    return gpio_set_state(MT25QL01GBBB_GPIO_HOLD_PIN, state);
}

int mt25ql01gbbb_gpio_set_reset(bool state)
{
    return gpio_set_state(MT25QL01GBBB_GPIO_RESET_PIN, state);
}

/** \} End of mt25ql01gbbb group */
