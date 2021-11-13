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
 * \version 0.8.33
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
