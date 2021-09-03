/*
 * cy15x102qn_wrap.c
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
 * \brief CY15x102QN driver wrap implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.7
 * 
 * \date 2021/08/08
 * 
 * \addtogroup cy15x102qn_wrap
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include "cy15x102qn_wrap.h"

int __wrap_cy15x102qn_init(cy15x102qn_config_t *conf)
{
    check_expected(conf->port);
    check_expected(conf->cs_pin);
    check_expected(conf->clock_hz);
    check_expected(conf->wp_pin);

    return mock_type(int);
}

int __wrap_cy15x102qn_set_write_enable(cy15x102qn_config_t *conf)
{
    check_expected(conf->port);
    check_expected(conf->cs_pin);
    check_expected(conf->clock_hz);
    check_expected(conf->wp_pin);

    return mock_type(int);
}

int __wrap_cy15x102qn_reset_write_enable(cy15x102qn_config_t *conf)
{
    check_expected(conf->port);
    check_expected(conf->cs_pin);
    check_expected(conf->clock_hz);
    check_expected(conf->wp_pin);

    return mock_type(int);
}

int __wrap_cy15x102qn_read_status_reg(cy15x102qn_config_t *conf, cy15x102qn_status_t *status)
{
    check_expected(conf->port);
    check_expected(conf->cs_pin);
    check_expected(conf->clock_hz);
    check_expected(conf->wp_pin);

    if (status != NULL)
    {
        *status = mock_type(cy15x102qn_status_t);
    }

    return mock_type(int);
}

int __wrap_cy15x102qn_write_status_reg(cy15x102qn_config_t *conf, cy15x102qn_status_t status)
{
    check_expected(conf->port);
    check_expected(conf->cs_pin);
    check_expected(conf->clock_hz);
    check_expected(conf->wp_pin);

    check_expected(status);

    return mock_type(int);
}

int __wrap_cy15x102qn_write(cy15x102qn_config_t *conf, uint32_t adr, uint8_t *data, uint32_t len)
{
    check_expected(conf->port);
    check_expected(conf->cs_pin);
    check_expected(conf->clock_hz);
    check_expected(conf->wp_pin);

    check_expected(adr);
    check_expected_ptr(data);
    check_expected(len);

    return mock_type(int);
}

int __wrap_cy15x102qn_read(cy15x102qn_config_t *conf, uint32_t adr, uint8_t *data, uint32_t len)
{
    check_expected(conf->port);
    check_expected(conf->cs_pin);
    check_expected(conf->clock_hz);
    check_expected(conf->wp_pin);

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

int __wrap_cy15x102qn_fast_read(cy15x102qn_config_t *conf, uint32_t adr, uint8_t *data, uint32_t len)
{
    check_expected(conf->port);
    check_expected(conf->cs_pin);
    check_expected(conf->clock_hz);
    check_expected(conf->wp_pin);

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

int __wrap_cy15x102qn_special_sector_write(cy15x102qn_config_t *conf, uint8_t adr, uint8_t *data, uint16_t len)
{
    check_expected(conf->port);
    check_expected(conf->cs_pin);
    check_expected(conf->clock_hz);
    check_expected(conf->wp_pin);

    check_expected(adr);
    check_expected_ptr(data);
    check_expected(len);

    return mock_type(int);
}

int __wrap_cy15x102qn_special_sector_read(cy15x102qn_config_t *conf, uint8_t adr, uint8_t *data, uint16_t len)
{
    check_expected(conf->port);
    check_expected(conf->cs_pin);
    check_expected(conf->clock_hz);
    check_expected(conf->wp_pin);

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

int __wrap_cy15x102qn_read_device_id(cy15x102qn_config_t *conf, cy15x102qn_device_id_t *id)
{
    check_expected(conf->port);
    check_expected(conf->cs_pin);
    check_expected(conf->clock_hz);
    check_expected(conf->wp_pin);

    check_expected(id->manufacturer_id);
    check_expected(id->family);
    check_expected(id->density);
    check_expected(id->inrush);
    check_expected(id->sub_type);
    check_expected(id->revision);
    check_expected(id->voltage);
    check_expected(id->frequency);

    return mock_type(int);
}

int __wrap_cy15x102qn_read_unique_id(cy15x102qn_config_t *conf, cy15x102qn_uid_t *uid)
{
    check_expected(conf->port);
    check_expected(conf->cs_pin);
    check_expected(conf->clock_hz);
    check_expected(conf->wp_pin);

    if (uid != NULL)
    {
        *uid = mock_type(cy15x102qn_uid_t);
    }

    return mock_type(int);
}

int __wrap_cy15x102qn_write_serial_number(cy15x102qn_config_t *conf, cy15x102qn_serial_number_t s_num)
{
    check_expected(conf->port);
    check_expected(conf->cs_pin);
    check_expected(conf->clock_hz);
    check_expected(conf->wp_pin);

    check_expected(s_num.customer_id);
    check_expected(s_num.unique_number);
    check_expected(s_num.crc);

    return mock_type(int);
}

int __wrap_cy15x102qn_read_serial_number(cy15x102qn_config_t *conf, cy15x102qn_serial_number_t *s_num)
{
    check_expected(conf->port);
    check_expected(conf->cs_pin);
    check_expected(conf->clock_hz);
    check_expected(conf->wp_pin);

    if (s_num != NULL)
    {
        s_num->customer_id = mock_type(uint16_t);
        s_num->unique_number = mock_type(uint64_t);
        s_num->crc = mock_type(uint8_t);
    }

    return mock_type(int);
}

int __wrap_cy15x102qn_deep_power_down_mode(cy15x102qn_config_t *conf)
{
    check_expected(conf->port);
    check_expected(conf->cs_pin);
    check_expected(conf->clock_hz);
    check_expected(conf->wp_pin);

    return mock_type(int);
}

int __wrap_cy15x102qn_hibernate_mode(cy15x102qn_config_t *conf)
{
    check_expected(conf->port);
    check_expected(conf->cs_pin);
    check_expected(conf->clock_hz);
    check_expected(conf->wp_pin);

    return mock_type(int);
}

int __wrap_cy15x102qn_spi_init(cy15x102qn_config_t *conf)
{
    check_expected(conf->port);
    check_expected(conf->cs_pin);
    check_expected(conf->clock_hz);
    check_expected(conf->wp_pin);

    return mock_type(int);
}

int __wrap_cy15x102qn_spi_write(cy15x102qn_config_t *conf, uint8_t *data, uint16_t len)
{
    check_expected(conf->port);
    check_expected(conf->cs_pin);
    check_expected(conf->clock_hz);
    check_expected(conf->wp_pin);

    check_expected_ptr(data);
    check_expected(len);

    return mock_type(int);
}

int __wrap_cy15x102qn_spi_read(cy15x102qn_config_t *conf, uint8_t *data, uint16_t len)
{
    check_expected(conf->port);
    check_expected(conf->cs_pin);
    check_expected(conf->clock_hz);
    check_expected(conf->wp_pin);

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

int __wrap_cy15x102qn_spi_transfer(cy15x102qn_config_t *conf, uint8_t *wdata, uint8_t *rdata, uint16_t len)
{
    check_expected(conf->port);
    check_expected(conf->cs_pin);
    check_expected(conf->clock_hz);
    check_expected(conf->wp_pin);

    check_expected_ptr(wdata);

    if (rdata != NULL)
    {
        uint16_t i = 0;
        for(i=0; i<len; i++)
        {
            rdata[i] = mock_type(uint8_t);
        }
    }

    check_expected(len);

    return mock_type(int);
}

int __wrap_cy15x102qn_spi_select(cy15x102qn_config_t *conf)
{
    check_expected(conf->port);
    check_expected(conf->cs_pin);
    check_expected(conf->clock_hz);
    check_expected(conf->wp_pin);

    return mock_type(int);
}

int __wrap_cy15x102qn_spi_unselect(cy15x102qn_config_t *conf)
{
    check_expected(conf->port);
    check_expected(conf->cs_pin);
    check_expected(conf->clock_hz);
    check_expected(conf->wp_pin);

    return mock_type(int);
}

int __wrap_cy15x102qn_spi_write_only(cy15x102qn_config_t *conf, uint8_t *data, uint16_t len)
{
    check_expected(conf->port);
    check_expected(conf->cs_pin);
    check_expected(conf->clock_hz);
    check_expected(conf->wp_pin);

    check_expected_ptr(data);
    check_expected(len);

    return mock_type(int);
}

int __wrap_cy15x102qn_spi_read_only(cy15x102qn_config_t *conf, uint8_t *data, uint16_t len)
{
    check_expected(conf->port);
    check_expected(conf->cs_pin);
    check_expected(conf->clock_hz);
    check_expected(conf->wp_pin);

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

int __wrap_cy15x102qn_spi_transfer_only(cy15x102qn_config_t *conf, uint8_t *wdata, uint8_t *rdata, uint16_t len)
{
    check_expected(conf->port);
    check_expected(conf->cs_pin);
    check_expected(conf->clock_hz);
    check_expected(conf->wp_pin);

    check_expected_ptr(wdata);

    if (rdata != NULL)
    {
        uint16_t i = 0;
        for(i=0; i<len; i++)
        {
            rdata[i] = mock_type(uint8_t);
        }
    }

    check_expected(len);

    return mock_type(int);
}

int __wrap_cy15x102qn_gpio_init(cy15x102qn_config_t *conf)
{
    check_expected(conf->port);
    check_expected(conf->cs_pin);
    check_expected(conf->clock_hz);
    check_expected(conf->wp_pin);

    return mock_type(int);
}

int __wrap_cy15x102qn_gpio_set_write_protect(cy15x102qn_config_t *conf)
{
    check_expected(conf->port);
    check_expected(conf->cs_pin);
    check_expected(conf->clock_hz);
    check_expected(conf->wp_pin);

    return mock_type(int);
}

int __wrap_cy15x102qn_gpio_clear_write_protect(cy15x102qn_config_t *conf)
{
    check_expected(conf->port);
    check_expected(conf->cs_pin);
    check_expected(conf->clock_hz);
    check_expected(conf->wp_pin);

    return mock_type(int);
}

/** \} End of cy15x102qn_wrap group */
