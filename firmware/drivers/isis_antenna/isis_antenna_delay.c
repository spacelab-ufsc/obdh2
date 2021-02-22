/*
 * isis_antenna_delay.c
 * 
 * Copyright (C) 2020, SpaceLab.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with OBDH 2.0. If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * \brief ISIS antenna delay functions implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.4.9
 * 
 * \date 23/09/2016
 * 
 * \defgroup isis_antenna_delay Delay
 * \ingroup isis_antenna
 * \{
 */

#include <FreeRTOS.h>
#include <task.h>

#include <config/config.h>

#include "isis_antenna.h"

void isis_antenna_delay_s(uint8_t s)
{
    vTaskDelay(pdMS_TO_TICKS(1000*s));
}

void isis_antenna_delay_ms(uint16_t ms)
{
    vTaskDelay(pdMS_TO_TICKS(ms));
}

/** \} End of isis_antenna_delay group */
