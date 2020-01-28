/*
 * tasks.h
 * 
 * Copyright (C) 2019, SpaceLab.
 * 
 * This file is part of OBDH 2.0.
 * 
 * OBDH 2.0 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * OBDH 2.0 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with OBDH 2.0. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Tasks definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.0
 * 
 * \date 02/11/2019
 * 
 * \defgroup tasks Tasks
 * \{
 */

#ifndef TASKS_H_
#define TASKS_H_

/**
 * \brief Creates the system tasks.
 *
 * \return None.
 */
void create_tasks();

/**
 * \brief Creates the system event groups.
 *
 * \return None.
 */
void create_event_groups();

#endif // TASKS_H_

//! \} End of tasks group
