/*
 * sl_eps2.c
 * 
 * Copyright The OBDH 2.0 Contributors.
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
 * along with OBDH 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief SpaceLab EPS 2.0 driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.8.42
 * 
 * \date 2020/02/05
 * 
 * \addtogroup sl_eps2
 * \{
 */

#include <stdbool.h>

#include "sl_eps2.h"

#define SL_EPS2_CRC8_INITIAL_VALUE          0U      /**< CRC8-CCITT initial value. */
#define SL_EPS2_CRC8_POLYNOMIAL             0x07U   /**< CRC8-CCITT polynomial. */

/**
 * \brief Computes the CRC-8 of a sequence of bytes.
 *
 * \param[in] data is an array of data to compute the CRC-8.
 *
 * \param[in] len is the number of bytes of the given array.
 *
 * \return The computed CRC-8 value of the given data.
 */
static uint8_t sl_eps2_crc8(uint8_t *data, uint8_t len);

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
static bool sl_eps2_check_crc(uint8_t *data, uint8_t len, uint8_t crc);

int sl_eps2_init(sl_eps2_config_t config)
{
    int err = 0;

    if (sl_eps2_i2c_init(config) != 0)
    {
        err = -1;   /* Error initializing the I2C port */
    }

    if (sl_eps2_check_device(config) != 0)
    {
        err = -1;
    }

    return err;
}

int sl_eps2_check_device(sl_eps2_config_t config)
{
    int err = -1;

    uint32_t buf = 0;

    if (sl_eps2_read_reg(config, SL_EPS2_REG_DEVICE_ID, &buf) == 0)
    {
        if (buf == SL_EPS2_DEVICE_ID)
        {
            err = 0;
        }
    }

    return err;
}

int sl_eps2_write_reg(sl_eps2_config_t config, uint8_t adr, uint32_t val)
{
    int err = 0;

    uint8_t buf[1 + 4 + 1] = {0};

    buf[0] = adr;
    buf[1] = (val >> 24) & 0xFFU;
    buf[2] = (val >> 16) & 0xFFU;
    buf[3] = (val >> 8)  & 0xFFU;
    buf[4] = (val >> 0)  & 0xFFU;
    buf[5] = sl_eps2_crc8(buf, 5);

    if (sl_eps2_i2c_write(config, buf, 6U) != TCA4311A_READY)
    {
        err = -1;
    }

    return err;
}

int sl_eps2_read_reg(sl_eps2_config_t config, uint8_t adr, uint32_t *val)
{

    int err = 0;

    uint8_t buf[1 + 4 + 1] = {0};

    buf[0] = adr;
    buf[1] = sl_eps2_crc8(buf, 1);

    if (sl_eps2_i2c_write(config, buf, 2U) != TCA4311A_READY)
    {
        err = -1;
    }

    sl_eps2_delay_ms(50);

    if (sl_eps2_i2c_read(config, buf, 6U) != TCA4311A_READY)
    {
        err = -1;
    }

    if (!sl_eps2_check_crc(buf, 5U, buf[5]))
    {
        err = -1;
    }

    *val = ((uint32_t)buf[1] << 24) |
           ((uint32_t)buf[2] << 16) |
           ((uint32_t)buf[3] << 8)  |
           ((uint32_t)buf[4] << 0);

    return err;
}

