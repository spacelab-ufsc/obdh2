/*
 * antenna_data.h
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
 * \brief Antenna data structure definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.8.4
 * 
 * \date 2020/07/24
 * 
 * \defgroup antenna_data Data
 * \ingroup antenna
 * \{
 */

#ifndef ANTENNA_DATA_H_
#define ANTENNA_DATA_H_

#include <stdint.h>

#include <drivers/isis_antenna/isis_antenna.h>

/**
 * \brief Antenna data type.
 */
typedef isis_antenna_data_t antenna_data_t;

#endif /* ANTENNA_DATA_H_ */

/** \} End of antenna_data group */
