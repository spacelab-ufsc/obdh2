/*
 * isis_antenna_config.h
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
 * \brief ISIS antenna driver general configuration.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.5.6
 * 
 * \date 21/09/2017
 * 
 * \defgroup isis_antenna_config Configuration
 * \ingroup isis_antenna
 * \{
 */

#ifndef ISIS_ANTENNA_CONFIG_H_
#define ISIS_ANTENNA_CONFIG_H_

#include <drivers/driverlib/driverlib.h>

#define ISIS_ANTENNA_MODULE_NAME                "ISIS Antenna"

#define ISIS_ANTENNA_I2C_SLAVE_ADDRESS_uC_A     0x31
#define ISIS_ANTENNA_I2C_SLAVE_ADDRESS_uC_B     0x32

#define ISIS_ANTENNA_I2C_SLAVE_ADDRESS          ISIS_ANTENNA_I2C_SLAVE_ADDRESS_uC_B

#if ISIS_ANTENNA_I2C_USCI == USCI_A
    #define ISIS_ANTENNA_I2C_CLOCK_SOURCE       USCI_A_I2C_CLOCKSOURCE_SMCLK
    #define ISIS_ANTENNA_I2C_DATA_RATE          USCI_A_I2C_SET_DATA_RATE_100KBPS
#elif ISIS_ANTENNA_I2C_USCI == USCI_B
    #define ISIS_ANTENNA_I2C_CLOCK_SOURCE       USCI_B_I2C_CLOCKSOURCE_SMCLK
    #define ISIS_ANTENNA_I2C_DATA_RATE          USCI_B_I2C_SET_DATA_RATE_100KBPS
#endif // ISIS_ANTENNA_I2C_USCI

#define ISIS_ANTENNA_I2C_CLOCK                  UCS_getSMCLK()

#define ISIS_ANTENNA_I2C_TIMEOUT                10000
#define ISIS_ANTENNA_I2C_TIMEOUT_MS             100

#endif // ISIS_ANTENNA_CONFIG_H_

//! \} End of isis_antenna_config group