int sl_eps2_read_data(sl_eps2_config_t config, sl_eps2_data_t *data)
{
    int err_counter = 0;

    /* Time counter */
    if (sl_eps2_read_time_counter(config, &(data->time_counter)) != 0)
    {
        err_counter++;
    }

    /* Temperature */
    if (sl_eps2_read_temp(config, &(data->temperature_uc)) != 0)
    {
        err_counter++;
    }

    /* Current */
    if (sl_eps2_read_current(config, &(data->current)) != 0)
    {
        err_counter++;
    }

    sl_eps2_delay_ms(5);

    /* Last reset cause */
    if (sl_eps2_read_reset_cause(config, &(data->last_reset_cause)) != 0)
    {
        err_counter++;
    }

    /* Reset counter */
    if (sl_eps2_read_reset_counter(config, &(data->reset_counter)) != 0)
    {
        err_counter++;
    }

    /* Solar panel voltage */
    if (sl_eps2_read_solar_panel_voltage(config, SL_EPS2_SOLAR_PANEL_3_0, &(data->solar_panel_voltage_my_px)) != 0)
    {
        err_counter++;
    }

    sl_eps2_delay_ms(5);

    if (sl_eps2_read_solar_panel_voltage(config, SL_EPS2_SOLAR_PANEL_1_4, &(data->solar_panel_voltage_mx_pz)) != 0)
    {
        err_counter++;
    }

    if (sl_eps2_read_solar_panel_voltage(config, SL_EPS2_SOLAR_PANEL_5_2, &(data->solar_panel_voltage_mz_py)) != 0)
    {
        err_counter++;
    }

    if (sl_eps2_read_solar_panel_voltage(config, SL_EPS2_SOLAR_PANEL_ALL, &(data->solar_panel_output_voltage)) != 0)
    {
        err_counter++;
    }

    sl_eps2_delay_ms(5);

    /* Solar panel current */
    if (sl_eps2_read_solar_panel_current(config, SL_EPS2_SOLAR_PANEL_0, &(data->solar_panel_current_my)) != 0)
    {
        err_counter++;
    }

    if (sl_eps2_read_solar_panel_current(config, SL_EPS2_SOLAR_PANEL_1, &(data->solar_panel_current_py)) != 0)
    {
        err_counter++;
    }

    if (sl_eps2_read_solar_panel_current(config, SL_EPS2_SOLAR_PANEL_2, &(data->solar_panel_current_mx)) != 0)
    {
        err_counter++;
    }

    sl_eps2_delay_ms(5);

    if (sl_eps2_read_solar_panel_current(config, SL_EPS2_SOLAR_PANEL_3, &(data->solar_panel_current_px)) != 0)
    {
        err_counter++;
    }

    if (sl_eps2_read_solar_panel_current(config, SL_EPS2_SOLAR_PANEL_4, &(data->solar_panel_current_mz)) != 0)
    {
        err_counter++;
    }

    if (sl_eps2_read_solar_panel_current(config, SL_EPS2_SOLAR_PANEL_5, &(data->solar_panel_current_pz)) != 0)
    {
        err_counter++;
    }

    sl_eps2_delay_ms(5);

    /* MPPT duty cycle */
    if (sl_eps2_read_mppt_duty_cycle(config, SL_EPS2_MPPT_1, &(data->mppt_1_duty_cycle)) != 0)
    {
        err_counter++;
    }

    if (sl_eps2_read_mppt_duty_cycle(config, SL_EPS2_MPPT_2, &(data->mppt_2_duty_cycle)) != 0)
    {
        err_counter++;
    }

    if (sl_eps2_read_mppt_duty_cycle(config, SL_EPS2_MPPT_3, &(data->mppt_3_duty_cycle)) != 0)
    {
        err_counter++;
    }

    sl_eps2_delay_ms(5);

    /* Main power bus voltage */
    if (sl_eps2_read_main_bus_voltage(config, &(data->main_power_bus_voltage)) != 0)
    {
        err_counter++;
    }

    /* RTDs temperature */
    if (sl_eps2_read_rtd_temperature(config, SL_EPS2_RTD_0, &(data->rtd_0_temperature)) != 0)
    {
        err_counter++;
    }

    if (sl_eps2_read_rtd_temperature(config, SL_EPS2_RTD_1, &(data->rtd_1_temperature)) != 0)
    {
        err_counter++;
    }

    sl_eps2_delay_ms(5);

    if (sl_eps2_read_rtd_temperature(config, SL_EPS2_RTD_2, &(data->rtd_2_temperature)) != 0)
    {
        err_counter++;
    }

    if (sl_eps2_read_rtd_temperature(config, SL_EPS2_RTD_3, &(data->rtd_3_temperature)) != 0)
    {
        err_counter++;
    }

    if (sl_eps2_read_rtd_temperature(config, SL_EPS2_RTD_4, &(data->rtd_4_temperature)) != 0)
    {
        err_counter++;
    }

    sl_eps2_delay_ms(5);

    if (sl_eps2_read_rtd_temperature(config, SL_EPS2_RTD_5, &(data->rtd_5_temperature)) != 0)
    {
        err_counter++;
    }

    if (sl_eps2_read_rtd_temperature(config, SL_EPS2_RTD_6, &(data->rtd_6_temperature)) != 0)
    {
        err_counter++;
    }

    /* Battery voltage */
    if (sl_eps2_read_battery_voltage(config, &(data->battery_voltage)) != 0)
    {
        err_counter++;
    }

    sl_eps2_delay_ms(5);

    /* Battery current */
    if (sl_eps2_read_battery_current(config, SL_EPS2_BATTERY_CURRENT, &(data->battery_current)) != 0)
    {
        err_counter++;
    }

    if (sl_eps2_read_battery_current(config, SL_EPS2_BATTERY_AVERAGE_CURRENT, &(data->battery_average_current)) != 0)
    {
        err_counter++;
    }

    if (sl_eps2_read_battery_current(config, SL_EPS2_BATTERY_ACC_CURRENT, &(data->battery_acc_current)) != 0)
    {
        err_counter++;
    }

    sl_eps2_delay_ms(5);

    /* Battery charge */
    if (sl_eps2_read_battery_charge(config, &(data->battery_charge)) != 0)
    {
        err_counter++;
    }

    /* Battery monitor */
    if (sl_eps2_read_battery_monitor_temp(config, &(data->battery_monitor_temperature)) != 0)
    {
        err_counter++;
    }

    if (sl_eps2_read_battery_monitor_status(config, &(data->battery_monitor_status)) != 0)
    {
        err_counter++;
    }

    sl_eps2_delay_ms(5);

    if (sl_eps2_read_battery_monitor_protection(config, &(data->battery_monitor_protection)) != 0)
    {
        err_counter++;
    }

    if (sl_eps2_read_battery_monitor_cycle_counter(config, &(data->battery_monitor_cycle_counter)) != 0)
    {
        err_counter++;
    }

    if (sl_eps2_read_battery_monitor_raac(config, &(data->raac)) != 0)
    {
        err_counter++;
    }

    sl_eps2_delay_ms(5);

    if (sl_eps2_read_battery_monitor_rsac(config, &(data->rsac)) != 0)
    {
        err_counter++;
    }

    if (sl_eps2_read_battery_monitor_rarc(config, &(data->rarc)) != 0)
    {
        err_counter++;
    }

    if (sl_eps2_read_battery_monitor_rsrc(config, &(data->rsrc)) != 0)
    {
        err_counter++;
    }

    sl_eps2_delay_ms(5);

    /* Heater duty cycle */
    if (sl_eps2_read_heater_duty_cycle(config, SL_EPS2_HEATER_1, (&data->battery_heater_1_duty_cycle)) != 0)
    {
        err_counter++;
    }

    if (sl_eps2_read_heater_duty_cycle(config, SL_EPS2_HEATER_2, (&data->battery_heater_2_duty_cycle)) != 0)
    {
        err_counter++;
    }

    /* MPPT mode */
    if (sl_eps2_get_mppt_mode(config, SL_EPS2_MPPT_1, &(data->mppt_1_mode)) != 0)
    {
        err_counter++;
    }

    if (sl_eps2_get_mppt_mode(config, SL_EPS2_MPPT_2, &(data->mppt_2_mode)) != 0)
    {
        err_counter++;
    }

    sl_eps2_delay_ms(5);

    if (sl_eps2_get_mppt_mode(config, SL_EPS2_MPPT_3, &(data->mppt_3_mode)) != 0)
    {
        err_counter++;
    }

    /* Heater mode */
    if (sl_eps2_get_heater_mode(config, SL_EPS2_HEATER_1, &(data->battery_heater_1_mode)) != 0)
    {
        err_counter++;
    }

    if (sl_eps2_get_heater_mode(config, SL_EPS2_HEATER_2, &(data->battery_heater_2_mode)) != 0)
    {
        err_counter++;
    }

    return err_counter;
}

