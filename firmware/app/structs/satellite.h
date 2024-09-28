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
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 0.10.19
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
 * \brief Packets IDs.
 */
typedef enum {
    PKT_ID_DOWNLINK_MESSAGE_BROADCAST =  0x01,
    PKT_ID_DOWNLINK_PING_ANS          =  0x02,
    PKT_ID_DOWNLINK_GENERAL_TELEMETRY =  0x10,
    PKT_ID_DOWNLINK_DATA_REQUEST_ANS  =  0x11,
    PKT_ID_DOWNLINK_PAYLOAD_DATA      =  0x12,
    PKT_ID_DOWNLINK_TC_FEEDBACK       =  0x13,
    PKT_ID_DOWNLINK_PARAM_VALUE       =  0x14,
    PKT_ID_DOWNLINK_PACKET_BROADCAST  =  0x15,
    PKT_ID_UPLINK_PING_REQ            =  0x40,
    PKT_ID_UPLINK_DATA_REQ            =  0x41,
    PKT_ID_UPLINK_BROADCAST_MSG       =  0x42,
    PKT_ID_UPLINK_ENTER_HIBERNATION   =  0x43,
    PKT_ID_UPLINK_LEAVE_HIBERNATION   =  0x44,
    PKT_ID_UPLINK_ACTIVATE_MODULE     =  0x45,
    PKT_ID_UPLINK_DEACTIVATE_MODULE   =  0x46,
    PKT_ID_UPLINK_ACTIVATE_PAYLOAD    =  0x47,
    PKT_ID_UPLINK_DEACTIVATE_PAYLOAD  =  0x48,
    PKT_ID_UPLINK_ERASE_MEMORY        =  0x49,
    PKT_ID_UPLINK_FORCE_RESET         =  0x4A,
    PKT_ID_UPLINK_GET_PAYLOAD_DATA    =  0x4B,
    PKT_ID_UPLINK_SET_PARAM           =  0x4C,
    PKT_ID_UPLINK_GET_PARAM           =  0x4D,
    PKT_ID_UPLINK_TRANSMIT_PACKET     =  0x4E,
    PKT_ID_UPLINK_UPDATE_TLE          =  0x4F,
} packet_id_e;

/**
 * \brief Subsystems IDs.
 */
typedef enum {
    SUBSYSTEM_ID_OBDH                 = 0,
    SUBSYSTEM_ID_TTC_1                = 1,
    SUBSYSTEM_ID_TTC_2                = 2,
    SUBSYSTEM_ID_EPS                  = 3,
} subsystem_id_e;

/**
 * \brief Payloads IDs.
 */
typedef enum {
    PL_ID_EDC_1                        = 1,
    PL_ID_EDC_2                        = 2,
    PL_ID_PAYLOAD_X                    = 3,
    PL_ID_RADIATION_MONITOR            = 4,
} payload_id_e;

/**
 * \brief Modules IDs.
 */
typedef enum {
    MODULE_ID_BATTERY_HEATER           = 1,
    MODULE_ID_BEACON                   = 2,
    MODULE_ID_PERIODIC_TELEMETRY       = 3,
} module_id_e;

/**
 * \brief Data IDs.
 */
typedef enum {
    DATA_ID_OBDH                       = 0,
    DATA_ID_EPS                        = 1,
    DATA_ID_TTC_0                      = 2,
    DATA_ID_TTC_1                      = 3,
    DATA_ID_ANT                        = 4,
    DATA_ID_SBCD_PKTS                  = 5,
    DATA_ID_PAYLOAD_INFO               = 6,
} data_id_e;

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
    uint8_t id;                     /**< Payload id [Ex: CONFIG_PL_ID_EDC_1] */
    uint8_t data[220];              /**< Payload data. */
} payload_telemetry_t;

/**
 * \brief Payloads state type.
 */
typedef struct
{
    payload_telemetry_t *c_edc;     /**< Pointer to the active EDC telemetry data. */
    payload_id_e active_payload[2]; /**< Active payloads. active_payload[0] = Payload EDC; active_payload[1] = Payload X,  */
    bool edc_active;                /**< EDC Active flag */
} payload_state_t;

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
    payload_state_t state;          /**< Payload state. */
} sat_data_t;

/**
 * \brief Satellite data buffer.
 */
extern sat_data_t sat_data_buf;

#endif /* SATELLITE_H_ */

/** \} End of sat_data group */
