/*
 * tasks.c
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
 * \brief Tasks implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 0.10.19
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
#include "read_sensors.h"
#include "beacon.h"
#include "time_control.h"
#include "read_edc.h"
#include "read_eps.h"
#include "read_ttc.h"
#include "read_antenna.h"
#include "data_log.h"
#include "process_tc.h"
#include "pos_det.h"
#include "read_px.h"
#include "housekeeping.h"
#include "mem_check.h"
#include "op_ctrl.h"
#include "mode_check.h"

void create_tasks(void)
{
    /* Startup task */
#if defined(CONFIG_TASK_STARTUP_ENABLED) && (CONFIG_TASK_STARTUP_ENABLED == 1)
    xTaskCreate(vTaskStartup, TASK_STARTUP_NAME, TASK_STARTUP_STACK_SIZE, NULL, TASK_STARTUP_PRIORITY, &xTaskStartupHandle);

    if (xTaskStartupHandle == NULL)
    {
        /* Error creating the startup task */
    }
#endif /* CONFIG_TASK_STARTUP_ENABLED */

    /* Watchdog reset task */
#if defined(CONFIG_TASK_WATCHDOG_RESET_ENABLED) && (CONFIG_TASK_WATCHDOG_RESET_ENABLED == 1)
    xTaskCreate(vTaskWatchdogReset, TASK_WATCHDOG_RESET_NAME, TASK_WATCHDOG_RESET_STACK_SIZE, NULL, TASK_WATCHDOG_RESET_PRIORITY, &xTaskWatchdogResetHandle);

    if (xTaskWatchdogResetHandle == NULL)
    {
        /* Error creating the watchdog reset task */
    }
#endif /* CONFIG_TASK_WATCHDOG_RESET_ENABLED */

    /* Heartbeat task */
#if defined(CONFIG_TASK_HEARTBEAT_ENABLED) && (CONFIG_TASK_HEARTBEAT_ENABLED == 1)
    xTaskCreate(vTaskHeartbeat, TASK_HEARTBEAT_NAME, TASK_HEARTBEAT_STACK_SIZE, NULL, TASK_HEARTBEAT_PRIORITY, &xTaskHeartbeatHandle);

    if (xTaskHeartbeatHandle == NULL)
    {
        /* Error creating the heartbeat task */
    }
#endif /* CONFIG_TASK_HEARTBEAT_ENABLED */

#if defined(CONFIG_TASK_SYSTEM_RESET_ENABLED) && (CONFIG_TASK_SYSTEM_RESET_ENABLED == 1)
    xTaskCreate(vTaskSystemReset, TASK_SYSTEM_RESET_NAME, TASK_SYSTEM_RESET_STACK_SIZE, NULL, TASK_SYSTEM_RESET_PRIORITY, &xTaskSystemResetHandle);

    if (xTaskSystemResetHandle == NULL)
    {
        /* Error creating the system reset task */
    }
#endif /* CONFIG_TASK_SYSTEM_RESET_ENABLED */

#if defined(CONFIG_TASK_READ_SENSORS_ENABLED) && (CONFIG_TASK_READ_SENSORS_ENABLED == 1)
    xTaskCreate(vTaskReadSensors, TASK_READ_SENSORS_NAME, TASK_READ_SENSORS_STACK_SIZE, NULL, TASK_READ_SENSORS_PRIORITY, &xTaskReadSensorsHandle);

    if (xTaskReadSensorsHandle == NULL)
    {
        /* Error creating the read sensors task */
    }
#endif /* CONFIG_TASK_READ_TEMP_ENABLED */

#if defined(CONFIG_TASK_BEACON_ENABLED) && (CONFIG_TASK_BEACON_ENABLED == 1)
    xTaskCreate(vTaskBeacon, TASK_BEACON_NAME, TASK_BEACON_STACK_SIZE, NULL, TASK_BEACON_PRIORITY, &xTaskBeaconHandle);

    if (xTaskBeaconHandle == NULL)
    {
        /* Error creating the beacon task */
    }
#endif /* CONFIG_TASK_BEACON_ENABLED */

