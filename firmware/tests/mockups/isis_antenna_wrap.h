/*
 * isis_antenna_wrap.h
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
 * \brief ISIS antenna driver wrap definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.2
 * 
 * \date 2021/08/07
 * 
 * \defgroup isis_antenna_wrap ISIS Antenna Wrap
 * \ingroup tests
 * \{
 */

#ifndef ISIS_ANTENNA_WRAP_H_
#define ISIS_ANTENNA_WRAP_H_

#include <stdint.h>

#include <drivers/isis_antenna/isis_antenna.h>

int __wrap_isis_antenna_init(void);

int __wrap_isis_antenna_arm(void);

int __wrap_isis_antenna_disarm(void);

int __wrap_isis_antenna_start_sequential_deploy(uint8_t sec);

int __wrap_isis_antenna_start_independent_deploy(uint8_t ant, uint8_t sec, uint8_t ovr);

int __wrap_isis_antenna_read_deployment_status_code(uint16_t *status);

int __wrap_isis_antenna_read_deployment_status(isis_antenna_status_t *status);

int __wrap_isis_antenna_get_antenna_status(uint8_t ant, uint8_t *ant_status);

int __wrap_isis_antenna_get_antenna_timeout(uint8_t ant, uint8_t *ant_timeout);

int __wrap_isis_antenna_get_burning(uint8_t ant, uint8_t *ant_burn);

int __wrap_isis_antenna_get_arming_status(bool *armed);

int __wrap_isis_antenna_get_raw_temperature(uint16_t *temp);

int16_t __wrap_isis_antenna_raw_to_temp_c(uint16_t raw);

int __wrap_isis_antenna_get_temperature_c(int16_t *temp);

void __wriap_isis_antenna_delay_s(uint8_t s);

void __wriap_isis_antenna_delay_ms(uint16_t ms);

#endif /* ISIS_ANTENNA_WRAP_H_ */

/** \} End of isis_antenna_wrap group */