int sl_eps2_read_time_counter(sl_eps2_config_t config, uint32_t *val)
{
    return sl_eps2_read_reg(config, SL_EPS2_REG_TIME_COUNTER_MS, val);
}

int sl_eps2_read_temp(sl_eps2_config_t config, sl_eps2_temp_t *val)
{
    uint32_t buf = UINT32_MAX;

    int res = sl_eps2_read_reg(config, SL_EPS2_REG_UC_TEMPERATURE_K, &buf);

    *val = (uint32_t)buf;

    return res;
}

int sl_eps2_read_current(sl_eps2_config_t config, sl_eps2_current_t *val)
{
    uint32_t buf = UINT32_MAX;

    int res = sl_eps2_read_reg(config, SL_EPS2_REG_CURRENT_MA, &buf);

    *val = (uint32_t)buf;

    return res;
}

int sl_eps2_read_reset_cause(sl_eps2_config_t config, uint8_t *val)
{
    uint32_t buf = UINT32_MAX;

    int res = sl_eps2_read_reg(config, SL_EPS2_REG_LAST_RESET_CAUSE, &buf);

    *val = (uint32_t)buf;

    return res;
}

int sl_eps2_read_reset_counter(sl_eps2_config_t config, uint16_t *val)
{
    uint32_t buf = UINT32_MAX;

    int res = sl_eps2_read_reg(config, SL_EPS2_REG_RESET_COUNTER, &buf);

    *val = (uint32_t)buf;

    return res;
}