#if defined(CONFIG_TASK_TIME_CONTROL_ENABLED) && (CONFIG_TASK_TIME_CONTROL_ENABLED == 1)
    xTaskCreate(vTaskTimeControl, TASK_TIME_CONTROL_NAME, TASK_TIME_CONTROL_STACK_SIZE, NULL, TASK_TIME_CONTROL_PRIORITY, &xTaskTimeControlHandle);

    if (xTaskTimeControlHandle == NULL)
    {
        /* Error creating the time control task */
    }
#endif /* CONFIG_TASK_BEACON_ENABLED */

#if defined(CONFIG_TASK_READ_EDC_ENABLED) && (CONFIG_TASK_READ_EDC_ENABLED == 1)
    xTaskCreate(vTaskReadEDC, TASK_READ_EDC_NAME, TASK_READ_EDC_STACK_SIZE, NULL, TASK_READ_EDC_PRIORITY, &xTaskReadEDCHandle);

    if (xTaskReadEDCHandle == NULL)
    {
        /* Error creating the read EDC task */
    }
#endif /* CONFIG_TASK_READ_EDC_ENABLED */

#if defined(CONFIG_TASK_READ_EPS_ENABLED) && (CONFIG_TASK_READ_EPS_ENABLED == 1)
    xTaskCreate(vTaskReadEPS, TASK_READ_EPS_NAME, TASK_READ_EPS_STACK_SIZE, NULL, TASK_READ_EPS_PRIORITY, &xTaskReadEPSHandle);

    if (xTaskReadEPSHandle == NULL)
    {
        /* Error creating the read EPS task */
    }
#endif /* CONFIG_TASK_READ_EPS_ENABLED */

#if defined(CONFIG_TASK_READ_TTC_ENABLED) && (CONFIG_TASK_READ_TTC_ENABLED == 1)
    xTaskCreate(vTaskReadTTC, TASK_READ_TTC_NAME, TASK_READ_TTC_STACK_SIZE, NULL, TASK_READ_TTC_PRIORITY, &xTaskReadTTCHandle);

    if (xTaskReadTTCHandle == NULL)
    {
        /* Error creating the read TTC task */
    }
#endif /* CONFIG_TASK_READ_TTC_ENABLED */

#if defined(CONFIG_TASK_READ_ANTENNA_ENABLED) && (CONFIG_TASK_READ_ANTENNA_ENABLED == 1)
    xTaskCreate(vTaskReadAntenna, TASK_READ_ANTENNA_NAME, TASK_READ_ANTENNA_STACK_SIZE, NULL, TASK_READ_ANTENNA_PRIORITY, &xTaskReadAntennaHandle);

    if (xTaskReadAntennaHandle == NULL)
    {
        /* Error creating the read Antenna task */
    }
#endif /* CONFIG_TASK_READ_ANTENNA_ENABLED */

#if defined(CONFIG_TASK_DATA_LOG_ENABLED) && (CONFIG_TASK_DATA_LOG_ENABLED == 1)
    xTaskCreate(vTaskDataLog, TASK_DATA_LOG_NAME, TASK_DATA_LOG_STACK_SIZE, NULL, TASK_DATA_LOG_PRIORITY, &xTaskDataLogHandle);

    if (xTaskDataLogHandle == NULL)
    {
        /* Error creating the data log task */
    }
#endif /* CONFIG_TASK_DATA_LOG_ENABLED */

#if defined(CONFIG_TASK_PROCESS_TC_ENABLED) && (CONFIG_TASK_PROCESS_TC_ENABLED == 1)
    xTaskCreate(vTaskProcessTC, TASK_PROCESS_TC_NAME, TASK_PROCESS_TC_STACK_SIZE, NULL, TASK_PROCESS_TC_PRIORITY, &xTaskProcessTCHandle);

    if (xTaskProcessTCHandle == NULL)
    {
        /* Error creating the process TC task */
    }
#endif /* CONFIG_TASK_PROCESS_TC_ENABLED */

