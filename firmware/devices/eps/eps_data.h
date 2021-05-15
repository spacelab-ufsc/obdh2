/*
 * eps_data.h
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
 * \brief EPS data structure definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.6.10
 * 
 * \date 2020/07/16
 * 
 * \defgroup eps_data EPS Data
 * \ingroup eps
 * \{
 */

#ifndef EPS_DATA_H_
#define EPS_DATA_H_

#include <stdint.h>

#include <drivers/sl_eps2/sl_eps2.h>

/**
 * \brief EPS data structure.
 */
typedef sl_eps2_data_t eps_data_t;

/**
 * \brief EPS battery voltage type.
 */
typedef sl_eps2_voltage_t eps_bat_voltage_t;

#endif /* EPS_DATA_H_ */

/** \} End of eps_data group */
