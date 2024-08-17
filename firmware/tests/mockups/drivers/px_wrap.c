/*
 * phj_wrap.c
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
 * \brief Payload-X driver wrap implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 0.10.19
 * 
 * \date 2023/09/18
 * 
 * \addtogroup px_wrap
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include "px_wrap.h"

int __wrap_px_init(px_config_t *conf)
{
    check_expected(conf->port);
    check_expected(conf->bitrate);
    check_expected(conf->en_pin);

    return mock_type(int);
}

int __wrap_px_write(px_config_t *conf, uint8_t *data, uint16_t len)
{
    check_expected(conf->port);
    check_expected(conf->bitrate);

    check_expected_ptr(data);
    check_expected(len);

    return mock_type(int);
}

int __wrap_px_read(px_config_t *conf, uint8_t *data, uint16_t len)
{
    check_expected(conf->port);
    check_expected(conf->bitrate);

    uint16_t i = 0U;
    for(i = 0U; i < len; i++)
    {
        data[i] = mock_type(int);
    }

    check_expected(len);

    return mock_type(int);
}

int __wrap_px_enable(px_config_t *conf)
{
    check_expected(conf->en_pin);

    return mock_type(int);
}

int __wrap_px_disable(px_config_t *conf)
{
    check_expected(conf->en_pin);

    return mock_type(int);
}

/** \} End of phj_wrap group */
