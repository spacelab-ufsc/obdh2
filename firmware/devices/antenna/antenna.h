/*
 * antenna.h
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
 * \brief Antenna device definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.2.4
 * 
 * \date 01/11/2019
 * 
 * \defgroup antenna Antenna
 * \ingroup devices
 * \{
 */

#ifndef ANTENNA_H_
#define ANTENNA_H_

/**
 * \brief Antenna initialization.
 *
 * \return The status/error code.
 */
int antenna_init();

/**
 * \brief Gets the antenna status.
 *
 * \return The status/error code of the antenna module.
 */
int antenna_get_status();

/**
 * \brief Execute the antenna deployment.
 *
 * \return The status/error code.
 */
int antenna_deploy();

#endif // ANTENNA_H_

/** \} End of antenna group */
