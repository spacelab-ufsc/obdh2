/*
 * watchdog.c
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
 * \brief Watchdog device implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.26
 * 
 * \date 2019/11/01
 * 
 * \addtogroup watchdog
 * \{
 */

#include <drivers/wdt/wdt.h>
#include <drivers/tps382x/tps382x.h>

#include "watchdog.h"

int watchdog_init(void)
{
    wdt_config_t int_wdt;

    int_wdt.clk_src = WDT_CLK_SRC_ACLK;
    int_wdt.clk_div = WDT_CLK_DIV_32K;

    tps382x_config_t ext_wdt;

    ext_wdt.wdi_pin = GPIO_PIN_66;

    return wdt_init(int_wdt) | tps382x_init(ext_wdt);
}

void watchdog_reset(void)
{
    wdt_reset();

    tps382x_config_t ext_wdt;

    ext_wdt.wdi_pin = GPIO_PIN_66;

    tps382x_trigger(ext_wdt);
}

/** \} End of watchdog group */
