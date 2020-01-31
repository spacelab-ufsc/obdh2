/*
 * i2c.c
 * 
 * Copyright (C) 2020, SpaceLab.
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
 * \brief I2C driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.5
 * 
 * \date 07/12/2019
 * 
 * \addtogroup i2c
 * \{
 */

#include <hal/usci_b_i2c.h>
#include <hal/gpio.h>

#include "i2c.h"

int i2c_init(i2c_port_t port, i2c_config_t config)
{
    switch(config.speed_hz)
    {
        case USCI_B_I2C_SET_DATA_RATE_100KBPS:      break;
        case USCI_B_I2C_SET_DATA_RATE_400KBPS:      break;
        default:
            return -1;  // Invalid transfer rate
    }

    uint16_t base_address;

    switch(port)
    {
        case I2C_PORT_0:
            base_address = USCI_B0_BASE;
            GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P2, GPIO_PIN1 + GPIO_PIN2);
            break;
        case I2C_PORT_1:
            base_address = USCI_B1_BASE;
            GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P8, GPIO_PIN4 + GPIO_PIN5);
            break;
        case I2C_PORT_2:
            base_address = USCI_B2_BASE;
            GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P9, GPIO_PIN5 + GPIO_PIN6);
            break;
        default:
            return -1;  // Invalid port
    }

    USCI_B_I2C_initMasterParam i2c_params;

    i2c_params.selectClockSource    = USCI_B_I2C_CLOCKSOURCE_SMCLK;
    i2c_params.i2cClk               = config.clk_hz;
    i2c_params.dataRate             = config.speed_hz;

    USCI_B_I2C_initMaster(base_address, &i2c_params);

    USCI_B_I2C_enable(base_address);

    return 0;
}

int i2c_write(i2c_port_t port, i2c_slave_adr_t adr, uint8_t *data, uint16_t len)
{
    return -1;
}

int i2c_read(i2c_port_t port, i2c_slave_adr_t adr, uint8_t *data, uint16_t len)
{
    return -1;
}

//! \} End of i2c group
