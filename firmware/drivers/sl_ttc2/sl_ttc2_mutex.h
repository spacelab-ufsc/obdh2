/*
 * sl_ttc2_mutex.h
 *
 * Copyright The OBDH 2.0 Contributors
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
 * along with OBDH 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 *
 */

/**
 * \brief SpaceLab TTC 2.0 driver mutex definition.
 *
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 *
 * \version 0.10.10
 *
 * \date 2024/02/26
 *
 * \defgroup sl_ttc2_mutex Mutex
 * \ingroup sl_ttc2
 * \{
 */
#ifndef DRIVERS_SL_TTC2_SL_TTC2_MUTEX_H_
#define DRIVERS_SL_TTC2_SL_TTC2_MUTEX_H_

#define SL_TTC2_MUTEX_WAIT_TIME_MS 600U

/**
 * \brief Creates the sl_ttc2 create.
 *
 * \return The status/error code.
 */
int sl_ttc2_mutex_create(void);

/**
 * \brief Takes the sl_ttc2 mutex.
 *
 * \return The status/error code.
 */
int sl_ttc2_mutex_take(void);

/**
 * \brief Gives the sl_ttc2 mutex.
 *
 * \return The status/error code.
 */
int sl_ttc2_mutex_give(void);

#endif /* DRIVERS_SL_TTC2_SL_TTC2_MUTEX_H_ */

/** \} End of sl_ttc2_mutex group */
