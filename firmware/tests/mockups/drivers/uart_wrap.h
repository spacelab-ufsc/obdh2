/*
 * uart_wrap.h
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
 * \brief UART driver wrap definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.8
 * 
 * \date 2021/08/25
 * 
 * \defgroup uart_wrap UART Wrap
 * \ingroup tests
 * \{
 */

#ifndef UART_WRAP_H_
#define UART_WRAP_H_

#include <stdint.h>

#include <drivers/uart/uart.h>

int __wrap_uart_init(uart_port_t port, uart_config_t config);

int __wrap_uart_available(uart_port_t port);

int __wrap_uart_flush(uart_port_t port);

int __wrap_uart_write(uart_port_t port, uint8_t *data, uint16_t len);

int __wrap_uart_read(uart_port_t port, uint8_t *data, uint16_t len);

#endif /* UART_WRAP_H_ */

/** \} End of uart_wrap group */
