/*
 * sl_eps2.c
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
 * \brief SpaceLab EPS 2.0 driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.16
 * 
 * \date 05/02/2020
 * 
 * \addtogroup sl_eps2
 * \{
 */

#include <drivers/tca4311a/tca4311a.h>
#include <drivers/i2c/i2c.h>
#include <drivers/gpio/gpio.h>

#include "sl_eps2.h"

tca4311a_config_t sl_eps2_config;

int sl_eps2_init()
{
    sl_eps2_config.i2c_port     = I2C_PORT_1;
    sl_eps2_config.i2c_config   = (i2c_config_t){.speed_hz=100000};
    sl_eps2_config.en_pin       = GPIO_PIN_17;
    sl_eps2_config.ready_pin    = GPIO_PIN_20;

    if (tca4311a_init(sl_eps2_config, true) != TCA4311A_READY)
    {
        return -1;      // Error initializing the I2C port
    }

    if (sl_eps2_check_device() != 0)
    {
        return -1;      // Error checking the connection
    }

    return 0;
}

int sl_eps2_check_device()
{
    uint8_t buf = SL_EPS2_REG_DEVICE_ID;

    if (tca4311a_write(sl_eps2_config, SL_EPS2_SLAVE_ADR, &buf, 1) != TCA4311A_READY)
    {
        return -1;      // Error writing the command
    }

    if (tca4311a_read(sl_eps2_config, SL_EPS2_SLAVE_ADR, &buf, 1) != TCA4311A_READY)
    {
        return -1;      // Error reading the command result
    }

    if (buf != SL_EPS2_DEVICE_ID)
    {
        return -1;      // Wrong device ID (connection error?)
    }

    return 0;
}

int sl_eps2_write_reg(uint8_t adr, uint32_t val)
{
    uint8_t buf[5];

    buf[0] = adr;
    buf[1] = (val >> 24) & 0xFF;
    buf[2] = (val >> 16) & 0xFF;
    buf[3] = (val >> 8)  & 0xFF;
    buf[4] = (val >> 0)  & 0xFF;

    if (tca4311a_write(sl_eps2_config, SL_EPS2_SLAVE_ADR, buf, 5) != TCA4311A_READY)
    {
        return -1;
    }

    return 0;
}

int sl_eps2_read_reg(uint8_t adr, uint32_t *val)
{
    if (tca4311a_write_byte(sl_eps2_config, SL_EPS2_SLAVE_ADR, adr) != TCA4311A_READY)
    {
        return -1;
    }

    uint8_t buf[4];

    if (tca4311a_read(sl_eps2_config, SL_EPS2_SLAVE_ADR, buf, 4) != TCA4311A_READY)
    {
        return -1;
    }

    *val = ((uint32_t)buf[0] << 24) |
           ((uint32_t)buf[1] << 16) |
           ((uint32_t)buf[2] << 8)  |
           ((uint32_t)buf[3] << 0);

    return 0;
}

//! \} End of sl_eps2 group
