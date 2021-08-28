/*
 * antenna_wrap.h
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
 * \brief Antenna device wrap definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.4
 * 
 * \date 2021/08/14
 * 
 * \defgroup antenna_wrap Antenna Wrap
 * \ingroup tests
 * \{
 */

#ifndef ANTENNA_WRAP_H_
#define ANTENNA_WRAP_H_

#include <devices/antenna/antenna.h>

int __wrap_antenna_init(void);

int __wrap_antenna_get_status(void);

int __wrap_antenna_deploy(void);

#endif /* ANTENNA_WRAP_H_ */

/** \} End of antenna_wrap group */
