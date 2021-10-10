/*
 * sys_log_mutes.h
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
 * along with OBDH 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief System log mutex implementation.
 *
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 *
 * \version 0.7.25
 *
 * \date 2019/11/03
 *
 * \defgroup sys_log_mutex Mutex
 * \ingroup sys_log
 * \{
 */

#include <FreeRTOS.h>
#include <semphr.h>

#include "sys_log.h"
#include "sys_log_config.h"

SemaphoreHandle_t xSysLogSemaphore = NULL;

int sys_log_mutex_create(void)
{
    /* Create a mutex type semaphore */
    xSysLogSemaphore = xSemaphoreCreateMutex();

    int err = 0;

    if (xSysLogSemaphore == NULL)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, SYS_LOG_DEVICE_NAME, "Error creating a mutex!");
        sys_log_new_line();

        err = -1;
    }

    return err;
}

int sys_log_mutex_take(void)
{
    int err = -1;

    if (xSysLogSemaphore != NULL)
    {
        /* See if we can obtain the semaphore. If the semaphore is not */
        /* available wait SYS_LOG_MUTEX_WAIT_TIME_MS ms to see if it becomes free */
        if (xSemaphoreTake(xSysLogSemaphore, pdMS_TO_TICKS(SYS_LOG_MUTEX_WAIT_TIME_MS)) == pdTRUE)
        {
            err = 0;
        }
    }

    return err;
}

int sys_log_mutex_give(void)
{
    int err = -1;

    if (xSysLogSemaphore != NULL)
    {
        xSemaphoreGive(xSysLogSemaphore);

        err = 0;
    }

    return err;
}

/** \} End of sys_log_mutex group */
