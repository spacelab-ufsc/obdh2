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
 * \version 0.6.12
 * 
 * \date 2020/02/05
 * 
 * \addtogroup sl_eps2
 * \{
 */

#include <stdbool.h>

#include <drivers/tca4311a/tca4311a.h>
#include <drivers/i2c/i2c.h>
#include <drivers/gpio/gpio.h>

#include "sl_eps2.h"

#define SL_EPS2_CRC8_INITIAL_VALUE          0       /**< CRC8-CCITT initial value. */
#define SL_EPS2_CRC8_POLYNOMIAL             0x07    /**< CRC8-CCITT polynomial. */

/**
 * \brief Computes the CRC-8 of a sequence of bytes.
 *
 * \param[in] data is an array of data to compute the CRC-8.
 *
 * \param[in] len is the number of bytes of the given array.
 *
 * \return The computed CRC-8 value of the given data.
 */
uint8_t sl_eps2_crc8(uint8_t *data, uint8_t len);

/**
 * \brief Checks the CRC value of a given sequence of bytes.
 *
 * \param[in] data is the data to check the CRC.
 *
 * \param[in] len is the number of bytes to check the CRC value.
 *
 * \param[in] crc is the CRC-8 value to check.
 *
 * \return TRUE/FALSE if the given CRC value is correct or not.
 */
bool sl_eps2_check_crc(uint8_t *data, uint8_t len, uint8_t crc);

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
    uint32_t buf = 0;

    if (sl_eps2_read_reg(config, SL_EPS2_REG_DEVICE_ID, &buf) != 0)
    {
        return -1;
    }

    if (buf != SL_EPS2_DEVICE_ID)
    {
        return -1;      /* Wrong device ID (connection error?) */
    }

    return 0;
}

int sl_eps2_write_reg(sl_eps2_config_t config, uint8_t adr, uint32_t val)
{
    uint8_t buf[1+4+1] = {0};

    buf[0] = adr;
    buf[1] = (val >> 24) & 0xFF;
    buf[2] = (val >> 16) & 0xFF;
    buf[3] = (val >> 8)  & 0xFF;
    buf[4] = (val >> 0)  & 0xFF;
    buf[5] = sl_eps2_crc8(buf, 5);

    if (tca4311a_write(config, SL_EPS2_SLAVE_ADR, buf, 6) != TCA4311A_READY)
    {
        return -1;
    }

    return 0;
}

