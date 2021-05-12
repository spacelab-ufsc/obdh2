/*
 * sl_ttc2.h
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
 * \brief SpaceLab TTC 2.0 driver definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.6.13
 * 
 * \date 2021/05/12
 * 
 * \defgroup sl_ttc2 SpaceLab TTC 2.0
 * \ingroup drivers
 * \{
 */

#ifndef SL_TTC2_H_
#define SL_TTC2_H_

#include <stdint.h>

#include <drivers/spi/spi.h>

/* TTC 2.0 IDs */
#define SL_TTC2_DEVICE_ID_RADIO_1               0xCC2A  /**< TTC 2.0 device ID (radio 1). */
#define SL_TTC2_DEVICE_ID_RADIO_2               0xCC2B  /**< TTC 2.0 device ID (radio 2). */

/* TTC 2.0 registers */
#define SL_TTC2_REG_TIME_COUNTER_MS             0       /**< Time counter in millseconds. */
#define SL_TTC2_REG_UC_TEMPERATURE_K            1       /**< Temperature of the uC in K. */

/**
 * \brief Temperature type.
 */
typedef uint16_t sl_ttc2_temp_t;

/**
 * \brief Voltage type.
 */
typedef uint16_t sl_ttc2_voltage_t;

/**
 * \brief Current type.
 */
typedef uint16_t sl_ttc2_current_t;

/**
 * \brief Voltage types.
 */
typedef enum
{
    SL_TTC2_VOLTAGE_MCU=0,                              /**< MCU input voltage. */
    SL_TTC2_VOLTAGE_RADIO                               /**< Radio input voltage. */
} sl_ttc2_voltage_e;

/**
 * \brief Current types.
 */
typedef enum
{
    SL_TTC2_CURRENT_MCU=0,                              /**< MCU input current. */
    SL_TTC2_CURRENT_RADIO                               /**< Radio input current. */
} sl_ttc2_current_e;

/**
 * \brief Temperature types.
 */
typedef enum
{
    SL_TTC2_TEMP_MCU=0,                                 /**< MCU temperature. */
    SL_TTC2_TEMP_RADIO                                  /**< Radio temperature. */
} sl_ttc2_temp_e;

/**
 * \brief Data structure.
 */
typedef struct
{
    uint32_t                time_counter;
    uint16_t                reset_counter;
    uint8_t                 last_reset_cause;
    sl_ttc2_voltage_t       voltage_mcu;
    sl_ttc2_current_t       current_mcu;
    sl_ttc2_temp_t          temperature_mcu;
    sl_ttc2_voltage_t       voltage_radio;
    sl_ttc2_current_t       current_radio;
    sl_ttc2_temp_t          temperature_radio
} sl_ttc2_hk_data_t;

/**
 * \brief TTC 2.0 configuration type.
 */
typedef struct
{
    spi_port_t port;                /**< SPI port. */
    spi_cs_t cs_pin;                /**< SPI CS pin. */
    spi_config_t port_config;       /**< SPI configuration. */
    uint16_t id;                    /**< Device ID (radio 1 or 2). */
} sl_ttc2_config_t;

/**
 * \brief Initialization of the TTC module driver.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \return The status/error code.
 */
int sl_ttc2_init(sl_ttc2_config_t config);

/**
 * \brief Verifies the TTC ID code.
 *
 * This function is useful to test the connection with the TTC module.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \return The status/error code.
 */
int sl_ttc2_check_device(sl_ttc2_config_t config);

/**
 * \brief Writes a value to a register of the TTC module.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in] adr is the register address to write.
 *
 * \param[in,out] val is the value to write to the given register.
 *
 * \return The status/error code.
 */
int sl_ttc2_write_reg(sl_ttc2_config_t config, uint8_t adr, uint32_t val);

/**
 * \brief Reads a register from the TTC module.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in] adr is the register address to read.
 *
 * \param[in,out] val is a pointer to store the read value from the given register.
 *
 * \return The status/error code.
 */
int sl_ttc2_read_reg(sl_ttc2_config_t config, uint8_t adr, uint32_t *val);

/**
 * \brief Reads all the TTC variables and parameters.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in,out] data is a pointer to store the read TTC data.
 *
 * \return The status/error code.
 */
int sl_ttc2_read_hk_data(sl_ttc2_config_t config, sl_ttc2_hk_data_t *data);

/**
 * \brief Reads the time counter value.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in,out] val is a pointer to store the read time counter value.
 *
 * \return The status/error code.
 */
int sl_ttc2_read_time_counter(sl_ttc2_config_t config, uint32_t *val);

/**
 * \brief Reads the input voltage of the TTC module.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in] volt is the voltage type to read. It can be:
 * \parblock
 *      -\b SL_TTC2_VOLTAGE_MCU
 *      -\b SL_TTC2_VOLTAGE_RADIO
 *      .
 * \endparblock
 *
 * \param[in,out] val .
 *
 * \note The unit of the read current is \b mV.
 *
 * \return The status/error code.
 */
int sl_ttc2_read_voltage(sl_ttc2_config_t config, uint8_t volt, sl_ttc2_voltage_t *val);

/**
 * \brief Reads the input current of the TTC module.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in] cur is current type to read. It can be:
 * \parblock
 *      -\b SL_TTC2_CURRENT_MCU
 *      -\b SL_TTC2_CURRENT_RADIO
 *      .
 * \endparblock
 *
 * \param[in,out] val is a pointer to store the read current.
 *
 * \note The unit of the read current is \b mA.
 *
 * \return The status/error code.
 */
int sl_ttc2_read_current(sl_ttc2_config_t config, uint8_t cur, sl_ttc2_current_t *val);

/**
 * \brief Reads the temperature of  the TTC microncontroller.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in] temp is the temperature type to read. It can be:
 * \parblock
 *      -\b SL_TTC2_TEMP_MCU
 *      -\b SL_TTC2_TEMP_RADIO
 *      .
 * \endparblock
 *
 * \param[in,out] val is a pointer to store the read temperature.
 *
 * \note The unit ot the read temperature is Kelvin.
 *
 * \return The status/error code.
 */
int sl_ttc2_read_temp(sl_ttc2_config_t config, uint8_t temp, sl_ttc2_temp_t *val);

/**
 * \brief Reads the last reset cause ID of the TTC.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in,out] val is a pointer to store the read reset cause ID.
 *
 * \return The status/error code.
 */
int sl_ttc2_read_reset_cause(sl_ttc2_config_t config, uint8_t *val);

/**
 * \brief Reads the reset counter value of the TTC module.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in,out] val is a pointer to store the read reset counter value.
 *
 * \return The status/error code.
 */
int sl_ttc2_read_reset_counter(sl_ttc2_config_t config, uint16_t *val);

/**
 * \brief Reads the hardware version of the TTC 2.0 module.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in,out] val is a pointer to store the read hardware version.
 *
 * \return The status/error code.
 */
int sl_ttc2_read_hardware_version(sl_ttc2_config_t config, uint8_t *val);

/**
 * \brief Reads the firmware version of the TTC 2.0 module.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in,out] val is a pointer to store the read firmware version.
 *
 * \return The status/error code.
 */
int sl_ttc2_read_firmware_version(sl_ttc2_config_t config, uint32_t *val);

/**
 * \brief Milliseconds delay.
 *
 * \param[in] ms is the time to delay in milliseconds.
 *
 * \return None.
 */
void sl_ttc2_delay_ms(uint32_t ms);

#endif /* SL_TTC2_H_ */

/** \} End of sl_ttc2 group */
