/*
 * system_wrap.h
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
 * \brief System wrap definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.9.0
 * 
 * \date 2022/03/14
 * 
 * \defgroup system_wrap System Wrap
 * \ingroup tests
 * \{
 */

#ifndef SYSTEM_WRAP_H_
#define SYSTEM_WRAP_H_

#include <system/system.h>

void __wrap_system_reset(void);

uint8_t __wrap_system_get_reset_cause(void);

void __wrap_system_set_time(sys_time_t tm);

void __wrap_system_increment_time(void);

sys_time_t __wrap_system_get_time(void);

sys_hw_version_t __wrap_system_get_hw_version(void);

#endif /* SYSTEM_WRAP_H_ */

/** \} End of system_wrap group */
