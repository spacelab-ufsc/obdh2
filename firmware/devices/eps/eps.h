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
 * \brief EPS device definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.8.35
 * 
 * \date 2020/02/01
 * 
 * \defgroup eps EPS
 * \ingroup devices
 * \{
 */

#ifndef EPS_H_
#define EPS_H_

#include <stdint.h>

#include "eps_data.h"

#define EPS_MODULE_NAME         "EPS"

/**
 * \brief Parameter ID type.
 */
typedef uint8_t eps_param_id_t;

/**
 * \brief Initialization of the EPS device.
 *
 * \return The status/error code.
 */
int eps_init(void);

/**
 * \brief Sets a parameter of the EPS device.
 *
 * \param[in] param is the parameter ID to set.
 *
 * \param[in] val is the new value of the given parameter.
 *
 * \return The status/error code.
 */
int eps_set_param(eps_param_id_t param, uint32_t val);

/**
 * \brief Gets a parameter from the EPS device.
 *
 * \param[in] param is the parameter ID to read.
 *
 * \param[in,out] val is a pointer to store the read value.
 *
 * \return The status/error code.
 */
int eps_get_param(eps_param_id_t param, uint32_t *val);

/**
 * \brief Gets the battery voltage from the EPS module.
 *
 * \param[in,out] bar_volt is a pointer to store the battery voltage.
 *
 * \return The status/error code.
 */
int eps_get_bat_voltage(eps_voltage_t *bat_volt);

/**
 * \brief Gets the battery current from the EPS module.
 *
 * \param[in,out] bat_cur is a pointer to store the raw battery current.
 *
 * \return The status/error code.
 */
int eps_get_bat_current(eps_current_t *bat_cur);

/**
 * \brief Gets the battery charge from the EPS module.
 *
 * \param[in,out] charge is a pointer to store the raw battery charge.
 *
 * \return The status/error code.
 */
int eps_get_bat_charge(eps_charge_t *charge);

/**
 * \brief Gets all the EPS available data.
 *
 * \param[in,out] data is a pointer to store the EPS data.
 *
 * \return The status/error code.
 */
int eps_get_data(eps_data_t *data);

#endif /* EPS_H_ */

/** \} End of eps group */
