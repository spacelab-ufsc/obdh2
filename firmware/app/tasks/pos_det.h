/*
 * pos_det.c
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
 * \brief Position determination task definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 1.0.0
 * 
 * \date 2023/07/19
 * 
 * \defgroup pos_det Position Determination
 * \ingroup tasks
 * \{
 */

#ifndef POS_DET_H_
#define POS_DET_H_

#include <stdint.h>
#include <stdbool.h>

#include <FreeRTOS.h>
#include <task.h>
#include <structs/satellite.h>

#define TASK_POS_DET_NAME                       "Position"          /**< Task name. */
#define TASK_POS_DET_STACK_SIZE                 1024                /**< Stack size in bytes. */
#define TASK_POS_DET_PRIORITY                   2                   /**< Task priority. */
#define TASK_POS_DET_PERIOD_MS                  60000               /**< Task period in milliseconds. */
#define TASK_POS_DET_INIT_TIMEOUT_MS            5000                /**< Wait time to initialize the task in milliseconds. */

#define POS_BRAZIL_LIM_N                        (6.0f)              /**< Brazil territory North limit (Rounded up) */
#define POS_BRAZIL_LIM_S                        (-34.0f)            /**< Brazil territory South limit (Rounded up) */
#define POS_BRAZIL_LIM_E                        (-35.0f)            /**< Brazil territory East limit (Rounded up) */
#define POS_BRAZIL_LIM_W                        (-74.0f)            /**< Brazil territory West limit (Rounded up) */

/**
 * \brief Position determination task handle.
 */
extern xTaskHandle xTaskPosDetHandle;

/**
 * \brief Checks if the satellite is orbiting over Brazil, based on the last available position determined.
 *
 * \param[in] latitude is the latitude of the satellite.
 *
 * \param[in] longitude is the longitude of the satellite.
 *
 * \return True if the satellite is over Brazil, false if it is not.
 */
static inline bool is_satellite_in_brazil(float latitude, float longitude)
{
    return ((latitude >= POS_BRAZIL_LIM_S) && (latitude <= POS_BRAZIL_LIM_N) && (longitude >= POS_BRAZIL_LIM_W) && (longitude <= POS_BRAZIL_LIM_E));
}

/**
 * \brief Update TLE lines used for position determination.
 *
 * \param[in,out] obdh is a pointer to the OBDH telemetry struct to store the 
 * updated TLEs.
 *
 * \param[in] bin_tle is the TLE lines in compact binary format.
 *
 * \return The status/error code.
 */
int update_tle_line(obdh_telemetry_t *obdh, const uint8_t *bin_tle);

/**
 * \brief Position determination task.
 *
 * \return None.
 */
void vTaskPosDet(void* p);

#endif /* POS_DET_H_ */

/** \} End of pos_det group */
