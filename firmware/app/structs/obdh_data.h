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
 * along with OBDH 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief OBDH data structure definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 0.10.19
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

#include <config/config.h>

#include <system/system.h>
#include <devices/media/media.h>

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
#define OBDH_PARAM_ID_INITIAL_HIB_EXECUTED      16  /**< Initial hibernation executed flag. */
#define OBDH_PARAM_ID_INITIAL_HIB_TIME_COUNTER  17  /**< Initial hibernation time counter in minutes. */
#define OBDH_PARAM_ID_ANT_DEPLOYMENT_EXECUTED   18  /**< Antenna deployment executed flag. */
#define OBDH_PARAM_ID_ANT_DEPLOYMENT_COUNTER    19  /**< Antenna deployment counter. */
#define OBDH_PARAM_ID_TLE_LINE1                 20  /**< TLE line 1. */
#define OBDH_PARAM_ID_TLE_LINE2                 21  /**< TLE line 2. */
#define OBDH_PARAM_ID_LATITUDE                  22  /**< Latitude in degrees. */
#define OBDH_PARAM_ID_LONGITUDE                 23  /**< Longitude in degrees. */
#define OBDH_PARAM_ID_ALTITUDE                  24  /**< Altitude in kilometers. */
#define OBDH_PARAM_ID_LAST_PAGE_OBDH_DATA       25  /**< Last used memory page of the OBDH data. */
#define OBDH_PARAM_ID_LAST_PAGE_EPS_DATA        26  /**< Last used memory page of the EPS data. */
#define OBDH_PARAM_ID_LAST_PAGE_TTC_0_DATA      27  /**< Last used memory page of the TTC 0 data. */
#define OBDH_PARAM_ID_LAST_PAGE_TTC_1_DATA      28  /**< Last used memory page of the TTC 1 data. */
#define OBDH_PARAM_ID_LAST_PAGE_ANT_DATA        29  /**< Last used memory page of the Antenna data. */
#define OBDH_PARAM_ID_LAST_PAGE_EDC_DATA        30  /**< Last used memory page of the EDC data. */
#define OBDH_PARAM_ID_LAST_PAGE_PX_DATA         31  /**< Last used memory page of the Payload-X data. */
#define OBDH_PARAM_ID_LAST_PAGE_SBCD_PKTS       32  /**< Last used memory page of the SBCD packets. */
#define OBDH_PARAM_ID_MANUAL_MODE_ON            33  /**< Manual operation mode selection flag. */
#define OBDH_PARAM_ID_MAIN_EDC                  34  /**< Main EDC ID. */

/* Default values */
#define OBDH_TIMESTAMP_DEFAULT_VAL                      0U
#define OBDH_PARAM_TEMPERATURE_UC_DEFAULT_VAL           0U
#define OBDH_PARAM_CURRENT_DEFAULT_VAL                  0U
#define OBDH_PARAM_VOLTAGE_DEFAULT_VAL                  0U
#define OBDH_PARAM_LAST_RESET_CAUSE_DEFAULT_VAL         0U
#define OBDH_PARAM_RESET_COUNTER_DEFAULT_VAL            0U
#define OBDH_PARAM_LAST_VALID_TC_DEFAULT_VAL            0U
#define OBDH_PARAM_HW_VERSION_DEFAULT_VAL               0U
#define OBDH_PARAM_FW_VERSION_DEFAULT_VAL               0U
#define OBDH_PARAM_MODE_DEFAULT_VAL                     0U
#define OBDH_PARAM_TS_LAST_MODE_CHANGE_DEFAULT_VAL      0U
#define OBDH_PARAM_MODE_DURATION_DEFAULT_VAL            0U
#define OBDH_PARAM_INITIAL_HIB_EXECUTED_DEFAULT_VAL     0U
#define OBDH_PARAM_INITIAL_HIB_TIME_COUNT_DEFAULT_VAL   0U
#define OBDH_PARAM_ANT_DEPLOYMENT_EXECUTED_DEFAULT_VAL  0U
#define OBDH_PARAM_ANT_DEPLOYMENT_COUNTER_DEFAULT_VAL   0U
#define OBDH_PARAM_MANUAL_MODE_ON_DEFAULT_VAL           0U
#define OBDH_PARAM_MAIN_EDC_DEFAULT_VAL                 0U
#define OBDH_PARAM_POSITION_TIMESTAMP_DEFAULT_VAL       0U
#define OBDH_PARAM_POSITION_TLE_LINE1_DEFAULT_VAL       "1 44885U 19093G   24055.47450690  .00005228  00000-0  56684-3 0  9991"
#define OBDH_PARAM_POSITION_TLE_LINE2_DEFAULT_VAL       "2 44885  97.8473 139.0522 0012434 335.2519  24.8100 14.88559378226444"
#define OBDH_PARAM_POSITION_LATITUDE_DEFAULT_VAL        0U
#define OBDH_PARAM_POSITION_LONGITUDE_DEFAULT_VAL       0U
#define OBDH_PARAM_POSITION_ALTITUDE_DEFAULT_VAL        0U
#define OBDH_PARAM_MEDIA_LAST_OBDH_DATA_DEFAULT_VAL     CONFIG_MEM_OBDH_DATA_START_PAGE
#define OBDH_PARAM_MEDIA_LAST_EPS_DATA_DEFAULT_VAL      CONFIG_MEM_EPS_DATA_START_PAGE
#define OBDH_PARAM_MEDIA_LAST_TTC_0_DATA_DEFAULT_VAL    CONFIG_MEM_TTC_0_DATA_START_PAGE
#define OBDH_PARAM_MEDIA_LAST_TTC_1_DATA_DEFAULT_VAL    CONFIG_MEM_TTC_1_DATA_START_PAGE
#define OBDH_PARAM_MEDIA_LAST_ANT_DATA_DEFAULT_VAL      CONFIG_MEM_ANT_DATA_START_PAGE
#define OBDH_PARAM_MEDIA_LAST_EDC_DATA_DEFAULT_VAL      CONFIG_MEM_EDC_DATA_START_PAGE
#define OBDH_PARAM_MEDIA_LAST_PX_DATA_DEFAULT_VAL       CONFIG_MEM_PX_DATA_START_PAGE
#define OBDH_PARAM_MEDIA_LAST_SBCD_PKTS_DEFAULT_VAL     CONFIG_MEM_SBCD_PKTS_START_PAGE