int sl_eps2_read_solar_panel_voltage(sl_eps2_config_t config, uint8_t sp, uint16_t *val)
{
    int res = -1;

    uint32_t buf = UINT32_MAX;

    switch(sp)
    {
        case SL_EPS2_SOLAR_PANEL_3_0:
            res = sl_eps2_read_reg(config, SL_EPS2_REG_SOLAR_PANEL_MY_PX_VOLT_MV, &buf);

            *val = (uint16_t)buf;

            break;
        case SL_EPS2_SOLAR_PANEL_1_4:
            res = sl_eps2_read_reg(config, SL_EPS2_REG_SOLAR_PANEL_MX_PZ_VOLT_MV, &buf);

            *val = (uint16_t)buf;

            break;
        case SL_EPS2_SOLAR_PANEL_5_2:
            res = sl_eps2_read_reg(config, SL_EPS2_REG_SOLAR_PANEL_MZ_PY_VOLT_MV, &buf);

            *val = (uint16_t)buf;

            break;
        case SL_EPS2_SOLAR_PANEL_ALL:
            res = sl_eps2_read_reg(config, SL_EPS2_REG_SOLAR_PANEL_TOTAL_VOLT_MV, &buf);

            *val = (uint16_t)buf;

            break;
        default:
            break;
    }

    return res;
}

int sl_eps2_read_solar_panel_current(sl_eps2_config_t config, uint8_t sp, sl_eps2_current_t *val)
{
    int res = -1;

    uint32_t buf = UINT32_MAX;

    switch(sp)
    {
        case SL_EPS2_SOLAR_PANEL_0:
            res = sl_eps2_read_reg(config, SL_EPS2_REG_SOLAR_PANEL_MY_CUR_MA, &buf);

            *val = (sl_eps2_current_t)buf;

            break;
        case SL_EPS2_SOLAR_PANEL_1:
            res = sl_eps2_read_reg(config, SL_EPS2_REG_SOLAR_PANEL_PY_CUR_MA, &buf);

            *val = (sl_eps2_current_t)buf;

            break;
        case SL_EPS2_SOLAR_PANEL_2:
            res = sl_eps2_read_reg(config, SL_EPS2_REG_SOLAR_PANEL_MX_CUR_MA, &buf);

            *val = (sl_eps2_current_t)buf;

            break;
        case SL_EPS2_SOLAR_PANEL_3:
            res = sl_eps2_read_reg(config, SL_EPS2_REG_SOLAR_PANEL_PX_CUR_MA, &buf);

            *val = (sl_eps2_current_t)buf;

            break;
        case SL_EPS2_SOLAR_PANEL_4:
            res = sl_eps2_read_reg(config, SL_EPS2_REG_SOLAR_PANEL_MZ_CUR_MA, &buf);

            *val = (sl_eps2_current_t)buf;

            break;
        case SL_EPS2_SOLAR_PANEL_5:
            res = sl_eps2_read_reg(config, SL_EPS2_REG_SOLAR_PANEL_PZ_CUR_MA, &buf);

            *val = (sl_eps2_current_t)buf;

            break;
        default:
            break;
    }

    return res;
}

int sl_eps2_read_mppt_duty_cycle(sl_eps2_config_t config, uint8_t mppt, sl_eps2_duty_cycle_t *val)
{
    int res = -1;

    uint32_t buf = UINT32_MAX;

    switch(mppt)
    {
        case SL_EPS2_MPPT_1:
            res = sl_eps2_read_reg(config, SL_EPS2_REG_MPPT_1_DUTY_CYCLE, &buf);

            *val = (sl_eps2_duty_cycle_t)buf;

            break;
        case SL_EPS2_MPPT_2:
            res = sl_eps2_read_reg(config, SL_EPS2_REG_MPPT_2_DUTY_CYCLE, &buf);

            *val = (sl_eps2_duty_cycle_t)buf;

            break;
        case SL_EPS2_MPPT_3:
            res = sl_eps2_read_reg(config, SL_EPS2_REG_MPPT_3_DUTY_CYCLE, &buf);

            *val = (sl_eps2_duty_cycle_t)buf;

            break;
        default:
            break;
    }

    return res;
}

