/*
 * sl_eps2.c
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
 * \brief SpaceLab EPS 2.0 driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.6.7
 * 
 * \date 2020/02/05
 * 
 * \addtogroup sl_eps2
 * \{
 */

#include <drivers/tca4311a/tca4311a.h>
#include <drivers/i2c/i2c.h>
#include <drivers/gpio/gpio.h>

#include "sl_eps2.h"

int sl_eps2_init(sl_eps2_config_t config)
{
    if (tca4311a_init(config, true) != TCA4311A_READY)
    {
        return -1;      /* Error initializing the I2C port */
    }

    if (sl_eps2_check_device(config) != 0)
    {
        return -1;      /* Error checking the connection */
    }

    return 0;
}

int sl_eps2_check_device(sl_eps2_config_t config)
{
    uint32_t buf = SL_EPS2_REG_DEVICE_ID;

    if (tca4311a_write(config, SL_EPS2_SLAVE_ADR, &buf, 1) != TCA4311A_READY)
    {
        return -1;      /* Error writing the command */
    }

    if (tca4311a_read(config, SL_EPS2_SLAVE_ADR, &buf, 1) != TCA4311A_READY)
    {
        return -1;      /* Error reading the command result */
    }

    if (buf != SL_EPS2_DEVICE_ID)
    {
        return -1;      /* Wrong device ID (connection error?) */
    }

    return 0;
}

int sl_eps2_write_reg(sl_eps2_config_t config, uint8_t adr, uint32_t val)
{
    uint8_t buf[5] = {0};

    buf[0] = adr;
    buf[1] = (val >> 24) & 0xFF;
    buf[2] = (val >> 16) & 0xFF;
    buf[3] = (val >> 8)  & 0xFF;
    buf[4] = (val >> 0)  & 0xFF;

    if (tca4311a_write(config, SL_EPS2_SLAVE_ADR, buf, 5) != TCA4311A_READY)
    {
        return -1;
    }

    return 0;
}

int sl_eps2_read_reg(sl_eps2_config_t config, uint8_t adr, uint32_t *val)
{
    if (tca4311a_write_byte(config, SL_EPS2_SLAVE_ADR, adr) != TCA4311A_READY)
    {
        return -1;
    }

    uint8_t buf[4] = {0};

    if (tca4311a_read(config, SL_EPS2_SLAVE_ADR, buf, 4) != TCA4311A_READY)
    {
        return -1;
    }

    *val = ((uint32_t)buf[0] << 24) |
           ((uint32_t)buf[1] << 16) |
           ((uint32_t)buf[2] << 8)  |
           ((uint32_t)buf[3] << 0);

    return 0;
}

int sl_eps2_read_data(sl_eps2_config_t config, sl_eps2_data_t *data)
{
    return -1;
}

int sl_eps2_read_time_counter(sl_eps2_config_t config, uint32_t *val)
{
    return sl_eps2_read_reg(config, SL_EPS2_REG_TIME_COUNTER_MS, val);
}

int sl_eps2_read_battery_voltage(sl_eps2_config_t config, sl_eps2_voltage_t *val)
{
    return sl_eps2_read_reg(config, SL_EPS2_REG_BATTERY_VOLT_MV, val);
}

int sl_eps2_read_battery_current(sl_eps2_config_t config, sl_eps2_current_t *val)
{
    return sl_eps2_read_reg(config, SL_EPS2_REG_BATTERY_CUR_MA, val);
}

int sl_eps2_read_battery_charge(sl_eps2_config_t config, sl_eps2_charge_t *val)
{
    return sl_eps2_read_reg(config, SL_EPS2_REG_BATTERY_CHARGE_MAH, val);
}

int sl_eps2_read_solar_panel_current(sl_eps2_config_t config, uint8_t sp, sl_eps2_current_t *val)
{
    switch(sp)
    {
        case SL_EPS2_SOLAR_PANEL_0:     return sl_eps2_read_reg(config, SL_EPS2_REG_SOLAR_PANEL_MY_CUR_MA, val);
        case SL_EPS2_SOLAR_PANEL_1:     return sl_eps2_read_reg(config, SL_EPS2_REG_SOLAR_PANEL_PY_CUR_MA, val);
        case SL_EPS2_SOLAR_PANEL_2:     return sl_eps2_read_reg(config, SL_EPS2_REG_SOLAR_PANEL_MX_CUR_MA, val);
        case SL_EPS2_SOLAR_PANEL_3:     return sl_eps2_read_reg(config, SL_EPS2_REG_SOLAR_PANEL_PX_CUR_MA, val);
        case SL_EPS2_SOLAR_PANEL_4:     return sl_eps2_read_reg(config, SL_EPS2_REG_SOLAR_PANEL_MZ_CUR_MA, val);
        case SL_EPS2_SOLAR_PANEL_5:     return sl_eps2_read_reg(config, SL_EPS2_REG_SOLAR_PANEL_PZ_CUR_MA, val);
        default:
            return -1;  /* Invalid solar panel */
    }
}

int sl_eps2_read_solar_panel_voltage(sl_eps2_config_t config, uint8_t sp, sl_eps2_voltage_t *val)
{
    switch(sp)
    {
        case SL_EPS2_SOLAR_PANEL_30:    return sl_eps2_read_reg(config, SL_EPS2_REG_SOLAR_PANEL_MY_PX_VOLT_MV, val);
        case SL_EPS2_SOLAR_PANEL_14:    return sl_eps2_read_reg(config, SL_EPS2_REG_SOLAR_PANEL_MX_PZ_VOLT_MV, val);
        case SL_EPS2_SOLAR_PANEL_52:    return sl_eps2_read_reg(config, SL_EPS2_REG_SOLAR_PANEL_MZ_PY_VOLT_MV, val);
        default:
            return -1;  /* Invalid solar panel set */
    }
}

/** \} End of sl_eps2 group */
