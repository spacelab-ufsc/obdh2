/*
 * px_wrap.h
 * 
 * Copyright The OBDH 2.0 Contributors.
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
 * \brief Payload-X driver wrap definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 0.10.19
 * 
 * \date 2023/09/18
 * 
 * \defgroup px_wrap Payload-X Driver Wrap
 * \ingroup tests
 * \{
 */

#ifndef PX_WRAP_H_
#define PX_WRAP_H_

#include <stdint.h>

#include <drivers/px/px.h>

int __wrap_px_init(px_config_t *conf);

int __wrap_px_write(px_config_t *conf, uint8_t *data, uint16_t len);

int __wrap_px_read(px_config_t *conf, uint8_t *data, uint16_t len);

int __wrap_px_enable(px_config_t *conf);

int __wrap_px_disable(px_config_t *conf);

#endif /* PX_WRAP_H_ */

/** \} End of px_wrap group */
