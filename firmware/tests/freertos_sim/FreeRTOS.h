/*
 * FreeRTOS.h
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
 * along with OBDH 2.0. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief FreeRTOS simulation definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.6.5
 * 
 * \date 2021/04/27
 * 
 * \defgroup freertos_sim FreeRTOS
 * \ingroup tests
 * \{
 */

#ifndef FREERTOS_SIM_H_
#define FREERTOS_SIM_H_

#include <stdint.h>

#define pdMS_TO_TICKS(x)    (x)

/**
 * \brief Tick type.
 */
typedef uint32_t TickType_t;

#endif /* FREERTOS_SIM_H_ */

/** \} End of freertos_sim group */
