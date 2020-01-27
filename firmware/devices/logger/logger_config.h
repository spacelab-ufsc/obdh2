/*
 * logger_config.h
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
 * \brief Logger device configuration parameters.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.0
 * 
 * \date 22/02/2019
 * 
 * \defgroup logger_config Configuration
 * \ingroup logger
 * \{
 */

#ifndef LOGGER_CONFIG_H_
#define LOGGER_CONFIG_H_

#include <config/config.h>

#include "logger.h"

// Device name
#define LOGGER_DEVICE_NAME              "Logger"

// UART
#define LOGGER_UART_BAUDRATE_BPS        115200

// Mutex config.
#define LOGGER_MUTEX_WAIT_TIME_MS       100

// Log messages colors
#define LOGGER_SYSTEM_TIME_COLOR        LOGGER_COLOR_GREEN
#define LOGGER_MODULE_NAME_COLOR        LOGGER_COLOR_MAGENTA
#define LOGGER_WARNING_COLOR            LOGGER_COLOR_YELLOW
#define LOGGER_ERROR_COLOR              LOGGER_COLOR_RED

#endif // LOGGER_CONFIG_H_

//! \} End of logger_config group
