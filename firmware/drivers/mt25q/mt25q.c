/*
 * mt25q.c
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
 * \brief MT25Q driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.5.31
 * 
 * \date 2019/11/15
 * 
 * \addtogroup mt25q
 * \{
 */

#include "mt25q.h"
#include "mt25q_reg.h"

int mt25q_init(void)
{
    if (mt25q_spi_init() != 0)
    {
        return -1;
    }

    if (mt25q_gpio_init() != 0)
    {
        return -1;
    }

    mt25q_delay_ms(10);

    return 0;
}

int mt25q_read_device_id(mt25q_dev_id_t *dev_id)
{
    uint8_t cmd[4] = {MT25Q_READ_ID_REG, 0, 0, 0};
    uint8_t ans[4] = {0};

    if (mt25q_spi_transfer(cmd, ans, 4) != 0)
    {
        return -1;
    }

    dev_id->manufacturer_id  = ans[1];
    dev_id->memory_type      = ans[2];
    dev_id->memory_capacity  = ans[3];

    return 0;
}

int mt25q_write(uint32_t adr, uint8_t *data, uint32_t len)
{
    return -1;
}

int mt25q_read(uint32_t adr, uint8_t *data, uint32_t len)
{
    return -1;
}

/** \} End of mt25q group */
