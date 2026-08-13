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
 * \version 1.0.0
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
#define OBDH_PARAM_ID_SYSTEM_TIME               0U   /**< System time in seconds. */
#define OBDH_PARAM_ID_TEMPERATURE_UC            1U  /**< Temperature of the uC in Kelvin. */
#define OBDH_PARAM_ID_INPUT_CURRENT             2U  /**< Input current in mA. */
#define OBDH_PARAM_ID_INPUT_VOLTAGE             3U  /**< Input voltage in mV. */
#define OBDH_PARAM_ID_LAST_RESET_CAUSE          4U  /**< Last reset cause. */
#define OBDH_PARAM_ID_RESET_COUNTER             5U  /**< Reset counter. */
#define OBDH_PARAM_ID_LAST_VALID_TC             6U  /**< Last valid telecommand (uplink packet ID). */
#define OBDH_PARAM_ID_HARDWARE_VERSION          7U  /**< Hardware version. */
#define OBDH_PARAM_ID_FIRMWARE_VERSION          8U  /**< Firmware version (ex.: "v1.2.3" = 0x00010203). */
#define OBDH_PARAM_ID_MODE                      9U  /**< Mode ("Normal" = 0, "Hibernation" = 1). */
#define OBDH_PARAM_ID_TIMESTAMP_LAST_MODE       10U /**< Timestamp of the last mode change. */
#define OBDH_PARAM_ID_MODE_DURATION             11U /**< Mode duration in sec. (valid only in hibernation mode). */
#define OBDH_PARAM_ID_INITIAL_HIB_EXECUTED      12U /**< Initial hibernation executed flag. */
#define OBDH_PARAM_ID_INITIAL_HIB_TIME_COUNTER  13U /**< Initial hibernation time counter in minutes. */
#define OBDH_PARAM_ID_ANT_DEPLOYMENT_EXECUTED   14U /**< Antenna deployment executed flag. */
#define OBDH_PARAM_ID_ANT_DEPLOYMENT_COUNTER    15U /**< Antenna deployment counter. */
#define OBDH_PARAM_ID_LATITUDE                  16U /**< Latitude in degrees. */
#define OBDH_PARAM_ID_LONGITUDE                 17U /**< Longitude in degrees. */
#define OBDH_PARAM_ID_ALTITUDE                  18U /**< Altitude in kilometers. */
#define OBDH_PARAM_ID_LAST_PAGE_OBDH_DATA       19U /**< Last used memory page of the OBDH data. */
#define OBDH_PARAM_ID_LAST_PAGE_EPS_DATA        20U /**< Last used memory page of the EPS data. */
#define OBDH_PARAM_ID_LAST_PAGE_TTC_0_DATA      21U /**< Last used memory page of the TTC 0 data. */
#define OBDH_PARAM_ID_LAST_PAGE_TTC_1_DATA      22U /**< Last used memory page of the TTC 1 data. */
#define OBDH_PARAM_ID_LAST_PAGE_ANT_DATA        23U /**< Last used memory page of the Antenna data. */
#define OBDH_PARAM_ID_LAST_PAGE_EDC_DATA        24U /**< Last used memory page of the EDC data. */
#define OBDH_PARAM_ID_LAST_PAGE_PX_DATA         25U /**< Last used memory page of the Payload-X data. */
#define OBDH_PARAM_ID_LAST_PAGE_SBCD_PKTS       26U /**< Last used memory page of the SBCD packets. */
#define OBDH_PARAM_ID_HIBERNATION_ON            27U /**< Satellite in hibernation flag. */
#define OBDH_PARAM_ID_MAIN_EDC                  28U /**< Main EDC ID. */
#define OBDH_PARAM_ID_GENERAL_TELEMETRY_ON      29U /**< General telemetry on flag. */
#define OBDH_PARAM_ID_RESET_DEVICE              30U /**< Resets device (Only write param, writing "0x01" to it resets OBDH). */
#define OBDH_PARAM_ID_TS_LAST_TLE_UPDATE        31U /**< Timestamp of the last TLE update. */
#define OBDH_PARAM_ID_TS_READ_SENSORS           32U /**< Timestamp of the last sensor readings. */
#define OBDH_PARAM_ID_MAIN_PAYLOAD_STATE        33U /**< Main payload state. (PL_ID for active and 0 for inactive.) */
#define OBDH_PARAM_ID_SEC_PAYLOAD_STATE         34U /**< Secundary payload state. (PL_ID for active and 0 for inactive.) */
#define OBDH_PARAM_ID_HIB_DURATION              35U /**< Remaining hibernation time in seconds. */
#define OBDH_PARAM_ID_BIN_TLE                   36U /**< Compact binary TLE line. More about the format on libpredict */
#define OBDH_PARAM_ID_TS_POSITION               37U /**< Timestamp of the last position determination. */
#define OBDH_PARAM_ID_TS_LAST_CONTACT           38U /**< Timestamp of the last received TC by ground station. */
#define OBDH_PARAM_ID_TS_COMMISSION_TIMEOUT     39U /**< Timestamp when commission mode will timeout. */
#define OBDH_PARAM_ID_LAST_TRAN_EV_ID           40U /**< Last valid event id that caused an mode transition. */
#define OBDH_PARAM_ID_EPS_BEACON_ON             41U /**< EPS beacon on flag. */
#define OBDH_PARAM_ID_BATT_CRITICAL_LEVEL_MV    42U /**< Battery critical level threshold in millivolts. */
#define OBDH_PARAM_ID_MANUAL_EXPERIMENT_ON      43U /**< Flag to state if the experiments are manually enabled. */
#define OBDH_PARAM_ID_RESET_TC_QUEUE            44U /**< Resets the TC Queue (Only write param, writing "0x01" to it resets the queue) */
#define OBDH_PARAM_ID_TC_QUEUE_SIZE             45U /**< Current TC Queue size */
#define OBDH_PARAM_ID_TS_NEXT_SCHED_TC          46U /**< Next scheduled telecommand execution timestamp. */

