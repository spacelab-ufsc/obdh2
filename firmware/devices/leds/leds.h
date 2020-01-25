/*
 * leds.h
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
 * \brief System LEDs definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.0
 * 
 * \date 20/01/2020
 * 
 * \defgroup leds LEDs
 * \ingroup devices
 * \{
 */

#ifndef LEDS_H_
#define LEDS_H_

#include <stdint.h>

/**
 * \brief System LEDs.
 */
typedef enum
{
    LED_SYSTEM=0,
    LED_FAULT
} leds_types_e;

/**
 * \brief LED type.
 */
typedef uint8_t led_t;

/**
 * \brief Initialization routine of the LEDs.
 *
 * \return The status/error code.
 */
int leds_init();

/**
 * \brief Sets a given LED.
 *
 * \param[in] l is the LED to set.
 *
 * \return The status/error code.
 */
int led_set(led_t l);

/**
 * \brief Clears a given LED.
 *
 * \param[in] l is the LED to clear.
 *
 * \return The status/error code.
 */
int led_clear(led_t l);

/**
 * \brief Toggles a given LED.
 *
 * \param[in] l is the LED to toggle.
 *
 * \return The status/error code.
 */
int led_toggle(led_t l);

#endif // LEDS_H_

//! \} End of leds group
