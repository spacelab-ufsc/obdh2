/*
 * adc.c
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
 * \brief ADC driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.2.17
 * 
 * \date 03/03/2020
 * 
 * \addtogroup adc
 * \{
 */

#include <hal/adc10_a.h>
#include <hal/adc12_a.h>

#include "adc.h"

int adc_init(adc_port_t port, adc_config_t config)
{
    return -1;
}

int adc_read(adc_port_t port, uint16_t *val)
{
    return -1;
}

/** \} End of i2c group */
