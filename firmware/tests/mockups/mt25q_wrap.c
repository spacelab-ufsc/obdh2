/*
 * mt25q_wrap.c
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
 * \brief MT25Q driver wrap implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.7
 * 
 * \date 2021/08/08
 * 
 * \addtogroup mt25q_wrap
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include "mt25q_wrap.h"

int __wrap_mt25q_init(void)
{
    return mock_type(int);
}

int __wrap_mt25q_reset(void)
{
    return mock_type(int);
}

int __wrap_mt25q_read_device_id(mt25q_dev_id_t *dev_id)
{
    if (dev_id != NULL)
    {
        dev_id->manufacturer_id = mock_type(uint8_t);
        dev_id->memory_type     = mock_type(uint8_t);
        dev_id->memory_capacity = mock_type(uint8_t);
    }

    return mock_type(int);
}

int __wrap_mt25q_read_flash_description(flash_description_t *fdo)
{
    if (fdo != NULL)
    {
        fdo->id                     = mock_type(uint32_t);
        fdo->type                   = mock_type(uint32_t);
        fdo->starting_address       = mock_type(uint32_t);
        fdo->address_mask           = mock_type(uint32_t);
        fdo->size                   = mock_type(uint32_t);
        fdo->otp_size               = mock_type(uint32_t);
        fdo->die_count              = mock_type(uint32_t);
        fdo->die_size               = mock_type(uint32_t);
        fdo->die_size_bit           = mock_type(uint32_t);
        fdo->sector_size            = mock_type(uint32_t);
        fdo->sector_size_bit        = mock_type(uint32_t);
        fdo->sector_count           = mock_type(uint32_t);
        fdo->sector_erase_cmd       = mock_type(uint32_t);
        fdo->sub_sector_size        = mock_type(uint32_t);
        fdo->sub_sector_size_bit    = mock_type(uint32_t);
        fdo->sub_sector_count       = mock_type(uint32_t);
        fdo->sub_sector_erase_cmd   = mock_type(uint32_t);
        fdo->page_size              = mock_type(uint32_t);
        fdo->page_count             = mock_type(uint32_t);
        fdo->buffer_size            = mock_type(uint32_t);
        fdo->data_width             = mock_type(uint32_t);
        fdo->num_adr_byte           = mock_type(uint8_t);
    }

    return mock_type(int);
}

int __wrap_mt25q_clear_flag_status_register(void)
{
    return mock_type(int);
}

int __wrap_mt25q_read_status(mt25q_status_t *status)
{
    check_expected(status->write_enable);
    check_expected(status->top_bottom);
    check_expected(status->bp);
    check_expected(status->write_enable_latch);
    check_expected(status->write_in_progress);

    return mock_type(int);
}

int __wrap_mt25q_enter_deep_power_down(void)
{
    return mock_type(int);
}

int __wrap_mt25q_release_from_deep_power_down(void)
{
    return mock_type(int);
}

int __wrap_mt25q_write_enable(void)
{
    return mock_type(int);
}

int __wrap_mt25q_write_disable(void)
{
    return mock_type(int);
}

bool __wrap_mt25q_is_busy(void)
{
    return mock_type(bool);
}

int __wrap_mt25q_die_erase(mt25q_sector_t die)
{
    check_expected(die);

    return mock_type(int);
}

int __wrap_mt25q_sector_erase(mt25q_sector_t sector)
{
    check_expected(sector);

    return mock_type(int);
}

int __wrap_mt25q_sub_sector_erase(mt25q_sector_t sub)
{
    check_expected(sub);

    return mock_type(int);
}

int __wrap_mt25q_write(uint32_t adr, uint8_t *data, uint16_t len)
{
    check_expected(adr);
    check_expected_ptr(data);
    check_expected(len);

    return mock_type(int);
}

int __wrap_mt25q_read(uint32_t adr, uint8_t *data, uint16_t len)
{
    check_expected(adr);

    if (data != NULL)
    {
        uint16_t i = 0;
        for(i=0; i<len; i++)
        {
            data[i] = mock_type(uint8_t);
        }
    }

    check_expected(len);

    return mock_type(int);
}

uint32_t __wrap_mt25q_get_max_address(void)
{
    return mock_type(int);
}

int __wrap_mt25q_enter_4_byte_address_mode(void)
{
    return mock_type(int);
}

int __wrap_mt25q_read_flag_status_register(uint8_t *flag)
{
    if (flag != NULL)
    {
        *flag = mock_type(uint8_t);
    }

    return mock_type(int);
}

flash_description_t __wrap_mt25q_get_flash_description(void)
{
    flash_description_t fdo = {0};

    fdo.id                      = mock_type(uint32_t);
    fdo.type                    = mock_type(uint32_t);
    fdo.starting_address        = mock_type(uint32_t);
    fdo.address_mask            = mock_type(uint32_t);
    fdo.size                    = mock_type(uint32_t);
    fdo.otp_size                = mock_type(uint32_t);
    fdo.die_count               = mock_type(uint32_t);
    fdo.die_size                = mock_type(uint32_t);
    fdo.die_size_bit            = mock_type(uint32_t);
    fdo.sector_size             = mock_type(uint32_t);
    fdo.sector_size_bit         = mock_type(uint32_t);
    fdo.sector_count            = mock_type(uint32_t);
    fdo.sector_erase_cmd        = mock_type(uint32_t);
    fdo.sub_sector_size         = mock_type(uint32_t);
    fdo.sub_sector_size_bit     = mock_type(uint32_t);
    fdo.sub_sector_count        = mock_type(uint32_t);
    fdo.sub_sector_erase_cmd    = mock_type(uint32_t);
    fdo.page_size               = mock_type(uint32_t);
    fdo.page_count              = mock_type(uint32_t);
    fdo.buffer_size             = mock_type(uint32_t);
    fdo.data_width              = mock_type(uint32_t);
    fdo.num_adr_byte            = mock_type(uint8_t);

    return fdo;
}

int __wrap_mt25q_spi_init(void)
{
    return mock_type(int);
}

int __wrap_mt25q_spi_write(uint8_t *data, uint16_t len)
{
    check_expected_ptr(data);
    check_expected(len);

    return mock_type(int);
}

int __wrap_mt25q_spi_read(uint8_t *data, uint16_t len)
{
    if (data != NULL)
    {
        data = mock_ptr_type(uint8_t*);
    }

    check_expected(len);

    return mock_type(int);
}

int __wrap_mt25q_spi_transfer(uint8_t *wdata, uint8_t *rdata, uint16_t len)
{
    check_expected_ptr(wdata);

    if (rdata != NULL)
    {
        rdata = mock_ptr_type(uint8_t*);
    }

    check_expected(len);

    return mock_type(int);
}

int __wrap_mt25q_spi_select(void)
{
    return mock_type(int);
}

int __wrap_mt25q_spi_unselect(void)
{
    return mock_type(int);
}

int __wrap_mt25q_spi_write_only(uint8_t *data, uint16_t len)
{
    check_expected_ptr(data);
    check_expected(len);

    return mock_type(int);
}

int __wrap_mt25q_spi_read_only(uint8_t *data, uint16_t len)
{
    if (data != NULL)
    {
        data = mock_ptr_type(uint8_t*);
    }

    check_expected(len);

    return mock_type(int);
}

int __wrap_mt25q_spi_transfer_only(uint8_t *wdata, uint8_t *rdata, uint16_t len)
{
    check_expected_ptr(wdata);

    if (rdata != NULL)
    {
        rdata = mock_ptr_type(uint8_t*);
    }

    check_expected(len);

    return mock_type(int);
}

int __wrap_mt25q_gpio_init(void)
{
    return mock_type(int);
}

int __wrap_mt25q_gpio_set_hold(bool state)
{
    check_expected(state);

    return mock_type(int);
}

int __wrap_mt25q_gpio_set_reset(bool state)
{
    check_expected(state);

    return mock_type(int);
}

void __wrap_mt25q_delay_ms(uint32_t ms)
{
    function_called();

    return;
}


/** \} End of mt25q_wrap group */
