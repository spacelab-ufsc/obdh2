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
    return 0;
}

int __wrap_isis_antenna_arm(void)
{
    return 0;
}

int __wrap_isis_antenna_disarm(void)
{
    return 0;
}

int __wrap_isis_antenna_start_sequential_deploy(uint8_t sec)
{
    return 0;
}

int __wrap_isis_antenna_start_independent_deploy(uint8_t ant, uint8_t sec, uint8_t ovr)
{
    return 0;
}

int __wrap_isis_antenna_read_deployment_status_code(uint16_t *status)
{
    return 0;
}

int __wrap_isis_antenna_read_deployment_status(isis_antenna_status_t *status)
{
    return 0;
}

int __wrap_isis_antenna_get_antenna_status(uint8_t ant, uint8_t *ant_status)
{
    return 0;
}

int __wrap_isis_antenna_get_antenna_timeout(uint8_t ant, uint8_t *ant_timeout)
{
    return 0;
}

int __wrap_isis_antenna_get_burning(uint8_t ant, uint8_t *ant_burn)
{
    return 0;
}

int __wrap_isis_antenna_get_arming_status(bool *armed)
{
    return 0;
}

int __wrap_isis_antenna_get_raw_temperature(uint16_t *temp)
{
    return 0;
}

int16_t __wrap_isis_antenna_raw_to_temp_c(uint16_t raw)
{
    return 0;
}

int __wrap_isis_antenna_get_temperature_c(int16_t *temp)
{
    return 0;
}

void __wriap_isis_antenna_delay_s(uint8_t s)
{
    return;
}

void __wriap_isis_antenna_delay_ms(uint16_t ms)
{
    return;
}

/** \} End of isis_antenna_wrap group */
