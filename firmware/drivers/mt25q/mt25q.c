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
 * \version 0.6.0
 * 
 * \date 2019/11/15
 * 
 * \addtogroup mt25q
 * \{
 */

#include "mt25q.h"
#include "mt25q_reg.h"

#define MT25Q_DUMMY_BYTE        0x00

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
    uint8_t cmd[4] = {MT25Q_READ_ID_REG, MT25Q_DUMMY_BYTE, MT25Q_DUMMY_BYTE, MT25Q_DUMMY_BYTE};
    uint8_t ans[4] = {MT25Q_DUMMY_BYTE};

    if (mt25q_spi_transfer(cmd, ans, 4) != 0)
    {
        return -1;
    }

    dev_id->manufacturer_id  = ans[1];
    dev_id->memory_type      = ans[2];
    dev_id->memory_capacity  = ans[3];

    return 0;
}

int mt25q_read_status(mt25q_status_t *status)
{
    uint8_t cmd[2] = {MT25Q_READ_STATUS_REGISTER, MT25Q_DUMMY_BYTE};
    uint8_t ans[2] = {MT25Q_DUMMY_BYTE};

    if (mt25q_spi_transfer(cmd, ans, 2) != 0)
    {
        return -1;
    }

    status->write_enable        = (ans[1] & MT25Q_REG_STATUS_WRITE_ENABLE) >> 7;
    status->top_bottom          = (ans[1] & MT25Q_REG_STATUS_TOP_BOTTOM) >> 5;
    status->bp                  = (ans[1] & MT25Q_REG_STATUS_BP) >> 2;
    status->bp                  = (((status->bp >> 1) & 0x08) | status->bp) & 0x0F;
    status->write_enable_latch  = (ans[1] & MT25Q_REG_STATUS_WRITE_ENABLE_LATCH) >> 1;
    status->write_in_progress   = (ans[1] & MT25Q_REG_STATUS_WRITE_IN_PROGRESS);

    return 0;
}

int mt25q_reset(void)
{
    uint8_t cmd = MT25Q_RESET_ENABLE;

    if (mt25q_spi_write(&cmd, 1) != 0)
    {
        return -1;
    }

    cmd = MT25Q_RESET_MEMORY;

    if (mt25q_spi_write(&cmd, 1) != 0)
    {
        return -1;
    }

    return 0;
}

int mt25q_enter_deep_power_down(void)
{
    uint8_t cmd = MT25Q_ENTER_DEEP_POWER_DOWN;

    if (mt25q_spi_write(&cmd, 1) != 0)
    {
        return -1;
    }

    return 0;
}

int mt25q_release_from_deep_power_down(void)
{
    uint8_t cmd = MT25Q_RELEASE_FROM_DEEP_POWER_DOWN;

    if (mt25q_spi_write(&cmd, 1) != 0)
    {
        return -1;
    }

    return 0;
}

int mt25q_write_enable(void)
{
    uint8_t cmd = MT25Q_WRITE_ENABLE;

    if (mt25q_spi_write(&cmd, 1) != 0)
    {
        return -1;
    }

    uint8_t i = 0;
    for(i=0; i<UINT8_MAX; i++)
    {
        mt25q_status_t status = {0};

        mt25q_read_status(&status);

        if (status.write_enable_latch == MT25Q_WRITE_ENABLED)
        {
            return 0;
        }
    }

    return -1;
}

int mt25q_write_disable(void)
{
    uint8_t cmd = MT25Q_WRITE_DISABLE;

    if (mt25q_spi_write(&cmd, 1) != 0)
    {
        return -1;
    }

    uint8_t i = 0;
    for(i=0; i<UINT8_MAX; i++)
    {
        mt25q_status_t status = {0};

        mt25q_read_status(&status);

        if (status.write_enable_latch == MT25Q_WRITE_DISABLED)
        {
            return 0;
        }
    }

    return -1;
}

bool mt25q_is_busy(void)
{
    mt25q_status_t status = {0};

    if (mt25q_read_status(&status) != 0)
    {
        /* Error reading the status register */
        return true;
    }

    if (status.write_in_progress == MT25Q_READY)
    {
        return false;
    }
    else
    {
        return true;
    }
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