/* Operation modes */
#define OBDH_MODE_NORMAL            0
#define OBDH_MODE_HIBERNATION       1
#define OBDH_MODE_STAND_BY          2

/**
 * \brief Position data.
 */
typedef struct
{
    sys_time_t timestamp;           /**< Timestamp of the position data. */
    uint8_t tle_line1[70];          /**< TLE line 1 in ASCII. */
    uint8_t tle_line2[70];          /**< TLE line 2 in ASCII. */
    int16_t latitude;               /**< Latitude in degrees. */
    int16_t longitude;              /**< Lontitude in degrees. */
    int16_t altitude;               /**< Altitude in kilometers. */
} position_data_t;

/**
 * \brief Media data.
 */
typedef struct
{
    uint32_t last_page_obdh_data;   /**< Last used memory page of the OBDH data. */
    uint32_t last_page_eps_data;    /**< Last used memory page of the EPS data. */
    uint32_t last_page_ttc_0_data;  /**< Last used memory page of the TTC 0 data. */
    uint32_t last_page_ttc_1_data;  /**< Last used memory page of the TTC 1 data. */
    uint32_t last_page_ant_data;    /**< Last used memory page of the Antenna data. */
    uint32_t last_page_edc_data;    /**< Last used memory page of the EDC data. */
    uint32_t last_page_px_data;     /**< Last used memory page of the Payload-X data. */
    uint32_t last_page_sbcd_pkts;   /**< Last used memory page of the SBCD packets. */
} media_data_t;

/**
 * \brief OBDH data.
 */
typedef struct
{
    uint16_t temperature;           /**< Temperature of the uC in Kelvin. */
    uint16_t current;               /**< Input current in mA. */
    uint16_t voltage;               /**< Input voltage in mV. */
    uint16_t reset_counter;         /**< Reset counter. */
    uint8_t last_reset_cause;       /**< Last reset cause code. */
    uint8_t last_valid_tc;          /**< Last valid telecommand ID. */
    uint8_t mode;                   /**< Satellite mode. */
    uint8_t ant_deployment_counter; /**< Antenna deployment counter. */
    uint8_t initial_hib_time_count; /**< Initial hibernation time counter in minutes. */
    uint8_t hw_version;             /**< Hardware version. */
    uint32_t fw_version;            /**< Firmware version (ex.: "v1.2.3" = 0x00010203). */
    sys_time_t ts_last_mode_change; /**< Timestamp of the last change in the operation mode. */
    sys_time_t mode_duration;       /**< Mode duration (valid only in hibernation mode). */
    bool initial_hib_executed;      /**< Initial hibernation executed flag. */
    bool ant_deployment_executed;   /**< Antenna deployment executed flag. */
    bool manual_mode_on;		    /**< Manual operation mode selection flag. */
    uint8_t main_edc;               /**< Main EDC. */
    media_data_t media;             /**< Memories data. */
    position_data_t position;       /**< Current position of the satellite. */
} obdh_data_t;

#endif /* OBDH_DATA_H_ */

/** \} End of obdh_data group */
