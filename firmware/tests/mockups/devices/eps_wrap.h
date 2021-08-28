/*
 * eps_wrap.h
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
 * \brief EPS device wrap definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.2
 * 
 * \date 2021/08/15
 * 
 * \defgroup eps_wrap EPS Wrap
 * \ingroup tests
 * \{
 */

#ifndef EPS_WRAP_H_
#define EPS_WRAP_H_

#include <stdint.h>

#include <devices/eps/eps.h>

int __wrap_eps_init(void);

int __wrap_eps_get_bat_voltage(eps_bat_voltage_t *bat_volt);

int __wrap_eps_get_bat_current(uint32_t *bat_cur);

int __wrap_eps_get_bat_charge(uint32_t *charge);

int __wrap_eps_get_data(eps_data_t *data);

#endif /* EPS_WRAP_H_ */

/** \} End of eps_wrap group */
