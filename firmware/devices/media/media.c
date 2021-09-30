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
 * along with OBDH 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Media device implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.26
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
    int err = -1;

    switch(med)
    {
        case MEDIA_INT_FLASH:
            err = flash_init();

            break;
        case MEDIA_FRAM:
            sys_log_print_event_from_module(SYS_LOG_INFO, MEDIA_MODULE_NAME, "Initializing FRAM memory...");
            sys_log_new_line();

            fram_conf.port      = SPI_PORT_0;
            fram_conf.cs_pin    = SPI_CS_5;
            fram_conf.clock_hz  = CONFIG_SPI_PORT_0_SPEED_BPS;
            fram_conf.wp_pin    = GPIO_PIN_62;

            if (cy15x102qn_init(&fram_conf) == 0)
            {
                err = 0;
            }
            else
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Error initializing the FRAM memory!");
                sys_log_new_line();
            }

            break;
        case MEDIA_NOR:
        {
            sys_log_print_event_from_module(SYS_LOG_INFO, MEDIA_MODULE_NAME, "Initializing NOR memory...");
            sys_log_new_line();

            if (mt25q_init() == 0)
            {
                mt25q_dev_id_t dev_id = {0};

                if (mt25q_read_device_id(&dev_id) == 0)
                {
                    if (dev_id.manufacturer_id == MT25Q_MANUFACTURER_ID)
                    {
                        sys_log_print_event_from_module(SYS_LOG_INFO, MEDIA_MODULE_NAME, "MT25Q with ");

                        switch(dev_id.memory_capacity)
                        {
                            case MT25Q_MEMORY_CAPACITY_2GB:     sys_log_print_msg("2 Gb capacity detected!");       break;
                            case MT25Q_MEMORY_CAPACITY_1GB:     sys_log_print_msg("1 Gb capacity detected!");       break;
                            case MT25Q_MEMORY_CAPACITY_512MB:   sys_log_print_msg("512 Mb capacity detected!");     break;
                            case MT25Q_MEMORY_CAPACITY_256MB:   sys_log_print_msg("256 Mb capacity detected!");     break;
                            case MT25Q_MEMORY_CAPACITY_128MB:   sys_log_print_msg("128 Mb capacity detected!");     break;
                            case MT25Q_MEMORY_CAPACITY_64MB:    sys_log_print_msg("64 Mb capacity detected!");      break;
                            default:                            sys_log_print_msg("UNKNOWN capacity detected!");    break;
                        }

                        sys_log_new_line();

                        err = 0;
                    }
                    else
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Wrong device device ID! (expected=");
                        sys_log_print_hex(MT25Q_MANUFACTURER_ID);
                        sys_log_print_msg(", read=");
                        sys_log_print_hex(dev_id.manufacturer_id);
                        sys_log_print_msg(")");
                        sys_log_new_line();
                    }
                }
                else
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Error reading the device ID of NOR memory!");
                    sys_log_new_line();
                }
            }
            else
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Error initializing the NOR memory!");
                sys_log_new_line();
            }

            break;
        }
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Invalid storage media to initialize!");
            sys_log_new_line();

            break;
    }

    return err;
}

int media_write(media_t med, uint32_t adr, uint8_t *data, uint16_t len)
{
    int err = -1;

    switch(med)
    {
        case MEDIA_INT_FLASH:
        {
            /* Address index */
            uint32_t adr_idx = adr + FLASH_SEG_A_ADR;

            uint16_t i = 0;
            for(i=0; i<len; i+=4U)
            {
                uint32_t adr_counter = adr_idx + i;

                flash_write_long((uint32_t)data[i], &adr_counter);
            }

            err = 0;

            break;
        }
        case MEDIA_FRAM:
            if (cy15x102qn_write(&fram_conf, adr, data, len) == 0)
            {
                err = 0;
            }
            else
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Error wriring data to the FRAM memory!");
                sys_log_new_line();
            }

            break;
        case MEDIA_NOR:
            if (mt25q_write(adr, data, len) == 0)
            {
                err = 0;
            }
            else
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Error writing data to the NOR memory!");
                sys_log_new_line();
            }

            mt25q_delay_ms(5);

            break;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Invalid storage media to write!");
            sys_log_new_line();

            break;
    }

    return err;
}

int media_read(media_t med, uint32_t adr, uint8_t *data, uint16_t len)
{
    int err = -1;

    switch(med)
    {
        case MEDIA_INT_FLASH:
        {
            /* Address index */
            uint32_t adr_idx = adr + FLASH_SEG_A_ADR;

            uint16_t i = 0;
            for(i=0; i<len; i+=4U)
            {
                uint32_t adr_counter = adr_idx + i;

                data[i] = (uint8_t)flash_read_long(&adr_counter);
            }

            err = 0;

            break;
        }
        case MEDIA_FRAM:
            if (cy15x102qn_read(&fram_conf, adr, data, len) == 0)
            {
                err = 0;
            }
            else
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Error reading data from the FRAM memory!");
                sys_log_new_line();
            }

            break;
        case MEDIA_NOR:
            if (mt25q_read(adr, data, len) == 0)
            {
                err = 0;
            }
            else
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Error reading data from the NOR memory!");
                sys_log_new_line();
            }

            break;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Invalid storage media to read!");
            sys_log_new_line();

            break;
    }

    return err;
}

int media_erase(media_t med, media_erase_t type, uint32_t sector)
{
    int err = -1;

    switch(med)
    {
        case MEDIA_INT_FLASH:
            uint8_t sector_conv = sector;

            flash_write_single(0xFF, &sector_conv);

            err = 0;

            break;
        case MEDIA_FRAM:
            /* The FRAM memory does not have an erase operation */
            err = 0;

            break;
        case MEDIA_NOR:
            switch(type)
            {
                case MEDIA_ERASE_DIE:
                    if (mt25q_die_erase(sector) == 0)
                    {
                        err = 0;
                    }
                    else
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Error erasing the die ");
                        sys_log_print_uint(sector);
                        sys_log_print_msg(" of the NOR memory!");
                        sys_log_new_line();
                    }

                    break;
                case MEDIA_ERASE_SECTOR:
                    if (mt25q_sector_erase(sector) == 0)
                    {
                        err = 0;
                    }
                    else
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Error erasing the sector ");
                        sys_log_print_uint(sector);
                        sys_log_print_msg(" of the NOR memory!");
                        sys_log_new_line();
                    }

                    break;
                case MEDIA_ERASE_SUB_SECTOR:
                    if (mt25q_sub_sector_erase(sector) == 0)
                    {
                        err = 0;
                    }
                    else
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Error erasing the sub-sector ");
                        sys_log_print_uint(sector);
                        sys_log_print_msg(" of the NOR memory!");
                        sys_log_new_line();
                    }

                    break;
                default:
                    sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Error erasing the NOR memory! Invalid erase operation!");
                    sys_log_new_line();

                    break;
            }

            break;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Invalid storage media to erase!");
            sys_log_new_line();

            break;
    }

    return err;
}

media_info_t media_get_info(media_t med)
{
    media_info_t info = {0};

    switch(med)
    {
        case MEDIA_INT_FLASH:                                           break;
        case MEDIA_FRAM:                                                break;
        case MEDIA_NOR:         info = mt25q_get_flash_description();   break;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, MEDIA_MODULE_NAME, "Invalid storage media to get the information!");
            sys_log_new_line();

            break;
    }

    return info;
}

/** \} End of media group */
