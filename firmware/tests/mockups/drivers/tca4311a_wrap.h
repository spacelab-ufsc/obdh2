/*
 * tca4311a_wrap.h
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
 * \brief TCA4311A driver wrap definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.8.0
 * 
 * \date 2021/10/11
 * 
 * \defgroup tca4311a_wrap TCA4311A Wrap
 * \ingroup tests
 * \{
 */

#ifndef TCA4311A_WRAP_H_
#define TCA4311A_WRAP_H_

#include <drivers/tca4311a/tca4311a.h>

int __wrap_tca4311a_init(tca4311a_config_t config, bool en);

int __wrap_tca4311a_enable(tca4311a_config_t config);

int __wrap_tca4311a_disable(tca4311a_config_t config);

int __wrap_tca4311a_is_ready(tca4311a_config_t config);

int __wrap_tca4311a_write(tca4311a_config_t config, i2c_slave_adr_t adr, uint8_t *data, uint16_t len);

int __wrap_tca4311a_read(tca4311a_config_t config, i2c_slave_adr_t adr, uint8_t *data, uint16_t len);

int __wrap_tca4311a_write_byte(tca4311a_config_t config, i2c_slave_adr_t adr, uint8_t byte);

int __wrap_tca4311a_read_byte(tca4311a_config_t config, i2c_slave_adr_t adr, uint8_t *byte);

#endif /* TCA4311A_WRAP_H_ */

/** \} End of tca4311a_wrap group */
