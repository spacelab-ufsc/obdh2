/*
 * system.c
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
 * along with OBDH 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief System management routines implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 0.10.19
 * 
 * \date 2020/01/29
 * 
 * \addtogroup system
 * \{
 */

#include <msp430.h>
#include <drivers/gpio/gpio.h>
#include <portmacro.h>

#include "system.h"

static sys_time_t sys_time = 0;

void system_reset(void)
{
    PMMCTL0 = PMMPW | PMMSWBOR;     /* Triggers a software BOR */

    WDTCTL = 0xDEAD;                /* Reset system by writing to the WDT register without using the proper password */
}

uint8_t system_get_reset_cause(void)
{
    return (SYSRSTIV & 0xFF);
}

void system_set_time(sys_time_t tm)
{
    portENTER_CRITICAL();
    sys_time = tm;
    portEXIT_CRITICAL();
}

void system_increment_time(void)
{
    /* This ensures the sys_time increment is done atomically */
    portENTER_CRITICAL();
    sys_time++;
    portEXIT_CRITICAL();
}

sys_time_t system_get_time(void)
{
    return sys_time;
}

sys_hw_version_t system_get_hw_version(void)
{
    gpio_pin_t bit_0_pin = GPIO_PIN_14;
    gpio_pin_t bit_1_pin = GPIO_PIN_15;

    sys_hw_version_t res = HW_VERSION_UNKNOWN;

    gpio_config_t pin_conf = {0};

    pin_conf.mode = GPIO_MODE_INPUT;

    /* Initializing bit 0 GPIO pin */
    if (gpio_init(bit_0_pin, pin_conf) == 0)
    {
        /* Initializing bit 1 GPIO pin */
        if (gpio_init(bit_1_pin, pin_conf) == 0)
        {
            int bit_0_state = gpio_get_state(bit_0_pin);
            int bit_1_state = gpio_get_state(bit_1_pin);

            if ((bit_0_state != -1) && (bit_1_state != -1))
            {
                res = ((uint8_t)bit_1_state << 1) | (uint8_t)bit_0_state;
            }
        }
    }

    return res;
}

/** \} End of system group */
