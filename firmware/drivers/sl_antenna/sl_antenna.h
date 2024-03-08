/*
 * sl_antenna.h
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
 * \brief SpaceLab Antenna Driver definition.
 *
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 *
 * \version 0.10.9
 *
 * \date 2024/03/06
 *
 * \defgroup sl_antenna SpaceLab Antenna
 * \ingroup drivers
 * \{
 */

#ifndef SL_ANTENNA_H_
#define SL_ANTENNA_H_

#include <stdint.h>

#include <drivers/gpio/gpio.h>

/**
 * \brief Deployment stage duration time in seconds.
 *
 * The deployment has three stages in which the gpio state must match
 * the expected state required by the antenna during the entire stage 
 * duration.
 */
#define SL_ANTENNA_STAGE_DURATION_SEC 20U

/**
 * \brief Deployment transition duration time in seconds.
 *
 * Between each stage there is a transition time for switching the 
 * gpio state for the required one.
 */
#define SL_ANTENNA_TRANSITION_DURATION_SEC 10U
#define SL_ANTENNA_HALF_TRANSITION_DURATION_SEC (SL_ANTENNA_TRANSITION_DURATION_SEC >> 1U)

/* Expected gpio states for each stage */
#define SL_ANTENNA_FIRST_STAGE_STATE GPIO_STATE_HIGH
#define SL_ANTENNA_SECOND_STAGE_STATE GPIO_STATE_LOW
#define SL_ANTENNA_THIRD_STAGE_STATE GPIO_STATE_HIGH

/**
 * \brief Antenna config.
 */
typedef gpio_config_t sl_antenna_config_t;

/**
 * \brief Driver initialization.
 * 
 * \return The status/error code.
 */
int sl_antenna_init(void);

/**
 * \brief Executes sequential deployment.
 * 
 * \return The status/error code.
 */
int sl_antenna_start_sequential_deploy(void);

/**
 * \brief Seconds delay.
 *
 * \param[in] s is the delay in seconds.
 *
 * \return None.
 */
void sl_antenna_delay_s(uint8_t s);

/**
 * \brief Milliseconds delay.
 *
 * \param[in] ms is the delay in milliseconds.
 *
 * \return None.
 */
void sl_antenna_delay_ms(uint16_t ms);

#endif /* DRIVERS_SL_ANTENNA_SL_ANTENNA_H_ */

/** \} End of sl_antenna group */
