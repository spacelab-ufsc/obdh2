/*
 * sl_eps2.c
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
 * \brief SpaceLab EPS 2.0 driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.15
 * 
 * \date 05/02/2020
 * 
 * \addtogroup sl_eps2
 * \{
 */

#include <drivers/spi/spi.h>

#include "eps.h"

int sl_eps2_init()
{
    return -1;
}

int sl_eps2_check_device()
{
    return -1;
}

int sl_eps2_write_reg(uint16_t adr, uint32_t val)
{
    return -1;
}

int sl_eps2_read_reg(uint16_t adr, uint32_t *val)
{
    return -1;
}

//! \} End of sl_eps2 group
