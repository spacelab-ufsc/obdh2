/*
 * spi.c
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
 * \brief SPI driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.9.16
 * 
 * \date 2019/12/07
 * 
 * \addtogroup spi
 * \{
 */

#include <hal/usci_a_spi.h>
#include <hal/usci_b_spi.h>
#include <hal/gpio.h>
#include <hal/ucs.h>

#include <config/config.h>
#include <system/sys_log/sys_log.h>

#include <drivers/gpio/gpio.h>

#include "spi.h"

static bool spi_port_0_is_open = false;
static bool spi_port_1_is_open = false;
static bool spi_port_2_is_open = false;
static bool spi_port_3_is_open = false;
static bool spi_port_4_is_open = false;
static bool spi_port_5_is_open = false;

/**
 * \brief Checks if a SPI port is already initialized or not.
 *
 * \param[in] port is the port to verify the initialization status.
 *
 * \return TRUE/FALSE if the given port is already initialized or not.
 */
static bool spi_check_port(spi_port_t port);

static int spi_setup_gpio(spi_port_t port)
{
    int err = 0;

    gpio_config_t conf = {0};

    conf.mode = GPIO_MODE_OUTPUT;

    switch(port)
    {
        case SPI_PORT_0:
            GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P2, GPIO_PIN0 + GPIO_PIN4 + GPIO_PIN5);

            gpio_init(GPIO_PIN_5, conf);
            gpio_init(GPIO_PIN_6, conf);
            gpio_init(GPIO_PIN_28, conf);
            gpio_init(GPIO_PIN_45, conf);
            gpio_init(GPIO_PIN_46, conf);
            gpio_init(GPIO_PIN_63, conf);

            /* Set all CS pins to high */
            gpio_set_state(GPIO_PIN_5, true);
            gpio_set_state(GPIO_PIN_6, true);
            gpio_set_state(GPIO_PIN_28, true);
            gpio_set_state(GPIO_PIN_45, true);
            gpio_set_state(GPIO_PIN_46, true);
            gpio_set_state(GPIO_PIN_63, true);

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
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_MODULE_NAME, "Error during GPIO configuration: Invalid port!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;   /* Invalid SPI port */

            break;
    }

    return err;
}

int spi_select_slave(spi_port_t port, spi_cs_t cs, bool active)
{
    int err = 0;

    switch(port)
    {
        case SPI_PORT_0:
            switch(cs)
            {
                case SPI_CS_0:      gpio_set_state(GPIO_PIN_5, !active);      break;
                case SPI_CS_1:      gpio_set_state(GPIO_PIN_6, !active);      break;
                case SPI_CS_2:      gpio_set_state(GPIO_PIN_28, !active);     break;
                case SPI_CS_3:      gpio_set_state(GPIO_PIN_45, !active);     break;
                case SPI_CS_4:      gpio_set_state(GPIO_PIN_46, !active);     break;
                case SPI_CS_5:      gpio_set_state(GPIO_PIN_63, !active);     break;
                case SPI_CS_NONE:                                             break;
                default:
                #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                    sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_MODULE_NAME, "Error selecting a slave from port 0: Invalid CS pin!");
                    sys_log_new_line();
                #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                    err = -1;   /* Invalid CS pin */

                    break;
            }

            break;
        case SPI_PORT_1:
            /* TODO: Define the CS pins pf port 1 */
            break;
        case SPI_PORT_2:
            /* TODO: Define the CS pins pf port 2 */
            break;
        case SPI_PORT_3:
            /* TODO: Define the CS pins pf port 3 */
            break;
        case SPI_PORT_4:
            /* TODO: Define the CS pins pf port 4 */
            break;
        case SPI_PORT_5:
            /* TODO: Define the CS pins pf port 5 */
            break;
        default:
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_MODULE_NAME, "Error selecting a slave: Invalid port!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;   /* Invalid SPI port */

            break;
    }

    return err;
}

