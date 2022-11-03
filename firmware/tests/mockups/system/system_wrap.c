/*
 * system_wrap.c
 * 
 * Copyright The OBDH 2.0 Contributors.
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
 * \brief System wrap implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.9.0
 * 
 * \date 2022/03/14
 * 
 * \addtogroup system_wrap
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include "system_wrap.h"

void __wrap_system_reset(void)
{
    function_called();
}

uint8_t __wrap_system_get_reset_cause(void)
{
    return mock_type(uint8_t);
}

void __wrap_system_set_time(sys_time_t tm)
{
    check_expected(tm);
}

void __wrap_system_increment_time(void)
{
    function_called();
}

sys_time_t __wrap_system_get_time(void)
{
    return mock_type(sys_time_t);
}

sys_hw_version_t __wrap_system_get_hw_version(void)
{
    return mock_type(sys_hw_version_t);
}

/** \} End of system_wrap group */
