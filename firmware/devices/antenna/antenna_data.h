/*
 * antenna_data.h
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
 * \brief Antenna data structure definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.4.7
 * 
 * \date 2020/07/24
 * 
 * \defgroup antenna_data Data
 * \ingroup antenna
 * \{
 */

#ifndef ANT_DATA_H_
#define ANT_DATA_H_

#include <stdint.h>

/**
 * \brief Antenna status type.
 */
typedef uint8_t ant_status;

/**
 * \brief Antena report.
 */
typedef struct
{
    uint8_t activation_count;       /**< Activation count. */
    uint16_t activation_time;       /**< Activation time. */
} ant_report_t;

/**
 * \brief Antenna data.
 */
typedef struct
{
    uint32_t timestamp;             /**< Data timestamp (system ticks). */
    uint16_t temperature;           /**< Antenna raw temperature. */
    ant_status_t status;            /**< Antenna status. */
    ant_report_t antenna_1;         /**< Antenna 1 report. */
    ant_report_t antenna_2;         /**< Antenna 2 report. */
    ant_report_t antenna_3;         /**< Antenna 3 report. */
    ant_report_t antenna_4;         /**< Antenna 4 report. */
} ant_data_t;

#endif /* ANT_DATA_H_ */

/** \} End of antenna_data group */
