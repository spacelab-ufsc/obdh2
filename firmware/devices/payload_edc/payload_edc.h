/*
 * payload_edc.h
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
 * \brief Payload EDC device definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.2.13
 * 
 * \date 18/02/2020
 * 
 * \defgroup payload_edc Payload EDC
 * \ingroup devices
 * \{
 */

#ifndef PAYLOAD_EDC_H_
#define PAYLOAD_EDC_H_

#define PAYLOAD_EDC_MODULE_NAME     "Payload EDC"

/**
 * \brief Payload EDC device initialization.
 *
 * \return The status/error code.
 */
int payload_edc_init();

/**
 * \brief Gets the general state of the payload.
 *
 * \return The status/error code.
 */
int payload_edc_get_state();

/**
 * \brief Enables the EDC payload.
 *
 * \return The status/error code.
 */
int payload_edc_enable();

/**
 * \brief Disables the EDC payload.
 *
 * \return Thet status/error code.
 */
int payload_edc_disable();

#endif /* PAYLOAD_EDC_H_ */

/** \} End of leds group */
