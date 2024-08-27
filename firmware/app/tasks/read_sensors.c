/*
 * read_sensors.h
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
 * \brief Read onboard sensors task implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.10.17
 * 
 * \date 2020/07/12
 * 
 * \addtogroup read_sensors
 * \{
 */

#include <devices/current_sensor/current_sensor.h>
#include <devices/voltage_sensor/voltage_sensor.h>
#include <devices/temp_sensor/temp_sensor.h>

#include <structs/satellite.h>
#include <system/sys_log/sys_log.h>

#include "read_sensors.h"
#include "startup.h"

xTaskHandle xTaskReadSensorsHandle;

void vTaskReadSensors(void)
{
    /* Wait startup task to finish */
    xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_READ_SENSORS_INIT_TIMEOUT_MS));

    while(1)
    {
        TickType_t last_cycle = xTaskGetTickCount();

        uint16_t buf = 0;

        /* OBDH current */
        if (current_sensor_read_ma(&buf) == 0)
        {
            sat_data_buf.obdh.data.current = buf;

            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_READ_SENSORS_NAME, "Current input current: ");
            sys_log_print_uint((uint32_t)buf);
            sys_log_print_msg(" mA");
            sys_log_new_line();
        }

        /* OBDH voltage */
        if (voltage_sensor_read_mv(&buf) == 0)
        {
            sat_data_buf.obdh.data.voltage = buf;

            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_READ_SENSORS_NAME, "Current input voltage: ");
            sys_log_print_uint((uint32_t)buf);
            sys_log_print_msg(" mV");
            sys_log_new_line();
        }

        /* OBDH temperature */
        if (temp_sensor_read_k(&buf) == 0)
        {
            sat_data_buf.obdh.data.temperature = buf;

            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_READ_SENSORS_NAME, "Current uC temperature: ");
            sys_log_print_uint((uint32_t)buf);
            sys_log_print_msg(" K");
            sys_log_new_line();
        }

        /* Data timestamp */
        sat_data_buf.obdh.timestamp = system_get_time();

        vTaskDelayUntil(&last_cycle, pdMS_TO_TICKS(TASK_READ_SENSORS_PERIOD_MS));
    }
}

/** \} End of read_sensors group */
