/*
 * rtc.c
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
 * along with OBDH 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief RTC driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.37
 * 
 * \date 2021/04/07
 * 
 * \addtogroup rtc
 * \{
 */

#include <config/config.h>
#include <system/sys_log/sys_log.h>

#include "rtc.h"

rtc_calendar_t calendar_buf = {0};

int rtc_init(void)
{
    /* Set the frequency of the output signal in RTCCLK pin */
    RTC_B_setCalibrationFrequency(RTC_B_BASE, RTC_B_CALIBRATIONFREQ_1HZ);

    /* Set interruptions */
    RTC_B_clearInterrupt(RTC_B_BASE, RTC_B_CLOCK_READ_READY_INTERRUPT);
    RTC_B_enableInterrupt(RTC_B_BASE, RTC_B_CLOCK_READ_READY_INTERRUPT);

    /* Start RTC_B clock */
    RTC_B_startClock(RTC_B_BASE);

    return 0;
}

rtc_calendar_t rtc_read_calendar(void)
{
    return calendar_buf;
}

int rtc_set_calendar(rtc_calendar_t calendar)
{
    int err_counter = 0;

    if (calendar.Seconds > 59)
    {
    #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
        sys_log_print_event_from_module(SYS_LOG_ERROR, RTC_MODULE_NAME, "Invalid calendar data! The seconds are out of range!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err_counter++;
    }

    if (calendar.Minutes > 59)
    {
    #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
        sys_log_print_event_from_module(SYS_LOG_ERROR, RTC_MODULE_NAME, "Invalid calendar data! The minutes are out of range!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err_counter++;
    }

    if (calendar.Hours > 23)
    {
    #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
        sys_log_print_event_from_module(SYS_LOG_ERROR, RTC_MODULE_NAME, "Invalid calendar data! The hours are out of range!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err_counter++;
    }

    if (calendar.DayOfWeek > 6)
    {
    #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
        sys_log_print_event_from_module(SYS_LOG_ERROR, RTC_MODULE_NAME, "Invalid calendar data! The day of the week is out of range!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err_counter++;
    }

    if (calendar.DayOfMonth > 31)
    {
    #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
        sys_log_print_event_from_module(SYS_LOG_ERROR, RTC_MODULE_NAME, "Invalid calendar data! The day of the month is out of range!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err_counter++;
    }

    if (calendar.Month > 12)
    {
    #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
        sys_log_print_event_from_module(SYS_LOG_ERROR, RTC_MODULE_NAME, "Invalid calendar data! The month is out of range!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err_counter++;
    }

    if (calendar.Year > 4095)
    {
    #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
        sys_log_print_event_from_module(SYS_LOG_ERROR, RTC_MODULE_NAME, "Invalid calendar data! The year is out of range!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err_counter++;
    }

    RTC_B_initCalendar(RTC_B_BASE, &calendar, RTC_B_FORMAT_BINARY);

    /* Start RTC_C clock */
    RTC_B_startClock(RTC_B_BASE);

    return err_counter;
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=RTC_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(RTC_VECTOR)))
#endif
void RTC_B_ISR(void)
{
    switch(__even_in_range(RTCIV, 16))
    {
        case 0:     break;  /* No interrupts */
        case 2:             /* RTCRDYIFG */
            calendar_buf = RTC_B_getCalendarTime(RTC_B_BASE);
            break;
        case 4:     break;  /* RTCEVIFG */
        case 6:     break;  /* RTCAIFG */
        case 8:     break;  /* RT0PSIFG */
        case 10:    break;  /* RT1PSIFG */
        case 12:    break;  /* Reserved */
        case 14:    break;  /* Reserved */
        case 16:    break;  /* Reserved */
        default:    break;
    }
}

/** \} End of rtc group */
