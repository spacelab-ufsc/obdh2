/*
 * ttc_wrap.c
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
 * \brief TTC device wrap implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.2
 * 
 * \date 2021/08/15
 * 
 * \addtogroup ttc_wrap
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include "ttc_wrap.h"

int __wrap_ttc_init(ttc_e dev)
{
    check_expected(dev);

    return mock_type(int);
}

int __wrap_ttc_get_data(ttc_e dev, ttc_data_t *data)
{
    check_expected(dev);

    return mock_type(int);
}

int __wrap_ttc_send(ttc_e dev, uint8_t *data, uint16_t len)
{
    check_expected(dev);
    check_expected_ptr(data);
    check_expected(len);

    return mock_type(int);
}

int __wrap_ttc_recv(ttc_e dev, uint8_t *data, uint16_t *len)
{
    check_expected(dev);

    if (data != NULL)
    {
        *data = mock_ptr_type(uint8_t*);
    }

    if (len != NULL)
    {
        *len = mock_type(uint16_t);
    }

    return mock_type(int);
}

int __wrap_ttc_avail(ttc_e dev)
{
    check_expected(dev);

    return mock_type(int);
}

int __wrap_ttc_enter_hibernation(ttc_e dev)
{
    check_expected(dev);

    return mock_type(int);
}

int __wrap_ttc_leave_hibernation(ttc_e dev)
{
    check_expected(dev);

    return mock_type(int);
}

/** \} End of ttc_wrap group */
