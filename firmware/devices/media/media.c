/*
 * media.c
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
 * \brief Media device implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.3.13
 * 
 * \date 21/07/2020
 * 
 * \addtogroup media
 * \{
 */

#include <system/sys_log/sys_log.h>

#include <drivers/flash/flash.h>

#include "media.h"

int media_init(media_t med)
{
    switch(med)
    {
        case MEDIA_INT_FLASH:
            return flash_init();
        case MEDIA_NOR:
            sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Initialization not implemented for the NOR memory!");
            sys_log_new_line();

            return -1;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Invalid storage media to initialize!");
            sys_log_new_line();

            return -1;
    }
}

int media_write(media_t med, uint32_t adr, uint8_t *data, uint16_t len)
{
    switch(med)
    {
        case MEDIA_INT_FLASH:
        case MEDIA_NOR:
            sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Write operation not implemented for the NOR memory!");
            sys_log_new_line();

            return -1;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Invalid storage media to write!");
            sys_log_new_line();

            return -1;
    }
}

int media_read(media_t med, uint32_t adr, uint8_t *data, uint16_t len)
{
    switch(med)
    {
        case MEDIA_INT_FLASH:
            return -1;
        case MEDIA_NOR:
            sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Read operation not implemented for the NOR memory!");
            sys_log_new_line();

            return -1;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Invalid storage media to read!");
            sys_log_new_line();

            return -1;
    }
}

int media_erase(media_t med, uint32_t adr)
{
    switch(med)
    {
        case MEDIA_INT_FLASH:
            return -1;
        case MEDIA_NOR:
            sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Erase operation not implemented for the NOR memory!");
            sys_log_new_line();

            return -1;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Invalid storage media to erase!");
            sys_log_new_line();

            return -1;
    }
}

/** \} End of media group */