int sl_eps2_read_reg(sl_eps2_config_t config, uint8_t adr, uint32_t *val)
{
    uint8_t buf[1+4+1] = {0};

    buf[0] = adr;
    buf[1] = sl_eps2_crc8(buf, 1);

    if (tca4311a_write(config, SL_EPS2_SLAVE_ADR, buf, 2) != TCA4311A_READY)
    {
        return -1;
    }

    sl_eps2_delay_ms(5);

    if (tca4311a_read(config, SL_EPS2_SLAVE_ADR, buf, 5) != TCA4311A_READY)
    {
        return -1;
    }

    if (!sl_eps2_check_crc(buf, 4, buf[4]))
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
    /* Time counter */
    if (sl_eps2_read_time_counter(config, &(data->time_counter)) != 0)
    {
        return -1;
    }

    /* Temperature */
    if (sl_eps2_read_temp(config, &(data->temperature_uc)) != 0)
    {
        return -1;
    }

    /* Current */
    if (sl_eps2_read_current(config, &(data->current)) != 0)
    {
        return -1;
    }

    /* Last reset cause */
    if (sl_eps2_read_reset_cause(config, &(data->last_reset_cause)) != 0)
    {
        return -1;
    }

    /* Reset counter */
    if (sl_eps2_read_reset_counter(config, &(data->reset_counter)) != 0)
    {
        return -1;
    }

    /* Solar panel voltage */
    if (sl_eps2_read_solar_panel_voltage(config, SL_EPS2_SOLAR_PANEL_3_0, &(data->solar_panel_voltage_my_px)) != 0)
    {
        return -1;
    }

    if (sl_eps2_read_solar_panel_voltage(config, SL_EPS2_SOLAR_PANEL_1_4, &(data->solar_panel_voltage_mx_pz)) != 0)
    {
        return -1;
    }

    if (sl_eps2_read_solar_panel_voltage(config, SL_EPS2_SOLAR_PANEL_5_2, &(data->solar_panel_voltage_mz_py)) != 0)
    {
        return -1;
    }

    if (sl_eps2_read_solar_panel_voltage(config, SL_EPS2_SOLAR_PANEL_ALL, &(data->solar_panel_output_voltage)) != 0)
    {
        return -1;
    }

    /* Solar panel current */
    if (sl_eps2_read_solar_panel_current(config, SL_EPS2_SOLAR_PANEL_0, &(data->solar_panel_current_my)) != 0)
    {
        return -1;
    }

    if (sl_eps2_read_solar_panel_current(config, SL_EPS2_SOLAR_PANEL_1, &(data->solar_panel_current_py)) != 0)
    {
        return -1;
    }

    if (sl_eps2_read_solar_panel_current(config, SL_EPS2_SOLAR_PANEL_2, &(data->solar_panel_current_mx)) != 0)
    {
        return -1;
    }

    if (sl_eps2_read_solar_panel_current(config, SL_EPS2_SOLAR_PANEL_3, &(data->solar_panel_current_px)) != 0)
    {
        return -1;
    }

    if (sl_eps2_read_solar_panel_current(config, SL_EPS2_SOLAR_PANEL_4, &(data->solar_panel_current_mz)) != 0)
    {
        return -1;
    }

    if (sl_eps2_read_solar_panel_current(config, SL_EPS2_SOLAR_PANEL_5, &(data->solar_panel_current_pz)) != 0)
    {
        return -1;
    }

    /* MPPT duty cycle */
    if (sl_eps2_read_mppt_duty_cycle(config, SL_EPS2_MPPT_1, &(data->mppt_1_duty_cycle)) != 0)
    {
        return -1;
    }

    if (sl_eps2_read_mppt_duty_cycle(config, SL_EPS2_MPPT_2, &(data->mppt_2_duty_cycle)) != 0)
    {
        return -1;
    }

    if (sl_eps2_read_mppt_duty_cycle(config, SL_EPS2_MPPT_3, &(data->mppt_3_duty_cycle)) != 0)
    {
        return -1;
    }

    /* Main power bus voltage */
    if (sl_eps2_read_main_bus_voltage(config, &(data->main_power_bus_voltage)) != 0)
    {
        return -1;
    }

    /* RTDs temperature */
    if (sl_eps2_read_rtd_temperature(config, SL_EPS2_REG_RTD0_TEMP_K, &(data->rtd_0_temperature)) != 0)
    {
        return -1;
    }

    if (sl_eps2_read_rtd_temperature(config, SL_EPS2_REG_RTD1_TEMP_K, &(data->rtd_1_temperature)) != 0)
    {
        return -1;
    }

    if (sl_eps2_read_rtd_temperature(config, SL_EPS2_REG_RTD2_TEMP_K, &(data->rtd_2_temperature)) != 0)
    {
        return -1;
    }

    if (sl_eps2_read_rtd_temperature(config, SL_EPS2_REG_RTD3_TEMP_K, &(data->rtd_3_temperature)) != 0)
    {
        return -1;
    }

    if (sl_eps2_read_rtd_temperature(config, SL_EPS2_REG_RTD4_TEMP_K, &(data->rtd_4_temperature)) != 0)
    {
        return -1;
    }

    if (sl_eps2_read_rtd_temperature(config, SL_EPS2_REG_RTD5_TEMP_K, &(data->rtd_5_temperature)) != 0)
    {
        return -1;
    }

    if (sl_eps2_read_rtd_temperature(config, SL_EPS2_REG_RTD6_TEMP_K, &(data->rtd_6_temperature)) != 0)
    {
        return -1;
    }

    return 0;
}

int sl_eps2_read_time_counter(sl_eps2_config_t config, uint32_t *val)
{
    return sl_eps2_read_reg(config, SL_EPS2_REG_TIME_COUNTER_MS, val);
}

int sl_eps2_read_temp(sl_eps2_config_t config, sl_eps2_temp_t *val)
{
    return sl_eps2_read_reg(config, SL_EPS2_REG_UC_TEMPERATURE_K, (uint32_t*)val);
}

int sl_eps2_read_current(sl_eps2_config_t config, sl_eps2_current_t *val)
{
    return sl_eps2_read_reg(config, SL_EPS2_REG_CURRENT_MA, (uint32_t*)val);
}

int sl_eps2_read_reset_cause(sl_eps2_config_t config, uint8_t *val)
{
    return sl_eps2_read_reg(config, SL_EPS2_REG_LAST_RESET_CAUSE, (uint32_t*)val);
}

