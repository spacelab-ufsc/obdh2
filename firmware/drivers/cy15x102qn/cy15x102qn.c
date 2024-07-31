/*
 * cy15x102qn.c
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
 * \brief CY15x102QN driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 0.10.17
 * 
 * \date 2021/06/04
 * 
 * \addtogroup cy15x102qn
 * \{
 */

#include <string.h>

#include <config/config.h>

#include <system/sys_log/sys_log.h>

#include "cy15x102qn.h"

#define CY15X102QN_DUMMY_BYTE   0x00

int cy15x102qn_init(cy15x102qn_config_t *conf)
{
    int err = -1;

    if (cy15x102qn_spi_init(conf) == 0)
    {
        if (cy15x102qn_gpio_init(conf) == 0)
        {
            err = 0;
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, CY15X102QN_MODULE_NAME, "Error initializing the GPIO pins!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        }
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, CY15X102QN_MODULE_NAME, "Error initializing the SPI interface!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return err;
}

int cy15x102qn_set_write_enable(cy15x102qn_config_t *conf)
{
    int err = 0;

    uint8_t cmd = CY15X102QN_OPCODE_WREN;

    if (cy15x102qn_spi_write(conf, &cmd, 1) != 0)
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, CY15X102QN_MODULE_NAME, "Error writing the WREN command!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err = -1;
    }

    return err;
}

int cy15x102qn_reset_write_enable(cy15x102qn_config_t *conf)
{
    int err = 0;

    uint8_t cmd = CY15X102QN_OPCODE_WRDI;

    if (cy15x102qn_spi_write(conf, &cmd, 1) != 0)
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, CY15X102QN_MODULE_NAME, "Error writing the WRDI command!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err = -1;
    }

    return err;
}

int cy15x102qn_read_status_reg(cy15x102qn_config_t *conf, cy15x102qn_status_t *status)
{
    int err = -1;

    uint8_t cmd[2] = {CY15X102QN_OPCODE_RDSR, 0};
    uint8_t ans[2];

    if (cy15x102qn_spi_transfer(conf, cmd, ans, 2) == 0)
    {
        *status = ans[1];

        err = 0;
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, CY15X102QN_MODULE_NAME, "Error writing the RDSR command!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return err;
}

int cy15x102qn_write_status_reg(cy15x102qn_config_t *conf, cy15x102qn_status_t status)
{
    int err = 0;

    uint8_t cmd[2] = {CY15X102QN_OPCODE_WRSR, 0};

    cmd[1] = status;

    if (cy15x102qn_spi_write(conf, cmd, 2) != 0)
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, CY15X102QN_MODULE_NAME, "Error writing the WRSR command!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err = -1;
    }

    return err;
}

int cy15x102qn_write(cy15x102qn_config_t *conf, uint32_t adr, uint8_t *data, uint32_t len)
{
    int err = -1;

    uint8_t cmd = CY15X102QN_OPCODE_WRITE;
    uint8_t adr_arr[3];

    adr_arr[0] = (adr >> 16) & 0xFFU;
    adr_arr[1] = (adr >> 8) & 0xFFU;
    adr_arr[2] = adr & 0xFFU;

    if (cy15x102qn_mutex_take() == 0)
    {
        if (cy15x102qn_set_write_enable(conf) == 0)
        {
            if (cy15x102qn_spi_select(conf) == 0)
            {
                /* Write opcode */
                if (cy15x102qn_spi_write_only(conf, &cmd, 1) == 0)
                {
                    /* Write address */
                    if (cy15x102qn_spi_write_only(conf, adr_arr, 3) == 0)
                    {
                        /* Write data */
                        if (cy15x102qn_spi_write_only(conf, data, len) == 0)
                        {
                            if (cy15x102qn_spi_unselect(conf) == 0)
                            {
                                err = 0;
                            }
                        }
                        else
                        {
                        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                            sys_log_print_event_from_module(SYS_LOG_ERROR, CY15X102QN_MODULE_NAME, "Error writing the data during WRITE command!");
                            sys_log_new_line();
                        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                            if (cy15x102qn_spi_unselect(conf) != 0)
                            {
                                err = -2;
                            }
                        }
                    }
                    else
                    {
                    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                        sys_log_print_event_from_module(SYS_LOG_ERROR, CY15X102QN_MODULE_NAME, "Error writing the address during WRITE command!");
                        sys_log_new_line();
                    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                        if (cy15x102qn_spi_unselect(conf) != 0)
                        {
                            err = -2;
                        }
                    }
                }
                else
                {
                #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                    sys_log_print_event_from_module(SYS_LOG_ERROR, CY15X102QN_MODULE_NAME, "Error writing the opcode during WRITE command!");
                    sys_log_new_line();
                #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                    if (cy15x102qn_spi_unselect(conf) != 0)
                    {
                        err = -2;
                    }
                }
            }
        }

        (void)cy15x102qn_mutex_give();
    }

    return err;
}