int spi_init(spi_port_t port, spi_config_t config)
{
    int err = 0;

    if (!spi_check_port(port))
    {
        uint16_t base_address = 0;

        switch(port)
        {
            case SPI_PORT_0:    base_address = USCI_A0_BASE;    break;
            case SPI_PORT_1:    base_address = USCI_A1_BASE;    break;
            case SPI_PORT_2:    base_address = USCI_A2_BASE;    break;
            case SPI_PORT_3:    base_address = USCI_B0_BASE;    break;
            case SPI_PORT_4:    base_address = USCI_B1_BASE;    break;
            case SPI_PORT_5:    base_address = USCI_B2_BASE;    break;
            default:
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_MODULE_NAME, "Error during initialization: Invalid port!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                err = -1;   /* Invalid SPI port */

                break;
        }

        if (err == 0)
        {
            if (spi_setup_gpio(port) == 0)
            {
                if ((port == SPI_PORT_0) || (port == SPI_PORT_1) || (port == SPI_PORT_2))
                {
                    USCI_A_SPI_initMasterParam spi_params;

                    spi_params.selectClockSource        = USCI_A_SPI_CLOCKSOURCE_SMCLK;
                    spi_params.clockSourceFrequency     = UCS_getSMCLK();
                    spi_params.desiredSpiClock          = config.speed_hz;
                    spi_params.msbFirst                 = USCI_A_SPI_MSB_FIRST;

                    /* SPI mode */
                    switch(config.mode)
                    {
                        case SPI_MODE_0:
                            spi_params.clockPhase       = USCI_A_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT;
                            spi_params.clockPolarity    = USCI_A_SPI_CLOCKPOLARITY_INACTIVITY_LOW;
                            break;
                        case SPI_MODE_1:
                            spi_params.clockPhase       = USCI_A_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT;
                            spi_params.clockPolarity    = USCI_A_SPI_CLOCKPOLARITY_INACTIVITY_LOW;
                            break;
                        case SPI_MODE_2:
                            spi_params.clockPhase       = USCI_A_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT;
                            spi_params.clockPolarity    = USCI_A_SPI_CLOCKPOLARITY_INACTIVITY_HIGH;
                            break;
                        case SPI_MODE_3:
                            spi_params.clockPhase       = USCI_A_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT;
                            spi_params.clockPolarity    = USCI_A_SPI_CLOCKPOLARITY_INACTIVITY_HIGH;
                            break;
                        default:
                        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                            sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_MODULE_NAME, "Error during initialization: Invalid mode!");
                            sys_log_new_line();
                        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                            err = -1;   /* Invalid SPI mode */

                            break;
                    }

                    if (USCI_A_SPI_initMaster(base_address, &spi_params) == STATUS_SUCCESS)
                    {
                        USCI_A_SPI_enable(base_address);
                    }
                    else
                    {
                    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                        sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_MODULE_NAME, "Error configuring as master!");
                        sys_log_new_line();
                    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                        err = -1;   /* Error initializing the SPI port */
                    }
                }
                else
                {
                    USCI_B_SPI_initMasterParam spi_params;

                    spi_params.selectClockSource        = USCI_B_SPI_CLOCKSOURCE_SMCLK;
                    spi_params.clockSourceFrequency     = UCS_getSMCLK();
                    spi_params.desiredSpiClock          = config.speed_hz;
                    spi_params.msbFirst                 = USCI_B_SPI_LSB_FIRST;

                    /* SPI mode */
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
                        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                            sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_MODULE_NAME, "Error during initialization: Invalid mode!");
                            sys_log_new_line();
                        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                            err = -1;   /* Invalid SPI mode */

                            break;
                    }

                    if (err == 0)
                    {
                        if (USCI_B_SPI_initMaster(base_address, &spi_params) == STATUS_SUCCESS)
                        {
                            USCI_B_SPI_enable(base_address);
                        }
                        else
                        {
                        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                            sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_MODULE_NAME, "Error configuring as master!");
                            sys_log_new_line();
                        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                            err = -1;   /* Error initializing the SPI port */
                        }
                    }
                }

                switch(port)
                {
                    case SPI_PORT_0:    spi_port_0_is_open = true;  break;
                    case SPI_PORT_1:    spi_port_1_is_open = true;  break;
                    case SPI_PORT_2:    spi_port_2_is_open = true;  break;
                    case SPI_PORT_3:    spi_port_3_is_open = true;  break;
                    case SPI_PORT_4:    spi_port_4_is_open = true;  break;
                    case SPI_PORT_5:    spi_port_5_is_open = true;  break;
                    default:                                        break;
                }
            }
            else
            {
                err = -1;
            }
        }
    }

    return err;
}

