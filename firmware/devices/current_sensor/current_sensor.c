/*
 * current_sensor.c
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
 * \brief Current sensor device implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.5.21
 * 
 * \date 2020/07/11
 * 
 * \addtogroup current_sensor
 * \{
 */

#include <drivers/adc/adc.h>

#include <system/sys_log/sys_log.h>

#include "current_sensor.h"

int current_sensor_init(void)
{
    sys_log_print_event_from_module(SYS_LOG_INFO, CURRENT_SENSOR_MODULE_NAME, "Initializing the current sensor...");
    sys_log_new_line();

    adc_config_t cur_sense_adc_config = {0};

    if (adc_init(CURRENT_SENSOR_ADC_PORT, cur_sense_adc_config) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, CURRENT_SENSOR_MODULE_NAME, "Error initializing the current sensor!");
        sys_log_new_line();

        return -1;
    }

    uint16_t cur = 0;
    if (current_sensor_read_ma(&cur) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, CURRENT_SENSOR_MODULE_NAME, "Error reading the current value!");
        sys_log_new_line();

        return -1;
    }

    sys_log_print_event_from_module(SYS_LOG_INFO, CURRENT_SENSOR_MODULE_NAME, "Current input current: ");
    sys_log_print_uint(cur);
    sys_log_print_msg(" mA");
    sys_log_new_line();

    return 0;
}

int current_sensor_read_raw(uint16_t *val)
{
    return adc_read(CURRENT_SENSOR_ADC_PORT, val);
}

uint16_t current_sensor_raw_to_ma(uint16_t raw)
{
    return (uint16_t)((1000UL*(uint32_t)raw*ADC_AVCC_MV)/(ADC_RANGE*CURRENT_SENSOR_RL_VALUE_KOHM*CURRENT_SENSOR_GAIN_MA_MV*CURRENT_SENSOR_RSENSE_VALUE_MOHM));
}

int current_sensor_read_ma(uint16_t *cur)
{
    uint16_t raw_cur = 0;

    if (current_sensor_read_raw(&raw_cur) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, CURRENT_SENSOR_MODULE_NAME, "Error reading the raw current value!");
        sys_log_new_line();

        return -1;
    }

    *cur = current_sensor_raw_to_ma(raw_cur);

    return 0;
}

/** \} End of current_sensor group */
