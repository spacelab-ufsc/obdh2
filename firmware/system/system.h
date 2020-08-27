/*
 * system.h
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
 * \brief System layer definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.4.14
 * 
 * \date 25/01/2020
 * 
 * \defgroup system System
 * \{
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <stdint.h>

#include "clocks.h"

/**
 * \brief System time type.
 */
typedef uint32_t sys_time_t;

/**
 * \brief System reset.
 *
 * \return None.
 */
void system_reset(void);

/**
 * \brief Gets the last reset cause code.
 *
 * This function reads the value of the reset interrupt vector (SYSRSTIV).
 *
 * \see SLAU208P - MSP430x5xx and MSP430x6xx Family User's Guide. Table 1-26. SYSRSTIV Register Description. Page 96.
 *
 * \return The last reset cause value. It can be:
 * \parblock
 *      -\b 0x00 = No interrupt pending
 *      -\b 0x02 = Brownout (BOR)
 *      -\b 0x04 = RST/NMI (BOR)
 *      -\b 0x06 = PMMSWBOR (BOR)
 *      -\b 0x08 = Wakeup from LPMx.5 (BOR)
 *      -\b 0x0A = Security violation (BOR)
 *      -\b 0x0C = SVSL (POR)
 *      -\b 0x0E = SVSH (POR)
 *      -\b 0x10 = SVML_OVP (POR)
 *      -\b 0x12 = SVMH_OVP (POR)
 *      -\b 0x14 = PMMSWPOR (POR)
 *      -\b 0x16 = WDT time out (PUC)
 *      -\b 0x18 = WDT password violation (PUC)
 *      -\b 0x1A = Flash password violation (PUC)
 *      -\b 0x1C = Reserved
 *      -\b 0x1E = PERF peripheral/configuration area fetch (PUC)
 *      -\b 0x20 = PMM password violation (PUC)
 *      -\b 0x22 to 0x3E = Reserved
 *      .
 * \endparblock
 */
uint8_t system_get_reset_cause(void);

/**
 * \brief Sets the system time.
 *
 * \param[in] tm is the new system time value.
 *
 * \return None.
 */
void system_set_time(sys_time_t tm);

/**
 * \brief Increments the system time.
 *
 * \return None.
 */
void system_increment_time(void);

/**
 * \brief Gets the system time.
 *
 * \return The current system time.
 */
sys_time_t system_get_time(void);

#endif /* SYSTEM_H_ */

/** \} End of system group */
