/*
 * watchdog.h
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
 * \brief Watchdog device definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.0
 * 
 * \date 01/11/2019
 * 
 * \defgroup watchdog Watchdog
 * \ingroup devices
 * \{
 */

#ifndef WATCHDOG_H_
#define WATCHDOG_H_

/**
 * \brief Watchdog configuration.
 */
typedef struct
{
    uint8_t clk_src;        /**< Clock source of the watchdog peripheral. */
    uint8_t clk_div;        /**< Clock divider. */
} watchdog_config_t;

/**
 * \brief Watchdog initialization.
 *
 * \param[in] config is the configuration parameters of the watchdog device.
 *
 * \return The status/error code.
 */
int watchdog_init(watchdog_config_t config);

/**
 * \brief Wwatchdog timer reset.
 *
 * \return None.
 */
void watchdog_reset();

#endif // WATCHDOG_H_

//! \} End of watchdog group
