/*
 * payload.h
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
 * \brief RO device definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * \author Renato Augusto Schenkel Meneghin Marchiori
 * 
 * \version 1.0.0
 * 
 * \date 2026/08/13
 * 
 * \defgroup ro RO
 * \ingroup devices
 * \{
 */

#ifndef RO_H_
#define RO_H_

#include <stdint.h>

#include <structs/satellite.h>

#define RO_MODULE_NAME         "RO"

/**
 * \brief RO type.
 */
typedef enum  
{ 
  RO_NONE = 0, /**< Means there's no RO active */
  RO_1 = PL_ID_RO_1, /**< RO 1. */
} 
ro_type_t;

/**
 * \brief RO data ID.
 */
typedef enum
{
    PAYLOAD_RO_RAW_STATE=0,     /**< RO raw state. */
    PAYLOAD_RO_STATE,           /**< RO state. */
    PAYLOAD_RO_RAW_EVENT,       /**< RO raw event. */
    PAYLOAD_RO_EVENT,           /**< RO event. */
    PAYLOAD_RO_RAW_NAVIGATION,  /**< RO raw navigation. */
    PAYLOAD_RO_NAVIGATION,      /**< RO navigation. */
    PAYLOAD_RO_RAW_OBSERVATION, /**< RO raw observables. */
    PAYLOAD_RO_OBSERVATION,     /**< RO observables. */
    PAYLOAD_RO_RAW_IQ_HEADER,   /**< RO raw IQ header. */
    PAYLOAD_RO_IQ_HEADER,       /**< RO IQ header. */
    PAYLOAD_RO_RAW_HK,          /**< RO raw housekeeping. */
    PAYLOAD_RO_HK,              /**< RO housekeeping. */
} ro_data_id_t;

/**
 * \brief RO command.
 */
typedef uint8_t payload_cmd_t;

/**
 * \brief RO device initialization.
 *
 * \param[in] ro is the RO device to initialize. It can be:
 * \parblock
 *      -\b PAYLOAD_RO_0
 * \endparblock
 *
 * \return The status/error code.
 */
int ro_payload_init(ro_type_t ro);

/**
 * \brief Enables a given payload payload.
 *
 * \param[in] ro is the RO device to enable. It can be:
 * \parblock
 *      -\b PAYLOAD_RO_0
 *      .
 * \endparblock
 *
 * \return The status/error code.
 */
int ro_payload_enable(ro_type_t ro);

/**
 * \brief Disables a given payload.
 *
 * \param[in] ro is the RO device to disable. It can be:
 * \parblock
 *      -\b PAYLOAD_RO_0
 * \endparblock
 *
 * \return Thet status/error code.
 */
int ro_payload_disable(ro_type_t ro);

/**
 * \brief Writes a command to a given RO payload.
 *
 * \param[in] ro is the RO device to write the command. It can be:
 * \parblock
 *      -\b PAYLOAD_RO_0
 *      .
 * \endparblock
 *
 * \param[in] cmd is the command to write to the given payload.
 *
 * \return Thet status/error code.
 */
int ro_payload_write_cmd(ro_type_t ro, ro_cmd_t cmd);

/**
 * \brief Gets data from a given RO payload.
 *
 * \param[in] ro is the RO device to get data from. It can be:
 * \parblock
 *      -\b PAYLOAD_RO_0
 *      .
 * \endparblock
 *
 * param[in] id is the data ID. It can be:
 * \parblock
 *      -\b PAYLOAD_RO_RAW_STATE
 *      -\b PAYLOAD_RO_STATE
 *      -\b PAYLOAD_RO_RAW_EVENT
 *      -\b PAYLOAD_RO_EVENT
 *      -\b PAYLOAD_RO_RAW_NAVIGATION
 *      -\b PAYLOAD_RO_NAVIGATION
 *      -\b PAYLOAD_RO_RAW_OBSERVATION
 *      -\b PAYLOAD_RO_OBSERVATION
  *      -\b PAYLOAD_RO_RAW_IQ_HEADER
  *      -\b PAYLOAD_RO_IQ_HEADER
  *      -\b PAYLOAD_RO_RAW_HK
  *      -\b PAYLOAD_RO_HK
 *      .
 * \endparblock
 *
 * \param[in,out] data is a pointer to the read data from the payload device.
 *
 * \param[in,out] len is the number of bytes returned by the payload device.
 *
 * \return The status/error code.
 */
int ro_get_data(ro_type_t ro, ro_data_id_t id, uint8_t *data, int32_t *len);


/**
 * \brief Sets the payload clock to a provided unix timestamp.
 *
 * \param[in] pl is the payload device to set the clock.
 *
 * \param[in] time is the unix timestamp to set in the payload.
 *
 * \return The status/error code.
 */
int ro_set_clock(const ro_type_t ro, const uint32_t time);

#endif /* PAYLOAD_H_ */

/** \} End of payload group */
