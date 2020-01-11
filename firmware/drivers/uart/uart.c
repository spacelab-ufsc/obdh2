/*
 * uart.c
 * 
 * Copyright (C) 2019, SpaceLab.
 * 
 * This file is part of OBDH 2.0.
 * 
 * OBDH 2.0 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * OBDH 2.0 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with OBDH 2.0. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief UART driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.0
 * 
 * \date 07/12/2019
 * 
 * \addtogroup uart
 * \{
 */

#include <hal/usci_a_uart.h>

#include "uart.h"

int uart_init(uart_port_t port, uart_config_t config)
{
    USCI_A_UART_initParam uart_params = {0};

    uart_params.selecClockSource    = USCI_A_UART_CLOCKSOURCE_SMCLK;
    uart_params.clockPrescalar      = 8;    // 115200 bps @ 16 MHz
    uart_params.firstModReg         = 11;   // 115200 bps @ 16 MHz
    uart_params.secondModReg        = 0;    // 115200 bps @ 16 MHz
    uart_params.parity              = config.parity;
    uart_params.msborLsbFirst       = USCI_A_UART_LSB_FIRST;
    uart_params.numberofStopBits    = config.stop_bits;
    uart_params.uartMode            = USCI_A_UART_MODE;
    uart_params.overSampling        = USCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION;

    uint16_t base_address;

    switch(port)
    {
        case UART_PORT_0:   base_address = USCI_A0_BASE;    break;
        case UART_PORT_1:   base_address = USCI_A1_BASE;    break;
        case UART_PORT_2:   base_address = USCI_A2_BASE;    break;
        default:            return -1;
    }

    if (USCI_A_UART_init(base_address, &param) != STATUS_SUCCESS)
    {
        return -1;
    }

    USCI_A_UART_enable(base_address);

    return 0;
}

int uart_write(uart_port_t port, uint8_t *data, uint16_t len)
{
    uint16_t base_address;

    switch(port)
    {
        case UART_PORT_0:   base_address = USCI_A0_BASE;    break;
        case UART_PORT_1:   base_address = USCI_A1_BASE;    break;
        case UART_PORT_2:   base_address = USCI_A2_BASE;    break;
        default:            return -1;
    }

    uint16_t i = 0;
    for(i=0; i<len; i++)
    {
        USCI_A_UART_transmitData(base_address, data[i]);
    }

    return 0;
}

int uart_read(uart_port_t port, uint8_t *data, uint16_t len)
{
    uint16_t base_address;

    switch(port)
    {
        case UART_PORT_0:   base_address = USCI_A0_BASE;    break;
        case UART_PORT_1:   base_address = USCI_A1_BASE;    break;
        case UART_PORT_2:   base_address = USCI_A2_BASE;    break;
        default:            return -1;
    }

    uint16_t i = 0;
    for(i=0; i<len; i++)
    {
        data[i] = USCI_A_UART_receiveData(base_address);
    }

    return 0;
}

//! \} End of uart group
