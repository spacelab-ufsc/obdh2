/*
 * task.c
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
 * \brief FreeRTOS task simulation implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.48
 * 
 * \date 2021/04/27
 * 
 * \addtogroup task_sim
 * \{
 */

#define _POSIX_C_SOURCE 200809L
#define _DEFAULT_SOURCE

#include <math.h>
#include <time.h>
#include <unistd.h>

#include "task.h"

uint64_t initial_time_ms = 0;

TickType_t xTaskGetTickCount(void)
{
    long ms;    /* Milliseconds */
    time_t s;   /* Seconds */
    struct timespec spec;

    clock_gettime(CLOCK_REALTIME, &spec);

    s = spec.tv_sec;
    ms = round(spec.tv_nsec / 1.0e6);   /* Convert nanoseconds to milliseconds */
    if (ms > 999)
    {
        s++;
        ms = 0;
    }

    uint64_t timestamp_ms = (1000*(uint64_t)s) + ms;

    if (initial_time_ms == 0)
    {
        initial_time_ms = timestamp_ms;
    }

    return timestamp_ms - initial_time_ms;
}

void vTaskSuspend(TaskHandle_t xTaskToSuspend)
{
    return;
}

void vTaskDelay(const TickType_t xTicksToDelay)
{
    usleep(1000*xTicksToDelay);
}

/** \} End of task_sim group */
