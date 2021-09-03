/*
 * voltage_sensor_wrap.c
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
 * \brief Voltage sensor device wrap implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.2
 * 
 * \date 2021/08/15
 * 
 * \addtogroup voltage_sensor_wrap
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include "voltage_sensor_wrap.h"

int __wrap_voltage_sensor_init(void);
{
    return mock_type(int);
}

int __wrap_voltage_sensor_read_raw(uint16_t *val);
{
    if (val != NULL)
    {
        *val = mock_type(uint16_t);
    }

    return mock_type(int);
}

uint16_t __wrap_voltage_sensor_raw_to_mv(uint16_t raw);
{
    check_expected(raw);

    return mock_type(uint16_t);
}

int __wrap_voltage_sensor_read_mv(uint16_t *volt);
{
    if (val != NULL)
    {
        *val = mock_type(uint16_t);
    }

    return mock_type(int);
}

/** \} End of voltage_sensor_wrap group */
