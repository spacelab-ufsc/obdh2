/*
 * spi_mutex.c
 *
 * Copyright The OBDH 2.0 Contributors.
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
 * along with OBDH 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 *
 */

/**
 * \brief SPI mutex implementation.
 *
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 *
 * \version 0.10.10
 *
 * \date 2024/02/27
 *
 * \addtogroup spi_mutex
 * \{
 */

#include <freertos/include/FreeRTOS.h>
#include <freertos/include/semphr.h>
#include <stdbool.h>

#include "spi.h"

static xSemaphoreHandle spi_port0_mutex = NULL;

static bool mutex_is_initialized = false;

int spi_mutex_create(void)
{
    int err = 0;
    
    if (!mutex_is_initialized)
    {
        spi_port0_mutex = xSemaphoreCreateMutex();

        if (spi_port0_mutex != NULL)
        {
            mutex_is_initialized = true;
        }
        else 
        {
            err = -1;
        }
    }

    return err;
}

int spi_mutex_take(void)
{
    int err = -1;

    if (spi_port0_mutex != NULL)
    {
        if (xSemaphoreTake(spi_port0_mutex, pdMS_TO_TICKS(SPI_MUTEX_WAIT_TIME_MS)) == pdTRUE)
        {
            err = 0;
        }
    }

    return err;
}

int spi_mutex_give(void)
{
    int err = -1;

    if (spi_port0_mutex != NULL)
    {
        xSemaphoreGive(spi_port0_mutex);

        err = 0;
    }

    return err;
}

/** \} End of spi_mutex group */
