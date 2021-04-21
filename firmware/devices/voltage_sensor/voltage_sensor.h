/*
 * voltage_sensor.h
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
 * \brief Voltage sensor device definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.5.27
 * 
 * \date 2020/07/11
 * 
 * \defgroup voltage_sensor Voltage Sensor
 * \ingroup devices
 * \{
 */

#ifndef VOLTAGE_SENSOR_H_
#define VOLTAGE_SENSOR_H_

#include <stdint.h>

#include <drivers/adc/adc.h>

#define VOLTAGE_SENSOR_MODULE_NAME     "Voltage Sensor"

#define VOLTAGE_SENSOR_ADC_PORT         ADC_PORT_4      /**< Voltage sensor ADC port (channel). */
#define VOLTAGE_SENSOR_DIV              2               /**< Voltage divisor factor. */

/**
 * \brief Voltage sensor device initialization routine.
 *
 * \return The status/error code.
 */
int voltage_sensor_init(void);

/**
 * \brief Reads the raw voltage from the voltage sensor.
 *
 * \param[in,out] val is a pointer to store the raw value of the voltage.
 *
 * \return The status/error code.
 */
int voltage_sensor_read_raw(uint16_t *val);

/**
 * \brief Converts a raw reading of the voltage sensor to a real voltage in mV.
 *
 * \param[in] raw is the raw reading of the voltage sensor.
 *
 * \return The converted voltage in mV.
 */
uint16_t voltage_sensor_raw_to_mv(uint16_t raw);

/**
 * \brief Reads the voltage from the voltage sensor in mV.
 *
 * \param[in,out] volt is a pointer to store the read voltage in mV.
 *
 * \return The status/error code.
 */
int voltage_sensor_read_mv(uint16_t *volt);

#endif /* VOLTAGE_SENSOR_H_ */

/** \} End of voltage_sensor group */