int sl_eps2_read_reset_counter(sl_eps2_config_t config, uint16_t *val)
{
    return sl_eps2_read_reg(config, SL_EPS2_REG_RESET_COUNTER, (uint32_t*)val);
}

int sl_eps2_read_solar_panel_voltage(sl_eps2_config_t config, uint8_t sp, uint16_t *val)
{
    switch(sp)
    {
        case SL_EPS2_SOLAR_PANEL_3_0:   return sl_eps2_read_reg(config, SL_EPS2_REG_SOLAR_PANEL_MY_PX_VOLT_MV, (uint32_t*)val);
        case SL_EPS2_SOLAR_PANEL_1_4:   return sl_eps2_read_reg(config, SL_EPS2_REG_SOLAR_PANEL_MX_PZ_VOLT_MV, (uint32_t*)val);
        case SL_EPS2_SOLAR_PANEL_5_2:   return sl_eps2_read_reg(config, SL_EPS2_REG_SOLAR_PANEL_MZ_PY_VOLT_MV, (uint32_t*)val);
        case SL_EPS2_SOLAR_PANEL_ALL:   return sl_eps2_read_reg(config, SL_EPS2_REG_SOLAR_PANEL_TOTAL_VOLT_MV, (uint32_t*)val);
        default:                        return -1;  /* Invalid solar panel set */
    }
}

int sl_eps2_read_solar_panel_current(sl_eps2_config_t config, uint8_t sp, sl_eps2_current_t *val)
{
    switch(sp)
    {
        case SL_EPS2_SOLAR_PANEL_0:     return sl_eps2_read_reg(config, SL_EPS2_REG_SOLAR_PANEL_MY_CUR_MA, (uint32_t*)val);
        case SL_EPS2_SOLAR_PANEL_1:     return sl_eps2_read_reg(config, SL_EPS2_REG_SOLAR_PANEL_PY_CUR_MA, (uint32_t*)val);
        case SL_EPS2_SOLAR_PANEL_2:     return sl_eps2_read_reg(config, SL_EPS2_REG_SOLAR_PANEL_MX_CUR_MA, (uint32_t*)val);
        case SL_EPS2_SOLAR_PANEL_3:     return sl_eps2_read_reg(config, SL_EPS2_REG_SOLAR_PANEL_PX_CUR_MA, (uint32_t*)val);
        case SL_EPS2_SOLAR_PANEL_4:     return sl_eps2_read_reg(config, SL_EPS2_REG_SOLAR_PANEL_MZ_CUR_MA, (uint32_t*)val);
        case SL_EPS2_SOLAR_PANEL_5:     return sl_eps2_read_reg(config, SL_EPS2_REG_SOLAR_PANEL_PZ_CUR_MA, (uint32_t*)val);
        default:                        return -1;  /* Invalid solar panel */
    }
}

int sl_eps2_read_mppt_duty_cycle(sl_eps2_config_t config, uint8_t mppt, sl_eps2_duty_cycle_t *val)
{
    switch(mppt)
    {
        case SL_EPS2_MPPT_1:    return sl_eps2_read_reg(config, SL_EPS2_REG_MPPT_1_DUTY_CYCLE, (uint32_t*)val);
        case SL_EPS2_MPPT_2:    return sl_eps2_read_reg(config, SL_EPS2_REG_MPPT_2_DUTY_CYCLE, (uint32_t*)val);
        case SL_EPS2_MPPT_3:    return sl_eps2_read_reg(config, SL_EPS2_REG_MPPT_3_DUTY_CYCLE, (uint32_t*)val);
        default:                return -1;  /* Invalid MPPT */
    }
}

int sl_eps2_read_main_bus_voltage(sl_eps2_config_t config, sl_eps2_voltage_t *val)
{
    return sl_eps2_read_reg(config, SL_EPS2_REG_MAIN_POWER_BUS_VOLT_MV, (uint32_t*)val);
}

