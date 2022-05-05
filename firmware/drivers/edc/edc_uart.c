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
    uart_config_t uart_conf = {0};

    uart_conf.baudrate  = 115200;
    uart_conf.data_bits = 8;
    uart_conf.parity    = UART_NO_PARITY;
    uart_conf.stop_bits = UART_ONE_STOP_BIT;

    int err = -1;

    if (uart_init(config.uart_port, uart_conf) == 0)
    {
        err = uart_rx_enable(config.uart_port);
    }

    return err;
}

int edc_uart_write(edc_config_t config, uint8_t *data, uint16_t len)
{
    return uart_write(config.uart_port, data, len);
}

int edc_uart_read(edc_config_t config, uint8_t *data, uint16_t len)
{
    return uart_read(config.uart_port, data, len);
}

int edc_uart_rx_available(edc_config_t config)
{
    return (int)uart_read_available(config.uart_port);
}

/** \} End of edc group */
