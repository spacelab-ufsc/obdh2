/*
 * ro_wrap.h
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
 * \brief Radio Occultation device wrap definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Renato Augusto Schenkel Meneghin Marchiori <renato.meneghin@posgrad.ufsc.br>
 * 
 * \version 0.0.1
 * 
 * \date 2026/09/29
 * 
 * \defgroup ro_wrap RO Wrap
 * \ingroup tests
 * \{
 */

#ifndef RO_WRAP_H_
#define RO_WRAP_H_

#include <stdint.h>

#include <devices/ro/ro.h>

int __wrap_ro_init(ro_type_t ro);

int __wrap_ro_enable(ro_type_t ro);

int __wrap_ro_disable(ro_type_t ro);

int __wrap_ro_write_cmd(ro_type_t ro, ro_cmd_t cmd);

int __wrap_ro_get_data(ro_type_t ro, ro_data_id_t id, uint8_t *data, uint32_t *len);

#endif /* RO_WRAP_H_ */

/** \} End of ro_wrap group */
