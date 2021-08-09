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
 * \version 0.7.2
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

    return 0;
}

int __wrap_isis_antenna_arm(void)
{
    function_called();

    return 0;
}

int __wrap_isis_antenna_disarm(void)
{
    function_called();

    return 0;
}

int __wrap_isis_antenna_start_sequential_deploy(uint8_t sec)
{
    function_called();

    return 0;
}

int __wrap_isis_antenna_start_independent_deploy(uint8_t ant, uint8_t sec, uint8_t ovr)
{
    check_expected(ant);
    check_expected(ovr);

    return 0;
}

int __wrap_isis_antenna_read_deployment_status_code(uint16_t *status)
{
    uint16_t status_val = mock_type(uint16_t);

    if (status != NULL)
    {
        *status = status_val;
    }

    return 0;
}

int __wrap_isis_antenna_read_deployment_status(isis_antenna_status_t *status)
{
    uint16_t code_val           = mock_type(uint16_t);
    uint8_t ant_1_status_val    = mock_type(uint8_t);
    uint8_t ant_1_timeout_val   = mock_type(uint8_t);
    uint8_t ant_1_burning_val   = mock_type(uint8_t);
    uint8_t ant_2_status_val    = mock_type(uint8_t);
    uint8_t ant_2_timeout_val   = mock_type(uint8_t);
    uint8_t ant_2_burning_val   = mock_type(uint8_t);
    uint8_t ant_3_status_val    = mock_type(uint8_t);
    uint8_t ant_3_timeout_val   = mock_type(uint8_t);
    uint8_t ant_3_burning_val   = mock_type(uint8_t);
    uint8_t ant_4_status_val    = mock_type(uint8_t);
    uint8_t ant_4_timeout_val   = mock_type(uint8_t);
    uint8_t ant_4_burning_bal   = mock_type(uint8_t);
    uint8_t ignoring_switches   = mock_type(uint8_t);
    uint8_t independent_burn    = mock_type(uint8_t);
    uint8_t armed               = mock_type(uint8_t);

    if (status != NULL)
    {
        status->code                = code_val;
        status->antenna_1.status    = ant_1_status_val;
        status->antenna_1.timeout   = ant_1_timeout_val;
        status->antenna_1.burning   = ant_1_burning_val;
        status->antenna_2.status    = ant_2_status_val;
        status->antenna_2.timeout   = ant_2_timeout_val;
        status->antenna_2.burning   = ant_2_burning_val;
        status->antenna_3.status    = ant_3_status_val;
        status->antenna_3.timeout   = ant_3_timeout_val;
        status->antenna_3.burning   = ant_3_burning_val;
        status->antenna_4.status    = ant_4_status_val;
        status->antenna_4.timeout   = ant_4_timeout_val;
        status->antenna_4.burning   = ant_4_burning_bal;
        status->ignoring_switches   = ignoring_switches;
        status->independent_burn    = independent_burn;
        status->armed               = armed;
    }

    return 0;
}

int __wrap_isis_antenna_get_antenna_status(uint8_t ant, uint8_t *ant_status)
{
    check_expected(ant);

    uint8_t status_val = mock_type(uint8_t);

    if (ant_status != NULL)
    {
        *ant_status = status_val;
    }

    return 0;
}

int __wrap_isis_antenna_get_antenna_timeout(uint8_t ant, uint8_t *ant_timeout)
{
    check_expected(ant);

    uint8_t ant_timeout_val = mock_type(uint8_t);

    if (ant_timeout != NULL)
    {
        *ant_timeout = ant_timeout_val;
    }

    return 0;
}

int __wrap_isis_antenna_get_burning(uint8_t ant, uint8_t *ant_burn)
{
    check_expected(ant);

    uint8_t ant_burn_val = mock_type(uint8_t);

    if (ant_burn != NULL)
    {
        *ant_burn = ant_burn_val;
    }

    return 0;
}

int __wrap_isis_antenna_get_arming_status(bool *armed)
{
    bool armed_val = mock_type(bool);

    if (armed != NULL)
    {
        *armed = armed_val;
    }

    return 0;
}

int __wrap_isis_antenna_get_raw_temperature(uint16_t *temp)
{
    uint16_t temp_val = mock_type(uint16_t);

    if (temp != NULL)
    {
        *temp = temp_val;
    }

    return 0;
}

int16_t __wrap_isis_antenna_raw_to_temp_c(uint16_t raw)
{
    return mock_type(int16_t);
}

int __wrap_isis_antenna_get_temperature_c(int16_t *temp)
{
    int16_t temp_val = mock_type(int16_t);

    if (temp != NULL)
    {
        *temp = temp_val;
    }

    return 0;
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
