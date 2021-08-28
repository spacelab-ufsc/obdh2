/*
 * temp_sensor_wrap.c
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
 * \brief Temperature sensor device wrap implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.2
 * 
 * \date 2021/08/15
 * 
 * \addtogroup temp_sensor_wrap
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include "temp_sensor_wrap.h"

int __wrap_temp_sensor_init(void)
{
    return mock_type(int);
}

int __wrap_temp_sensor_read_raw(uint16_t *val)
{
    if (val != NULL)
    {
        *val = mock_type(uint16_t);
    }

    return mock_type(int);
}

int16_t __wrap_temp_sensor_raw_to_c(uint16_t raw)
{
    check_expected(raw);

    return mock_type(int16_t);
}

uint16_t __wrap_temp_sensor_raw_to_k(uint16_t raw)
{
    check_expected(raw);

    return mock_type(uint16_t);
}

int __wrap_temp_sensor_read_c(int16_t *temp)
{
    if (temp != NULL)
    {
        *temp = mock_type(int16_t);
    }

    return mock_type(int);
}

int __wrap_temp_sensor_read_k(uint16_t *temp)
{
    if (temp != NULL)
    {
        *temp = mock_type(uint16_t);
    }

    return mock_type(int);
}

/** \} End of temp_sensor_wrap group */
