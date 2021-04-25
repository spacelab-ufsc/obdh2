/*
 * temp_sensor.h
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
 * \brief Temperature sensor device definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.6.3
 * 
 * \date 2020/03/17
 * 
 * \defgroup temp_sensor Temperature Sensor
 * \ingroup devices
 * \{
 */

#ifndef TEMP_SENSOR_H_
#define TEMP_SENSOR_H_

#include <stdint.h>

#define TEMP_SENSOR_MODULE_NAME     "Temperature Sensor"

#define TEMP_SENSOR_ADC_PORT        ADC_PORT_0                  /**< Temperature sensors ADC port (channel). */

/**
 * \brief Temperature sensor device initialization routine.
 *
 * \return The status/error code.
 */
int temp_sensor_init(void);

/**
 * \brief Reads the raw temperature from the temperature sensor.
 *
 * \param[in,out] val is a pointer to store the raw value of the temperature.
 *
 * \return The status/error code.
 */
int temp_sensor_read_raw(uint16_t *val);

/**
 * \brief Converts a raw reading of the temperature sensor to a real temperature in oC.
 *
 * \param[in] raw is the raw reading of the temperature.
 *
 * \return The converted temperature in Celsius.
 */
uint16_t temp_sensor_raw_to_c(uint16_t raw);

/**
 * \brief Converts a raw reading of the temperature sensor to a real temperature in Kelvin.
 *
 * \param[in] raw is the raw reading of the temperature.
 *
 * \return The converted temperature in Kelvin.
 */
uint16_t temp_sensor_raw_to_k(uint16_t raw);

/**
 * \brief Reads the temperature from the temperature sensor in oC.
 *
 * \param[in,out] temp is a pointer to store the read temperature.
 *
 * \return The status/error code.
 */
int temp_sensor_read_c(float *temp);

/**
 * \brief Reads the temperature from the temperature sensor in Kelvin.
 *
 * \param[in,out] temp is a pointer to store the read temperature.
 *
 * \return The status/error code.
 */
int temp_sensor_read_k(uint16_t *temp);

#endif /* TEMP_SENSOR_H_ */

/** \} End of temp_sensor group */
