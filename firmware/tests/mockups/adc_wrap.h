/*
 * adc_wrap.h
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
 * \brief ADC driver wrap definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.5.3
 * 
 * \date 2021/02/13
 * 
 * \defgroup adc_wrap ADC Wrap
 * \ingroup tests
 * \{
 */

#ifndef ADC_WRAP_H_
#define ADC_WRAP_H_

#include <stdint.h>

#include <drivers/adc/adc.h>

/**
 * \brief Mockup of the ADC initialization function implemented as an wrap function.
 *
 * \param[in] port is the ADC port to initialize. It can be:
 * \parblock
 *      -\b ADC_PORT_0
 *      -\b ADC_PORT_1
 * \endparblock
 *
 * \param[in] config is the configuration of the ADC port.
 *
 * \return The status/error code.
 */
int __wrap_adc_init(adc_port_t port, adc_config_t config);

/**
 * \brief Mockup of the ADC read function implemented as an wrap function.
 *
 * \param[in] port is the ADC port to read. It can be:
 * \parblock
 *      -\b ADC_PORT_0
 *      -\b ADC_PORT_1
 *      -\b ADC_PORT_2
 *      -\b ADC_PORT_3
 *      -\b ADC_PORT_4
 *      -\b ADC_PORT_5
 *      -\b ADC_PORT_6
 *      -\b ADC_PORT_7
 *      -\b ADC_PORT_8
 *      -\b ADC_PORT_9
 *      -\b ADC_PORT_10
 *      -\b ADC_PORT_11
 *      -\b ADC_PORT_12
 *      -\b ADC_PORT_13
 *      -\b ADC_PORT_14
 *      -\b ADC_PORT_15
 *      .
 * \endparblock
 *
 * \param[in] val is a pointer to store the read value.
 *
 * \return The status/error code.
 */
int __wrap_adc_read(adc_port_t port, uint16_t *val);

#endif /* ADC_WRAP_H_ */

/** \} End of adc_wrap group */
