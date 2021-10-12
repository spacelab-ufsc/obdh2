/*
 * isis_antenna_wrap.c
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
 * \brief ISIS antenna driver wrap implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.8.1
 * 
 * \date 2021/08/07
 * 
 * \addtogroup isis_antenna_wrap
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include "isis_antenna_wrap.h"

int __wrap_isis_antenna_init(void)
{
    function_called();

    return mock_type(int);
}

int __wrap_isis_antenna_arm(void)
{
    function_called();

    return mock_type(int);
}

int __wrap_isis_antenna_disarm(void)
{
    function_called();

    return mock_type(int);
}

int __wrap_isis_antenna_start_sequential_deploy(uint8_t sec)
{
    check_expected(sec);

    return mock_type(int);
}

int __wrap_isis_antenna_start_independent_deploy(isis_antenna_ant_t ant, uint8_t sec, isis_antenna_override_t ovr)
{
    check_expected(ant);
    check_expected(sec);
    check_expected(ovr);

    return mock_type(int);
}

int __wrap_isis_antenna_read_deployment_status_code(uint16_t *status)
{
    if (status != NULL)
    {
        *status = mock_type(uint16_t);
    }

    return mock_type(int);
}

int __wrap_isis_antenna_read_deployment_status(isis_antenna_status_t *status)
{
    if (status != NULL)
    {
        status->code                = mock_type(uint16_t);
        status->antenna_1.status    = mock_type(uint8_t);
        status->antenna_1.timeout   = mock_type(uint8_t);
        status->antenna_1.burning   = mock_type(uint8_t);
        status->antenna_2.status    = mock_type(uint8_t);
        status->antenna_2.timeout   = mock_type(uint8_t);
        status->antenna_2.burning   = mock_type(uint8_t);
        status->antenna_3.status    = mock_type(uint8_t);
        status->antenna_3.timeout   = mock_type(uint8_t);
        status->antenna_3.burning   = mock_type(uint8_t);
        status->antenna_4.status    = mock_type(uint8_t);
        status->antenna_4.timeout   = mock_type(uint8_t);
        status->antenna_4.burning   = mock_type(uint8_t);
        status->ignoring_switches   = mock_type(uint8_t);
        status->independent_burn    = mock_type(uint8_t);
        status->armed               = mock_type(uint8_t);
    }

    return mock_type(int);
}

int __wrap_isis_antenna_get_antenna_status(isis_antenna_ant_t ant, uint8_t *ant_status)
{
    check_expected(ant);

    if (ant_status != NULL)
    {
        *ant_status = mock_type(uint8_t);
    }

    return mock_type(int);
}

int __wrap_isis_antenna_get_antenna_timeout(isis_antenna_ant_t ant, uint8_t *ant_timeout)
{
    check_expected(ant);

    if (ant_timeout != NULL)
    {
        *ant_timeout = mock_type(uint8_t);
    }

    return mock_type(int);
}

int __wrap_isis_antenna_get_burning(isis_antenna_ant_t ant, uint8_t *ant_burn)
{
    check_expected(ant);

    if (ant_burn != NULL)
    {
        *ant_burn = mock_type(uint8_t);
    }

    return mock_type(int);
}

int __wrap_isis_antenna_get_arming_status(bool *armed)
{
    if (armed != NULL)
    {
        *armed = mock_type(bool);
    }

    return mock_type(int);
}

int __wrap_isis_antenna_get_raw_temperature(uint16_t *temp)
{
    if (temp != NULL)
    {
        *temp = mock_type(uint16_t);
    }

    return mock_type(int);
}

int16_t __wrap_isis_antenna_raw_to_temp_c(uint16_t raw)
{
    return mock_type(int16_t);
}

int __wrap_isis_antenna_get_temperature_c(int16_t *temp)
{
    if (temp != NULL)
    {
        *temp = mock_type(int16_t);
    }

    return mock_type(int);
}

void __wrap_isis_antenna_delay_s(uint8_t s)
{
    function_called();

    return;
}

void __wrap_isis_antenna_delay_ms(uint16_t ms)
{
    function_called();

    return;
}

/** \} End of isis_antenna_wrap group */