int sl_eps2_read_main_bus_voltage(sl_eps2_config_t config, sl_eps2_voltage_t *val)
{
    uint32_t buf = UINT32_MAX;

    int res = sl_eps2_read_reg(config, SL_EPS2_REG_MAIN_POWER_BUS_VOLT_MV, &buf);

    *val = (sl_eps2_voltage_t)buf;

    return res;
}

int sl_eps2_read_rtd_temperature(sl_eps2_config_t config, uint8_t rtd, sl_eps2_temp_t *val)
{
    int res = -1;

    uint32_t buf = UINT32_MAX;

    switch(rtd)
    {
        case SL_EPS2_RTD_0:
            res = sl_eps2_read_reg(config, SL_EPS2_REG_RTD0_TEMP_K, &buf);

            *val = (sl_eps2_temp_t)buf;

            break;
        case SL_EPS2_RTD_1:
            res = sl_eps2_read_reg(config, SL_EPS2_REG_RTD1_TEMP_K, &buf);

            *val = (sl_eps2_temp_t)buf;

            break;
        case SL_EPS2_RTD_2:
            res = sl_eps2_read_reg(config, SL_EPS2_REG_RTD2_TEMP_K, &buf);

            *val = (sl_eps2_temp_t)buf;

            break;
        case SL_EPS2_RTD_3:
            res = sl_eps2_read_reg(config, SL_EPS2_REG_RTD3_TEMP_K, &buf);

            *val = (sl_eps2_temp_t)buf;

            break;
        case SL_EPS2_RTD_4:
            res = sl_eps2_read_reg(config, SL_EPS2_REG_RTD4_TEMP_K, &buf);

            *val = (sl_eps2_temp_t)buf;

            break;
        case SL_EPS2_RTD_5:
            res = sl_eps2_read_reg(config, SL_EPS2_REG_RTD5_TEMP_K, &buf);

            *val = (sl_eps2_temp_t)buf;

            break;
        case SL_EPS2_RTD_6:
            res = sl_eps2_read_reg(config, SL_EPS2_REG_RTD6_TEMP_K, &buf);

            *val = (sl_eps2_temp_t)buf;

            break;
        default:
            break;
    }

    return res;
}

int sl_eps2_read_battery_voltage(sl_eps2_config_t config, sl_eps2_voltage_t *val)
{
    uint32_t buf = UINT32_MAX;

    int res = sl_eps2_read_reg(config, SL_EPS2_REG_BATTERY_VOLT_MV, &buf);

    *val = (sl_eps2_voltage_t)buf;

    return res;
}

int sl_eps2_read_battery_current(sl_eps2_config_t config, uint8_t cur, sl_eps2_current_t *val)
{
    int res = -1;

    uint32_t buf = UINT32_MAX;

    switch(cur)
    {
        case SL_EPS2_BATTERY_CURRENT:
            res = sl_eps2_read_reg(config, SL_EPS2_REG_BATTERY_CUR_MA, &buf);

            *val = (sl_eps2_current_t)buf;

            break;
        case SL_EPS2_BATTERY_AVERAGE_CURRENT:
            res = sl_eps2_read_reg(config, SL_EPS2_REG_BATTERY_AVEG_CUR_MA, &buf);

            *val = (sl_eps2_current_t)buf;

            break;
        case SL_EPS2_BATTERY_ACC_CURRENT:
            res = sl_eps2_read_reg(config, SL_EPS2_REG_BATTERY_ACC_CUR_MA, &buf);

            *val = (sl_eps2_current_t)buf;

            break;
        default:
            break;
    }

    return res;
}

int sl_eps2_read_battery_charge(sl_eps2_config_t config, sl_eps2_charge_t *val)
{
    uint32_t buf = UINT32_MAX;

    int res = sl_eps2_read_reg(config, SL_EPS2_REG_BATTERY_CHARGE_MAH, &buf);

    *val = (sl_eps2_charge_t)buf;

    return res;
}

