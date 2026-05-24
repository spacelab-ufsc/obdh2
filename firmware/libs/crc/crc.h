/*
 * crc.h
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
 * \brief Crc definition.
 *
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version
 *
 * \date
 *
 * \defgroup crc Crc
 * \ingroup crc
 * \{
 */

#ifndef LIBS_CRC_H_
#define LIBS_CRC_H_

#include <stdint.h>

uint8_t crc8_get_val(uint8_t *data, uint8_t len);

#endif /* LIBS_CRC_H_ */
