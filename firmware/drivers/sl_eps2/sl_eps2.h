/*
 * sl_eps2.h
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
 * \brief SpaceLab EPS 2.0 driver definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.35
 * 
 * \date 2020/02/01
 * 
 * \defgroup sl_eps2 SpaceLab EPS 2.0
 * \ingroup drivers
 * \{
 */

#ifndef SL_EPS2_H_
#define SL_EPS2_H_

#include <stdint.h>

#include <drivers/tca4311a/tca4311a.h>

#define SL_EPS2_SLAVE_ADR                       0x36    /**< 7-bit slave address. */

#define SL_EPS2_DEVICE_ID                       0xEEE2U /**< EPS 2.0 device ID. */

/* EPS 2.0 registers */
#define SL_EPS2_REG_TIME_COUNTER_MS             0       /**< Time counter in millseconds. */
#define SL_EPS2_REG_UC_TEMPERATURE_K            1       /**< Temperature of the uC in K. */
#define SL_EPS2_REG_CURRENT_MA                  2       /**< EPS circuitry and Beacon MCU current in mA. */
#define SL_EPS2_REG_LAST_RESET_CAUSE            3       /**< Last reset cause. */
#define SL_EPS2_REG_RESET_COUNTER               4       /**< Reset counter. */
#define SL_EPS2_REG_SOLAR_PANEL_MY_PX_VOLT_MV   5       /**< Solar panel -Y +X voltage in mV. */
#define SL_EPS2_REG_SOLAR_PANEL_MX_PZ_VOLT_MV   6       /**< Solar panel -X +Z voltage in mV. */
#define SL_EPS2_REG_SOLAR_PANEL_MZ_PY_VOLT_MV   7       /**< Solar panel -Z +Y voltage in mV. */
#define SL_EPS2_REG_SOLAR_PANEL_MY_CUR_MA       8       /**< Solar panel +X current in mA. */
#define SL_EPS2_REG_SOLAR_PANEL_PY_CUR_MA       9       /**< Solar panel -X current in mA. */
#define SL_EPS2_REG_SOLAR_PANEL_MX_CUR_MA       10      /**< Solar panel +Y current in mA. */
#define SL_EPS2_REG_SOLAR_PANEL_PX_CUR_MA       11      /**< Solar panel -Y current in mA. */
#define SL_EPS2_REG_SOLAR_PANEL_MZ_CUR_MA       12      /**< Solar panel +Z current in mA. */
#define SL_EPS2_REG_SOLAR_PANEL_PZ_CUR_MA       13      /**< Solar panel -Z current in mA. */
#define SL_EPS2_REG_MPPT_1_DUTY_CYCLE           14      /**< MPPT 1 duty cycle in %. */
#define SL_EPS2_REG_MPPT_2_DUTY_CYCLE           15      /**< MPPT 2 duty cycle in %. */
#define SL_EPS2_REG_MPPT_3_DUTY_CYCLE           16      /**< MPPT 3 duty cycle in %. */
#define SL_EPS2_REG_SOLAR_PANEL_TOTAL_VOLT_MV   17      /**< Total solar panels output voltage after MPPT in mV. */
#define SL_EPS2_REG_MAIN_POWER_BUS_VOLT_MV      18      /**< Main power bus voltage in mV. */
#define SL_EPS2_REG_RTD0_TEMP_K                 19      /**< RTD0 temperature in K. */
#define SL_EPS2_REG_RTD1_TEMP_K                 20      /**< RTD1 temperature in K. */
#define SL_EPS2_REG_RTD2_TEMP_K                 21      /**< RTD2 temperature in K. */
#define SL_EPS2_REG_RTD3_TEMP_K                 22      /**< RTD3 temperature in K. */
#define SL_EPS2_REG_RTD4_TEMP_K                 23      /**< RTD4 temperature in K. */
#define SL_EPS2_REG_RTD5_TEMP_K                 24      /**< RTD5 temperature in K. */
#define SL_EPS2_REG_RTD6_TEMP_K                 25      /**< RTD6 temperature in K. */
#define SL_EPS2_REG_BATTERY_VOLT_MV             26      /**< Battery voltage in mV. */
#define SL_EPS2_REG_BATTERY_CUR_MA              27      /**< Battery current in mA. */
#define SL_EPS2_REG_BATTERY_AVEG_CUR_MA         28      /**< Battery average current in mA. */
#define SL_EPS2_REG_BATTERY_ACC_CUR_MA          29      /**< Battery accumulated current in mA. */
#define SL_EPS2_REG_BATTERY_CHARGE_MAH          30      /**< Battery charge in mAh. */
#define SL_EPS2_REG_BAT_MONITOR_TEMP_K          31      /**< Battery monitor IC temperature in K. */
#define SL_EPS2_REG_BAT_MONITOR_STATUS          32      /**< Battery monitor status register. */
#define SL_EPS2_REG_BAT_MONITOR_PROTECTION      33      /**< Battery monitor protection register. */
#define SL_EPS2_REG_BAT_MONITOR_CYCLE_COUNTER   34      /**< Battery monitor cycle counter. */
#define SL_EPS2_REG_BAT_MONITOR_RAAC_MAH        35      /**< Battery monitor Remaining Active-Absolute Capacity (RAAC) in mAh. */
#define SL_EPS2_REG_BAT_MONITOR_RSAC_MAH        36      /**< Battery monitor Remaining Standby-Absolute Capacity (RSAC) in mAh. */
#define SL_EPS2_REG_BAT_MONITOR_RARC_PERC       37      /**< Battery monitor Remaining Active-Relative Capacity (RARC) in %. */
#define SL_EPS2_REG_BAT_MONITOR_RSRC_PERC       38      /**< Battery monitor Remaining Standby-Relative Capacity (RSRC) in %. */
#define SL_EPS2_REG_BAT_HEATER_1_DUTY_CYCLE     39      /**< Battery heater 1 duty cycle in %. */
#define SL_EPS2_REG_BAT_HEATER_2_DUTY_CYCLE     40      /**< Battery heater 2 duty cycle in %. */
#define SL_EPS2_REG_HARDWARE_VERSION            41      /**< Hardware version. */
#define SL_EPS2_REG_FIRMWARE_VERSION            42      /**< Firmware version (ex.: “v1.2.3”’ = 0x00010203). */
#define SL_EPS2_REG_MPPT_1_MODE                 43      /**< MPPT 1 mode (0x00 = automatic, 0x01 = manual). */
#define SL_EPS2_REG_MPPT_2_MODE                 44      /**< MPPT 2 mode (0x00 = automatic, 0x01 = manual). */
#define SL_EPS2_REG_MPPT_3_MODE                 45      /**< MPPT 3 mode (0x00 = automatic, 0x01 = manual). */
#define SL_EPS2_REG_BAT_HEATER_1_MODE           46      /**< Battery heater 1 mode (0x00 = automatic, 0x01 = manual). */
#define SL_EPS2_REG_BAT_HEATER_2_MODE           47      /**< Battery heater 2 mode (0x00 = automatic, 0x01 = manual). */
#define SL_EPS2_REG_DEVICE_ID                   48      /**< Device ID (0xEEE2). */