/* Default values */
#define OBDH_TIMESTAMP_DEFAULT_VAL                      0U
#define OBDH_PARAM_TEMPERATURE_UC_DEFAULT_VAL           0U
#define OBDH_PARAM_CURRENT_DEFAULT_VAL                  0U
#define OBDH_PARAM_VOLTAGE_DEFAULT_VAL                  0U
#define OBDH_PARAM_LAST_RESET_CAUSE_DEFAULT_VAL         0U
#define OBDH_PARAM_RESET_COUNTER_DEFAULT_VAL            0U
#define OBDH_PARAM_LAST_VALID_TC_DEFAULT_VAL            0U
#define OBDH_PARAM_HW_VERSION_DEFAULT_VAL               0U
#define OBDH_PARAM_FW_VERSION_DEFAULT_VAL               0x00010000U
#define OBDH_PARAM_MODE_DEFAULT_VAL                     0U
#define OBDH_PARAM_TS_LAST_MODE_CHANGE_DEFAULT_VAL      0U
#define OBDH_PARAM_HIB_DURATION_DEFAULT_VAL             0U
#define OBDH_PARAM_INITIAL_HIB_EXECUTED_DEFAULT_VAL     0U
#define OBDH_PARAM_INITIAL_HIB_TIME_COUNT_DEFAULT_VAL   0U
#define OBDH_PARAM_ANT_DEPLOYMENT_EXECUTED_DEFAULT_VAL  0U
#define OBDH_PARAM_ANT_DEPLOYMENT_COUNTER_DEFAULT_VAL   0U
#define OBDH_PARAM_HIBERNATION_ON_DEFAULT_VAL           1U
#define OBDH_PARAM_MAIN_EDC_DEFAULT_VAL                 1U
#define OBDH_PARAM_MAIN_PAYLOAD_STATE_DEFAULT_VAL       0U
#define OBDH_PARAM_SEC_PAYLOAD_STATE_DEFAULT_VAL        0U
#define OBDH_PARAM_GENERAL_TELEMETRY_ON_DEFAUL_VAL      0U
#define OBDH_PARAM_MANUAL_EXPERIMENT_ON_DEFAUL_VAL      1U
#define OBDH_PARAM_POSITION_TIMESTAMP_DEFAULT_VAL       0U
#define OBDH_PARAM_POSITION_TS_LAST_TLE_UPT_DEFAULT_VAL 0U
#define OBDH_PARAM_POSITION_BIN_TLE_DEFAULT_VAL         {0x00, 0x19, 0x40, 0x50, 0x14, 0x90, 0xFC, 0x7C, 0x92, 0xCE, 0x00, 0x00, 0x1C, 0xA3, 0x43, 0x4E, 0x4F, 0xCC, 0x43, 0x19, 0xCA, 0x37, 0x3A, 0x87, 0x80, 0x92, 0x40, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x4C, 0x4F, 0x4B, 0xC6, 0xA7, 0xEF, 0x9E, 0x40, 0x2E, 0x9D, 0x69, 0xF3, 0xE4, 0x93, 0xBE} /* HORYU-4 TLE converted to libpredict compact format */
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
#define OBDH_PARAM_TS_READ_SENSORS_DEFAULT_VAL          0U
#define OBDH_PARAM_TS_LAST_CONTACT_TIME_DEFAULT_VAL     0U
#define OBDH_PARAM_TS_COMMISSION_TIMEOUT_DEFAULT_VAL    0U
#define OBDH_PARAM_LAST_TRAN_EV_ID_DEFAULT_VAL          0U
#define OBDH_PARAM_EPS_BEACON_ON_DEFAULT_VAL            0U
#define OBDH_PARAM_BATT_CRITICAL_LEVEL_MV_DEFAULT_VAL   6500U
#define OBDH_PARAM_TC_QUEUE_SIZE_DEFAULT_VAL            0U             
#define OBDH_PARAM_TS_NEXT_SCHED_TC_DEFAULT_VAL         0U

