/*
 * payload.h
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
 * \brief Payload device definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.8.16
 * 
 * \date 2021/08/15
 * 
 * \defgroup payload Payload
 * \ingroup devices
 * \{
 */

#ifndef PAYLOAD_H_
#define PAYLOAD_H_

#include <stdint.h>

#define PAYLOAD_MODULE_NAME         "Payload"

/**
 * \brief Payload type.
 */
typedef enum
{
    PAYLOAD_EDC_0=0,            /**< Payload EDC 0. */
    PAYLOAD_EDC_1,              /**< Payload EDC 1. */
    PAYLOAD_X,                  /**< Payload-X. */
    PAYLOAD_PHJ,                /**< Payload PHJ. */
    PAYLOAD_HARSH               /**< Payload Harsh. */
} payload_t;

/**
 * \brief Payload data ID.
 */
typedef enum
{
    PAYLOAD_EDC_RAW_STATE=0,    /**< EDC raw state. */
    PAYLOAD_EDC_RAW_HK          /**< EDC raw housekeeping. */
} payload_data_id_t;

/**
 * \brief Payload command.
 */
typedef uint8_t payload_cmd_t;

/**
 * \brief Payload device initialization.
 *
 * \param[in] pl is the payload device to initialize. It can be:
 * \parblock
 *      -\b PAYLOAD_EDC
 *      -\b PAYLOAD_X
 *      -\b PAYLOAD_PHJ
 *      -\b PAYLOAD_HARSH
 *      .
 * \endparblock
 *
 * \return The status/error code.
 */
int payload_init(payload_t pl);

/**
 * \brief Enables a given payload payload.
 *
 * \param[in] pl is the payload device to enable. It can be:
 * \parblock
 *      -\b PAYLOAD_EDC
 *      -\b PAYLOAD_X
 *      -\b PAYLOAD_PHJ
 *      -\b PAYLOAD_HARSH
 *      .
 * \endparblock
 *
 * \return The status/error code.
 */
int payload_enable(payload_t pl);

/**
 * \brief Disables a given payload.
 *
 * \param[in] pl is the payload device to disable. It can be:
 * \parblock
 *      -\b PAYLOAD_EDC
 *      -\b PAYLOAD_X
 *      -\b PAYLOAD_PHJ
 *      -\b PAYLOAD_HARSH
 *      .
 * \endparblock
 *
 * \return Thet status/error code.
 */
int payload_disable(payload_t pl);

/**
 * \brief Writes a command to a given payload.
 *
 * \param[in] pl is the payload device to disable. It can be:
 * \parblock
 *      -\b PAYLOAD_EDC
 *      -\b PAYLOAD_X
 *      -\b PAYLOAD_PHJ
 *      -\b PAYLOAD_HARSH
 *      .
 * \endparblock
 *
 * \param[in] cmd is the command to write to the given payload.
 *
 * \return Thet status/error code.
 */
int payload_write_cmd(payload_t pl, payload_cmd_t cmd);

/**
 * \brief Gets data from a given payload.
 *
 * \param[in] pl is the payload device to initialize. It can be:
 * \parblock
 *      -\b PAYLOAD_EDC
 *      -\b PAYLOAD_X
 *      -\b PAYLOAD_PHJ
 *      -\b PAYLOAD_HARSH
 *      .
 * \endparblock
 *
 * param[in] id is the data ID. It can be:
 * \parblock
 *      -\b PAYLOAD_EDC_RAW_STATE
 *      -\b PAYLOAD_EDC_RAW_HK
 *      .
 * \endparblock
 *
 * \param[in,out] data is a pointer to the read data from the payload device.
 *
 * \param[in,out] len is the number of bytes returned by the payload device.
 *
 * \return The status/error code.
 */
int payload_get_data(payload_t pl, payload_data_id_t id, uint8_t *data, uint32_t *len);

#endif /* PAYLOAD_H_ */

/** \} End of payload group */
