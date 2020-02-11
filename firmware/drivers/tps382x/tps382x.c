/*
 * tps382x.c
 * 
 * Copyright (C) 2019, SpaceLab.
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
 * \brief TPS382x driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.0
 * 
 * \date 15/01/2020
 * 
 * \addtogroup tps382x
 * \{
 */

#include "tps382x.h"

int tps382x_init(tps382x_config_t config)
{
    return gpio_init(config.wdi_pin, (gpio_config_t){.mode=GPIO_MODE_OUTPUT});
}

void tps382x_trigger(tps382x_config_t config)
{
    gpio_toggle(config.wdi_pin);
}

//! \} End of tps382x group
