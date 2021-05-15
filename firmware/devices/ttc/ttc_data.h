/*
 * ttc_data.h
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
 * \brief TTC data structure definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.6.22
 * 
 * \date 2021/05/15
 * 
 * \defgroup ttc_data TTC Data
 * \ingroup ttc
 * \{
 */

#ifndef TTC_DATA_H_
#define TTC_DATA_H_

#include "drivers/sl_ttc2/sl_ttc2.h"

/**
 * \brief TTC data.
 */
typedef sl_ttc2_hk_data_t ttc_data_t;

#endif /* TTC_DATA_H_ */

/** \} End of ttc_data group */
