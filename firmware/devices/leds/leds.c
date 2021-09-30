/*
 * leds.c
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
 * \brief System LEDs implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.26
 * 
 * \date 2020/01/20
 * 
 * \addtogroup leds
 * \{
 */

#include <system/sys_log/sys_log.h>
#include <drivers/gpio/gpio.h>

#include "leds.h"

/* GPIO configuration */
#define LED_SYSTEM_PIN          GPIO_PIN_35
#define LED_FAULT_PIN           GPIO_PIN_34

int leds_init(void)
{
    sys_log_print_event_from_module(SYS_LOG_INFO, LEDS_MODULE_NAME, "Initializing system LEDs...");
    sys_log_new_line();

    int err = -1;

    gpio_config_t config_sys = {0};
    config_sys.mode = GPIO_MODE_OUTPUT;

    gpio_config_t config_fault = {0};
    config_fault.mode = GPIO_MODE_OUTPUT;

    if ((gpio_init(LED_SYSTEM_PIN, config_sys) == 0) &&
        (gpio_init(LED_FAULT_PIN, config_fault) == 0))
    {
        err = 0;
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, LEDS_MODULE_NAME, "Error initializing the system LEDs!");
        sys_log_new_line();
    }

    return err;
}

int led_set(led_t l)
{
    int err = -1;

    switch(l)
    {
        case LED_SYSTEM:    err = gpio_set_state(LED_SYSTEM_PIN, true);     break;
        case LED_FAULT:     err = gpio_set_state(LED_FAULT_PIN, true);      break;
        default:            break;  /* Invalid LED */
    }

    return err;
}

int led_clear(led_t l)
{
    int err = -1;

    switch(l)
    {
        case LED_SYSTEM:    err = gpio_set_state(LED_SYSTEM_PIN, false);    break;
        case LED_FAULT:     err = gpio_set_state(LED_FAULT_PIN, false);     break;
        default:            break;  /* Invalid LED */
    }

    return err;
}

int led_toggle(led_t l)
{
    int err = -1;

    switch(l)
    {
        case LED_SYSTEM:    err = gpio_toggle(LED_SYSTEM_PIN);              break;
        case LED_FAULT:     err = gpio_toggle(LED_FAULT_PIN);               break;
        default:            break;  /* Invalid LED */
    }

    return err;
}

/** \} End of leds group */
