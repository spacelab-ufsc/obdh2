/*
 * eps.h
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
 * along with OBDH 2.0. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief REPS device definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Pedro Ferrari Barbosa <pedro.ferraribarbosa2007@gmail.com>
 * 
 * \version 1.0.0
 * 
 * \date 2026/11/12
 * 
 * \defgroup eps EPS
 * \ingroup devices
 * \{
 */

#ifndef REPS_H_
#define REPS_H_

#include <stdint.h>

#include "reps_data.h"


#define REPS_MODULE_NAME         "REPS"

int reps_init(void);

int reps_get_voltage(reps_hk_voltage_e which, uint16_t *data);

int reps_get_current(reps_hk_current_e which, uint16_t *data);


#endif /* REPS_H_ */

/** \} End of eps group */
