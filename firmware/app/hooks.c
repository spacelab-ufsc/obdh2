/*
 * hooks.c
 * 
 * Copyright (C) 2019, SpaceLab.
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
 * \brief FreeRTOS hooks implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.0
 * 
 * \date 22/01/2020
 * 
 * \defgroup hooks FreeRTOS Hooks
 * \ingroup app
 * \{
 */

#include <FreeRTOS.h>
#include <task.h>

void vApplicationIdleHook(void)
{
    // Called on each iteration of the idle task. In this case the idle task just enters a low(ish) power mode
    __bis_SR_register(LPM1_bits + GIE);
}

void vApplicationMallocFailedHook(void)
{
    // Called if a call to pvPortMalloc() fails because there is insufficient free memory available in the
    // FreeRTOS heap. pvPortMalloc() is called internally by FreeRTOS API functions that create tasks, queues
    // or semaphores
    taskDISABLE_INTERRUPTS();

    while(1);
}

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    (void)pxTask;
    (void)pcTaskName;

    // Run time stack overflow checking is performed if configconfigCHECK_FOR_STACK_OVERFLOW is defined to 1 or
    // 2. This hook function is called if a stack overflow is detected
    taskDISABLE_INTERRUPTS();

    while(1);
}

//! \} End of hooks group
