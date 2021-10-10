/*
 * adc_wrap.c
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
 * \brief ADC driver wrap implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.28
 * 
 * \date 2021/02/13
 * 
 * \addtogroup adc_wrap
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include "adc_wrap.h"

int __wrap_adc_init(void)
{
    return mock_type(int);
}

int __wrap_adc_read(adc_port_t port, uint16_t *val)
{
    check_expected(port);

    if (val != NULL)
    {
        *val = mock_type(uint16_t);
    }

    return mock_type(int);
}

float __wrap_adc_temp_get_mref(void)
{
    return mock_type(float);
}

float __wrap_adc_temp_get_nref(void)
{
    return mock_type(float);
}

/** \} End of adc_wrap group */
