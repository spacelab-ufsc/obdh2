/*
 * eps.c
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
 * \brief EPS device implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.18
 * 
 * \date 01/02/2020
 * 
 * \addtogroup eps
 * \{
 */

#include <drivers/sl_eps2/sl_eps2.h>

#include "eps.h"

int eps_init()
{
    return -1;
}

int eps_get_bat_voltage(eps_bat_voltage_t *bat_volt)
{
    return -1;
}

int eps_get_bat_current(uint32_t *bat_cur)
{
    return -1;
}

int eps_get_bat_charge(uint32_t *charge)
{
    return -1;
}

int eps_get_data(eps_data_t *data)
{
    return -1;
}

/** \} End of eps group */