/**
 * \brief Solar panels.
 */
typedef enum
{
    SL_EPS2_SOLAR_PANEL_0=0,                            /**< Panel 0. */
    SL_EPS2_SOLAR_PANEL_1,                              /**< Panel 1. */
    SL_EPS2_SOLAR_PANEL_2,                              /**< Panel 2. */
    SL_EPS2_SOLAR_PANEL_3,                              /**< Panel 3. */
    SL_EPS2_SOLAR_PANEL_4,                              /**< Panel 4. */
    SL_EPS2_SOLAR_PANEL_5,                              /**< Panel 5. */
    SL_EPS2_SOLAR_PANEL_3_0,                            /**< Solar panel -Y +X. */
    SL_EPS2_SOLAR_PANEL_1_4,                            /**< Solar panel -X +Z. */
    SL_EPS2_SOLAR_PANEL_5_2,                            /**< Solar panel -Z +Y. */
    SL_EPS2_SOLAR_PANEL_ALL                             /**< All solar panels. */
} sl_eps2_solar_panels_e;

/**
 * \brief Solar panels MPPTs.
 */
typedef enum
{
    SL_EPS2_MPPT_1=0,                                   /**< MPPT 1. */
    SL_EPS2_MPPT_2,                                     /**< MPPT 2. */
    SL_EPS2_MPPT_3                                      /**< MPPT 3. */
} sl_eps2_mppt_e;

/**
 * \brief MPPT modes.
 */
typedef enum
{
    SL_EPS2_MPPT_MODE_AUTOMATIC=0,                      /**< MPPT mode = automatic. */
    SL_EPS2_MPPT_MODE_MANUAL                            /**< MPPT mode = manual. */
} sl_eps2_mppt_mode_e;

/**
 * \brief Battery heaters.
 */