int cy15x102qn_read(cy15x102qn_config_t *conf, uint32_t adr, uint8_t *data, uint32_t len)
{
    int err = -1;

    uint8_t cmd = CY15X102QN_OPCODE_READ;
    uint8_t adr_arr[3];

    adr_arr[0] = (adr >> 16) & 0xFFU;
    adr_arr[1] = (adr >> 8) & 0xFFU;
    adr_arr[2] = adr & 0xFFU;

    if (cy15x102qn_mutex_take() == 0)
    {
        if (cy15x102qn_spi_select(conf) == 0)
        {
            /* Write opcode */
            if (cy15x102qn_spi_write_only(conf, &cmd, 1) == 0)
            {
                /* Write address */
                if (cy15x102qn_spi_write_only(conf, adr_arr, 3) == 0)
                {
                    /* Read data */
                    if (cy15x102qn_spi_read_only(conf, data, len) == 0)
                    {
                        if (cy15x102qn_spi_unselect(conf) == 0)
                        {
                            err = 0;
                        }
                    }
                    else
                    {
                    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                        sys_log_print_event_from_module(SYS_LOG_ERROR, CY15X102QN_MODULE_NAME, "Error reading the data during READ command!");
                        sys_log_new_line();
                    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                        if (cy15x102qn_spi_unselect(conf) != 0)
                        {
                            err = -2;
                        }
                    }
                }
                else
                {
                #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                    sys_log_print_event_from_module(SYS_LOG_ERROR, CY15X102QN_MODULE_NAME, "Error writing the address during READ command!");
                    sys_log_new_line();
                #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                    if (cy15x102qn_spi_unselect(conf) != 0)
                    {
                        err = -2;
                    }
                }
            }
            else
            {
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_ERROR, CY15X102QN_MODULE_NAME, "Error writing the opcode during READ command!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                if (cy15x102qn_spi_unselect(conf) != 0)
                {
                    err = -2;
                }
            }
        }

        (void)cy15x102qn_mutex_give();
    }

    return err;
}

int cy15x102qn_fast_read(cy15x102qn_config_t *conf, uint32_t adr, uint8_t *data, uint32_t len)
{
    int err = -1;

    uint8_t cmd = CY15X102QN_OPCODE_FSTRD;
    uint8_t adr_arr[3];
    uint8_t dummy = CY15X102QN_DUMMY_BYTE;

    adr_arr[0] = (adr >> 16) & 0xFFU;
    adr_arr[1] = (adr >> 8) & 0xFFU;
    adr_arr[2] = adr & 0xFFU;

    if (cy15x102qn_spi_select(conf) == 0)
    {
        /* Write opcode */
        if (cy15x102qn_spi_write_only(conf, &cmd, 1) == 0)
        {
            /* Write address */
            if (cy15x102qn_spi_write_only(conf, adr_arr, 3) == 0)
            {
                /* Write dummy byte */
                if (cy15x102qn_spi_write_only(conf, &dummy, 1) == 0)
                {
                    /* Read data */
                    if (cy15x102qn_spi_read_only(conf, data, len) == 0)
                    {
                        if (cy15x102qn_spi_unselect(conf) == 0)
                        {
                            err = 0;
                        }
                    }
                    else
                    {
                    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                        sys_log_print_event_from_module(SYS_LOG_ERROR, CY15X102QN_MODULE_NAME, "Error reading the data during READ command!");
                        sys_log_new_line();
                    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                        if (cy15x102qn_spi_unselect(conf) != 0)
                        {
                            err = -2;
                        }
                    }
                }
                else
                {
                #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                    sys_log_print_event_from_module(SYS_LOG_ERROR, CY15X102QN_MODULE_NAME, "Error writing the dummy byte during FSTRD command!");
                    sys_log_new_line();
                #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                    if (cy15x102qn_spi_unselect(conf) != 0)
                    {
                        err = -2;
                    }
                }
            }
            else
            {
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_ERROR, CY15X102QN_MODULE_NAME, "Error writing the address during FSTRD command!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                if (cy15x102qn_spi_unselect(conf) != 0)
                {
                    err = -2;
                }
            }
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, CY15X102QN_MODULE_NAME, "Error writing the opcode during FSTRD command!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            if (cy15x102qn_spi_unselect(conf) != 0)
            {
                err = -2;
            }
        }
    }

    return err;
}

