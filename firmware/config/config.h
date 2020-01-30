/*
 * config.h
 * 
 * Copyright (C) 2020, SpaceLab.
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
 * \brief Configuration parameters definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.3
 * 
 * \date 26/10/2019
 * 
 * \defgroup config Configuration
 * \{
 */

#ifndef CONFIG_H_
#define CONFIG_H_

// Tasks
#define CONFIG_TASK_STARTUP_ENABLED                     1
#define CONFIG_TASK_WATCHDOG_RESET_ENABLED              1
#define CONFIG_TASK_HEARTBEAT_ENABLED                   1
#define CONFIG_TASK_SYSTEM_RESET_ENABLED                1
#define CONFIG_TASK_RADIO_RESET_ENABLED                 1

#endif // CONFIG_H_

//! \} End of config group
