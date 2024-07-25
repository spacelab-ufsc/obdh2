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
 * \version 0.10.17
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

#include <app/structs/satellite.h>

#define MEM_MNG_NAME "Memory Management"

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
 * \brief Loads the OBDH data from default values.
 *
 * \param[in] tel is a pointer to store the default values of the OBDH data.
 *
 * \return None.
 */
void mem_mng_load_obdh_data_from_default_values(obdh_telemetry_t *tel);

/**
 * \brief Saves the OBDH data to the FRAM memory.
 *
 * \param[in] tel is the OBDH telemetry data to be saved.
 *
 * \return The status/error code.
 */
int mem_mng_save_obdh_data_to_fram(obdh_telemetry_t *tel);

/**
 * \brief Loads the last saved OBDH data from the FRAM memory.
 *
 * \param[in] tel is a pointer to store the read data.
 *
 * \return The status/error code.
 */
int mem_mng_load_obdh_data_from_fram(obdh_telemetry_t *tel);

#endif /* MEM_MNG_H_ */

/** \} End of mem_mng group */
