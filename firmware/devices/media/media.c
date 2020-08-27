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
 * \version 0.4.14
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

int media_write(media_t med, uint32_t adr, uint32_t *data, uint16_t len)
{
    switch(med)
    {
        case MEDIA_INT_FLASH:
        {
            /* Address index */
            adr += FLASH_SEG_A_ADR;

            uint16_t i = 0;
            for(i=0; i<len; i+=4)
            {
                flash_write_long(data[i], (uint32_t*)(adr + i));
            }

            return 0;
        }
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

int media_read(media_t med, uint32_t adr, uint32_t *data, uint16_t len)
{
    switch(med)
    {
        case MEDIA_INT_FLASH:
        {
            /* Address index */
            adr += FLASH_SEG_A_ADR;

            uint16_t i = 0;
            for(i=0; i<len; i+=4)
            {
                data[i] = flash_read_long((uint32_t*)(adr + i));
            }

            return 0;
        }
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
            flash_write_single(0xFF, (uint8_t*)adr);

            return 0;
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
