/*
 * config.h
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
 * \brief Configuration parameters definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.10.1
 * 
 * \date 2019/10/26
 * 
 * \defgroup config Configuration
 * \{
 */

#ifndef CONFIG_H_
#define CONFIG_H_

/* Tasks */
#define CONFIG_TASK_STARTUP_ENABLED                     1
#define CONFIG_TASK_WATCHDOG_RESET_ENABLED              1
#define CONFIG_TASK_HEARTBEAT_ENABLED                   1
#define CONFIG_TASK_SYSTEM_RESET_ENABLED                1
#define CONFIG_TASK_READ_SENSORS_ENABLED                1
#define CONFIG_TASK_BEACON_ENABLED                      0
#define CONFIG_TASK_TIME_CONTROL_ENABLED                1
#define CONFIG_TASK_READ_EDC_ENABLED                    1
#define CONFIG_TASK_READ_EPS_ENABLED                    1
#define CONFIG_TASK_READ_TTC_ENABLED                    1
#define CONFIG_TASK_READ_ANTENNA_ENABLED                1
#define CONFIG_TASK_DATA_LOG_ENABLED                    1
#define CONFIG_TASK_PROCESS_TC_ENABLED                  1
#define CONFIG_TASK_ANTENNA_DEPLOYMENT_ENABLED          0
#define CONFIG_TASK_PAYLOAD_X_ENABLED                   1

/* Devices */
#define CONFIG_DEV_MEDIA_INT_ENABLED                    1
#define CONFIG_DEV_MEDIA_FRAM_ENABLED                   1
#define CONFIG_DEV_MEDIA_NOR_ENABLED                    1
#define CONFIG_DEV_LEDS_ENABLED                         1
#define CONFIG_DEV_CURRENT_SENSOR_ENABLED               1
#define CONFIG_DEV_VOLTAGE_SENSOR_ENABLED               1
#define CONFIG_DEV_TEMP_SENSOR_ENABLED                  1
#define CONFIG_DEV_EPS_ENABLED                          1
#define CONFIG_DEV_PAYLOAD_EDC_ENABLED                  1
#define CONFIG_DEV_ANTENNA_ENABLED                      1
#define CONFIG_DEV_PAYLOAD_X_ENABLED                    1

/* Drivers */
#define CONFIG_DRV_ISIS_ANTENNA_ENABLED                 1

/* Debug and log messages */
#define CONFIG_DRIVERS_DEBUG_ENABLED                    0

#define CONFIG_SATELLITE_CALLSIGN                       "PY0EFS"

/* Packets IDs */
#define CONFIG_PKT_ID_DOWNLINK_GENERAL_TELEMETRY        0x20
#define CONFIG_PKT_ID_DOWNLINK_PING_ANS                 0x21
#define CONFIG_PKT_ID_DOWNLINK_DATA_REQUEST_ANS         0x22
#define CONFIG_PKT_ID_DOWNLINK_MESSAGE_BROADCAST        0x23
#define CONFIG_PKT_ID_DOWNLINK_PAYLOAD_DATA             0x24
#define CONFIG_PKT_ID_DOWNLINK_TC_FEEDBACK              0x25
#define CONFIG_PKT_ID_DOWNLINK_PARAM_VALUE              0x26
#define CONFIG_PKT_ID_UPLINK_PING_REQ                   0x40
#define CONFIG_PKT_ID_UPLINK_DATA_REQ                   0x41
#define CONFIG_PKT_ID_UPLINK_BROADCAST_MSG              0x42
#define CONFIG_PKT_ID_UPLINK_ENTER_HIBERNATION          0x43
#define CONFIG_PKT_ID_UPLINK_LEAVE_HIBERNATION          0x44
#define CONFIG_PKT_ID_UPLINK_ACTIVATE_MODULE            0x45
#define CONFIG_PKT_ID_UPLINK_DEACTIVATE_MODULE          0x46
#define CONFIG_PKT_ID_UPLINK_ACTIVATE_PAYLOAD           0x47
#define CONFIG_PKT_ID_UPLINK_DEACTIVATE_PAYLOAD         0x48
#define CONFIG_PKT_ID_UPLINK_ERASE_MEMORY               0x49
#define CONFIG_PKT_ID_UPLINK_FORCE_RESET                0x4A
#define CONFIG_PKT_ID_UPLINK_GET_PAYLOAD_DATA           0x4B
#define CONFIG_PKT_ID_UPLINK_SET_PARAM                  0x4C
#define CONFIG_PKT_ID_UPLINK_GET_PARAM                  0x4D

/* Subsystem IDs */
#define CONFIG_SUBSYSTEM_ID_OBDH                        0
#define CONFIG_SUBSYSTEM_ID_TTC_1                       1
#define CONFIG_SUBSYSTEM_ID_TTC_2                       2
#define CONFIG_SUBSYSTEM_ID_EPS                         3

/* Payloads IDs */
#define CONFIG_PL_ID_EDC_1                              1
#define CONFIG_PL_ID_EDC_2                              2
#define CONFIG_PL_ID_PAYLOAD_X                          3
#define CONFIG_PL_ID_RADIATION_MONITOR                  4

/* Modules IDs */
#define CONFIG_MODULE_ID_BATTERY_HEATER                 1
#define CONFIG_MODULE_ID_BEACON                         2
#define CONFIG_MODULE_ID_PERIODIC_TELEMETRY             3

/* Data IDs */
#define CONFIG_DATA_ID_OBDH                             0
#define CONFIG_DATA_ID_EPS                              1
#define CONFIG_DATA_ID_TTC_0                            2
#define CONFIG_DATA_ID_TTC_1                            3

/* Ports */
#define CONFIG_SPI_PORT_0_SPEED_BPS                     100000UL

/* Antenna */
#define CONFIG_ANTENNA_INDEP_DEPLOY_BURN_TIME_SEC       10U
#define CONFIG_ANTENNA_SEQ_DEPLOY_BURN_TIME_SEC         20U
#define CONFIG_ANTENNA_DEPLOYMENT_ATTEMPTS              10U
#define CONFIG_ANTENNA_DEPLOYMENT_HIBERNATION_MIN       45

/* Memory addresses */
#define CONFIG_MEM_ADR_SYS_TIME                         0

#endif /* CONFIG_H_ */

/** \} End of config group */