int sl_eps2_read_battery_monitor_temp(sl_eps2_config_t config, sl_eps2_temp_t *val)
{
    uint32_t buf = UINT32_MAX;

    int res = sl_eps2_read_reg(config, SL_EPS2_REG_BAT_MONITOR_TEMP_K, &buf);

    *val = (sl_eps2_temp_t)buf;

    return res;
}

int sl_eps2_read_battery_monitor_status(sl_eps2_config_t config, uint8_t *val)
{
    uint32_t buf = UINT32_MAX;

    int res = sl_eps2_read_reg(config, SL_EPS2_REG_BAT_MONITOR_STATUS, &buf);

    *val = (uint8_t)buf;

    return res;
}

int sl_eps2_read_battery_monitor_protection(sl_eps2_config_t config, uint8_t *val)
{
    uint32_t buf = UINT32_MAX;

    int res = sl_eps2_read_reg(config, SL_EPS2_REG_BAT_MONITOR_PROTECTION, &buf);

    *val = (uint8_t)buf;

    return res;
}

int sl_eps2_read_battery_monitor_cycle_counter(sl_eps2_config_t config, uint8_t *val)
{
    uint32_t buf = UINT32_MAX;

    int res = sl_eps2_read_reg(config, SL_EPS2_REG_BAT_MONITOR_CYCLE_COUNTER, &buf);

    *val = (uint8_t)buf;

    return res;
}

int sl_eps2_read_battery_monitor_raac(sl_eps2_config_t config, sl_eps2_charge_t *val)
{
    uint32_t buf = UINT32_MAX;

    int res = sl_eps2_read_reg(config, SL_EPS2_REG_BAT_MONITOR_RAAC_MAH, &buf);

    *val = (sl_eps2_charge_t )buf;

    return res;
}

int sl_eps2_read_battery_monitor_rsac(sl_eps2_config_t config, sl_eps2_charge_t *val)
{
    uint32_t buf = UINT32_MAX;

    int res = sl_eps2_read_reg(config, SL_EPS2_REG_BAT_MONITOR_RSAC_MAH, &buf);

    *val = (sl_eps2_charge_t )buf;

    return res;
}

int sl_eps2_read_battery_monitor_rarc(sl_eps2_config_t config, uint8_t *val)
{
    uint32_t buf = UINT32_MAX;

    int res = sl_eps2_read_reg(config, SL_EPS2_REG_BAT_MONITOR_RARC_PERC, &buf);

    *val = (uint8_t)buf;

    return res;
}

int sl_eps2_read_battery_monitor_rsrc(sl_eps2_config_t config, uint8_t *val)
{
    uint32_t buf = UINT32_MAX;

    int res = sl_eps2_read_reg(config, SL_EPS2_REG_BAT_MONITOR_RSRC_PERC, &buf);

    *val = (uint8_t)buf;

    return res;
}

int sl_eps2_read_heater_duty_cycle(sl_eps2_config_t config, uint8_t heater, sl_eps2_duty_cycle_t *val)
{
    int res = -1;

    uint32_t buf = UINT32_MAX;

    switch(heater)
    {
        case SL_EPS2_HEATER_1:
            res = sl_eps2_read_reg(config, SL_EPS2_REG_BAT_HEATER_1_DUTY_CYCLE, &buf);

            *val = (sl_eps2_duty_cycle_t)buf;

            break;
        case SL_EPS2_HEATER_2:
            res = sl_eps2_read_reg(config, SL_EPS2_REG_BAT_HEATER_2_DUTY_CYCLE, &buf);

            *val = (sl_eps2_duty_cycle_t)buf;

            break;
        default:
            break;
    }

    return res;
}

int sl_eps2_read_hardware_version(sl_eps2_config_t config, uint8_t *val)
{
    uint32_t buf = UINT32_MAX;

    int res = sl_eps2_read_reg(config, SL_EPS2_REG_HARDWARE_VERSION, &buf);

    *val = (uint8_t)buf;

    return res;
}

int sl_eps2_read_firmware_version(sl_eps2_config_t config, uint32_t *val)
{
    uint32_t buf = UINT32_MAX;

    int res = sl_eps2_read_reg(config, SL_EPS2_REG_FIRMWARE_VERSION, &buf);

    *val = buf;

    return res;
}

