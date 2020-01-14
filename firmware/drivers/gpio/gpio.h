/*
 * gpio.h
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
 * \brief GPIO driver definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.0
 * 
 * \date 13/01/2020
 * 
 * \defgroup gpio GPIO
 * \ingroup drivers
 * \{
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <stdbool.h>

#define GPIO_STATE_HIGH     (true)
#define GPIO_STATE_LOW      (false)

/**
 * \brief GPIO pins.
 */
typedef enum
{
    GPIO_PIN_0=0,           /**< GPIO pin 0. */
    GPIO_PIN_1,             /**< GPIO pin 1. */
    GPIO_PIN_2,             /**< GPIO pin 2. */
    GPIO_PIN_3,             /**< GPIO pin 3. */
    GPIO_PIN_4,             /**< GPIO pin 4. */
    GPIO_PIN_5,             /**< GPIO pin 5. */
    GPIO_PIN_6,             /**< GPIO pin 6. */
    GPIO_PIN_7,             /**< GPIO pin 7. */
    GPIO_PIN_8,             /**< GPIO pin 8. */
    GPIO_PIN_9,             /**< GPIO pin 9. */
    GPIO_PIN_10,            /**< GPIO pin 10. */
    GPIO_PIN_11,            /**< GPIO pin 11. */
    GPIO_PIN_12,            /**< GPIO pin 12. */
    GPIO_PIN_13,            /**< GPIO pin 13. */
    GPIO_PIN_14,            /**< GPIO pin 14. */
    GPIO_PIN_15,            /**< GPIO pin 15. */
    GPIO_PIN_16,            /**< GPIO pin 16. */
    GPIO_PIN_17,            /**< GPIO pin 17. */
    GPIO_PIN_18,            /**< GPIO pin 18. */
    GPIO_PIN_19,            /**< GPIO pin 19. */
    GPIO_PIN_20,            /**< GPIO pin 20. */
    GPIO_PIN_21,            /**< GPIO pin 21. */
    GPIO_PIN_22,            /**< GPIO pin 22. */
    GPIO_PIN_23,            /**< GPIO pin 23. */
    GPIO_PIN_24,            /**< GPIO pin 24. */
    GPIO_PIN_25,            /**< GPIO pin 25. */
    GPIO_PIN_26,            /**< GPIO pin 26. */
    GPIO_PIN_27,            /**< GPIO pin 27. */
    GPIO_PIN_28,            /**< GPIO pin 28. */
    GPIO_PIN_29             /**< GPIO pin 29. */
} gpio_pins_e;

/**
 * \brief GPIO modes.
 */
typedef enum
{
    GPIO_MODE_OUTPUT=0,     /**< GPIO as output. */
    GPIO_MODE_INPUT         /**< GPIO as input. */
} gpio_modes_e;

/**
 * \brief GPIO pin configuration.
 */
typedef struct
{
    uint8_t mode;
} gpio_config_t;

/**
 * \brief GPIO pin.
 */
typedef uint8_t gpio_pin_t;

/**
 * \brief Initialization routine of a GPIO pin.
 *
 * \param[in] pin is the GPIO pin to initialize.
 *
 * \param[in] config is the configuration to initialize the GPIO pin.
 *
 * \return The status/error code.
 */
int gpio_init(gpio_pin_t pin, gpio_config_t config);

/**
 * \brief Sets the state of a given output GPIO pin.
 *
 * \param[in] pin is the GPIO pin to set the state.
 *
 * \param[in] level is the new state of the given GPIO pin. It can be:
 * \parblock
 *      - GPIO_STATE_HIGH
 *      - GPIO_STATE_LOW
 *      .
 * \endparblock
 *
 * \return The status/error code.
 */
int gpio_set_state(gpio_pin_t pin, bool state);

/**
 * \brief Gets the state of a given input GPIO pin.
 *
 * \param[in] pin is the the GPIO pin to get the state.
 *
 * \return The GPIO pin state. It can be:
 * \parblock
 *      - GPIO_STATE_HIGH
 *      - GPIO_STATE_LOW
 *      - -1 on error
 *      .
 * \endparblock
 */
int gpio_get_state(gpio_pin_t pin);

#endif // GPIO_H_

//! \} End of gpio group