int cy15x102qn_special_sector_write(cy15x102qn_config_t *conf, uint8_t adr, uint8_t *data, uint16_t len)
{
    int err = -1;

    uint8_t cmd = CY15X102QN_OPCODE_SSWR;

    if (cy15x102qn_spi_select(conf) == 0)
    {
        if (cy15x102qn_set_write_enable(conf) == 0)
        {   
            /* Write opcode */
            if (cy15x102qn_spi_write_only(conf, &cmd, 1) == 0)
            {
                /* Write address */
                if (cy15x102qn_spi_write_only(conf, &adr, 1) == 0)
                {
                    /* Write data */
                    if (cy15x102qn_spi_write_only(conf, data, len) == 0)
                    {
                        if (cy15x102qn_spi_unselect(conf) == 0)
                        {
                            err = 0;
                        }
                    }
                    else
                    {
                    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                        sys_log_print_event_from_module(SYS_LOG_ERROR, CY15X102QN_MODULE_NAME, "Error writing the data during SSWR command!");
                        sys_log_new_line();
                    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                        if (cy15x102qn_spi_unselect(conf) != 0)
                        {
                            err = -2;
                        }
                    }
                }
                else
                {
                #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                    sys_log_print_event_from_module(SYS_LOG_ERROR, CY15X102QN_MODULE_NAME, "Error writing the address during SSWR command!");
                    sys_log_new_line();
                #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                    if (cy15x102qn_spi_unselect(conf) != 0)
                    {
                        err = -2;
                    }
                }
            }
            else
            {
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_ERROR, CY15X102QN_MODULE_NAME, "Error writing the opcode during SSWR command!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                if (cy15x102qn_spi_unselect(conf) != 0)
                {
                    err = -2;
                }
            }
        }
    }

    return err;
}

int cy15x102qn_special_sector_read(cy15x102qn_config_t *conf, uint8_t adr, uint8_t *data, uint16_t len)
{
    int err = -1;

    uint8_t cmd = CY15X102QN_OPCODE_SSRD;

    if (cy15x102qn_spi_select(conf) == 0)
    {
        /* Write opcode */
        if (cy15x102qn_spi_write_only(conf, &cmd, 1) == 0)
        {
            /* Write address */
            if (cy15x102qn_spi_write_only(conf, &adr, 1) == 0)
            {
                /* Read data */
                if (cy15x102qn_spi_read_only(conf, data, len) == 0)
                {
                    if (cy15x102qn_spi_unselect(conf) == 0)
                    {
                        err = 0;
                    }
                }
                else
                {
                #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                    sys_log_print_event_from_module(SYS_LOG_ERROR, CY15X102QN_MODULE_NAME, "Error reading the data during SSRD command!");
                    sys_log_new_line();
                #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                    if (cy15x102qn_spi_unselect(conf) != 0)
                    {
                        err = -2;
                    }
                }
            }
            else
            {
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_ERROR, CY15X102QN_MODULE_NAME, "Error writing the address during SSRD command!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                if (cy15x102qn_spi_unselect(conf) != 0)
                {
                    err = -2;
                }
            }
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, CY15X102QN_MODULE_NAME, "Error writing the opcode during SSRD command!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            if (cy15x102qn_spi_unselect(conf) != 0)
            {
                err = -2;
            }
        }
    }

    return err;
}

