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
 * along with OBDH 2.0. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief CY15x102QN driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.1
 * 
 * \date 2021/06/04
 * 
 * \addtogroup cy15x102qn
 * \{
 */

#include <string.h>

#include "cy15x102qn.h"

#define CY15X102QN_DUMMY_BYTE   0x00

int cy15x102qn_init(void)
{
    return -1;
}

int cy15x102qn_set_write_enable(void)
{
    return -1;
}

int cy15x102qn_reset_write_enable(void)
{
    return -1;
}

int cy15x102qn_write_status_reg(cy15x102qn_status_t status)
{
    return -1;
}

int cy15x102qn_write(uint32_t adr, uint8_t *data, uint32_t len)
{
    return -1;
}

int cy15x102qn_read(uint32_t adr, uint8_t *data, uint32_t len)
{
    return -1;
}

int cy15x102qn_fast_read(uint32_t adr, uint8_t *data, uint32_t len)
{
    return -1;
}

int cy15x102qn_special_sector_write(uint32_t adr, uint8_t *data, uint16_t len)
{
    return -1;
}

int cy15x102qn_special_sector_read(uint32_t adr, uint8_t *data, uint16_t len)
{
    return -1;
}

int cy15x102qn_read_device_id(cy15x102qn_device_id_t *id)
{
    return -1;
}

int cy15x102qn_read_unique_id(cy15x102qn_uid_t *uid)
{
    return -1;
}

int cy15x102qn_write_serial_number(cy15x102qn_serial_number_t s_num)
{
    return -1;
}

int cy15x102qn_read_serial_number(cy15x102qn_serial_number_t *s_num)
{
    return -1;
}

int cy15x102qn_deep_power_down_mode(void)
{
    return -1;
}

int cy15x102qn_hibernate_mode(void)
{
    return -1;
}

/** \} End of cy15x102qn group */
