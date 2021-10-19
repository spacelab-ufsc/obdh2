/*
 * edc_gpio.c
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
 * \brief EDC driver GPIO routines implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.8.15
 * 
 * \date 2021/10/16
 * 
 * \addtogroup edc
 * \{
 */

#include <config/config.h>
#include <system/sys_log/sys_log.h>

#include "edc.h"

int edc_gpio_init(edc_config_t config)
{
    gpio_config_t conf = {0};

    conf.mode = GPIO_MODE_OUTPUT;

    return gpio_init(config.en_pin, conf);
}

int edc_gpio_set(edc_config_t config)
{
    return gpio_set_state(config.en_pin, true);
}

int edc_gpio_clear(edc_config_t config)
{
    return gpio_set_state(config.en_pin, false);
}

/** \} End of edc group */