int cy15x102qn_read_device_id(cy15x102qn_config_t *conf, cy15x102qn_device_id_t *id)
{
    int err = -1;

    uint8_t cmd = CY15X102QN_OPCODE_RDID;
    uint8_t raw_id[9];

    if (cy15x102qn_spi_select(conf) == 0)
    {
        /* Write opcode */
        if (cy15x102qn_spi_write_only(conf, &cmd, 1) == 0)
        {
            /* Read data */
            if (cy15x102qn_spi_read_only(conf, raw_id, 9) == 0)
            {
                if (cy15x102qn_spi_unselect(conf) == 0)
                {
                    id->manufacturer_id = ((uint64_t)raw_id[8] << 48) |
                                          ((uint64_t)raw_id[7] << 40) |
                                          ((uint64_t)raw_id[6] << 32) |
                                          ((uint64_t)raw_id[5] << 24) |
                                          ((uint64_t)raw_id[4] << 16) |
                                          ((uint64_t)raw_id[3] << 8) |
                                          (uint64_t)raw_id[2];
                    id->family          = (raw_id[1] & 0xE0U) >> 5;
                    id->density         = (raw_id[1] & 0x1EU) >> 1;
                    id->inrush          = raw_id[1] & 0x01U;
                    id->sub_type        = (raw_id[0] & 0xE0U) >> 5;
                    id->revision        = (raw_id[0] & 0x18U) >> 3;
                    id->voltage         = (raw_id[0] & 0x04U) >> 2;
                    id->frequency       = raw_id[0] & 0x03U;

                    err = 0;
                }
            }
            else
            {
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_ERROR, CY15X102QN_MODULE_NAME, "Error reading the data during RDID command!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                if (cy15x102qn_spi_unselect(conf) != 0)
                {
                    err = -2;
                }
            }
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, CY15X102QN_MODULE_NAME, "Error writing the opcode during RDID command!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            if (cy15x102qn_spi_unselect(conf) != 0)
            {
                err = -2;
            }
        }
    }

    return err;
}

int cy15x102qn_read_unique_id(cy15x102qn_config_t *conf, cy15x102qn_uid_t *uid)
{
    int err = -1;

    uint8_t cmd = CY15X102QN_OPCODE_RUID;
    uint8_t raw_uid[8];

    if (cy15x102qn_spi_select(conf) == 0)
    {
        /* Write opcode */
        if (cy15x102qn_spi_write_only(conf, &cmd, 1) == 0)
        {
            /* Read data */
            if (cy15x102qn_spi_read_only(conf, raw_uid, 8) == 0)
            {
                if (cy15x102qn_spi_unselect(conf) == 0)
                {
                    *uid = ((uint64_t)raw_uid[7] << 56) |
                           ((uint64_t)raw_uid[6] << 48) |
                           ((uint64_t)raw_uid[5] << 40) |
                           ((uint64_t)raw_uid[4] << 32) |
                           ((uint64_t)raw_uid[3] << 24) |
                           ((uint64_t)raw_uid[2] << 16) |
                           ((uint64_t)raw_uid[1] << 8) |
                           (uint64_t)raw_uid[0];

                    err = 0;
                }
            }
            else
            {
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_ERROR, CY15X102QN_MODULE_NAME, "Error reading the data during RUID command!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                if (cy15x102qn_spi_unselect(conf) != 0)
                {
                    err = -2;
                }
            }
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, CY15X102QN_MODULE_NAME, "Error writing the opcode during RUID command!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            if (cy15x102qn_spi_unselect(conf) != 0)
            {
                err = -2;
            }
        }
    }

    return err;
}

