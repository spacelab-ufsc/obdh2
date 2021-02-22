/*
 * temp_sensor.c
 * 
 * Copyright (C) 2020, SpaceLab.
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
 * \brief Temperature sensor device implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.4.3
 * 
 * \date 17/03/2020
 * 
 * \addtogroup temp_sensor
 * \{
 */

#include <drivers/adc/adc.h>

#include <system/sys_log/sys_log.h>

#include "temp_sensor.h"

int temp_sensor_init()
{
    sys_log_print_event_from_module(SYS_LOG_INFO, TEMP_SENSOR_MODULE_NAME, "Initializing the temperature sensor...");
    sys_log_new_line();

    if (adc_init(TEMP_SENSOR_ADC_PORT, (adc_config_t){}) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TEMP_SENSOR_MODULE_NAME, "Error initializing the temperature sensor!");
        sys_log_new_line();

        return -1;
    }

    float temp = 0;
    if (temp_sensor_read(&temp) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TEMP_SENSOR_MODULE_NAME, "Error reading the temperature value!");
        sys_log_new_line();

        return -1;
    }

    sys_log_print_event_from_module(SYS_LOG_INFO, TEMP_SENSOR_MODULE_NAME, "Current temperature: ");
    sys_log_print_float(temp, 2);
    sys_log_print_msg(" oC");
    sys_log_new_line();

    return 0;
}

int temp_sensor_read_raw(uint16_t *val)
{
    return adc_read(TEMP_SENSOR_ADC_PORT, val);
}

int temp_sensor_read(float *temp)
{
    uint16_t raw_temp = 0;

    if (temp_sensor_read_raw(&raw_temp) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TEMP_SENSOR_MODULE_NAME, "Error reading the raw temperature value!");
        sys_log_new_line();

        return -1;
    }

	*temp = (float)(((int32_t)raw_temp * 2 - TEMP_SENSOR_CAL_15V_30C) * (85 - 30)) / (TEMP_SENSOR_CAL_15V_85C - TEMP_SENSOR_CAL_15V_30C) + 30.0f;

    return 0;
}

/** \} End of temp_sensor group */
