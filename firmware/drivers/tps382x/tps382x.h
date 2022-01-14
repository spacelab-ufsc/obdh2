/*
 * tps382x.h
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
 * along with OBDH 2.0. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief TPS382x driver definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.8.20
 * 
 * \date 2020/01/15
 * 
 * \defgroup tps382x TPS382x
 * \ingroup drivers
 * \{
 */

#ifndef TPS382X_H_
#define TPS382X_H_

#include <drivers/gpio/gpio.h>

#define TPS382X_MODULE_NAME     "TPS382x"

/**
 * \brief TPS382x configuration.
 */
typedef struct
{
    gpio_pin_t wdi_pin;     /**< WDI pin. */
    gpio_pin_t mr_pin;      /**< MR pin. */
} tps382x_config_t;

/**
 * \brief TPS382x initialization routine.
 *
 * This function configures the WDI pin as an GPIO output pin.
 *
 * \param[in] config is the configuration parameters of the TPS382x driver.
 *
 * \return The status/error code.
 */
int tps382x_init(tps382x_config_t config);

/**
 * \brief Triggers the WDI pin of the device by toggling a GPIO pin.
 *
 * If WDI remains high or low longer than the timeout period, then reset is triggered.
 * The timer clears when reset is asserted or when WDI sees a rising edge or a falling edge.
 *
 * \see SLVS165L - TPS382x Voltage Monitor With Watchdog Timer.
 *
 * \param[in] config is the configuration parameters of the TPS382x driver.
 *
 * \return The status/error code.
 */
int tps382x_trigger(tps382x_config_t config);

/**
 * \brief Sets the MR pin low to force a manual reset.
 *
 * Manual-reset input. Pull low to force a reset. RESET remains low as long as MR is low
 * and for the time-out period after MR goes high. Leave unconnected or connect to VDD when unused.
 *
 * \see SLVS165L - TPS382x Voltage Monitor With Watchdog Timer.
 *
 * \param[in] config is the configuration parameters of the TPS382x driver.
 *
 * \return The status/error code.
 */
int tps382x_manual_reset(tps382x_config_t config);

#endif /* TPS382X_H_ */

/** \} End of tps382x group */
