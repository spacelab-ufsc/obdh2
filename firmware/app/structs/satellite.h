/*
 * satellite.h
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
 * along with OBDH 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Satellite data structure definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.10.9
 * 
 * \date 2020/07/16
 * 
 * \defgroup sat_data sat Data
 * \ingroup structs
 * \{
 */

#ifndef SATELLITE_H_
#define SATELLITE_H_

#include <stdint.h>
#include <stdbool.h>

#include <system/system.h>

#include <devices/eps/eps_data.h>
#include <devices/ttc/ttc_data.h>
#include <devices/antenna/antenna_data.h>

#include "obdh_data.h"

/**
 * \brief OBDH telemetry type.
 */
typedef struct
{
    sys_time_t timestamp;           /**< Timestamp of the OBDH data. */
    obdh_data_t data;               /**< OBDH data. */
} obdh_telemetry_t;

/**
 * \brief EPS telemetry type.
 */
typedef struct
{
    sys_time_t timestamp;           /**< Timestamp of the EPS data. */
    eps_data_t data;                /**< EPS data. */
} eps_telemetry_t;

/**
 * \brief TTC telemetry type.
 */
typedef struct
{
    sys_time_t timestamp;           /**< Timestamp of the TTC data. */
    ttc_data_t data;                /**< TTC data. */
} ttc_telemetry_t;

/**
 * \brief Antenna telemetry type.
 */
typedef struct
{
    sys_time_t timestamp;           /**< Timestamp of the Antenna data. */
    antenna_data_t data;            /**< Antenna data. */
} antenna_telemetry_t;

/**
 * \brief Payload telemetry type.
 */
typedef struct
{
    sys_time_t timestamp;           /**< Timestamp of the Payload data. */
    bool enabled;                   /**< Payload enable flag. */
    uint8_t data[220];              /**< Payload data. */
} payload_telemetry_t;

/**
 * \brief Satellite data.
 */
typedef struct
{
    obdh_telemetry_t obdh;          /**< OBDH telemetry. */
    eps_telemetry_t eps;            /**< EPS telemetry. */
    ttc_telemetry_t ttc_0;          /**< TTC 0 telemetry. */
    ttc_telemetry_t ttc_1;          /**< TTC 1 telemetry. */
    antenna_telemetry_t antenna;    /**< Antenna telemetry. */
    payload_telemetry_t edc_0;      /**< EDC 0 telemetry. */
    payload_telemetry_t edc_1;      /**< EDC 1 telemetry. */
    payload_telemetry_t payload_x;  /**< Payload-X telemetry. */
} sat_data_t;

/**
 * \brief Satellite data buffer.
 */
extern sat_data_t sat_data_buf;

#endif /* SATELLITE_H_ */

/** \} End of sat_data group */