typedef enum
{
    SL_EPS2_HEATER_1=0,                                 /**< Battery heater 1. */
    SL_EPS2_HEATER_2                                    /**< Battery heater 2. */
} sl_eps2_heater_e;

/**
 * \brief Heater modes.
 */
typedef enum
{
    SL_EPS2_HEATER_MODE_AUTOMATIC=0,                    /**< Heater mode = automatic. */
    SL_EPS2_HEATER_MODE_MANUAL                          /**< Heater mode = manual. */
} sl_eps2_heater_mode_e;

/**
 * \brief RTDs.
 */
typedef enum
{
    SL_EPS2_RTD_0=0,                                    /**< RTD 0. */
    SL_EPS2_RTD_1,                                      /**< RTD 1. */
    SL_EPS2_RTD_2,                                      /**< RTD 2. */
    SL_EPS2_RTD_3,                                      /**< RTD 3. */
    SL_EPS2_RTD_4,                                      /**< RTD 4. */
    SL_EPS2_RTD_5,                                      /**< RTD 5. */
    SL_EPS2_RTD_6                                       /**< RTD 6. */
} sl_eps2_rtd_e;

/**
 * \brief Battery current types.
 */
typedef enum
{
    SL_EPS2_BATTERY_CURRENT=0,                          /**< Current battery current. */
    SL_EPS2_BATTERY_AVERAGE_CURRENT,                    /**< Average battery current. */
    SL_EPS2_BATTERY_ACC_CURRENT                         /**< Accumulated battery current. */
} sl_eps2_bat_cur_e;

/**
 * \brief Configuration parameters structure of the driver.
 */
typedef tca4311a_config_t sl_eps2_config_t;

/**
 * \brief Temperature type.
 */
typedef uint16_t sl_eps2_temp_t;

/**
 * \brief Voltage type.
 */
typedef uint16_t sl_eps2_voltage_t;

/**
 * \brief Current type.
 */
typedef uint16_t sl_eps2_current_t;

/**
 * \brief Charge type.
 */
typedef uint16_t sl_eps2_charge_t;

/**
 * \brief Duty cycle type.
 */
typedef uint8_t sl_eps2_duty_cycle_t;

/**
 * \brief Data structure.
 */
typedef struct
{
    uint32_t                time_counter;
    sl_eps2_temp_t          temperature_uc;
    sl_eps2_current_t       current;
    uint8_t                 last_reset_cause;
    uint16_t                reset_counter;
    sl_eps2_voltage_t       solar_panel_voltage_my_px;
    sl_eps2_voltage_t       solar_panel_voltage_mx_pz;
    sl_eps2_voltage_t       solar_panel_voltage_mz_py;
    sl_eps2_current_t       solar_panel_current_my;
    sl_eps2_current_t       solar_panel_current_py;
    sl_eps2_current_t       solar_panel_current_mx;
    sl_eps2_current_t       solar_panel_current_px;
    sl_eps2_current_t       solar_panel_current_mz;
    sl_eps2_current_t       solar_panel_current_pz;
    sl_eps2_duty_cycle_t    mppt_1_duty_cycle;
    sl_eps2_duty_cycle_t    mppt_2_duty_cycle;
    sl_eps2_duty_cycle_t    mppt_3_duty_cycle;
    sl_eps2_voltage_t       solar_panel_output_voltage;
    sl_eps2_voltage_t       main_power_bus_voltage;
    sl_eps2_temp_t          rtd_0_temperature;
    sl_eps2_temp_t          rtd_1_temperature;
    sl_eps2_temp_t          rtd_2_temperature;
    sl_eps2_temp_t          rtd_3_temperature;
    sl_eps2_temp_t          rtd_4_temperature;
    sl_eps2_temp_t          rtd_5_temperature;
    sl_eps2_temp_t          rtd_6_temperature;
    sl_eps2_voltage_t       battery_voltage;
    sl_eps2_current_t       battery_current;
    sl_eps2_current_t       battery_average_current;
    sl_eps2_current_t       battery_acc_current;
    sl_eps2_charge_t        battery_charge;
    sl_eps2_temp_t          battery_monitor_temperature;
    uint8_t                 battery_monitor_status;
    uint8_t                 battery_monitor_protection;
    uint8_t                 battery_monitor_cycle_counter;
    sl_eps2_charge_t        raac;
    sl_eps2_charge_t        rsac;
    uint8_t                 rarc;
    uint8_t                 rsrc;
    sl_eps2_duty_cycle_t    battery_heater_1_duty_cycle;
    sl_eps2_duty_cycle_t    battery_heater_2_duty_cycle;
    uint8_t                 mppt_1_mode;
    uint8_t                 mppt_2_mode;
    uint8_t                 mppt_3_mode;
    uint8_t                 battery_heater_1_mode;
    uint8_t                 battery_heater_2_mode;
} sl_eps2_data_t;

