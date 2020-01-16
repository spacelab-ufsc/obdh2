/*
 * tps382x.h
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
 * \brief TPS382x driver definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.0
 * 
 * \date 15/01/2020
 * 
 * \defgroup tps382x TPS382x
 * \ingroup drivers
 * \{
 */

#ifndef TPS382X_H_
#define TPS382X_H_

#include <drivers/gpio/gpio.h>

/**
 * \brief TPS382x configuration.
 */
typedef struct
{
    gpio_pin_t wdi_pin;
} tps382x_config_t;

/**
 * \brief TPS382x initialization routine.
 *
 * \param[in] config is the configuration parameters of the TPS382x driver.
 *
 * \return The status/error code.
 */
int tps382x_init(tps382x_config_t config);

/**
 * \brief Triggers the WDI pin of the device by toggling a GPIO pin.
 *
 * \param[in] config is the configuration parameters of the TPS382x driver.
 *
 * \return None.
 */
void tps271x_trigger(tps382x_config_t config);

#endif // TPS382X_H_

//! \} End of tps382x group