int sl_eps2_set_mppt_mode(sl_eps2_config_t config, uint8_t mppt, uint8_t mode)
{
    int err = 0;

    switch(mode)
    {
        case SL_EPS2_MPPT_MODE_AUTOMATIC:   break;
        case SL_EPS2_MPPT_MODE_MANUAL:      break;
        default:
            err = -1;   /* Invalid mode */

            break;
    }

    if (err == 0)
    {
        switch(mppt)
        {
            case SL_EPS2_MPPT_1:
                err = sl_eps2_write_reg(config, SL_EPS2_REG_MPPT_1_MODE, (uint32_t)mode);

                break;
            case SL_EPS2_MPPT_2:
                err = sl_eps2_write_reg(config, SL_EPS2_REG_MPPT_2_MODE, (uint32_t)mode);

                break;
            case SL_EPS2_MPPT_3:
                err = sl_eps2_write_reg(config, SL_EPS2_REG_MPPT_3_MODE, (uint32_t)mode);

                break;
            default:
                err = -1;   /* Invalid MPPT */

                break;
        }
    }

    return err;
}

int sl_eps2_get_mppt_mode(sl_eps2_config_t config, uint8_t mppt, uint8_t *val)
{
    int res = -1;

    uint32_t buf = UINT32_MAX;

    switch(mppt)
    {
        case SL_EPS2_MPPT_1:
            res = sl_eps2_read_reg(config, SL_EPS2_REG_MPPT_1_MODE, &buf);

            *val = (uint8_t)buf;

            break;
        case SL_EPS2_MPPT_2:
            res = sl_eps2_read_reg(config, SL_EPS2_REG_MPPT_2_MODE, &buf);

            *val = (uint8_t)buf;

            break;
        case SL_EPS2_MPPT_3:
            res = sl_eps2_read_reg(config, SL_EPS2_REG_MPPT_3_MODE, &buf);

            *val = (uint8_t)buf;

            break;
        default:
            res = -1;   /* Invalid MPPT */

            break;
    }

    return res;
}

int sl_eps2_set_heater_mode(sl_eps2_config_t config, uint8_t heater, uint8_t mode)
{
    int err = 0;

    switch(mode)
    {
        case SL_EPS2_HEATER_MODE_AUTOMATIC:     break;
        case SL_EPS2_HEATER_MODE_MANUAL:        break;
        default:
            err = -1;   /* Invalid mode */

            break;
    }

    if (err == 0)
    {
        switch(heater)
        {
            case SL_EPS2_HEATER_1:
                err = sl_eps2_write_reg(config, SL_EPS2_REG_BAT_HEATER_1_MODE, (uint32_t)mode);

                break;
            case SL_EPS2_HEATER_2:
                err = sl_eps2_write_reg(config, SL_EPS2_REG_BAT_HEATER_2_MODE, (uint32_t)mode);

                break;
            default:
                err = -1;   /* Invalid heater */

                break;
        }
    }

    return err;
}

int sl_eps2_get_heater_mode(sl_eps2_config_t config, uint8_t heater, uint8_t *val)
{
    int res = -1;

    uint32_t buf = UINT32_MAX;

    switch(heater)
    {
        case SL_EPS2_HEATER_1:
            res = sl_eps2_read_reg(config, SL_EPS2_REG_BAT_HEATER_1_MODE, &buf);

            *val = (uint8_t)buf;

            break;
        case SL_EPS2_HEATER_2:
            res = sl_eps2_read_reg(config, SL_EPS2_REG_BAT_HEATER_2_MODE, &buf);

            *val = (uint8_t)buf;

            break;
        default:
            break;
    }

    return res;
}

static uint8_t sl_eps2_crc8(uint8_t *data, uint8_t len)
{
    uint8_t crc = SL_EPS2_CRC8_INITIAL_VALUE;

    uint8_t i = 0U;
    for(i = 0; i < len; i++)
    {
        crc ^= data[i];

        uint8_t j = 0U;
        for (j = 0U; j < 8U; j++)
        {
            crc = (crc << 1) ^ ((crc & 0x80U) ? SL_EPS2_CRC8_POLYNOMIAL : 0U);
        }

        crc &= 0xFFU;
    }

    return crc;
}

static bool sl_eps2_check_crc(uint8_t *data, uint8_t len, uint8_t crc)
{
    return (crc == sl_eps2_crc8(data, len));
}

/** \} End of sl_eps2 group */