static void spi_write_byte(uint16_t base_address, uint8_t byte)
{
    if ((base_address == USCI_A0_BASE) || (base_address == USCI_A1_BASE) || (base_address == USCI_A2_BASE))
    {
        /* Wait until TX buffer ready */
        while(!USCI_A_SPI_getInterruptStatus(base_address, USCI_A_SPI_TRANSMIT_INTERRUPT))
        {
            ;
        }

        USCI_A_SPI_clearInterrupt(base_address, USCI_A_SPI_TRANSMIT_INTERRUPT);

        USCI_A_SPI_transmitData(base_address, byte);
    }
    else
    {
        /* Wait until TX buffer ready */
        while(!USCI_B_SPI_getInterruptStatus(base_address, USCI_B_SPI_TRANSMIT_INTERRUPT))
        {
            ;
        }

        USCI_B_SPI_clearInterrupt(base_address, USCI_B_SPI_TRANSMIT_INTERRUPT);

        USCI_B_SPI_transmitData(base_address, byte);
    }
}

static uint8_t spi_read_byte(uint16_t base_address)
{
    uint8_t res = 0;

    if ((base_address == USCI_A0_BASE) || (base_address == USCI_A1_BASE) || (base_address == USCI_A2_BASE))
    {
        /* Wait until new data was written into RX buffer */
        while(!USCI_A_SPI_getInterruptStatus(base_address, USCI_A_SPI_RECEIVE_INTERRUPT))
        {
            ;
        }

        USCI_A_SPI_clearInterrupt(base_address, USCI_A_SPI_RECEIVE_INTERRUPT);

        res = USCI_A_SPI_receiveData(base_address);
    }
    else
    {
        /* Wait until new data was written into RX buffer */
        while(!USCI_B_SPI_getInterruptStatus(base_address, USCI_B_SPI_RECEIVE_INTERRUPT))
        {
            ;
        }

        USCI_B_SPI_clearInterrupt(base_address, USCI_B_SPI_RECEIVE_INTERRUPT);

        res = USCI_B_SPI_receiveData(base_address);
    }

    return res;
}

static uint8_t spi_transfer_byte(uint16_t base_address, uint8_t wb)
{
    spi_write_byte(base_address, wb);

    return spi_read_byte(base_address);
}

int spi_write(spi_port_t port, spi_cs_t cs, uint8_t *data, uint16_t len)
{
    int err = 0;

    uint16_t base_address = 0;

    switch(port)
    {
        case SPI_PORT_0:    base_address = USCI_A0_BASE;    break;
        case SPI_PORT_1:    base_address = USCI_A1_BASE;    break;
        case SPI_PORT_2:    base_address = USCI_A2_BASE;    break;
        case SPI_PORT_3:    base_address = USCI_B0_BASE;    break;
        case SPI_PORT_4:    base_address = USCI_B1_BASE;    break;
        case SPI_PORT_5:    base_address = USCI_B2_BASE;    break;
        default:
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_MODULE_NAME, "Error during writing: Invalid port!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;   /* Invalid SPI port */

            break;
    }

    if (err == 0)
    {
        if (spi_check_port(port))
        {
            /* Enable the CS pin */
            if (spi_select_slave(port, cs, true) == 0)
            {
                /* Write data */
                uint16_t i = 0;
                for(i = 0; i < len; i++)
                {
                    uint8_t dummy = spi_transfer_byte(base_address, data[i]);
                }

                /* Disable the CS pin */
                err = spi_select_slave(port, cs, false);
            }
            else
            {
                err = -1;   /* Invalid CS pin */
            }
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_MODULE_NAME, "Error during writing: Port ");
            sys_log_print_uint(port);
            sys_log_print_msg(" is not initialized!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;
        }
    }

    return err;
}

