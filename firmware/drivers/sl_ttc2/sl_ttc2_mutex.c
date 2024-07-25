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
 * \brief SpaceLab TTC 2.0 driver mutex implementation.
 *
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 *
 * \version 0.10.14
 *
 * \date 2024/02/26
 *
 * \addtogroup sl_ttc2
 * \{
 */

#include <stddef.h>

#include <FreeRTOS.h>
#include <semphr.h>
#include <drivers/spi/spi.h>

#include "sl_ttc2.h"

int sl_ttc2_mutex_take(void)
{
    return spi_mutex_take();
}

int sl_ttc2_mutex_give(void)
{
    return spi_mutex_give();
}

/** \} End of sl_ttc2_mutex group */
