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
 * \version 0.7.2
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
    return 0;
}

int __wrap_mt25q_reset(void)
{
    return 0;
}

int __wrap_mt25q_read_device_id(mt25q_dev_id_t *dev_id)
{
    return 0;
}

int __wrap_mt25q_read_flash_description(flash_description_t *fdo)
{
    return 0;
}

int __wrap_mt25q_clear_flag_status_register(void)
{
    return 0;
}

int __wrap_mt25q_read_status(mt25q_status_t *status)
{
    return 0;
}

int __wrap_mt25q_enter_deep_power_down(void)
{
    return 0;
}

int __wrap_mt25q_release_from_deep_power_down(void)
{
    return 0;
}

int __wrap_mt25q_write_enable(void)
{
    return 0;
}

int __wrap_mt25q_write_disable(void)
{
    return 0;
}

bool __wrap_mt25q_is_busy(void)
{
    return true;
}

int __wrap_mt25q_die_erase(mt25q_sector_t die)
{
    return 0;
}

int __wrap_mt25q_sector_erase(mt25q_sector_t sector)
{
    return 0;
}

int __wrap_mt25q_sub_sector_erase(mt25q_sector_t sub)
{
    return 0;
}

int __wrap_mt25q_write(uint32_t adr, uint8_t *data, uint16_t len)
{
    return 0;
}

int __wrap_mt25q_read(uint32_t adr, uint8_t *data, uint16_t len)
{
    return 0;
}

uint32_t __wrap_mt25q_get_max_address(void)
{
    return 0;
}

int __wrap_mt25q_enter_4_byte_address_mode(void)
{
    return 0;
}

int __wrap_mt25q_read_flag_status_register(uint8_t *flag)
{
    return 0;
}

flash_description_t __wrap_mt25q_get_flash_description(void)
{
    flash_description_t fdo = {0};

    return fdo;
}

int __wrap_mt25q_spi_init(void)
{
    return 0;
}

int __wrap_mt25q_spi_write(uint8_t *data, uint16_t len)
{
    return 0;
}

int __wrap_mt25q_spi_read(uint8_t *data, uint16_t len)
{
    return 0;
}

int __wrap_mt25q_spi_transfer(uint8_t *wdata, uint8_t *rdata, uint16_t len)
{
    return 0;
}

int __wrap_mt25q_spi_select(void)
{
    return 0;
}

int __wrap_mt25q_spi_unselect(void)
{
    return 0;
}

int __wrap_mt25q_spi_write_only(uint8_t *data, uint16_t len)
{
    return 0;
}

int __wrap_mt25q_spi_read_only(uint8_t *data, uint16_t len)
{
    return 0;
}

int __wrap_mt25q_spi_transfer_only(uint8_t *wdata, uint8_t *rdata, uint16_t len)
{
    return 0;
}

int __wrap_mt25q_gpio_init(void)
{
    return 0;
}

int __wrap_mt25q_gpio_set_hold(bool state)
{
    return 0;
}

int __wrap_mt25q_gpio_set_reset(bool state)
{
    return 0;
}

void __wrap_mt25q_delay_ms(uint32_t ms)
{
    return;
}


/** \} End of mt25q_wrap group */
