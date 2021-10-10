/*
 * sl_eps.h
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
 * \brief SpaceLab EPS driver definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.46
 * 
 * \date 2020/07/18
 * 
 * \defgroup sl_eps SpaceLab EPS
 * \ingroup drivers
 * \{
 */

#ifndef SL_EPS_H_
#define SL_EPS_H_

#include <stdint.h>

#include <drivers/i2c/i2c.h>

#define SL_EPS_MODULE_NAME      "SL-EPS"    /**< Module name. */

#define SL_EPS_SLAVE_ADR        0x36        /**< 7-bit slave address. */

/**
 * \brief I2C port configuration.
 */
typedef struct
{
    i2c_port_t port;
    uint32_t bitrate;
} sl_eps_i2c_config_t;

/**
 * \brief EPS data structure.
 */
typedef struct
{
    uint16_t voltage;
} sl_eps_data_t;

/**
 * \brief Configuration parameters structure of the driver.
 */
typedef i2c_config_t sl_eps_config_t;

/**
 * \brief Initialization of the EPS module driver.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \return The status/error code.
 */
int sl_eps_init(i2c_port_t port, uint32_t bitrate);

/**
 * \brief Reads the EPS data.
 *
 * \param[in,out] data is a pointer to store the read EPS data.
 *
 * \return The status/error code.
 */
int sl_eps_read(sl_eps_data_t *data);

#endif /* SL_EPS_H_ */

/** \} End of sl_eps group */
