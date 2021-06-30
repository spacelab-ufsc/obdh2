/*
 * media.c
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
 * \brief Media device implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.2
 * 
 * \date 2020/07/21
 * 
 * \addtogroup media
 * \{
 */

#include <config/config.h>
#include <system/sys_log/sys_log.h>

#include <drivers/flash/flash.h>
#include <drivers/mt25q/mt25q.h>
#include <drivers/cy15x102qn/cy15x102qn.h>

#include "media.h"

cy15x102qn_config_t fram_conf = {0};

int media_init(media_t med)
{
    switch(med)
    {
        case MEDIA_INT_FLASH:
            return flash_init();
        case MEDIA_FRAM:
            sys_log_print_event_from_module(SYS_LOG_INFO, MEDIA_MODULE_NAME, "Initializing FRAM memory...");
            sys_log_new_line();

            fram_conf.port      = SPI_PORT_0;
            fram_conf.cs_pin    = SPI_CS_5;
            fram_conf.clock_hz  = CONFIG_SPI_PORT_0_SPEED_BPS;
            fram_conf.wp_pin    = GPIO_PIN_62;

            if (cy15x102qn_init(&fram_conf) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Error initializing the FRAM memory!");
                sys_log_new_line();

                return -1;
            }

            return 0;
        case MEDIA_NOR:
            sys_log_print_event_from_module(SYS_LOG_INFO, MEDIA_MODULE_NAME, "Initializing NOR memory...");
            sys_log_new_line();

            if (mt25q_init() != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Error initializing the NOR memory!");
                sys_log_new_line();

                return -1;
            }

            mt25q_dev_id_t dev_id = {0};

            if (mt25q_read_device_id(&dev_id) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Error reading the device ID of NOR memory!");
                sys_log_new_line();

                return -1;
            }

            if (dev_id.manufacturer_id != MT25Q_MANUFACTURER_ID)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Wrong device device ID! (expected=");
                sys_log_print_hex(MT25Q_MANUFACTURER_ID);
                sys_log_print_msg(", read=");
                sys_log_print_hex(dev_id.manufacturer_id);
                sys_log_print_msg(")");
                sys_log_new_line();

                return -1;
            }

            sys_log_print_event_from_module(SYS_LOG_INFO, MEDIA_MODULE_NAME, "MT25Q with ");

            switch(dev_id.memory_capacity)
            {
                case MT25Q_MEMORY_CAPACITY_2GB:     sys_log_print_msg("2 Gb capacity detected!");       break;
                case MT25Q_MEMORY_CAPACITY_1GB:     sys_log_print_msg("1 Gb capacity detected!");       break;
                case MT25Q_MEMORY_CAPACITY_512MB:   sys_log_print_msg("512 Mb capacity detected!");     break;
                case MT25Q_MEMORY_CAPACITY_256MB:   sys_log_print_msg("256 Mb capacity detected!");     break;
                case MT25Q_MEMORY_CAPACITY_128MB:   sys_log_print_msg("128 Mb capacity detected!");     break;
                case MT25Q_MEMORY_CAPACITY_64MB:    sys_log_print_msg("64 Mb capacity detected!");      break;
                default:                            sys_log_print_msg("UNKNOWN capacity detected!");
            }

            sys_log_new_line();

            return 0;
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
        {
            /* Address index */
            adr += FLASH_SEG_A_ADR;

            uint16_t i = 0;
            for(i=0; i<len; i+=4)
            {
                flash_write_long((uint32_t)data[i], (uint32_t*)(adr + i));
            }

            return 0;
        }
        case MEDIA_FRAM:
            if (cy15x102qn_write(&fram_conf, adr, data, len) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Error wriring data to the FRAM memory!");
                sys_log_new_line();

                return -1;
            }

            return 0;
        case MEDIA_NOR:
            if (mt25q_write(adr, data, len) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Error writing data to the NOR memory!");
                sys_log_new_line();

                return -1;
            }

            mt25q_delay_ms(5);

            return 0;
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
        {
            /* Address index */
            adr += FLASH_SEG_A_ADR;

            uint16_t i = 0;
            for(i=0; i<len; i+=4)
            {
                data[i] = (uint8_t)flash_read_long((uint32_t*)(adr + i));
            }

            return 0;
        }
        case MEDIA_FRAM:
            if (cy15x102qn_read(&fram_conf, adr, data, len) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Error reading data from the FRAM memory!");
                sys_log_new_line();
            }

            return -1;
        case MEDIA_NOR:
            if (mt25q_read(adr, data, len) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Error reading data from the NOR memory!");
                sys_log_new_line();

                return -1;
            }

            return 0;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Invalid storage media to read!");
            sys_log_new_line();

            return -1;
    }
}

int media_erase(media_t med, media_erase_t type, uint32_t sector)
{
    switch(med)
    {
        case MEDIA_INT_FLASH:
            flash_write_single(0xFF, (uint8_t*)sector);

            return 0;
        case MEDIA_FRAM:
            sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "The erase operation of the FRAM media is not implemented!");
            sys_log_new_line();

            return -1;
        case MEDIA_NOR:
            switch(type)
            {
                case MEDIA_ERASE_DIE:
                    if (mt25q_die_erase(sector) != 0)
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Error erasing the die ");
                        sys_log_print_uint(sector);
                        sys_log_print_msg(" of the NOR memory!");
                        sys_log_new_line();

                        return -1;
                    }

                    break;
                case MEDIA_ERASE_SECTOR:
                    if (mt25q_sector_erase(sector) != 0)
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Error erasing the sector ");
                        sys_log_print_uint(sector);
                        sys_log_print_msg(" of the NOR memory!");
                        sys_log_new_line();

                        return -1;
                    }

                    break;
                case MEDIA_ERASE_SUB_SECTOR:
                    if (mt25q_sub_sector_erase(sector) != 0)
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Error erasing the sub-sector ");
                        sys_log_print_uint(sector);
                        sys_log_print_msg(" of the NOR memory!");
                        sys_log_new_line();

                        return -1;
                    }

                    break;
                default:
                    sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Error erasing the NOR memory! Invalid erase operation!");
                    sys_log_new_line();
            }

            return 0;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Invalid storage media to erase!");
            sys_log_new_line();

            return -1;
    }
}

media_info_t media_get_info(media_t med)
{
    switch(med)
    {
        case MEDIA_INT_FLASH:   return (media_info_t){0};
        case MEDIA_FRAM:        return (media_info_t){0};
        case MEDIA_NOR:         return mt25q_get_flash_description();
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Invalid storage media to get the information!");
            sys_log_new_line();

            return (media_info_t){0};
    }
}

/** \} End of media group */
