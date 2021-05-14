/*
 * ttc.c
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
 * \brief TTC device implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.6.17
 * 
 * \date 2020/02/01
 * 
 * \addtogroup ttc
 * \{
 */

#include "ttc.h"

int ttc_init(ttc_config_t config)
{
    return -1;
}

int ttc_get_data(ttc_config_t config, ttc_data_t *data)
{
    return -1;
}

int ttc_send(ttc_config_t config, uint8_t *data, uint16_t len)
{
    return -1;
}

int ttc_avail(ttc_config_t config)
{
    return -1;
}

int ttc_enter_hibernation(ttc_config_t config)
{
    return -1;
}

int ttc_leave_hibernation(ttc_config_t config)
{
    return -1;
}

/** \} End of ttc group */
