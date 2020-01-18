/*
 * watchdog.c
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
 * \brief Watchdog device implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.0
 * 
 * \date 01/11/2019
 * 
 * \addtogroup watchdog
 * \{
 */

#include <drivers/wdt.h>
#include <drivers/tps382x.h>

#include "watchdog.h"

int watchdog_init()
{
    wdt_config_t int_wdt;

    int_wdt.clk_src = WDT_A_CLOCKSOURCE_SMCLK;
    int_wdt.clk_div = WDT_A_CLOCKDIVIDER_32K;

    tps382x_config_t ext_wdt;

    ext_wdt.wdi_pin = GPIO_PIN_66;

    return wdt_init(int_wdt) | tps382x_init(ext_wdt);
}

void watchdog_reset()
{
    wdt_reset();

    tps382x_trigger();
}

//! \} End of watchdog group
