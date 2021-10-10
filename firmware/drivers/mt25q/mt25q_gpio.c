/*
 * mt25q_gpio.c
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
 * \brief MT25Q driver GPIO interface implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.42
 * 
 * \date 2021/04/18
 * 
 * \addtogroup mt25q
 * \{
 */

#include <drivers/gpio/gpio.h>

#include "mt25q.h"

#define MT25Q_GPIO_HOLD_PIN         GPIO_PIN_26
#define MT25Q_GPIO_RESET_PIN        GPIO_PIN_27

int mt25q_gpio_init(void)
{
    int err = -1;

    gpio_config_t conf = {0};

    conf.mode = GPIO_MODE_OUTPUT;

    /* HOLD pin */
    if (gpio_init(MT25Q_GPIO_HOLD_PIN, conf) == 0)
    {
        if (mt25q_gpio_set_hold(true) == 0)
        {
            /* RESET pin */
            if (gpio_init(MT25Q_GPIO_RESET_PIN, conf) == 0)
            {
                if (mt25q_gpio_set_reset(true) == 0)
                {
                    err = 0;
                }
            }
        }
    }

    return err;
}

int mt25q_gpio_set_hold(bool state)
{
    return gpio_set_state(MT25Q_GPIO_HOLD_PIN, state);
}

int mt25q_gpio_set_reset(bool state)
{
    return gpio_set_state(MT25Q_GPIO_RESET_PIN, state);
}

/** \} End of mt25q group */
