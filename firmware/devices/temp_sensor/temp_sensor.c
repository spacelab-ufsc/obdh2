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
 * along with OBDH 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Temperature sensor device implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.10.17
 * 
 * \date 2020/03/17
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

    int err = -1;

    if (adc_init() == 0)
    {
        int16_t temp = 0;

        if (temp_sensor_read_c(&temp) == 0)
        {
            sys_log_print_event_from_module(SYS_LOG_INFO, TEMP_SENSOR_MODULE_NAME, "Current temperature: ");
            sys_log_print_int(temp);
            sys_log_print_msg(" oC");
            sys_log_new_line();

            err = 0;
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TEMP_SENSOR_MODULE_NAME, "Error reading the temperature value!");
            sys_log_new_line();
        }
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TEMP_SENSOR_MODULE_NAME, "Error initializing the temperature sensor!");
        sys_log_new_line();
    }

    return err;
}

int temp_sensor_read_raw(uint16_t *val)
{
    return adc_read(TEMP_SENSOR_ADC_PORT, val);
}

int16_t temp_sensor_raw_to_c(uint16_t raw)
{
    float buf = (raw - adc_temp_get_nref())/adc_temp_get_mref();

    if (buf < (-273.0))
    {
        buf = (-273.0);
    }

    return (int16_t)buf;
}

uint16_t temp_sensor_raw_to_k(uint16_t raw)
{
    int16_t temp_c = temp_sensor_raw_to_c(raw);

    if (temp_c < -273)
    {
        temp_c = -273;
    }

    uint16_t res = temp_c + 273;

    return res;
}

int temp_sensor_read_c(int16_t *temp)
{
    int err = -1;

    uint16_t raw_temp = 0;

    if (temp_sensor_read_raw(&raw_temp) == 0)
    {
        *temp = temp_sensor_raw_to_c(raw_temp);

        err = 0;
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TEMP_SENSOR_MODULE_NAME, "Error reading the raw temperature value!");
        sys_log_new_line();
    }

    return err;
}

int temp_sensor_read_k(uint16_t *temp)
{
    int err = -1;

    uint16_t raw_temp = 0;

    if (temp_sensor_read_raw(&raw_temp) == 0)
    {
        *temp = temp_sensor_raw_to_k(raw_temp);

        err = 0;
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TEMP_SENSOR_MODULE_NAME, "Error reading the raw temperature value!");
        sys_log_new_line();
    }

    return err;
}

/** \} End of temp_sensor group */