#if defined(CONFIG_TASK_ANTENNA_DEPLOYMENT_ENABLED) && (CONFIG_TASK_ANTENNA_DEPLOYMENT_ENABLED == 1)
    xTaskCreate(vTaskAntennaDeployment, TASK_ANTENNA_DEPLOYMENT_NAME, TASK_ANTENNA_DEPLOYMENT_STACK_SIZE, NULL, TASK_ANTENNA_DEPLOYMENT_PRIORITY, &xTaskAntennaDeploymentHandle);

    if (xTaskAntennaDeploymentHandle == NULL)
    {
        /* Error creating the antenna deployment task */
    }
#endif /* CONFIG_TASK_ANTENNA_DEPLOYMENT_ENABLED */

#if defined(CONFIG_TASK_POSITION_DETERMINATION_ENABLED) && (CONFIG_TASK_POSITION_DETERMINATION_ENABLED == 1)
    xTaskCreate(vTaskPosDet, TASK_POS_DET_NAME, TASK_POS_DET_STACK_SIZE, NULL, TASK_POS_DET_PRIORITY, &xTaskPosDetHandle);

    if (xTaskPosDetHandle == NULL)
    {
        /* Error creating the position determination task */
    }
#endif /* CONFIG_TASK_POSITION_DETERMINATION_ENABLED */

#if defined(CONFIG_TASK_PAYLOAD_X_ENABLED) && (CONFIG_TASK_PAYLOAD_X_ENABLED == 1)
    xTaskCreate(vTaskReadPX, TASK_READ_PX_NAME, TASK_READ_PX_STACK_SIZE, NULL, TASK_READ_PX_PRIORITY, &xTaskReadPXHandle);

    if (xTaskReadPXHandle == NULL)
    {
        /* Error creating the Read PX task */
    }
#endif /* CONFIG_TASK_PAYLOAD_X_ENABLED */

#if defined(CONFIG_TASK_HOUSEKEEPING_ENABLED) && (CONFIG_TASK_HOUSEKEEPING_ENABLED == 1)
    xTaskCreate(vTaskHousekeeping, TASK_HOUSEKEEPING_NAME, TASK_HOUSEKEEPING_STACK_SIZE, NULL, TASK_HOUSEKEEPING_PRIORITY, &xTaskHousekeepingHandle);

    if (xTaskHousekeepingHandle == NULL)
    {
        /* Error creating the Housekeeping task */
    }
#endif /* CONFIG_TASK_HOUSEKEEPING_ENABLED */

#if defined(CONFIG_TASK_HEALTH_CHECK_MEM_ENABLED) && (CONFIG_TASK_HEALTH_CHECK_MEM_ENABLED == 1)
    xTaskCreate(vTaskHealthCheckMem, TASK_HEALTH_CHECK_MEM_NAME, TASK_HEALTH_CHECK_MEM_STACK_SIZE, NULL, TASK_HEALTH_CHECK_MEM_PRIORITY, &xTaskHealthCheckMemHandle);

    if (xTaskHealthCheckMemHandle == NULL)
    {
        /* Error creating the Health Check Memory task */
    }
#endif /* CONFIG_TASK_HEALTH_CHECK_MEM_ENABLED */

#if defined(CONFIG_TASK_OP_CTRL_ENABLED) && (CONFIG_TASK_OP_CTRL_ENABLED == 1)
    xTaskCreate(vTaskOpCtrl, TASK_OP_CTRL_NAME, TASK_OP_CTRL_STACK_SIZE, NULL, TASK_OP_CTRL_PRIORITY, &xTaskOpCtrlHandle);

    if (xTaskOpCtrlHandle == NULL)
    {
        /* Error creating the Operation Control task */
    }
#endif /* CONFIG_TASK_OP_CTRL_ENABLED */

#if defined(CONFIG_TASK_HEALTH_CHECK_MODE_ENABLED) && (CONFIG_TASK_HEALTH_CHECK_MODE_ENABLED == 1)
    xTaskCreate(vTaskHealthCheckMode, TASK_HEALTH_CHECK_MODE_NAME, TASK_HEALTH_CHECK_MODE_STACK_SIZE, NULL, TASK_HEALTH_CHECK_MODE_PRIORITY, &xTaskHealthCheckModeHandle);

    if (xTaskHealthCheckModeHandle == NULL)
    {
        /* Error creating the Health Check Operation Mode task */
    }
#endif /* CONFIG_TASK_HEALTH_CHECK_MODE_ENABLED */

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
