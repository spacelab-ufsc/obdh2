/*
 * adc.h
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
 * \brief ADC driver definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.2.17
 * 
 * \date 03/03/2020
 * 
 * \defgroup adc ADC
 * \ingroup drivers
 * \{
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

/**
 * \brief ADC ports.
 */
typedef enum
{
    ADC_PORT_0=0,       /**< ADC port 0 (10-bits). */
    ADC_PORT_1          /**< ADC port 1 (12-bits). */
} adc_port_e;

/**
 * \brief ADC peripheral configuration parameters.
 */
typedef struct
{
    uint16_t sh_src;    /**< Sample-and-Hold signal source. */
    uint8_t clk_src;    /**< Clock source. */
    uint16_t clk_div;   /**< Clock source divider. */
} adc_config_t;

/**
 * \brief ADC port.
 */
typedef uint8_t adc_port_t;

/**
 * \brief ADC interface initialization.
 *
 * \param[in] port is the ADC port to initialize. It can be:
 * \parblock
 *      -\b ADC_PORT_0: 10-bit resolution.
 *      -\b ADC_PORT_1: 12-bit resolution.
 * \endparblock
 *
 * \param[in] config is the configuration of the ADC port.
 *
 * \return The status/error code.
 */
int adc_init(adc_port_t port, adc_config_t config);

/**
 * \brief Reads data from a given ADC port.
 *
 * \param[in] port is the ADC port to read. It can be:
 * \parblock
 *      -\b ADC_PORT_0: 10-bit resolution.
 *      -\b ADC_PORT_1: 12-bit resolution.
 * \endparblock
 *
 * \param[in] val is a pointer to store the read value.
 *
 * \return The status/error code.
 */
int adc_read(adc_port_t port, uint16_t *val);

#endif /* ADC_H_ */

/** \} End of adc group */
