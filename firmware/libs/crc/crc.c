/*
 * crc.c
 *
 * Copyright The TTC 2.0 Contributors.
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
 * along with TTC 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 *
 */

/**
 * \brief Crc implementation.
 *
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 1.0.0
 *
 * \date 2025/03/09
 *
 * \addtogroup crc
 * \{
 */
#include <stdint.h>

#include "crc.h"

#define CRC8_INITIAL_VAL   0x00U       /* CRC8-CCITT initial value. */
#define CRC8_POLYNOMIAL    0x07U       /* CRC8-CCITT polynomial. */

uint8_t crc8_get_val(uint8_t *data, uint8_t len)
{
    uint8_t crc = CRC8_INITIAL_VAL;
    uint8_t i = 0U;
    uint8_t j = 0U;

    for(i = 0U; i < len; i++)
    {
        crc ^= data[i];

        j = 0U;
        for (j = 0U; j < 8U; j++)
        {
            crc = (crc << 1) ^ ((crc & 0x80U) ? CRC8_POLYNOMIAL : 0U);
        }

        crc &= 0xFFU;
    }

    return crc;
}

/**< \} End of crc group */
