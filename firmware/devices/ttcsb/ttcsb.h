/*
 * ttc.h
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
 * \brief TTC device definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.8.35
 * 
 * \date 2020/02/01
 * 
 * \defgroup ttcsb TTCSB
 * \ingroup devices
 * \{
 */

#ifndef TTCSB_H_
#define TTCSB_H_

#include <stdint.h>

#include <drivers/sl_ttcsb/sl_ttcsb.h> 

#include "ttcsb_data.h"

#define TTCSB_MODULE_NAME         "TTC SBAND"

#define TTCSB_SLAVE_ADDRESS           0x13    /**< 7-bit slave address. */

/* Commands IDs */
#define EDC_CMD_GET_HK_PKG          0x32U   /**< Updates HK Frame information */


typedef enum
{
    TTCSB_0=0,        /**< TTCSB 0. */
    TTCSB_1           /**< TTCSB 1. */
} ttcsb_e;

/**
 * \brief TTC configuration parameters.
 */
typedef sl_ttcsb_config_t ttcsb_config_t;

/**
 * \brief Parameter ID type.
 */
typedef uint8_t ttc_param_id_t;




int ttcsb_init();

/**
 * \brief Sets a parameter of the TTC device.
 *
 * \param[in] dev is the TTC device to set a parameter. It can be:
 * \parblock
 *      -\b TTC_0
 *      -\b TTC_1
 *      .
 * \endparblock
 *
 * \param[in] param is the parameter ID to set.
 *
 * \param[in] val is the new value of the given parameter.
 *
 * \return The status/error code.
 */
int ttcsb_set_param(ttcsb_e dev, ttc_param_id_t param, uint32_t val);

/**
 * \brief Gets a parameter from the TTC device.
 *
 * \param[in] dev is the TTC device to get a parameter. It can be:
 * \parblock
 *      -\b TTC_0
 *      -\b TTC_1
 *      .
 * \endparblock
 *
 * \param[in] param is the parameter ID to read.
 *
 * \param[in,out] val is a pointer to store the read value.
 *
 * \return The status/error code.
 */
int ttcsb_get_param(ttcsb_e dev, ttc_param_id_t param, uint32_t *val);

/**
 * \brief Reads the housekeeping data from the TTC device.
 *
 * \param[in] dev is the TTC device to initialized. It can be:
 * \parblock
 *      -\b TTC_0
 *      -\b TTC_1
 *      .
 * \endparblock
 *
 * \param[in,out] data is a pointer to store the read TTC data.
 *
 * \return The status/error code.
 */
int ttcsb_get_data(ttcsb_e dev, ttcsb_data_t *data);

/**
 * \brief Sends a downlink packet to the TTC device.
 *
 * \param[in] dev is the TTC device to initialized. It can be:
 * \parblock
 *      -\b TTC_0
 *      -\b TTC_1
 *      .
 * \endparblock
 *
 * \param[in] data is the data to transmit.
 *
 * \param[in] len is the number of bytes to transmit.
 *
 * \return The status/error code.
 */

#endif /* TTC_H_ */

/** \} End of ttc group */
