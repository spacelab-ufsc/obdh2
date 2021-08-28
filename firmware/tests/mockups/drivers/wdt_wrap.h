/*
 * wdt_wrap.h
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
 * \brief WDT driver wrap definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.5.11
 * 
 * \date 2021/02/16
 * 
 * \defgroup wdt_wrap WDT Wrap
 * \ingroup tests
 * \{
 */

#ifndef WDT_WRAP_H_
#define WDT_WRAP_H_

#include <drivers/wdt/wdt.h>

/**
 * \brief Mockup of the WDT initialization function implemented as an wrap function.
 *
 * \param[in] config is the configuration to initialize the WDT.
 *
 * \return The status/error code.
 */
int __wrap_wdt_init(wdt_config_t config);

/**
 * \brief Mockup of the WDT reset function implemented as an wrap function.
 *
 * \return None.
 */
void __wrap_wdt_reset(void);

#endif /* WDT_WRAP_H_ */

/** \} End of wdt_wrap group */
