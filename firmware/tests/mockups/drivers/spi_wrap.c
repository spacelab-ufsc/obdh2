/*
 * spi_wrap.c
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
 * \brief SPI driver wrap implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.10
 * 
 * \date 2021/08/25
 * 
 * \addtogroup spi_wrap
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include "spi_wrap.h"

int __wrap_spi_init(spi_port_t port, spi_config_t config)
{
    check_expected(port);
    check_expected(config.speed_hz);
    check_expected(config.mode);

    return mock_type(int);
}

int __wrap_spi_select_slave(spi_port_t port, spi_cs_t cs, bool active)
{
    check_expected(port);
    check_expected(cs);
    check_expected(active);

    return mock_type(int);
}

int __wrap_spi_write(spi_port_t port, spi_cs_t cs, uint8_t *data, uint16_t len)
{
    check_expected(port);
    check_expected(cs);
    check_expected_ptr(data);
    check_expected(len);

    return mock_type(int);
}

int __wrap_spi_read(spi_port_t port, spi_cs_t cs, uint8_t *data, uint16_t len)
{
    check_expected(port);
    check_expected(cs);
    check_expected(len);

    if (data != NULL)
    {
        uint16_t i = 0;
        for(i=0; i<len; i++)
        {
            data[i] = mock_type(uint8_t);
        }
    }

    return mock_type(int);
}

int __wrap_spi_transfer(spi_port_t port, spi_cs_t cs, uint8_t *wd, uint8_t *rd, uint16_t len)
{
    check_expected(port);
    check_expected(cs);
    check_expected_ptr(wd);
    check_expected(len);

    if (rd != NULL)
    {
        uint16_t i = 0;
        for(i=0; i<len; i++)
        {
            rd[i] = mock_type(uint8_t);
        }
    }

    return mock_type(int);
}

int __wrap_spi_mutex_create(void)
{
    return mock_type(int);
}

int __wrap_spi_mutex_take(void)
{
    return mock_type(int);
}

int __wrap_spi_mutex_give(void)
{
    return mock_type(int);
}

/** \} End of spi_wrap group */
