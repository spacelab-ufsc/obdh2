/*
 * semphr.h
 * 
 * Copyright (C) 2021, SpaceLab.
 * 
 * This file is part of TTC 2.0.
 * 
 * TTC 2.0 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * TTC 2.0 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with TTC 2.0. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief FreeRTOS semphr simulation definition.
 * 
 * \author Miguel Boing <miguelboing13@gmail.com>
 * 
 * \version 5.1.0
 * 
 * \date 2024/04/25
 * 
 * \defgroup semphr_sim FreeRTOS semphr
 * \ingroup tests
 * \{
 */

#ifndef SEMPHR_SIM_H_
#define SEMPHR_SIM_H_

#include <stdint.h>
#include "FreeRTOS.h"

typedef enum
{
	pdFALSE=0x00,
	pdTRUE=0x01,
}BaseType_t;

typedef int* SemaphoreHandle_t;

SemaphoreHandle_t xSemaphoreCreateMutex(void);

BaseType_t xSemaphoreTake(SemaphoreHandle_t xSemaphore, TickType_t xBlockTime);

BaseType_t xSemaphoreGive(SemaphoreHandle_t xSemaphore);

#endif /* SEMPHR_SIM_H_ */

/** \} End of semphr_sim group */
