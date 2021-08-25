/*
 * uart_wrap.c
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
 * \brief UART driver wrap implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.8
 * 
 * \date 2021/08/25
 * 
 * \addtogroup uart_wrap
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include "uart_wrap.h"

int __wrap_uart_init(uart_port_t port, uart_config_t config)
{
    check_expected(port);
    check_expected(config.clock);
    check_expected(config.baudrate);
    check_expected(config.data_bits);
    check_expected(config.parity);
    check_expected(config.stop_bits);

    return mock_type(int);
}

int __wrap_uart_available(uart_port_t port)
{
    check_expected(port);

    return mock_type(int);
}

int __wrap_uart_flush(uart_port_t port)
{
    check_expected(port);

    return mock_type(int);
}

int __wrap_uart_write(uart_port_t port, uint8_t *data, uint16_t len)
{
    check_expected(port);
    check_expected_ptr(data);
    check_expected(len);

    return mock_type(int);
}

int __wrap_uart_read(uart_port_t port, uint8_t *data, uint16_t len)
{
    check_expected(port);

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

/** \} End of uart_wrap group */
