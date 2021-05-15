/*
 * ttc.h
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
 * \brief TTC device definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.6.22
 * 
 * \date 2020/02/01
 * 
 * \defgroup ttc TTC
 * \ingroup devices
 * \{
 */

#ifndef TTC_H_
#define TTC_H_

#include <stdint.h>

#include "drivers/sl_ttc2/sl_ttc2.h"
#include "ttc_data.h"

#define TTC_MODULE_NAME         "TTC"

/**
 * \brief TTC configuration parameters.
 */
typedef sl_ttc2_config_t ttc_config_t;

/**
 * \brief TTC devices.
 */
typedef enum
{
    TTC_0=0,        /**< TTC 0. */
    TTC_1           /**< TTC 1. */
} ttc_e;

/**
 * \brief Initialization routine of the TTC device.
 *
 * \param[in] dev is the TTC device to initialized. It can be:
 * \parblock
 *      -\b TTC_0
 *      -\b TTC_1
 *      .
 * \endparblock
 *
 * \return The status/error code.
 */
int ttc_init(ttc_e dev);

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
int ttc_get_data(ttc_e dev, ttc_data_t *data);

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
int ttc_send(ttc_e dev, uint8_t *data, uint16_t len);

/**
 * \brief Receives an uplink packet from the TTC device.
 *
 * \param[in] dev is the TTC device to initialized. It can be:
 * \parblock
 *      -\b TTC_0
 *      -\b TTC_1
 *      .
 * \endparblock
 *
 * \param[in,out] data is a pointer to store the received packet.
 *
 * \param[in,out] len is the number of bytes of the received packet.
 *
 * \return The status/error code.
 */
int ttc_recv(ttc_e dev, uint8_t *data, uint16_t *len);

/**
 * \brief Gets the number available packets to read.
 *
 * \param[in] dev is the TTC device to initialized. It can be:
 * \parblock
 *      -\b TTC_0
 *      -\b TTC_1
 *      .
 * \endparblock
 *
 * \return The status/error code.
 */
int ttc_avail(ttc_e dev);

/**
 * \brief Enables the TTC hibernation for a given period.
 *
 * \param[in] dev is the TTC device to initialized. It can be:
 * \parblock
 *      -\b TTC_0
 *      -\b TTC_1
 *      .
 * \endparblock
 *
 * \return The status/error code.
 */
int ttc_enter_hibernation(ttc_e dev);

/**
 * \brief Disables the TTC hibernation.
 *
 * \param[in] dev is the TTC device to initialized. It can be:
 * \parblock
 *      -\b TTC_0
 *      -\b TTC_1
 *      .
 * \endparblock
 *
 * \return The status/error code.
 */
int ttc_leave_hibernation(ttc_e dev);

#endif /* TTC_H_ */

/** \} End of ttc group */