int sl_eps2_read_rtd_temperature(sl_eps2_config_t config, uint8_t rtd, sl_eps2_temp_t *val)
{
    switch(rtd)
    {
        case SL_EPS2_RTD_0:     return sl_eps2_read_reg(config, SL_EPS2_REG_RTD0_TEMP_K, (uint32_t*)val);
        case SL_EPS2_RTD_1:     return sl_eps2_read_reg(config, SL_EPS2_REG_RTD1_TEMP_K, (uint32_t*)val);
        case SL_EPS2_RTD_2:     return sl_eps2_read_reg(config, SL_EPS2_REG_RTD2_TEMP_K, (uint32_t*)val);
        case SL_EPS2_RTD_3:     return sl_eps2_read_reg(config, SL_EPS2_REG_RTD3_TEMP_K, (uint32_t*)val);
        case SL_EPS2_RTD_4:     return sl_eps2_read_reg(config, SL_EPS2_REG_RTD4_TEMP_K, (uint32_t*)val);
        case SL_EPS2_RTD_5:     return sl_eps2_read_reg(config, SL_EPS2_REG_RTD5_TEMP_K, (uint32_t*)val);
        case SL_EPS2_RTD_6:     return sl_eps2_read_reg(config, SL_EPS2_REG_RTD6_TEMP_K, (uint32_t*)val);
        default:                return -1;
    }
}

int sl_eps2_read_battery_voltage(sl_eps2_config_t config, sl_eps2_voltage_t *val)
{
    return sl_eps2_read_reg(config, SL_EPS2_REG_BATTERY_VOLT_MV, (uint32_t*)val);
}

int sl_eps2_read_battery_current(sl_eps2_config_t config, sl_eps2_current_t *val)
{
    return sl_eps2_read_reg(config, SL_EPS2_REG_BATTERY_CUR_MA, (uint32_t*)val);
}

int sl_eps2_read_battery_charge(sl_eps2_config_t config, sl_eps2_charge_t *val)
{
    return sl_eps2_read_reg(config, SL_EPS2_REG_BATTERY_CHARGE_MAH, (uint32_t*)val);
}

int sl_eps2_read_hardware_version(sl_eps2_config_t config, uint8_t *val)
{
    return sl_eps2_read_reg(config, SL_EPS2_REG_HARDWARE_VERSION, (uint32_t*)val);
}

int sl_eps2_read_firmware_version(sl_eps2_config_t config, uint32_t *val)
{
    return sl_eps2_read_reg(config, SL_EPS2_REG_FIRMWARE_VERSION, val);
}

int sl_eps2_set_mppt_mode(sl_eps2_config_t config, uint8_t mppt, uint8_t mode)
{
    switch(mode)
    {
        case SL_EPS2_MPPT_MODE_AUTOMATIC:   break;
        case SL_EPS2_MPPT_MODE_MANUAL:      break;
        default:                            return -1;  /* Invalid mode */
    }

    switch(mppt)
    {
        case SL_EPS2_MPPT_1:
            return sl_eps2_write_reg(config, SL_EPS2_REG_MPPT_1_MODE, (uint32_t)mode);
        case SL_EPS2_MPPT_2:
            return sl_eps2_write_reg(config, SL_EPS2_REG_MPPT_2_MODE, (uint32_t)mode);
        case SL_EPS2_MPPT_3:
            return sl_eps2_write_reg(config, SL_EPS2_REG_MPPT_3_MODE, (uint32_t)mode);
        default:
            return -1;  /* Invalid MPPT */
    }
}

int sl_eps2_set_heater_mode(sl_eps2_config_t config, uint8_t heater, uint8_t mode)
{
    switch(mode)
    {
        case SL_EPS2_HEATER_MODE_AUTOMATIC:     break;
        case SL_EPS2_HEATER_MODE_MANUAL:        break;
        default:                                return -1;  /* Invalid mode */
    }

    switch(heater)
    {
        case SL_EPS2_HEATER_1:
            return sl_eps2_write_reg(config, SL_EPS2_REG_BAT_HEATER_1_MODE, (uint32_t)mode);
        case SL_EPS2_HEATER_2:
            return sl_eps2_write_reg(config, SL_EPS2_REG_BAT_HEATER_2_MODE, (uint32_t)mode);
        default:
            return -1;  /* Invalid heater */
    }
}

uint8_t sl_eps2_crc8(uint8_t *data, uint8_t len)
{
    uint8_t crc = SL_EPS2_CRC8_INITIAL_VALUE;

    while(len--)
    {
        crc ^= *data++;

        uint8_t j = 0;
        for (j=0; j<8; j++)
        {
            crc = (crc << 1) ^ ((crc & 0x80)? SL_EPS2_CRC8_POLYNOMIAL : 0);
        }

        crc &= 0xFF;
    }

    return crc;
}

bool sl_eps2_check_crc(uint8_t *data, uint8_t len, uint8_t crc)
{
    if (crc != sl_eps2_crc8(data, len))
    {
        return false;
    }

    return true;
}

/** \} End of sl_eps2 group */
