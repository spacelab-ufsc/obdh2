/*
 * tca4311a.c
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
 * \brief TCA4311A driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.10
 * 
 * \date 01/02/2020
 * 
 * \addtogroup tca4311a
 * \{
 */

#include "tca4311a.h"

int tca4311a_init(tca4311a_config_t config, bool en)
{
    return TCA4311A_ERROR;
}

int tca4311a_enable(tca4311a_config_t config)
{
    return TCA4311A_ERROR;
}

int tca4311a_disable(tca4311a_config_t config)
{
    return TCA4311A_ERROR;
}

int tca4311a_ready(tca4311a_config_t config)
{
    return TCA4311A_ERROR;
}

int tca4311a_write(tca4311a_config_t config, i2c_slave_adr_t adr, uint8_t *data, uint16_t len)
{
    return TCA4311A_ERROR;
}

int tca4311a_read(tca4311a_config_t config, i2c_slave_adr_t adr, uint8_t *data, uint16_t len)
{
    return TCA4311A_ERROR;
}

int tca4311a_write_byte(tca4311a_config_t config, i2c_slave_adr_t adr, uint8_t byte)
{
    return TCA4311A_ERROR;
}

int tca4311a_read_byte(tca4311a_config_t config, i2c_slave_adr_t adr, uint8_t *byte)
{
    return TCA4311A_ERROR;
}

//! \} End of tca4311a group
