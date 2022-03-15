/*
 * edc_uart.c
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
 * along with OBDH 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 *
 */

/**
 * \brief EDC driver UART routines implementation.
 *
 * \author Bruno Benedetti <brunobenedetti45@gmail.com>
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 *
 * \version 0.9.2
 *
 * \date 2022/03/15
 *
 * \addtogroup edc
 * \{
 */

#include <drivers/uart/uart.h>

#include "edc.h"

int edc_uart_init(edc_config_t config)
{
    /* int uart_init(uart_port_t port, uart_config_t config); */
    return -1;
}

int edc_uart_write(edc_config_t config, uint8_t *data, uint16_t len)
{
    /* return uart_write(uart_port_t port, uint8_t *data, uint16_t len); */
    return -1;
}

int edc_uart_read(edc_config_t config, uint8_t *data, uint16_t len)
{
    /* return uart_read(uart_port_t port, uint8_t *data, uint16_t len); */
    return -1;
}

int edc_uart_rx_available(edc_config_t config)
{
    /* return uart_available(uart_port_t port); */
    return -1;
}

/** \} End of edc group */
