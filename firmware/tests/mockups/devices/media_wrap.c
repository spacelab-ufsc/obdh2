/*
 * media_wrap.c
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
 * \brief Media device wrap implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.2
 * 
 * \date 2021/08/15
 * 
 * \addtogroup media_wrap
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include "media_wrap.h"

int __wrap_media_init(media_t med)
{
    check_expected(med);

    return mock_type(int);
}

int __wrap_media_write(media_t med, uint32_t adr, uint8_t *data, uint16_t len)
{
    check_expected(med);
    check_expected(adr);
    check_expected_ptr(data);
    check_expected(len);

    return mock_type(int);
}

int __wrap_media_read(media_t med, uint32_t adr, uint8_t *data, uint16_t len)
{
    check_expected(med);
    check_expected(adr);
    check_expected_ptr(data);
    check_expected(len);

    return mock_type(int);
}

int __wrap_media_erase(media_t med, media_erase_t type, uint32_t sector)
{
    check_expected(med);
    check_expected(type);
    check_expected(sector);

    return mock_type(int);
}

media_info_t __wrap_media_get_info(media_t med)
{
    check_expected(med);

    media_info_t info = {0};

    info.id                     = mock_type(uint32_t);
    info.type                   = mock_type(uint32_t);
    info.starting_address       = mock_type(uint32_t);
    info.address_mask           = mock_type(uint32_t);
    info.size                   = mock_type(uint32_t);
    info.otp_size               = mock_type(uint32_t);
    info.die_count              = mock_type(uint32_t);
    info.die_size               = mock_type(uint32_t);
    info.die_size_bit           = mock_type(uint32_t);
    info.sector_size            = mock_type(uint32_t);
    info.sector_size_bit        = mock_type(uint32_t);
    info.sector_count           = mock_type(uint32_t);
    info.sector_erase_cmd       = mock_type(uint32_t);
    info.sub_sector_size        = mock_type(uint32_t);
    info.sub_sector_size_bit    = mock_type(uint32_t);
    info.sub_sector_count       = mock_type(uint32_t);
    info.sub_sector_erase_cmd   = mock_type(uint32_t);
    info.page_size              = mock_type(uint32_t);
    info.page_count             = mock_type(uint32_t);
    info.buffer_size            = mock_type(uint32_t);
    info.data_width             = mock_type(uint32_t);
    info.num_adr_byte           = mock_type(uint8_t);

    return info;
}

/** \} End of media_wrap group */