/**
 * \brief Initialization of the EPS module driver.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \return The status/error code.
 */
int sl_eps2_init(sl_eps2_config_t config);

/**
 * \brief Verifies the EPS ID code.
 *
 * This function is useful to test the connection with the EPS module.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \return The status/error code.
 */
int sl_eps2_check_device(sl_eps2_config_t config);

/**
 * \brief Writes a value to a register of the EPS module.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in] adr is the register address to write.
 *
 * \param[in,out] val is the value to write to the given register.
 *
 * \return The status/error code.
 */
int sl_eps2_write_reg(sl_eps2_config_t config, uint8_t adr, uint32_t val);

/**
 * \brief Reads a register from the EPS module.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in] adr is the register address to read.
 *
 * \param[in,out] val is a pointer to store the read value from the given register.
 *
 * \return The status/error code.
 */
int sl_eps2_read_reg(sl_eps2_config_t config, uint8_t adr, uint32_t *val);

/**
 * \brief Reads all the EPS variables and parameters.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in,out] data is a pointe to store the read EPS data.
 *
 * \return The status/error code.
 */
int sl_eps2_read_data(sl_eps2_config_t config, sl_eps2_data_t *data);

/**
 * \brief Reads the time counter value.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in,out] val is a pointer to store the read time counter value.
 *
 * \return The status/error code.
 */
int sl_eps2_read_time_counter(sl_eps2_config_t config, uint32_t *val);

/**
 * \brief Reads the temperature of  the EPS microncontroller.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in,out] val is a pointer to store the read temperature.
 *
 * \note The unit ot the read temperature is Kelvin.
 *
 * \return The status/error code.
 */
int sl_eps2_read_temp(sl_eps2_config_t config, sl_eps2_temp_t *val);

/**
 * \brief Reads the input current (uC circuit) of the EPS module.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in,out] val is a pointer to store the read current.
 *
 * \note The unit of the read current is mA.
 *
 * \return The status/error code.
 */
int sl_eps2_read_current(sl_eps2_config_t config, sl_eps2_current_t *val);

/**
 * \brief Reads the last reset cause ID of the EPS.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in,out] val is a pointer to store the read reset cause ID.
 *
 * \return The status/error code.
 */
int sl_eps2_read_reset_cause(sl_eps2_config_t config, uint8_t *val);

/**
 * \brief Reads the reset counter value of the EPS module.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in,out] val is a pointer to store the read reset counter value.
 *
 * \return The status/error code.
 */
int sl_eps2_read_reset_counter(sl_eps2_config_t config, uint16_t *val);

/**
 * \brief Reads the voltage of a given solar panel set.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in] sp is the solar panel set to read the voltage. It can be:
 * \parblock
 *      -\b SL_EPS2_SOLAR_PANEL_3_0
 *      -\b SL_EPS2_SOLAR_PANEL_1_4
 *      -\b SL_EPS2_SOLAR_PANEL_5_2
 *      -\b SL_EPS2_SOLAR_PANEL_ALL
 *      .
 * \endparblock
 *
 * \param[in,out] val is a pointer to read the solar panel voltage.
 *
 * \note The unit of the read voltage is \b mV.
 *
 * \return The status/error code.
 */
int sl_eps2_read_solar_panel_voltage(sl_eps2_config_t config, uint8_t sp, uint16_t *val);

/**
 * \brief Reads the current of a given solar panel in mA.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in] sp is the solar panel to read the current. It can be:
 * \parblock
 *      -\b SL_EPS2_SOLAR_PANEL_0
 *      -\b SL_EPS2_SOLAR_PANEL_1
 *      -\b SL_EPS2_SOLAR_PANEL_2
 *      -\b SL_EPS2_SOLAR_PANEL_3
 *      -\b SL_EPS2_SOLAR_PANEL_4
 *      -\b SL_EPS2_SOLAR_PANEL_5
 *      .
 * \endparblock
 *
 * \param[in,out] val is a pointer to store the current value of the given solar panel current.
 *
 * \note The unit of the read current is \b mA.
 *
 * \return The status/error code.
 */
int sl_eps2_read_solar_panel_current(sl_eps2_config_t config, uint8_t sp, sl_eps2_current_t *val);

