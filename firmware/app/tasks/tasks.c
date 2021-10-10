/*
 * tasks.c
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
 * along with OBDH 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Tasks implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.47
 * 
 * \date 2019/11/02
 * 
 * \addtogroup tasks
 * \{
 */

#include <FreeRTOS.h>
#include <task.h>

#include <config/config.h>

#include "tasks.h"
#include "startup.h"
#include "watchdog_reset.h"
#include "heartbeat.h"
#include "system_reset.h"
#include "radio_reset.h"
#include "read_sensors.h"
#include "beacon.h"
#include "uplink.h"
#include "time_control.h"
#include "csp_server.h"
#include "read_edc.h"
#include "read_eps.h"
#include "read_ttc.h"
#include "data_log.h"
#include "process_tc.h"

void create_tasks(void)
{
    /* Startup task */
#if CONFIG_TASK_STARTUP_ENABLED == 1
    xTaskCreate(vTaskStartup, TASK_STARTUP_NAME, TASK_STARTUP_STACK_SIZE, NULL, TASK_STARTUP_PRIORITY, &xTaskStartupHandle);

    if (xTaskStartupHandle == NULL)
    {
        /* Error creating the startup task */
    }
#endif /* CONFIG_TASK_STARTUP_ENABLED */

    /* Watchdog reset task */
#if CONFIG_TASK_WATCHDOG_RESET_ENABLED == 1
    xTaskCreate(vTaskWatchdogReset, TASK_WATCHDOG_RESET_NAME, TASK_WATCHDOG_RESET_STACK_SIZE, NULL, TASK_WATCHDOG_RESET_PRIORITY, &xTaskWatchdogResetHandle);

    if (xTaskWatchdogResetHandle == NULL)
    {
        /* Error creating the watchdog reset task */
    }
#endif /* CONFIG_TASK_WATCHDOG_RESET_ENABLED */

    /* Heartbeat task */
#if CONFIG_TASK_HEARTBEAT_ENABLED == 1
    xTaskCreate(vTaskHeartbeat, TASK_HEARTBEAT_NAME, TASK_HEARTBEAT_STACK_SIZE, NULL, TASK_HEARTBEAT_PRIORITY, &xTaskHeartbeatHandle);

    if (xTaskHeartbeatHandle == NULL)
    {
        /* Error creating the heartbeat task */
    }
#endif /* CONFIG_TASK_HEARTBEAT_ENABLED */

#if CONFIG_TASK_SYSTEM_RESET_ENABLED == 1
    xTaskCreate(vTaskSystemReset, TASK_SYSTEM_RESET_NAME, TASK_SYSTEM_RESET_STACK_SIZE, NULL, TASK_SYSTEM_RESET_PRIORITY, &xTaskSystemResetHandle);

    if (xTaskSystemResetHandle == NULL)
    {
        /* Error creating the system reset task */
    }
#endif /* CONFIG_TASK_SYSTEM_RESET_ENABLED */

#if CONFIG_TASK_RADIO_RESET_ENABLED == 1
    xTaskCreate(vTaskRadioReset, TASK_RADIO_RESET_NAME, TASK_RADIO_RESET_STACK_SIZE, NULL, TASK_RADIO_RESET_PRIORITY, &xTaskRadioResetHandle);

    if (xTaskRadioResetHandle == NULL)
    {
        /* Error creating the radio reset task */
    }
#endif /* CONFIG_TASK_RADIO_RESET_ENABLED */

#if CONFIG_TASK_READ_SENSORS_ENABLED == 1
    xTaskCreate(vTaskReadSensors, TASK_READ_SENSORS_NAME, TASK_READ_SENSORS_STACK_SIZE, NULL, TASK_READ_SENSORS_PRIORITY, &xTaskReadSensorsHandle);

    if (xTaskReadSensorsHandle == NULL)
    {
        /* Error creating the read sensors task */
    }
#endif /* CONFIG_TASK_READ_TEMP_ENABLED */

#if CONFIG_TASK_BEACON_ENABLED == 1
    xTaskCreate(vTaskBeacon, TASK_BEACON_NAME, TASK_BEACON_STACK_SIZE, NULL, TASK_BEACON_PRIORITY, &xTaskBeaconHandle);

    if (xTaskBeaconHandle == NULL)
    {
        /* Error creating the beacon task */
    }
#endif /* CONFIG_TASK_BEACON_ENABLED */

#if CONFIG_TASK_UPLINK_ENABLED == 1
    xTaskCreate(vTaskUplink, TASK_UPLINK_NAME, TASK_UPLINK_STACK_SIZE, NULL, TASK_UPLINK_PRIORITY, &xTaskUplinkHandle);

    if (xTaskUplinkHandle == NULL)
    {
        /* Error creating the uplink task */
    }
#endif /* CONFIG_TASK_UPLINK_ENABLED */

#if CONFIG_TASK_TIME_CONTROL_ENABLED == 1
    xTaskCreate(vTaskTimeControl, TASK_TIME_CONTROL_NAME, TASK_TIME_CONTROL_STACK_SIZE, NULL, TASK_TIME_CONTROL_PRIORITY, &xTaskTimeControlHandle);

    if (xTaskTimeControlHandle == NULL)
    {
        /* Error creating the time control task */
    }
#endif /* CONFIG_TASK_BEACON_ENABLED */

#if CONFIG_TASK_CSP_SERVER_ENABLED == 1
    xTaskCreate(vTaskCSPServer, TASK_CSP_SERVER_NAME, TASK_CSP_SERVER_STACK_SIZE, NULL, TASK_CSP_SERVER_PRIORITY, &xTaskCSPServerHandle);

    if (xTaskCSPServerHandle == NULL)
    {
        /* Error creating the CSP server task */
    }
#endif /* CONFIG_TASK_CSP_SERVER_ENABLED */

#if CONFIG_TASK_READ_EDC_ENABLED == 1
    xTaskCreate(vTaskReadEDC, TASK_READ_EDC_NAME, TASK_READ_EDC_STACK_SIZE, NULL, TASK_READ_EDC_PRIORITY, &xTaskReadEDCHandle);

    if (xTaskReadEDCHandle == NULL)
    {
        /* Error creating the read EDC task */
    }
#endif /* CONFIG_TASK_READ_EDC_ENABLED */

#if CONFIG_TASK_READ_EPS_ENABLED == 1
    xTaskCreate(vTaskReadEPS, TASK_READ_EPS_NAME, TASK_READ_EPS_STACK_SIZE, NULL, TASK_READ_EPS_PRIORITY, &xTaskReadEPSHandle);

    if (xTaskReadEPSHandle == NULL)
    {
        /* Error creating the read EPS task */
    }
#endif /* CONFIG_TASK_READ_EPS_ENABLED */

#if CONFIG_TASK_READ_TTC_ENABLED == 1
    xTaskCreate(vTaskReadTTC, TASK_READ_TTC_NAME, TASK_READ_TTC_STACK_SIZE, NULL, TASK_READ_TTC_PRIORITY, &xTaskReadTTCHandle);

    if (xTaskReadTTCHandle == NULL)
    {
        /* Error creating the read TTC task */
    }
#endif /* CONFIG_TASK_READ_TTC_ENABLED */

#if CONFIG_TASK_DATA_LOG_ENABLED == 1
    xTaskCreate(vTaskDataLog, TASK_DATA_LOG_NAME, TASK_DATA_LOG_STACK_SIZE, NULL, TASK_DATA_LOG_PRIORITY, &xTaskDataLogHandle);

    if (xTaskDataLogHandle == NULL)
    {
        /* Error creating the data log task */
    }
#endif /* CONFIG_TASK_DATA_LOG_ENABLED */

#if CONFIG_TASK_PROCESS_TC_ENABLED == 1
    xTaskCreate(vTaskProcessTC, TASK_PROCESS_TC_NAME, TASK_PROCESS_TC_STACK_SIZE, NULL, TASK_PROCESS_TC_PRIORITY, &xTaskProcessTCHandle);

    if (xTaskProcessTCHandle == NULL)
    {
        /* Error creating the process TC task */
    }
#endif /* CONFIG_TASK_PROCESS_TC_ENABLED */

    create_event_groups();
}

void create_event_groups(void)
{
    task_startup_status = xEventGroupCreate();

    if (task_startup_status == NULL)
    {
        /* Error creating the startup status event group */
    }
}

/** \} End of tasks group */
