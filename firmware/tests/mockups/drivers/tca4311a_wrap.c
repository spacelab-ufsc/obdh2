/*
 * tca4311a_wrap.c
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
 * \brief TCA4311A driver wrap implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.8.0
 * 
 * \date 2021/10/11
 * 
 * \addtogroup tca4311a_wrap
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include "tca4311a_wrap.h"

int __wrap_tca4311a_init(tca4311a_config_t config, bool en)
{
    check_expected(config.i2c_port);
    check_expected(config.i2c_config.speed_hz);
    check_expected(config.en_pin);
    check_expected(config.ready_pin);

    check_expected(en);

    return mock_type(int);
}

int __wrap_tca4311a_enable(tca4311a_config_t config)
{
    check_expected(config.i2c_port);
    check_expected(config.i2c_config.speed_hz);
    check_expected(config.en_pin);
    check_expected(config.ready_pin);

    return mock_type(int);
}

int __wrap_tca4311a_disable(tca4311a_config_t config)
{
    check_expected(config.i2c_port);
    check_expected(config.i2c_config.speed_hz);
    check_expected(config.en_pin);
    check_expected(config.ready_pin);

    return mock_type(int);
}

int __wrap_tca4311a_is_ready(tca4311a_config_t config)
{
    check_expected(config.i2c_port);
    check_expected(config.i2c_config.speed_hz);
    check_expected(config.en_pin);
    check_expected(config.ready_pin);

    return mock_type(int);
}

int __wrap_tca4311a_write(tca4311a_config_t config, i2c_slave_adr_t adr, uint8_t *data, uint16_t len)
{
    check_expected(config.i2c_port);
    check_expected(config.i2c_config.speed_hz);
    check_expected(config.en_pin);
    check_expected(config.ready_pin);

    check_expected(adr);
    check_expected_ptr(data);
    check_expected(len);

    return mock_type(int);
}

int __wrap_tca4311a_read(tca4311a_config_t config, i2c_slave_adr_t adr, uint8_t *data, uint16_t len)
{
    check_expected(config.i2c_port);
    check_expected(config.i2c_config.speed_hz);
    check_expected(config.en_pin);
    check_expected(config.ready_pin);

    check_expected(adr);
    check_expected(len);

    if (data != NULL)
    {
        uint16_t i = 0;
        for(i = 0; i < len; i++)
        {
            data[i] = mock_type(uint8_t);
        }
    }

    return mock_type(int);
}

int __wrap_tca4311a_write_byte(tca4311a_config_t config, i2c_slave_adr_t adr, uint8_t byte)
{
    check_expected(config.i2c_port);
    check_expected(config.i2c_config.speed_hz);
    check_expected(config.en_pin);
    check_expected(config.ready_pin);

    check_expected(adr);
    check_expected(byte);

    return mock_type(int);
}

int __wrap_tca4311a_read_byte(tca4311a_config_t config, i2c_slave_adr_t adr, uint8_t *byte)
{
    check_expected(config.i2c_port);
    check_expected(config.i2c_config.speed_hz);
    check_expected(config.en_pin);
    check_expected(config.ready_pin);

    check_expected(adr);

    if (byte != NULL)
    {
        *byte = mock_type(uint8_t);
    }

    return mock_type(int);
}

/** \} End of tca4311a_wrap group */