/**
 * \brief Reads the current duty cycle of a given MPPT.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in] mppt is the MPPT to read the duty cycle. It can be:
 * \parblock
 *      -\b SL_EPS2_MPPT_1
 *      -\b SL_EPS2_MPPT_2
 *      -\b SL_EPS2_MPPT_3
 *      .
 * \endparblock
 *
 * \param[in,out] val is a pointer to store the read duty cycle.
 *
 * \return The status/error code.
 */
int sl_eps2_read_mppt_duty_cycle(sl_eps2_config_t config, uint8_t mppt, sl_eps2_duty_cycle_t *val);

/**
 * \brief Reads the voltage (in \b mV) of the main power bus.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in,out] val is a pointer to store the read voltage of the main power bus.
 *
 * \return The status/error code.
 */
int sl_eps2_read_main_bus_voltage(sl_eps2_config_t config, sl_eps2_voltage_t *val);

/**
 * \brief Reads the temperature of a given RTD.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in] rtd is the RTD to read the temperature. It can be:
 * \parblock
 *      -\b SL_EPS2_RTD_0
 *      -\b SL_EPS2_RTD_1
 *      -\b SL_EPS2_RTD_2
 *      -\b SL_EPS2_RTD_3
 *      -\b SL_EPS2_RTD_4
 *      -\b SL_EPS2_RTD_5
 *      -\b SL_EPS2_RTD_6
 *      .
 * \endparblock
 *
 * \param[in,out] val is a pointer to store the read RTD temperature.
 *
 * \note The unit of the read temperature if \b K.
 *
 * \return The status/error code.
 */
int sl_eps2_read_rtd_temperature(sl_eps2_config_t config, uint8_t rtd, sl_eps2_temp_t *val);

/**
 * \brief Reads the current battery voltage in \b mV.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in,out] val is a pointer to store the current value of the battery voltage.
 *
 * \return The status/error code.
 */
int sl_eps2_read_battery_voltage(sl_eps2_config_t config, sl_eps2_voltage_t *val);

/**
 * \brief Reads the current battery current in mA.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in] cur is current type to read. It can be:
 * \parblock
 *      -\b SL_EPS2_BATTERY_CURRENT
 *      -\b SL_EPS2_BATTERY_AVERAGE_CURRENT
 *      -\b SL_EPS2_BATTERY_ACC_CURRENT
 *      .
 * \endparblock
 *
 * \param[in,out] val is a pointer to store the current value of the battery current.
 *
 * \return The status/error code.
 */
int sl_eps2_read_battery_current(sl_eps2_config_t config, uint8_t cur, sl_eps2_current_t *val);

/**
 * \brief Reads the battery charge in \b mAh.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in,out] val is a pointer to store the current value of the battery charge.
 *
 * \return The status/error code.
 */
int sl_eps2_read_battery_charge(sl_eps2_config_t config, sl_eps2_charge_t *val);

/**
 * \brief Reads the battery monitor temperature in \b K.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in,out] val is a pointer to store the read temperature from the battery monitor.
 *
 * \return The status/error code.
 */
int sl_eps2_read_battery_monitor_temp(sl_eps2_config_t config, sl_eps2_temp_t *val);

/**
 * \brief Reads the battery monitor status.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in,out] val is a pointer to store the battery monitor status value.
 *
 * \return The status/error code.
 */
int sl_eps2_read_battery_monitor_status(sl_eps2_config_t config, uint8_t *val);

/**
 * \brief Reads the battery monitor protection register.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in,out] val is a pointer to store the protection register value.
 *
 * \return The status/error code.
 */
int sl_eps2_read_battery_monitor_protection(sl_eps2_config_t config, uint8_t *val);

/**
 * \brief Reads the cycle counter of the battery monitor.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in,out] val is a pointer to store the cycle counter value.
 *
 * \return The status/error code.
 */
int sl_eps2_read_battery_monitor_cycle_counter(sl_eps2_config_t config, uint8_t *val);

/**
 * \brief Reads the battery monitor RAAC value in \b mAh.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in,out] val is a pointer to store the read RAAC value.
 *
 * \return The status/error code.
 */
int sl_eps2_read_battery_monitor_raac(sl_eps2_config_t config, sl_eps2_charge_t *val);

/**
 * \brief Reads the battery monitor RSAC value in \b mAh.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in,out] val is a poiter to store the read RSAC value.
 *
 * \return The status/error code.
 */
