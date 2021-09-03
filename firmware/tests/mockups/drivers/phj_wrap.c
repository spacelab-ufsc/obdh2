/*
 * phj_wrap.c
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
 * \brief PHJ driver wrap implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.6
 * 
 * \date 2021/08/18
 * 
 * \addtogroup phj_wrap
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include "phj_wrap.h"

int __wrap_phj_init_i2c(phj_config_i2c config)
{
    check_expected(config.port);
    check_expected(config.bitrate);

    return mock_type(int);
}

int __wrap_phj_init_gpio(phj_config_gpio config)
{
    check_expected(config.pin);
    check_expected(config.mode);

    return mock_type(int);
}

int __wrap_phj_read(uint8_t *data, uint16_t len)
{
    return mock_type(int);
}

int __wrap_phj_check_converter(void)
{
    return mock_type(int);
}

int __wrap_phj_check_message(void)
{
    return mock_type(int);
}

/** \} End of phj_wrap group */
