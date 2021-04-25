/*
 * temp_sensor.c
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
 * \brief Temperature sensor device implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.6.3
 * 
 * \date 17/03/2020
 * 
 * \addtogroup temp_sensor
 * \{
 */

#include <drivers/adc/adc.h>

#include <system/sys_log/sys_log.h>

#include "temp_sensor.h"

int temp_sensor_init(void)
{
    sys_log_print_event_from_module(SYS_LOG_INFO, TEMP_SENSOR_MODULE_NAME, "Initializing the temperature sensor...");
    sys_log_new_line();

    adc_config_t temp_sense_adc_config = {0};

    if (adc_init(TEMP_SENSOR_ADC_PORT, temp_sense_adc_config) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TEMP_SENSOR_MODULE_NAME, "Error initializing the temperature sensor!");
        sys_log_new_line();

        return -1;
    }

    float temp = 0;
    if (temp_sensor_read_c(&temp) != 0)
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

uint16_t temp_sensor_raw_to_c(uint16_t raw)
{
    return (raw - adc_temp_get_nref())/adc_temp_get_mref();
}

uint16_t temp_sensor_raw_to_k(uint16_t raw)
{
    return (uint16_t)(temp_sensor_raw_to_c(raw) + 273);
}

int temp_sensor_read_c(float *temp)
{
    uint16_t raw_temp = 0;

    if (temp_sensor_read_raw(&raw_temp) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TEMP_SENSOR_MODULE_NAME, "Error reading the raw temperature value!");
        sys_log_new_line();

        return -1;
    }

    *temp = temp_sensor_raw_to_c(raw_temp);

    return 0;
}

int temp_sensor_read_k(uint16_t *temp)
{
    uint16_t raw_temp = 0;

    if (temp_sensor_read_raw(&raw_temp) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TEMP_SENSOR_MODULE_NAME, "Error reading the raw temperature value!");
        sys_log_new_line();

        return -1;
    }

    *temp = temp_sensor_raw_to_k(raw_temp);

    return 0;
}

/** \} End of temp_sensor group */
