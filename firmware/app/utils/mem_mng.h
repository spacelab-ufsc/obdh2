/*
 * mem_mng.h
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
 * \brief Memory management routines.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.10.9
 * 
 * \date 2024/02/24
 * 
 * \defgroup mem_mng Memory Management
 * \ingroup app
 * \{
 */

#ifndef MEM_MNG_H_
#define MEM_MNG_H_

#include <system/system.h>

#include <app/structs/obdh_data.h>

/**
 * \brief Checks if the FRAM memory is initialized or not.
 *
 * \return The status/error code.
 */
int mem_mng_check_fram(void);

/**
 * \brief Initializes the FRAM memory.
 *
 * \return The status/error code.
 */
int mem_mng_init_fram(void);

/**
 * \brief Resets the FRAM memory.
 *
 * \return The status/error code.
 */
int mem_mng_reset_fram(void);

/**
 * \brief Saves the OBDH data to the FRAM memory.
 *
 * \param[in] data is the OBDH data structure with the data to be saved.
 *
 * \param[in] ts is the timestamp of the given data.
 *
 * \return The status/error code.
 */
int mem_mng_save_obdh_data_to_fram(obdh_data_t data, sys_time_t ts);

/**
 * \brief Loads the last saved OBDH data from the FRAM memory.
 *
 * \param[in] data is a pointer to store the read data.
 *
 * \param[in] ts is a pointer to store the read timestamp of the data.
 *
 * \return The status/error code.
 */
int mem_mng_load_obdh_data_from_fram(obdh_data_t *data, sys_time_t *ts);

#endif /* MEM_MNG_H_ */

/** \} End of mem_mng group */
