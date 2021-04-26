/*
 * rtc.h
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
 * \brief RTC driver definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.5.19
 * 
 * \date 2021/04/07
 * 
 * \defgroup rtc RTC
 * \ingroup drivers
 * \{
 */

#ifndef RTC_H_
#define RTC_H_

#include <stdint.h>

#include <hal/rtc_b.h>

#define RTC_MODULE_NAME         "RTC"

/**
 * \brief RTC calendar type.
 */
typedef Calendar rtc_calendar_t;

/**
 * \brief RTC peripheral initialization.
 *
 * This function initializes the RTC_B in calendar mode.
 *
 * \return The status/error code.
 */
int rtc_init(void);

/**
 * \brief Reads the current calendar from the RTC.
 *
 * \param[in,out] calendar is a pointer to store the read calendar data.
 *
 * \return The status/error code.
 */
rtc_calendar_t rtc_read_calendar(void);

/**
 * \brief Sets the calendar of the RTC.
 *
 * \param[in] calendar is the calendar data to set.
 *
 * \return The status/error code.
 */
int rtc_set_calendar(rtc_calendar_t calendar);

#endif /* RTC_H_ */

/** \} End of rtc group */
