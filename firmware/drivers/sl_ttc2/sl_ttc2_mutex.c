/*
 * sl_ttc2_mutex.c
 *
 * Copyright The OBDH 2.0 Contributors
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
 * \brief SpaceLab TTC 2.0 driver mutex definition.
 *
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 *
 * \version 0.10.10
 *
 * \date 2024/02/26
 *
 * \addtogroup sl_ttc2
 * \{
 */

#include <stddef.h>

#include <FreeRTOS.h>
#include <semphr.h>

#include "sl_ttc2.h"

static SemaphoreHandle_t sl_ttc2_mutex = NULL;

int sl_ttc2_mutex_create(void)
{
    int err = 0;

    sl_ttc2_mutex = xSemaphoreCreateMutex();

    if (sl_ttc2_mutex == NULL)
    {
        err = -1;
    }

    return err;
}

int sl_ttc2_mutex_take(void)
{
    int err = -1;

    if (sl_ttc2_mutex != NULL)
    {
        /* See if we can obtain the semaphore. If the semaphore is not */
        /* available wait SL_TTC2_MUTEX_WAIT_TIME_MS ms to see if it becomes free */
        if (xSemaphoreTake(sl_ttc2_mutex, pdMS_TO_TICKS(SL_TTC2_MUTEX_WAIT_TIME_MS)) == pdTRUE)
        {
            err = 0;
        }
    }

    return err;
}

int sl_ttc2_mutex_give(void)
{
    int err = -1;

    if (sl_ttc2_mutex != NULL)
    {
        xSemaphoreGive(sl_ttc2_mutex);

        err = 0;
    }

    return err;
}

/** \} End of sl_ttc2_mutex group */
