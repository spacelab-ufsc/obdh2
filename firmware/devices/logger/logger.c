/*
 * logger.c
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with OBDH 2.0. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Logger device implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.0
 * 
 * \date 03/11/2019
 * 
 * \addtogroup logger
 * \{
 */

#include <math.h>

#include <FreeRTOS.h>
#include <task.h>

#include <version.h>

#include "logger.h"
#include "logger_config.h"

int logger_init()
{
    if (logger_uart_init())
    {
        logger_new_line();

        logger_print_license_msg();

        logger_print_splash_screen();

        logger_print_msg("==========================================================================");
        logger_new_line();
        logger_print_msg("Version:\t");
        logger_print_firmware_version();
        logger_new_line();

        logger_print_msg("Status:\t\t");
        logger_print_msg(FIRMWARE_STATUS);
        logger_new_line();

        logger_print_msg("Author:\t\t");
        logger_print_msg(FIRMWARE_AUTHOR);
        logger_print_msg(" <");
        logger_print_msg(FIRMWARE_AUTHOR_EMAIL);
        logger_print_msg(">");
        logger_new_line();
        logger_print_msg("==========================================================================");
        logger_new_line();
        logger_new_line();
        logger_new_line();

        logger_mutex_create();

        return 0;
    }
    else
    {
        return -1;
    }
}

void logger_set_color(uint8_t color)
{
    switch(color)
    {
        case LOGGER_COLOR_BLACK:
            logger_print_msg("\033[1;30m");
            break;
        case LOGGER_COLOR_RED:
            logger_print_msg("\033[1;31m");
            break;
        case LOGGER_COLOR_GREEN:
            logger_print_msg("\033[1;32m");
            break;
        case LOGGER_COLOR_YELLOW:
            logger_print_msg("\033[1;33m");
            break;
        case LOGGER_COLOR_BLUE:
            logger_print_msg("\033[1;34m");
            break;
        case LOGGER_COLOR_MAGENTA:
            logger_print_msg("\033[1;35m");
            break;
        case LOGGER_COLOR_CYAN:
            logger_print_msg("\033[1;36m");
            break;
        case LOGGER_COLOR_WHITE:
            logger_print_msg("\033[1;37m");
            break;
        default:
            logger_reset_color();
    }
}

void logger_reset_color()
{
    logger_print_msg("\033[0m");
}

void logger_print_event(uint8_t type, const char *event)
{
    logger_mutex_take();

    logger_print_system_time();
    logger_print_msg(" ");

    switch(type)
    {
        case LOGGER_INFO:
            break;
        case LOGGER_WARNING:
            logger_set_color(LOGGER_WARNING_COLOR);
            break;
        case LOGGER_ERROR:
            logger_set_color(LOGGER_ERROR_COLOR);
            break;
        default:
            break;
    }

    logger_print_msg(event);
}

void logger_print_event_from_module(uint8_t type, const char *module, const char *event)
{
    logger_mutex_take();

    logger_print_system_time();

    logger_set_color(LOGGER_MODULE_NAME_COLOR);
    logger_print_msg(" ");
    logger_print_msg(module);
    logger_reset_color();
    logger_print_msg(": ");

    switch(type)
    {
        case LOGGER_INFO:
            break;
        case LOGGER_WARNING:
            logger_set_color(LOGGER_WARNING_COLOR);
            break;
        case LOGGER_ERROR:
            logger_set_color(LOGGER_ERROR_COLOR);
            break;
        default:
            break;
    }

    logger_print_msg(event);
}

void logger_print_msg(const char *msg)
{
    uint16_t i = 0;
    while(msg[i] != '\0')
    {
        logger_print_byte(msg[i]);
        i++;
    }
}

void logger_new_line()
{
    logger_reset_color();
    logger_print_msg("\n\r");
    logger_mutex_give();
}

void logger_print_digit(uint8_t digit)
{
    if (digit < 0x0A)
    {
        logger_print_byte(digit + 0x30);    // 0x30 = ascii 0
    }
    else if (digit <= 0x0F)
    {
        logger_print_byte(digit + 0x37);    // 0x37 = ascii 7
    }
    else
    {
        logger_print_byte('N');
    }
}

