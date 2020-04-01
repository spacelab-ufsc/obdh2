/*
 * drivers.h
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with OBDH 2.0. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Drivers layer definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.3.5
 * 
 * \date 26/10/2019
 * 
 * \defgroup drivers Drivers
 * \{
 */

#ifndef DRIVERS_H_
#define DRIVERS_H_

#include "edc/edc.h"
#include "i2c/i2c.h"
#include "isis_antenna/isis_antenna.h"
#include "mt25ql01gbbb/mt25ql01gbbb.h"
#include "spi/spi.h"
#include "si446x/si446x.h"
#include "uart/uart.h"
#include "gpio/gpio.h"
#include "tps382x/tps382x.h"
#include "wdt/wdt.h"
#include "tca4311a/tca4311a.h"
#include "sl_eps2/sl_eps2.h"
#include "adc/adc.h"
#include "flash/flash.h"

#endif /* DRIVERS_H_ */

/** \} End of drivers group */
