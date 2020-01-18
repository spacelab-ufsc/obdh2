/*
 * wdt.h
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
 * \brief Internal watchdog timer driver definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.0
 * 
 * \date 17/01/2020
 * 
 * \defgroup wdt WDT
 * \ingroup drivers
 * \{
 */

#ifndef WDT_H_
#define WDT_H_

/**
 * \brief Watchdog timer configuration.
 */
typedef struct
{
    uint8_t clk_src;        /**< Clock source of the watchdog peripheral. */
    uint8_t clk_div;        /**< Clock divider. */
} wdt_config_t;

/**
 * \brief Internal watchdog initialization routine.
 *
 * This function configures the WDT peripheral of the MCU.
 *
 * \param[in] config is the configuration parameters of the watchdog timer peripheral.
 *
 * \return The status/error code.
 */
int wdt_init(wdt_config_t config);

/**
 * \brief Resets the watchdog timer counter.
 *
 * \param[in] config is the configuration parameters of the watchdog timer peripheral.
 *
 * \return None.
 */
void wdt_reset(wdt_config_t config);

#endif // WDT_H_

//! \} End of wdt group
