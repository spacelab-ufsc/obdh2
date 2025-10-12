/*
 * lpl.c
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
 * \brief LPL device implementation.
 * 
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 1.0.0
 * 
 * \date 2025/09/28
 * 
 * \addtogroup lpl
 * \{
 */

#include <stdint.h>
#include <stdbool.h>

#include <system/sys_log/sys_log.h>

#include <drivers/uart/uart.h>
#include <drivers/gpio/gpio.h>

#include "lpl.h"

int lpl_init(lpl_t *dev) 
{
    gpio_config_t conf = {0};

    conf.mode = GPIO_MODE_OUTPUT;

    dev->uart_conf.baudrate  = 115200;
    dev->uart_conf.data_bits = 8;
    dev->uart_conf.parity    = UART_NO_PARITY;
    dev->uart_conf.stop_bits = UART_ONE_STOP_BIT;
    dev->uart_port = UART_PORT_1;
    dev->en_pin = GPIO_PIN_29;

    int err = -1;

    if (uart_init(dev->uart_port, dev->uart_conf) == 0)
    {
        err = uart_rx_enable(dev->uart_port);

        if (err == 0)
        {
            err = gpio_init(dev->en_pin, conf);
        }
    }

    return err;
}

 
int lpl_enable(const lpl_t *dev)
{
    return gpio_set_state(dev->en_pin, true);
}

int lpl_disable(const lpl_t *dev)
{
    return gpio_set_state(dev->en_pin, false);
}

int lpl_available(const lpl_t *dev)
{
    return uart_read_available(dev->uart_port);
}

int lpl_read(lpl_t *dev, uint8_t *buf, const uint16_t size)
{
    return uart_read(dev->uart_port, buf, size);
}

/** \} End of payload group */
