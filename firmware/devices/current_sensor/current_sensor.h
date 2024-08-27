/*
 * current_sensor.h
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
 * \brief Current sensor device definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.10.17
 * 
 * \date 2020/07/11
 * 
 * \defgroup current_sensor Current Sensor
 * \ingroup devices
 * \{
 */

#ifndef CURRENT_SENSOR_H_
#define CURRENT_SENSOR_H_

#include <stdint.h>

#include <drivers/adc/adc.h>

#define CURRENT_SENSOR_MODULE_NAME          "Current Sensor"

#define CURRENT_SENSOR_ADC_PORT             ADC_PORT_8      /**< Current sensor ADC port (channel). */

/* Rsense value */
#define CURRENT_SENSOR_RSENSE_VALUE_OHM     0.05            /**< Rsense resistor value in Ohm. */
#define CURRENT_SENSOR_RSENSE_VALUE_MOHM    50              /**< Rsense resistor value in mOhm. */

/* Rload value */
#define CURRENT_SENSOR_RL_VALUE_OHM         12000           /**< RL resistor value in Ohms. */
#define CURRENT_SENSOR_RL_VALUE_KOHM        12              /**< RL resistor value in kOhms. */

/* Gain value */
#define CURRENT_SENSOR_GAIN_A_V             0.025           /**< Current gain of max9934 in A/V. */
#define CURRENT_SENSOR_GAIN_MA_MV           25              /**< Current gain of max9934 in mA/mV. */

/**
 * \brief Current sensor device initialization routine.
 *
 * \return The status/error code.
 */
int current_sensor_init(void);

/**
 * \brief Reads the raw current from the current sensor.
 *
 * \param[in,out] cur is a pointer to store the raw value of the current.
 *
 * \return The status/error code.
 */
int current_sensor_read_raw(uint16_t *cur);

/**
 * \brief Converts a raw reading of the current sensor to a real current in mA.
 *
 * \param[in] raw is the raw reading of the current sensor.
 *
 * \return The converted current in mA.
 */
uint16_t current_sensor_raw_to_ma(uint16_t raw);

/**
 * \brief Reads the current from the current sensor in mA.
 *
 * \param[in,out] cur is a pointer to store the read current in mA.
 *
 * \return The status/error code.
 */
int current_sensor_read_ma(uint16_t *cur);

#endif /* CURRENT_SENSOR_H_ */

/** \} End of current_sensor group */