int cy15x102qn_write_serial_number(cy15x102qn_config_t *conf, cy15x102qn_serial_number_t s_num)
{
    int err = -1;

    uint8_t cmd = CY15X102QN_OPCODE_WRSN;
    uint8_t s_num_arr[8];

    s_num_arr[0] = s_num.customer_id >> 8;
    s_num_arr[1] = s_num.customer_id & 0xFFU;
    s_num_arr[2] = (s_num.unique_number >> 32) & 0xFFU;
    s_num_arr[3] = (s_num.unique_number >> 24) & 0xFFU;
    s_num_arr[4] = (s_num.unique_number >> 16) & 0xFFU;
    s_num_arr[5] = (s_num.unique_number >> 8) & 0xFFU;
    s_num_arr[6] = s_num.unique_number & 0xFFU;
    s_num_arr[7] = s_num.crc;

    if (cy15x102qn_spi_select(conf) == 0)
    {
        if (cy15x102qn_set_write_enable(conf) == 0)
        {
            /* Write opcode */
            if (cy15x102qn_spi_write_only(conf, &cmd, 1) == 0)
            {
                /* Write data */
                if (cy15x102qn_spi_write_only(conf, s_num_arr, 8) == 0)
                {
                    if (cy15x102qn_spi_unselect(conf) == 0)
                    {
                        err = 0;
                    }
                }
                else
                {
                #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                    sys_log_print_event_from_module(SYS_LOG_ERROR, CY15X102QN_MODULE_NAME, "Error writing the data during WRSN command!");
                    sys_log_new_line();
                #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                    if (cy15x102qn_spi_unselect(conf) != 0)
                    {
                        err = -2;
                    }
                }
            }
            else
            {
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_ERROR, CY15X102QN_MODULE_NAME, "Error writing the opcode during WRSN command!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                if (cy15x102qn_spi_unselect(conf) != 0)
                {
                    err = -2;
                }
            }
        }
    }

    return err;
}

int cy15x102qn_read_serial_number(cy15x102qn_config_t *conf, cy15x102qn_serial_number_t *s_num)
{
    int err = -1;

    uint8_t cmd = CY15X102QN_OPCODE_SNR;
    uint8_t raw_snr[8];

    if (cy15x102qn_spi_select(conf) == 0)
    {
        /* Write opcode */
        if (cy15x102qn_spi_write_only(conf, &cmd, 1) == 0)
        {
            /* Read data */
            if (cy15x102qn_spi_read_only(conf, raw_snr, 8) == 0)
            {
                if (cy15x102qn_spi_unselect(conf) == 0)
                {
                    s_num->customer_id      = ((uint16_t)raw_snr[0] << 8) | (uint16_t)raw_snr[1];
                    s_num->unique_number    = ((uint64_t)raw_snr[2] << 32) | ((uint64_t)raw_snr[3] << 24) |
                                              ((uint64_t)raw_snr[4] << 16) | ((uint64_t)raw_snr[5] << 8) |
                                              (uint64_t)raw_snr[6];
                    s_num->crc              = raw_snr[7];

                    err = 0;
                }
            }
            else
            {
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_ERROR, CY15X102QN_MODULE_NAME, "Error reading the data during SNR command!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                if (cy15x102qn_spi_unselect(conf) != 0)
                {
                    err = -2;
                }
            }
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, CY15X102QN_MODULE_NAME, "Error writing the opcode during SNR command!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            if (cy15x102qn_spi_unselect(conf) != 0)
            {
                err = -2;
            }
        }
    }

    return err;
}

int cy15x102qn_deep_power_down_mode(cy15x102qn_config_t *conf)
{
    int err = 0;

    uint8_t cmd = CY15X102QN_OPCODE_DPD;

    if (cy15x102qn_spi_write(conf, &cmd, 1) != 0)
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, CY15X102QN_MODULE_NAME, "Error writing the DPD command!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err = -1;
    }

    return err;
}

int cy15x102qn_hibernate_mode(cy15x102qn_config_t *conf)
{
    int err = 0;

    uint8_t cmd = CY15X102QN_OPCODE_HBN;

    if (cy15x102qn_spi_write(conf, &cmd, 1) != 0)
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, CY15X102QN_MODULE_NAME, "Error writing the HBN command!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err = -1;
    }

    return err;
}

/** \} End of cy15x102qn group */