int sl_eps2_read_battery_monitor_rsac(sl_eps2_config_t config, sl_eps2_charge_t *val);

/**
 * \brief Reads the battery monitor RARC value.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in,out] val is a pointer to store the read RARC value.
 *
 * \return The status/error code.
 */
int sl_eps2_read_battery_monitor_rarc(sl_eps2_config_t config, uint8_t *val);

/**
 * \brief Reads the battery monitor RSRC value.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in,out] val is a pointer to store the read RSRC value.
 *
 * \return The status/error code.
 */
int sl_eps2_read_battery_monitor_rsrc(sl_eps2_config_t config, uint8_t *val);

/**
 * \brief Reads the current duty cycle of a given heater.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in] heater is the heater to read the duty cycle. It can be:
 * \parblock
 *      -\b SL_EPS2_HEATER_1
 *      -\b SL_EPS2_HEATER_2
 *      .
 * \endparblock
 *
 * \param[in,out] val is a pointer to store the read duty cycle.
 *
 * \return The status/error code.
 */
int sl_eps2_read_heater_duty_cycle(sl_eps2_config_t config, uint8_t heater, sl_eps2_duty_cycle_t *val);

/**
 * \brief Reads the hardware version of the EPS 2.0 module.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in,out] val is a pointer to store the read hardware version.
 *
 * \return The status/error code.
 */
int sl_eps2_read_hardware_version(sl_eps2_config_t config, uint8_t *val);

/**
 * \brief Reads the firmware version of the EPS 2.0 module.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in,out] val is a pointer to store the read firmware version.
 *
 * \return The status/error code.
 */
int sl_eps2_read_firmware_version(sl_eps2_config_t config, uint32_t *val);

/**
 * \brief Sets the operation mode of a given MPPT.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in] mppt is the MPPT to set the operation mode. It can be:
 * \parblock
 *      -\b SL_EPS2_MPPT_1
 *      -\b SL_EPS2_MPPT_2
 *      -\b SL_EPS2_MPPT_3
 *      .
 * \endparblock
 *
 * \param[in] mode is the operation mode to set in the given MPPT. It can be:
 * \parblock
 *      -\b SL_EPS2_MPPT_MODE_AUTOMATIC
 *      -\b SL_EPS2_MPPT_MODE_MANUAL
 *      .
 * \endparblock
 *
 * \return The status/error code.
 */
int sl_eps2_set_mppt_mode(sl_eps2_config_t config, uint8_t mppt, uint8_t mode);

/**
 * \brief Gets the operation mode of a given MPPT.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in] mppt is the MPPT to get the operation mode. It can be:
 * \parblock
 *      -\b SL_EPS2_MPPT_1
 *      -\b SL_EPS2_MPPT_2
 *      -\b SL_EPS2_MPPT_3
 *      .
 * \endparblock
 *
 * \param[in,out] val is a pointer to store the read MPPT mode.
 *
 * \return The status/error code.
 */
int sl_eps2_get_mppt_mode(sl_eps2_config_t config, uint8_t mppt, uint8_t *val);

/**
 * \brief Sets the operation mode of a given heater.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in] heater is the heater to set the operation mode. It can be:
 * \parblock
 *      -\b SL_EPS2_HEATER_1
 *      -\b SL_EPS2_HEATER_2
 *      .
 * \endparblock
 *
 * \param[in] mode is the heater mode to set. It can be:
 * \parblock
 *      -\b SL_EPS2_HEATER_MODE_AUTOMATIC
 *      -\b SL_EPS2_HEATER_MODE_MANUAL
 *      .
 * \endparblock
 *
 * \return The status/error code.
 */
int sl_eps2_set_heater_mode(sl_eps2_config_t config, uint8_t heater, uint8_t mode);

/**
 * \brief Gets the operation mode of a given heater.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in] heater is the heater to get the operation mode. It can be:
 * \parblock
 *      -\b SL_EPS2_HEATER_1
 *      -\b SL_EPS2_HEATER_2
 *      .
 * \endparblock
 *
 * \param[in,out] val is a pointer to store the read opeartion mode of the given heater.
 *
 * \return The status/error code.
 */
int sl_eps2_get_heater_mode(sl_eps2_config_t config, uint8_t heater, uint8_t *val);

/**
 * \brief Milliseconds delay.
 *
 * \param[in] ms is the time to delay in milliseconds.
 *
 * \return None.
 */
void sl_eps2_delay_ms(uint32_t ms);

#endif /* SL_EPS2_H_ */

/** \} End of sl_eps2 group */
