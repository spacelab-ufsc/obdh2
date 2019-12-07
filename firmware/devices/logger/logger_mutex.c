/*
 * debug_mutex.h
 * 
 * Copyright (C) 2017-2019, SpaceLab.
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
 * \brief Logger device mutex implementation.
 *
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 *
 * \version 0.1.0
 *
 * \date 03/11/2019
 *
 * \defgroup logger_mutex Mutex
 * \ingroup logger
 * \{
 */

#include "logger.h"
#include "logger_config.h"

SemaphoreHandle_t xLoggerSemaphore = NULL;

bool logger_mutex_create()
{
    // Create a mutex type semaphore
    xLoggerSemaphore = xSemaphoreCreateMutex();

    if (xLoggerSemaphore == NULL)
    {
        logger_print_event_from_module(LOGGER_ERROR, LOGGER_DEVICE_NAME, "Error creating a mutex!");
        logger_new_line();

        return false;
    }

    return true;
}

bool logger_mutex_take()
{
    if (xLoggerSemaphore != NULL)
    {
        // See if we can obtain the semaphore. If the semaphore is not
        // available wait LOGGER_MUTEX_WAIT_TIME_MS ms to see if it becomes free
        if (xSemaphoreTake(xLoggerSemaphore, pdMS_TO_TICKS(LOGGER_MUTEX_WAIT_TIME_MS)) == pdTRUE)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool logger_mutex_give()
{
    if (xLoggerSemaphore != NULL)
    {
        xSemaphoreGive(xLoggerSemaphore);

        return true;
    }
    else
    {
        return false;
    }
}

//! \} End of logger_mutex group