void logger_print_dec(uint32_t dec)
{
    if (dec == 0)
    {
        logger_print_digit(0);
    }
    else
    {
        uint8_t dec_str[10];                // 32-bits = decimal with 10 digits

        uint8_t digits = log10(dec) + 1;

        uint8_t i = 0;
        for(i=0; i<digits; ++i, dec /= 10)
        {
            dec_str[i] = dec % 10;
        }

        uint8_t j = 0;
        for(j=i; j>0; j--)
        {
            logger_print_digit(dec_str[j-1]);
        }
    }
}

void logger_print_hex(uint32_t hex)
{
    logger_print_msg("0x");
    
    if (hex > 0x00FFFFFF)
    {
        logger_print_digit((uint8_t)(hex >> 28) & 0x0F);
        logger_print_digit((uint8_t)(hex >> 24) & 0x0F);
    }

    if (hex > 0x0000FFFF)
    {
        logger_print_digit((uint8_t)(hex >> 20) & 0x0F);
        logger_print_digit((uint8_t)(hex >> 16) & 0x0F);
    }

    if (hex > 0x000000FF)
    {
        logger_print_digit((uint8_t)(hex >> 12) & 0x0F);
        logger_print_digit((uint8_t)(hex >> 8) & 0x0F);
    }

    logger_print_digit((uint8_t)(hex >> 4) & 0x0F);
    logger_print_digit((uint8_t)(hex & 0x0F));
}

void logger_print_byte(uint8_t byte)
{
    logger_uart_write_byte(byte);
}

void logger_print_system_time()
{
    logger_set_color(LOGGER_SYSTEM_TIME_COLOR);

    logger_print_msg("[ ");
    logger_print_dec(xTaskGetTickCount()*10);    // System time in milliseconds
    logger_print_msg(" ]");

    logger_reset_color();
}

void logger_print_license_msg()
{
    logger_print_msg("OBDH 2.0 Copyright (C) 2020, SpaceLab;");
    logger_new_line();
    logger_print_msg("This program comes with ABSOLUTELY NO WARRANTY.");
    logger_new_line();
    logger_print_msg("This is free software, and you are welcome to redistribute it");
    logger_new_line();
    logger_print_msg("under certain conditions.");
    logger_new_line();
    logger_new_line();
    logger_print_msg("Source code: https://github.com/spacelab-ufsc/obdh2");
    logger_new_line();
    logger_print_msg("Documentation: https://github.com/spacelab-ufsc/obdh2/doc");
    logger_new_line();
}

void logger_print_splash_screen()
{
    logger_print_msg("                                                                          ");
    logger_new_line();
    logger_print_msg("                                                                          ");
    logger_new_line();
    logger_print_msg("..........................................................................");
    logger_new_line();
    logger_print_msg("..........................................................................");
    logger_new_line();
    logger_print_msg("..........                                                      ..........");
    logger_new_line();
    logger_print_msg("..........  _____ ____        _         ___  ____  ____  _   _  ..........");
    logger_new_line();
    logger_print_msg(".......... |  ___/ ___|  __ _| |_      / _ \\| __ )|  _ \\| | | | ..........");
    logger_new_line();
    logger_print_msg(".......... | |_  \\___ \\ / _` | __|____| | | |  _ \\| | | | |_| | ..........");
    logger_new_line();
    logger_print_msg(".......... |  _|  ___) | (_| | ||_____| |_| | |_) | |_| |  _  | ..........");
    logger_new_line();
    logger_print_msg(".......... |_|   |____/ \\__,_|\\__|     \\___/|____/|____/|_| |_| ..........");
    logger_new_line();
    logger_print_msg("..........                                                      ..........");
    logger_new_line();
    logger_print_msg("..........................................................................");
    logger_new_line();
    logger_print_msg("..........................................................................");
    logger_new_line();
    logger_print_msg("                                                                          ");
    logger_new_line();
    logger_print_msg("                                                                          ");
    logger_new_line();
}

void logger_print_firmware_version()
{
    logger_print_msg("[ ");
    logger_print_msg(FIRMWARE_VERSION);
    logger_print_msg(" ]");
}

//! \} End of logger group