/**
 * \brief Position data.
 */
typedef struct
{
    sys_time_t timestamp;           /**< Timestamp of the position data. */
    sys_time_t ts_last_tle_update;  /**< Timestamp of the last TLE update. */
    int16_t latitude;               /**< Latitude in degrees. */
    int16_t altitude;               /**< Altitude in kilometers. */
    int16_t longitude;              /**< Lontitude in degrees. */
    uint8_t bin_tle[50];            /**< Binary compact TLE lines. */
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
    media_data_t media;                 /**< Memories data. */
    sys_time_t ts_last_mode_change;     /**< Timestamp of the last change in the operation mode. */
    sys_time_t ts_read_sensors;         /**< Timestamp of the last sensor readings. */
    sys_time_t ts_last_contact;         /**< Timestamp of the last TC reception. */
    sys_time_t ts_commission_timeout;   /**< Timestamp when the commission mode will timeout. */
    sys_time_t hib_duration;            /**< Hibernation duration in seconds. */
    sys_time_t ts_next_sched_tc;        /**< Next scheduled telecommand execution timestamp. */
    uint32_t fw_version;                /**< Firmware version (ex.: "v1.2.3" = 0x00010203). */
    uint16_t temperature;               /**< Temperature of the uC in Kelvin. */
    uint16_t current;                   /**< Input current in mA. */
    uint16_t voltage;                   /**< Input voltage in mV. */
    uint16_t reset_counter;             /**< Reset counter. */
    uint16_t last_tran_ev_id;           /**< Last valid event id that caused an mode transition */
    uint16_t batt_crit_level_mv;        /**< Battery critical level threshold in millivolts */
    uint8_t mode;                       /**< Satellite mode. */
    uint8_t last_reset_cause;           /**< Last reset cause code. */
    uint8_t last_valid_tc;              /**< Last valid telecommand ID. */
    uint8_t ant_deployment_counter;     /**< Antenna deployment counter. */
    uint8_t initial_hib_time_count;     /**< Initial hibernation time counter in minutes. */
    uint8_t hw_version;                 /**< Hardware version. */
    uint8_t main_edc;                   /**< Main EDC. */
    uint8_t main_payload_state;         /**< Main payload state. (PL_ID for active and 0 for inactive.) */
    uint8_t sec_payload_state;          /**< Secundary payload state. (PL_ID for active and 0 for inactive.) */
    uint8_t tc_queue_size;              /**< Current TC Queue size */
    bool initial_hib_executed;          /**< Initial hibernation executed flag. */
    bool ant_deployment_executed;       /**< Antenna deployment executed flag. */
    bool hibernation_on;		        /**< Satellite in hibernation flag. */
    bool general_telemetry_on;          /**< General telemetry on flag. */
    bool eps_beacon_on;                 /**< EPS beacon on. */
    bool manual_experiments;            /**< Flag to state if the experiments are manually enabled. */
    position_data_t position;           /**< Current position of the satellite. */
} obdh_data_t; 

/**
 * \brief Reads a OBDH parameter.
 *
 * \param[in] param_id is the parameter id to read.
 *
 * \param[out] data is a pointer to a variable to store the OBDH parameter.
 *
 * \return The status/error code.
 */
int8_t obdh_get_param(uint8_t param_id, uint32_t *buf);

/**
 * \brief Changes a OBDH parameter.
 *
 * \param[in] param_id is the parameter id to set.
 *
 * \param[in] data is a pointer to a variable that contains the new value for the OBDH parameter.
 *
 * \return The status/error code.
 */
int8_t obdh_set_param(uint8_t param_id, uint32_t *buf);


#endif /* OBDH_DATA_H_ */

/** \} End of obdh_data group */
