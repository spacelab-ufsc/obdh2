/*
 * phj_wrap.h
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
 * \brief PHJ driver wrap definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.45
 * 
 * \date 2021/08/18
 * 
 * \defgroup phj_wrap PHJ Wrap
 * \ingroup tests
 * \{
 */

#ifndef PHJ_WRAP_H_
#define PHJ_WRAP_H_

#include <stdint.h>

#include <drivers/phj/phj.h>

int __wrap_phj_init_i2c(phj_config_i2c_t config);

int __wrap_phj_init_gpio(phj_config_gpio_t config);

int __wrap_phj_read(uint8_t *data, uint16_t len);

int __wrap_phj_check_converter(void);

int __wrap_phj_check_message(void);

#endif /* PHJ_WRAP_H_ */

/** \} End of phj_wrap group */
