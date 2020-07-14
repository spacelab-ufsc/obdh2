/*
 * data.h
 * 
 * Copyright (C) 2020, SpaceLab.
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
 * \brief Data structures definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.4.5
 * 
 * \date 2020/07/13
 * 
 * \defgroup obdh_data OBDH Data
 * \{
 */

#ifndef DATA_H_
#define DATA_H_

#include <stdint.h>

/**
 * \brief OBDH data structure.
 */
typedef struct
{
    /* OBDH data */
    uint16_t obdh_temp;             /**< OBDH uC temperature. */
    uint16_t obdh_current;          /**< OBDH board current. */
    uint16_t obdh_voltage;          /**< OBDH board voltage. */
    /* EPS data */
} obdh_data_t;

/**
 * \brief OBDH data buffer.
 */
extern obdh_data_t obdh_data_buf;

#endif /* DATA_H_ */

/** \} End of data group */
