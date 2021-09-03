/*
 * tps382x_wrap.h
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
 * \brief TPS382x driver wrap definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.5.11
 * 
 * \date 2021/02/17
 * 
 * \defgroup tps382x_wrap TPS382x Wrap
 * \ingroup tests
 * \{
 */

#ifndef TPS382X_WRAP_H_
#define TPS382X_WRAP_H_

#include <drivers/tps382x/tps382x.h>

/**
 * \brief Mockup of the TPS382x initialization function implemented as an wrap function.
 *
 * \param[in] config is the configuration parameters of the TPS382x driver.
 *
 * \return The status/error code.
 */
int __wrap_tps382x_init(tps382x_config_t config);

/**
 * \brief Mockup of the TPS382x trigger function implemented as an wrap function.
 *
 * \param[in] config is the configuration parameters of the TPS382x driver.
 *
 * \return None.
 */
void __wrap_tps382x_trigger(tps382x_config_t config);

#endif /* TPS382X_WRAP_H_ */

/** \} End of tps382x_wrap group */
