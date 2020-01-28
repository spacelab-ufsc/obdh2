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
#include <hal/gpio.h>

#include "uart.h"

int uart_init(uart_port_t port, uart_config_t config)
{
    USCI_A_UART_initParam uart_params = {0};

    switch(config.baudrate)
    {
        case 1200:
            uart_params.clockPrescalar      = 1665; // 1200 bps @ 31.981568 MHz
            uart_params.firstModReg         = 11;   // 1200 bps @ 31.981568 MHz
            uart_params.secondModReg        = 0;    // 1200 bps @ 31.981568 MHz
            break;
        case 2400:
            uart_params.clockPrescalar      = 832;  // 2400 bps @ 31.981568 MHz
            uart_params.firstModReg         = 14;   // 2400 bps @ 31.981568 MHz
            uart_params.secondModReg        = 0;    // 2400 bps @ 31.981568 MHz
            break;
        case 4800:
            uart_params.clockPrescalar      = 416;  // 4800 bps @ 31.981568 MHz
            uart_params.firstModReg         = 7;    // 4800 bps @ 31.981568 MHz
            uart_params.secondModReg        = 0;    // 4800 bps @ 31.981568 MHz
            break;
        case 9600:
            uart_params.clockPrescalar      = 208;  // 9600 bps @ 31.981568 MHz
            uart_params.firstModReg         = 3;    // 9600 bps @ 31.981568 MHz
            uart_params.secondModReg        = 0;    // 9600 bps @ 31.981568 MHz
            break;
        case 19200:
            uart_params.clockPrescalar      = 104;  // 19200 bps @ 31.981568 MHz
            uart_params.firstModReg         = 2;    // 19200 bps @ 31.981568 MHz
            uart_params.secondModReg        = 0;    // 19200 bps @ 31.981568 MHz
            break;
        case 38400:
            uart_params.clockPrescalar      = 52;   // 38400 bps @ 31.981568 MHz
            uart_params.firstModReg         = 1;    // 38400 bps @ 31.981568 MHz
            uart_params.secondModReg        = 0;    // 38400 bps @ 31.981568 MHz
            break;
        case 57600:
            uart_params.clockPrescalar      = 34;   // 57600 bps @ 31.981568 MHz
            uart_params.firstModReg         = 11;   // 57600 bps @ 31.981568 MHz
            uart_params.secondModReg        = 0;    // 57600 bps @ 31.981568 MHz
            break;
        case 115200:
            uart_params.clockPrescalar      = 17;   // 115200 bps @ 31.981568 MHz
            uart_params.firstModReg         = 6;    // 115200 bps @ 31.981568 MHz
            uart_params.secondModReg        = 0;    // 115200 bps @ 31.981568 MHz
            break;
        case 230400:
            uart_params.clockPrescalar      = 8;    // 230400 bps @ 31.981568 MHz
            uart_params.firstModReg         = 11;   // 230400 bps @ 31.981568 MHz
            uart_params.secondModReg        = 5;    // 230400 bps @ 31.981568 MHz
            break;
        case 460800:
            uart_params.clockPrescalar      = 4;    // 460800 bps @ 31.981568 MHz
            uart_params.firstModReg         = 3;    // 460800 bps @ 31.981568 MHz
            uart_params.secondModReg        = 5;    // 460800 bps @ 31.981568 MHz
            break;
        default:
            return -1;      // Invalid baudrate value
    }

    uart_params.selectClockSource   = USCI_A_UART_CLOCKSOURCE_SMCLK;
    uart_params.parity              = config.parity;
    uart_params.msborLsbFirst       = USCI_A_UART_LSB_FIRST;
    uart_params.numberofStopBits    = config.stop_bits;
    uart_params.uartMode            = USCI_A_UART_MODE;
    uart_params.overSampling        = USCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION;

    uint16_t base_address;

    switch(port)
    {
        case UART_PORT_0:
            base_address = USCI_A0_BASE;

            GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P2, GPIO_PIN4 + GPIO_PIN5);

            break;
        case UART_PORT_1:
            base_address = USCI_A1_BASE;

            GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P8, GPIO_PIN2 + GPIO_PIN3);

            break;
        case UART_PORT_2:
            base_address = USCI_A2_BASE;

            GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P9, GPIO_PIN2 + GPIO_PIN3);

            break;
        default:
            return -1;      // Invalid port
    }

    if (USCI_A_UART_init(base_address, &uart_params) != STATUS_SUCCESS)
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
