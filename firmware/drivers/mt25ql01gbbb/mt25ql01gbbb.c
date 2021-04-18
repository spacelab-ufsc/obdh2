/*
 * mt25ql01gbbb.c
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
 * \brief MT25QL01GBBB driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.5.23
 * 
 * \date 2019/11/15
 * 
 * \addtogroup mt25ql01gbbb
 * \{
 */

#include "mt25ql01gbbb.h"

int mt25ql01gbbb_init(void)
{
    if (mt25ql01gbbb_gpio_init() != 0)
    {
        return -1;
    }

    return 0;
}

int mt25ql01gbbb_write(uint32_t adr, uint8_t *data, uint32_t len)
{
    return -1;
}

int mt25ql01gbbb_read(uint32_t adr, uint8_t *data, uint32_t len)
{
    return -1;
}

/** \} End of mt25ql01gbbb group */
