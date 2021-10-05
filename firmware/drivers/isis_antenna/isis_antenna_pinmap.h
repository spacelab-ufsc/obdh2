/*
 * isis_antenna_pinmap.h
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with OBDH 2.0. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief ISIS antenna driver pin map.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.2.1
 * 
 * \date 2017/09/21
 * 
 * \defgroup isis_antenna_pinmap
 * \{
 */

#ifndef ISIS_ANTENNA_PINMAP_H_
#define ISIS_ANTENNA_PINMAP_H_

#include <config/pinmap.h>

#define ISIS_ANTENNA_I2C_PORT                   ANTENNA_I2C_PORT
#define ISIS_ANTENNA_I2C_USCI                   ANTENNA_I2C_USCI
#define ISIS_ANTENNA_I2C_BASE_ADDRESS           ANTENNA_I2C_BASE_ADDRESS

#define ISIS_ANTENNA_I2C_SDA_PORT               ANTENNA_I2C_SDA_PORT
#define ISIS_ANTENNA_I2C_SDA_PIN                ANTENNA_I2C_SDA_PIN

#define ISIS_ANTENNA_I2C_SCL_PORT               ANTENNA_I2C_SCL_PORT
#define ISIS_ANTENNA_I2C_SCL_PIN                ANTENNA_I2C_SCL_PIN

#define ISIS_ANTENNA_GPIO_DEBUG_PORT            ANTENNA_GPIO_DEBUG_PORT
#define ISIS_ANTENNA_GPIO_DEBUG_PIN             ANTENNA_GPIO_DEBUG_PIN

#endif /* ISIS_ANTENNA_PINMAP_H_ */

/** \} End of isis_antenna_pinmap group */
