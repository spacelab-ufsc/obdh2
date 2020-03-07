/*
 * isis_antenna_delay.c
 * 
 * Copyright (C) 2017-2019, Universidade Federal de Santa Catarina.
 * 
 * This file is part of FloripaSat-TTC.
 * 
 * FloripaSat-TTC is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * FloripaSat-TTC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with FloripaSat-TTC. If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * \brief ISIS antenna delay functions implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.2.1
 * 
 * \date 23/09/2016
 * 
 * \defgroup isis_antenna_delay Delay
 * \ingroup isis_antenna
 * \{
 */

#include <msp430.h>
#include <config/config.h>

#include "isis_antenna.h"

void isis_antenna_delay_s(uint8_t s)
{
    while(s--)
    {
        __delay_cycles(BEACON_CPU_FREQ_HZ);
    }
}

void isis_antenna_delay_ms(uint16_t ms)
{
    while(ms--)
    {
        __delay_cycles(BEACON_CPU_FREQ_HZ/1000);
    }
}

void isis_antenna_delay_us(uint32_t us)
{
    while(us--)
    {
        __delay_cycles(BEACON_CPU_FREQ_HZ/1000000);
    }
}

//! \} End of isis_antenna_delay group
