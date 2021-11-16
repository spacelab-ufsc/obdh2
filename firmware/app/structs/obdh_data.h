/*
 * obdh_data.h
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
 * \brief OBDH data structure definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.8.36
 * 
 * \date 2020/07/16
 * 
 * \defgroup obdh_data OBDH Data
 * \ingroup structs
 * \{
 */

#ifndef OBDH_DATA_H_
#define OBDH_DATA_H_

#include <stdint.h>

/* OBDH parameter ID */
#define OBDH_PARAM_ID_TIME_COUNTER              0   /**< Time counter in milliseconds. */
#define OBDH_PARAM_ID_TEMPERATURE_UC            1   /**< Temperature of the uC in Kelvin. */
#define OBDH_PARAM_ID_INPUT_CURRENT             2   /**< Input current in mA. */
#define OBDH_PARAM_ID_INPUT_VOLTAGE             3   /**< Input voltage in mV. */
#define OBDH_PARAM_ID_LAST_RESET_CAUSE          4   /**< Last reset cause. */
#define OBDH_PARAM_ID_RESET_COUNTER             5   /**< Reset counter. */
#define OBDH_PARAM_ID_LAST_VALID_TC             6   /**< Last valid telecommand (uplink packet ID). */
#define OBDH_PARAM_ID_TEMPERATURE_RADIO         7   /**< Temperature of the radio in Kelvin. */
#define OBDH_PARAM_ID_RSSI_LAST_TC              8   /**< RSSI of the last valid telecommand. */
#define OBDH_PARAM_ID_TEMPERATURE_ANTENNA       9   /**< Temperature of the antenna in Kelvin. */
#define OBDH_PARAM_ID_ANTENNA_STATUS            10  /**< Antenna status bits. */
#define OBDH_PARAM_ID_HARDWARE_VERSION          11  /**< Hardware version. */
#define OBDH_PARAM_ID_FIRMWARE_VERSION          12  /**< Firmware version (ex.: "v1.2.3" = 0x00010203). */
#define OBDH_PARAM_ID_MODE                      13  /**< Mode ("Normal" = 0, "Hibernation" = 1). */
#define OBDH_PARAM_ID_TIMESTAMP_LAST_MODE       14  /**< Timestamp of the last mode change. */
#define OBDH_PARAM_ID_MODE_DURATION             15  /**< Mode duration in sec. (valid only in hibernation mode). */

/* Operation modes */
#define OBDH_MODE_NORMAL            0
#define OBDH_MODE_HIBERNATION       1

/**
 * \brief Radio data.
 */
typedef struct
{
    uint16_t temperature;           /**< Temperature in Kelvin. */
    uint16_t last_valid_tc_rssi;    /**< RSSI of the last valid telecommand. */
} radio_data_t;

/**
 * \brief OBDH data.
 */
typedef struct
{
    uint16_t temperature;           /**< Temperature of the uC in Kelvin. */
    uint16_t current;               /**< Input current in mA. */
    uint16_t voltage;               /**< Input voltage in mV. */
    uint8_t last_reset_cause;       /**< Last reset cause code. */
    uint16_t reset_counter;         /**< Reset counter. */
    uint8_t last_valid_tc;          /**< Last valid telecommand ID. */
    radio_data_t radio;             /**< Radio data. */
    uint8_t hw_version;             /**< Hardware version. */
    uint32_t fw_version;            /**< Firmware version (ex.: "v1.2.3" = 0x00010203). */
    uint8_t mode;                   /**< Satellite mode. */
    sys_time_t ts_last_mode_change; /**< Timestamp of the last change in the operation mode. */
    sys_time_t mode_duration;       /**< Mode duration (valid only in hibernation mode). */
} obdh_data_t;

#endif /* OBDH_DATA_H_ */

/** \} End of obdh_data group */
