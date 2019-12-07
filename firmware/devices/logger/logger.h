/*
 * logger.h
 * 
 * Copyright (C) 2019, SpaceLab.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with OBDH 2.0. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Functions for printing messages and bytes over an UART port.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.0
 * 
 * \date 03/11/2019
 * 
 * \defgroup logger Logger
 * \ingroup devices
 * \{
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdint.h>
#include <stdbool.h>

#include "FreeRTOS.h"
#include "semphr.h"

/**
 * \brief Event types.
 */
typedef enum
{
    LOGGER_INFO,                /**< Information message. */
    LOGGER_WARNING,             /**< Warning message. */
    LOGGER_ERROR                /**< Error message. */
} logger_event_type_e;

/**
 * \brief Logger text colors list.
 */
typedef enum
{
    LOGGER_COLOR_BLACK = 0,     /**< Color black. */
    LOGGER_COLOR_RED,           /**< Color red. */
    LOGGER_COLOR_GREEN,         /**< Color green. */
    LOGGER_COLOR_YELLOW,        /**< Color yellow. */
    LOGGER_COLOR_BLUE,          /**< Color blue. */
    LOGGER_COLOR_MAGENTA,       /**< Color magenta. */
    LOGGER_COLOR_CYAN,          /**< Color cyan. */
    LOGGER_COLOR_WHITE          /**< Color white. */
} logger_colors_e;

/**
 * \brief Logger device mutex.
 */
extern SemaphoreHandle_t xLoggerSemaphore;

/**
 * \brief Initialization of the logger mode.
 * 
 * \return TRUE/FALSE if successful or not:
 */
bool logger_init();

/**
 * \brief Sets the foreground color for the next logger message.
 *
 * This function uses the ANSI color code to output color logger messages.
 *
 * \param[in] color is the foreground color of text to select. It can be:
 * \parblock
 *      -\b LOGGER_COLOR_BLACK
 *      -\b LOGGER_COLOR_RED
 *      -\b LOGGER_COLOR_GREEN
 *      -\b LOGGER_COLOR_YELLOW
 *      -\b LOGGER_COLOR_BLUE
 *      -\b LOGGER_COLOR_MAGENTA
 *      -\b LOGGER_COLOR_CYAN
 *      -\b LOGGER_COLOR_WHITE
 *      .
 * \endparblock
 *
 * \return None.
 */
void logger_set_color(uint8_t color);

/**
 * \brief Resets the color back to normal.
 *
 * \return None.
 */
void logger_reset_color();

/**
 * \brief Prints a general event.
 *
 * \param[in] type is the type of event. It can be:
 * \parblock
 *      -\b LOGGER_INFO
 *      -\b LOGGER_WARNING
 *      -\b LOGGER_ERROR
 *      .
 * \endparblock
 * \param[in] event is the event text.
 *
 * \return None.
 */
void logger_print_event(uint8_t type, const char *event);

/**
 * \brief Prints an event from a system module.
 *
 * \param[in] type is the type of event. It can be:
 * \parblock
 *      -\b LOGGER_INFO
 *      -\b LOGGER_WARNING
 *      -\b LOGGER_ERROR
 *      .
 * \endparblock
 * \param[in] module is the module name.
 * \param[in] event is the event text.
 *
 * \return None.
 */
void logger_print_event_from_module(uint8_t type, const char *module, const char *event);

/**
 * \brief Prints a message over the UART.
 * 
 * \param[in] msg is the message to be written.
 * 
 * \return None.
 */
void logger_print_msg(const char *msg);

/**
 * \brief Goes to the next line.
 *
 * \return None.
 */
void logger_new_line();

/**
 * \brief Prints a integer digit over the logger device.
 * 
 * \param[in] d is the digit to be written (0 to 9)
 * 
 * \return None.
 */
void logger_print_digit(uint8_t d);

/**
 * \brief Prints a decimal number over the logger device.
 *
 * \param[in] dec is the decimal number to print.
 *
 * \return None.
 */
void logger_print_dec(uint32_t dec);

/**
 * \brief Prints a hexadecimal value over the UART port.
 * 
 * Example:
 *      - Integer   = 0x65
 *      - Output    = "0x65"
 * 
 * \note The integer is printed in ASCII code.
 * 
 * \param hex is the hexadecimal to be written.
 * 
 * \return None.
 */
void logger_print_hex(uint32_t hex);

/**
 * \brief Prints a raw byte over the UART.
 * 
 * \param[in] byte is the byte to be printed.
 * 
 * \return None.
 */
void logger_print_byte(uint8_t byte);

/**
 * \brief Prints the system time in milliseconds.
 *
 * \return None.
 */
void logger_print_system_time();

/**
 * \brief Prints the license text and genreal firmware information.
 *
 * \return None.
 */
void logger_print_license_msg();

/**
 * \brief Prints the splash screen of the firmware.
 *
 * \return None.
 */
void logger_print_splash_screen();

/**
 * \brief Writes the current firmware version.
 *
 * \return None.
 */
void logger_print_firmware_version();

/**
 * \brief Puts the program in an infinite loop.
 * 
 * This function can be used in case of a critical error during debug mode.
 * 
 * \return None.
 */
void logger_abort();

/**
 * \brief Initialization of the logger UART port.
 * 
 * UART settings:
 *      - MCU interface = USCI_A1
 *      - Baudrate      = 115200 bps
 *      - Data bits     = 8
 *      - Parity bit    = None
 *      - Stop bits     = 1
 *      .
 * 
 * \return TRUE/FALSE if successful or not.
 */
bool logger_uart_init();

/**
 * \brief Writes a byte over the UART port.
 *
 * \param[in] byte is the byte to be written.
 *
 * \return None.
 */
void logger_uart_write_byte(uint8_t byte);

/**
 * \brief Creates a mutex to use the logger device.
 *
 * \return TRUE/FALSE if successful or not.
 */
bool logger_mutex_create();

/**
 * \brief Holds the resource (logger device).
 *
 * \return TRUE/FALSE if successful or not.
 */
bool logger_mutex_take();

/**
 * \brief Frees the resource (logger device).
 *
 * \return TRUE/FALSE if successful or not.
 */
bool logger_mutex_give();

#endif // LOGGER_H_

//! \} End of logger group
