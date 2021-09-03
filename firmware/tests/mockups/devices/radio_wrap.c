/*
 * radio_wrap.c
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
 * \brief Radio device wrap implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.2
 * 
 * \date 2021/08/15
 * 
 * \addtogroup radio_wrap
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include "radio_wrap.h"

int __wrap_radio_init(void)
{
    return mock_type(int);
}

int __wrap_radio_send(uint8_t *data, uint16_t len, uint32_t timeout_ms)
{
    check_expected_ptr(data);
    check_expected(len);
    check_expected(timeout_ms);

    return mock_type(int);
}

int __wrap_radio_recv(uint8_t *data, uint16_t len, uint32_t timeout_ms)
{
    check_expected_ptr(data);
    check_expected(len);
    check_expected(timeout_ms);

    return mock_type(int);
}

int __wrap_radio_available(void)
{
    return mock_type(int);
}

int __wrap_radio_sleep(void)
{
    return mock_type(int);
}

/** \} End of radio_wrap group */