int spi_read(spi_port_t port, spi_cs_t cs, uint8_t *data, uint16_t len)
{
    int err = 0;

    uint16_t base_address = 0;

    switch(port)
    {
        case SPI_PORT_0:    base_address = USCI_A0_BASE;    break;
        case SPI_PORT_1:    base_address = USCI_A1_BASE;    break;
        case SPI_PORT_2:    base_address = USCI_A2_BASE;    break;
        case SPI_PORT_3:    base_address = USCI_B0_BASE;    break;
        case SPI_PORT_4:    base_address = USCI_B1_BASE;    break;
        case SPI_PORT_5:    base_address = USCI_B2_BASE;    break;
        default:
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_MODULE_NAME, "Error during reading: Invalid port!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;   /* Invalid SPI port */

            break;
    }

    if (err == 0)
    {
        if (spi_check_port(port))
        {
            /* Enable the CS pin */
            if (spi_select_slave(port, cs, true) == 0)
            {
                /* Read data */
                uint16_t i = 0;
                for(i = 0; i < len; i++)
                {
                    data[i] = spi_transfer_byte(base_address, 0);
                }

                /* Disable the CS pin */
                err = spi_select_slave(port, cs, false);
            }
            else
            {
                err = -1;   /* Invalid CS pin */
            }
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_MODULE_NAME, "Error during writing: Port ");
            sys_log_print_uint(port);
            sys_log_print_msg(" is not initialized!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;
        }
    }

    return err;
}

int spi_transfer(spi_port_t port, spi_cs_t cs, uint8_t *wd, uint8_t *rd, uint16_t len)
{
    int err = 0;

    uint16_t base_address = 0;

    switch(port)
    {
        case SPI_PORT_0:    base_address = USCI_A0_BASE;    break;
        case SPI_PORT_1:    base_address = USCI_A1_BASE;    break;
        case SPI_PORT_2:    base_address = USCI_A2_BASE;    break;
        case SPI_PORT_3:    base_address = USCI_B0_BASE;    break;
        case SPI_PORT_4:    base_address = USCI_B1_BASE;    break;
        case SPI_PORT_5:    base_address = USCI_B2_BASE;    break;
        default:            err = -1;                       break;
    }

    if (err == 0)
    {
        if (spi_check_port(port))
        {
            /* Enable the CS pin */
            if (spi_select_slave(port, cs, true) == 0)
            {
                /* Transfer data (write and read) */
                uint16_t i = 0;
                for(i = 0; i < len; i++)
                {
                    rd[i] = spi_transfer_byte(base_address, wd[i]);
                }

                /* Disable the CS pin */
                err = spi_select_slave(port, cs, false);
            }
            else
            {
                err = -1;   /* Invalid CS pin */
            }
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_MODULE_NAME, "Error during writing: Port ");
            sys_log_print_uint(port);
            sys_log_print_msg(" is not initialized!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;
        }
    }

    return err;
}

bool spi_check_port(spi_port_t port)
{
    bool state = false;

    switch(port)
    {
        case SPI_PORT_0:    state = spi_port_0_is_open;     break;
        case SPI_PORT_1:    state = spi_port_1_is_open;     break;
        case SPI_PORT_2:    state = spi_port_2_is_open;     break;
        case SPI_PORT_3:    state = spi_port_3_is_open;     break;
        case SPI_PORT_4:    state = spi_port_4_is_open;     break;
        case SPI_PORT_5:    state = spi_port_5_is_open;     break;
        default:            state = false;                  break;
    }

    return state;
}

/** \} End of spi group */
