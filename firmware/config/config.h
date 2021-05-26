/*
 * config.h
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
 * \brief Configuration parameters definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.0
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
#define CONFIG_TASK_RADIO_RESET_ENABLED                 1
#define CONFIG_TASK_READ_SENSORS_ENABLED                1
#define CONFIG_TASK_BEACON_ENABLED                      0
#define CONFIG_TASK_UPLINK_ENABLED                      0
#define CONFIG_TASK_TIME_CONTROL_ENABLED                1
#define CONFIG_TASK_CSP_SERVER_ENABLED                  0
#define CONFIG_TASK_READ_EDC_ENABLED                    1
#define CONFIG_TASK_READ_EPS_ENABLED                    1
#define CONFIG_TASK_READ_TTC_ENABLED                    1
#define CONFIG_TASK_DATA_LOG_ENABLED                    1

/* Devices */
#define CONFIG_DEV_MEDIA_INT_ENABLED                    1
#define CONFIG_DEV_MEDIA_NOR_ENABLED                    1
#define CONFIG_DEV_LEDS_ENABLED                         1
#define CONFIG_DEV_CURRENT_SENSOR_ENABLED               1
#define CONFIG_DEV_VOLTAGE_SENSOR_ENABLED               1
#define CONFIG_DEV_TEMP_SENSOR_ENABLED                  1
#define CONFIG_DEV_EPS_ENABLED                          1
#define CONFIG_DEV_RADIO_ENABLED                        0
#define CONFIG_DEV_PAYLOAD_EDC_ENABLED                  1
#define CONFIG_DEV_ANTENNA_ENABLED                      1

/* Debug and log messages */
#define CONFIG_DRIVERS_DEBUG_ENABLED                    0

#define CONFIG_SATELLITE_CALLSIGN                       "PY0EFS"

/* Packets IDs */
#define CONFIG_PKT_ID_BEACON                            0x10

/* CSP */
#define CONFIG_CSP_ENABLED                              0
#define CONFIG_CSP_MY_ADDRESS                           0
#define CONFIG_CSP_TTC_ADDRESS                          1
#define CONFIG_CSP_EPS_ADDRESS                          2
#define CONFIG_CSP_BUFFER_MAX_PKTS                      5
#define CONFIG_CSP_BUFFER_MAX_SIZE                      300
#define CONFIG_CSP_ROUTER_WORD_STACK                    300
#define CONFIG_CSP_ROUTER_TASK_PRIORITY                 1
#define CONFIG_CSP_MAX_CONN                             10
#define CONFIG_CSP_WAIT_CONN_TIMEOUT_MS                 (10*1000)
#define CONFIG_CSP_READ_PKT_TIMEOUT_MS                  1000
#define CONFIG_CSP_PORT                                 10

/* Ports */
#define CONFIG_SPI_PORT_0_SPEED_BPS                     1000000UL

/* Radio */
#define SI446X_XO_TUNE_REG_VALUE                        97

/* Drivers */
#define ISIS_ANTENNA_ENABLED                            1

/* Memory addresses */
#define CONFIG_MEM_ADR_SYS_TIME                         0

#endif /* CONFIG_H_ */

/** \} End of config group */
