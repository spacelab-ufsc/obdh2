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
 * \version 0.3.1
 * 
 * \date 17/03/2020
 * 
 * \addtogroup temp_sensor
 * \{
 */

#include <drivers/adc/adc.h>

#include <devices/logger/logger.h>

#include "temp_sensor.h"

int temp_sensor_init()
{
    logger_print_event_from_module(LOGGER_INFO, TEMP_SENSOR_MODULE_NAME, "Initializing temperature sensor...");
    logger_new_line();

    return -1;
}

int temp_sensor_read_raw(uint16_t *val)
{
    return -1;
}

int temp_sensor_read(float *temp)
{
    return -1;
}

/** \} End of temp_sensor group */
