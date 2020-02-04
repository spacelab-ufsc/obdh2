/*
 * spi.c
 * 
 * Copyright (C) 2020, SpaceLab.
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
 * \brief SPI driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.13
 * 
 * \date 07/12/2019
 * 
 * \addtogroup spi
 * \{
 */

#include <hal/usci_a_spi.h>
#include <hal/usci_b_spi.h>
#include <hal/gpio.h>
#include <hal/ucs.h>

#include <drivers/gpio/gpio.h>

#include "spi.h"

int spi_setup_gpio(spi_port_t port)
{
    switch(port)
    {
        case SPI_PORT_0:
            GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P2, GPIO_PIN0 + GPIO_PIN3 + GPIO_PIN4);

            gpio_init(GPIO_PIN_5, (gpio_config_t){.mode=GPIO_MODE_OUTPUT});
            gpio_init(GPIO_PIN_6, (gpio_config_t){.mode=GPIO_MODE_OUTPUT});
            gpio_init(GPIO_PIN_28, (gpio_config_t){.mode=GPIO_MODE_OUTPUT});
            gpio_init(GPIO_PIN_45, (gpio_config_t){.mode=GPIO_MODE_OUTPUT});
            gpio_init(GPIO_PIN_46, (gpio_config_t){.mode=GPIO_MODE_OUTPUT});

            // Set all CS pins to high
            gpio_set_state(GPIO_PIN_5, true);
            gpio_set_state(GPIO_PIN_6, true);
            gpio_set_state(GPIO_PIN_28, true);
            gpio_set_state(GPIO_PIN_45, true);
            gpio_set_state(GPIO_PIN_46, true);

            break;
        case SPI_PORT_1:
            GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P8, GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN3);
            break;
        case SPI_PORT_2:
            GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P9, GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN3);
            break;
        case SPI_PORT_3:
            GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P2, GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN3);
            break;
        case SPI_PORT_4:
            GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P8, GPIO_PIN1 + GPIO_PIN5 + GPIO_PIN6);
            break;
        case SPI_PORT_5:
            GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P9, GPIO_PIN1 + GPIO_PIN5 + GPIO_PIN6);
            break;
        default:
            return -1;  // Invalid SPI port
    }

    return 0;
}

int spi_init(spi_port_t port, spi_config_t config)
{
    uint16_t base_address;

    switch(port)
    {
        case SPI_PORT_0:    base_address = USCI_A0_BASE;    break;
        case SPI_PORT_1:    base_address = USCI_A1_BASE;    break;
        case SPI_PORT_2:    base_address = USCI_A2_BASE;    break;
        case SPI_PORT_3:    base_address = USCI_B0_BASE;    break;
        case SPI_PORT_4:    base_address = USCI_B1_BASE;    break;
        case SPI_PORT_5:    base_address = USCI_B2_BASE;    break;
        default:
            return -1;  // Invalid SPI port
    }

    if (spi_setup_gpio(port) != 0)
    {
        return -1;
    }

    if ((port == SPI_PORT_0) || (port == SPI_PORT_1) || (port == SPI_PORT_2))
    {
        USCI_A_SPI_initMasterParam spi_params;

        spi_params.selectClockSource        = USCI_A_SPI_CLOCKSOURCE_SMCLK;
        spi_params.clockSourceFrequency     = UCS_getSMCLK();
        spi_params.desiredSpiClock          = config.speed_hz;
        spi_params.msbFirst                 = USCI_A_SPI_LSB_FIRST;

        // SPI mode
        switch(config.mode)
        {
            case SPI_MODE_0:
                spi_params.clockPhase       = USCI_A_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT;
                spi_params.clockPolarity    = USCI_A_SPI_CLOCKPOLARITY_INACTIVITY_LOW;
                break;
            case SPI_MODE_1:
                spi_params.clockPhase       = USCI_A_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT;
                spi_params.clockPolarity    = USCI_A_SPI_CLOCKPOLARITY_INACTIVITY_LOW;
                break;
            case SPI_MODE_2:
                spi_params.clockPhase       = USCI_A_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT;
                spi_params.clockPolarity    = USCI_A_SPI_CLOCKPOLARITY_INACTIVITY_HIGH;
                break;
            case SPI_MODE_3:
                spi_params.clockPhase       = USCI_A_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT;
                spi_params.clockPolarity    = USCI_A_SPI_CLOCKPOLARITY_INACTIVITY_HIGH;
                break;
            default:
                return -1;  // Invalid SPI mode
        }

        if (USCI_A_SPI_initMaster(base_address, &spi_params) != STATUS_SUCCESS)
        {
            return -1;      // Error initializing the SPI port
        }
    }
    else
    {
        USCI_B_SPI_initMasterParam spi_params;

        spi_params.selectClockSource        = USCI_B_SPI_CLOCKSOURCE_SMCLK;
        spi_params.clockSourceFrequency     = UCS_getSMCLK();
        spi_params.desiredSpiClock          = config.speed_hz;
        spi_params.msbFirst                 = USCI_B_SPI_LSB_FIRST;

        // SPI mode
        switch(config.mode)
        {
            case SPI_MODE_0:
                spi_params.clockPhase       = USCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT;
                spi_params.clockPolarity    = USCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW;
                break;
            case SPI_MODE_1:
                spi_params.clockPhase       = USCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT;
                spi_params.clockPolarity    = USCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW;
                break;
            case SPI_MODE_2:
                spi_params.clockPhase       = USCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT;
                spi_params.clockPolarity    = USCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH;
                break;
            case SPI_MODE_3:
                spi_params.clockPhase       = USCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT;
                spi_params.clockPolarity    = USCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH;
                break;
            default:
                return -1;  // Invalid SPI mode
        }

        if (USCI_B_SPI_initMaster(base_address, &spi_params) != STATUS_SUCCESS)
        {
            return -1;      // Error initializing the SPI port
        }
    }

    return 0;
}

int spi_transfer(spi_port_t port, spi_cs_t cs, uint8_t *data_w, uint8_t *data_r, uint16_t len)
{
    return -1;
}

int spi_write(spi_port_t port, spi_cs_t cs, uint8_t *data, uint16_t len)
{
    return -1;
}

int spi_read(spi_port_t port, spi_cs_t cs, uint8_t *data, uint16_t len)
{
    return -1;
}

//! \} End of spi group
