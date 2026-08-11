/*
 * mt25q_mutex.c
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
 * \brief Mt25q driver mutex implementation.
 *
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 *
 * \version 0.10.17
 *
 * \date 2024/02/26
 *
 * \addtogroup mt25q
 * \{
 */

#include <stddef.h>

#include <FreeRTOS.h>
#include <semphr.h>
#include <drivers/spi/spi.h>

#include "mt25q.h"

int mt25q_mutex_take(void)
{
    return spi_mutex_take();
}

int mt25q_mutex_give(void)
{
    return spi_mutex_give();
}

/** \} End of mt25q group */
