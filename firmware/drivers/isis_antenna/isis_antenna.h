/*
 * isis_antenna.h
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with OBDH 2.0. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief ISIS antenna driver definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.8.3
 * 
 * \date 2020/02/01
 * 
 * \defgroup isis_antenna ISIS antenna
 * \ingroup drivers
 * \{
 */

#ifndef ISIS_ANTENNA_H_
#define ISIS_ANTENNA_H_

#include <stdint.h>
#include <stdbool.h>

#include <drivers/i2c/i2c.h>

#define ISIS_ANTENNA_MODULE_NAME                "ISIS Antenna"

/* I2C addresses */
#define ISIS_ANTENNA_I2C_SLAVE_ADDRESS_UC_A     0x31
#define ISIS_ANTENNA_I2C_SLAVE_ADDRESS_UC_B     0x32

/* Status mask */
#define ISIS_ANTENNA_STATUS_MASK                0x8888U     /**< Status mask (all antenna not deployed and disarmed). */

#define ISIS_ANTENNA_REF_VOLTAGE                3300        /**< Reference voltage in millivolts. */

#define ISIS_ANTENNA_MIN_TEMP                   (-50)       /**< Minimum read temperature. */
#define ISIS_ANTENNA_MAX_TEMP                   132         /**< Maximum read temperature. */

#define ISIS_ANTENNA_TEMP_MIN_VOUT              630         /**< Minimum output voltage of the temperature sensor. */
#define ISIS_ANTENNA_TEMP_MAX_VOUT              2616        /**< Maximum output voltage of the temperature sensor. */

/**
 * \brief Temperature type.
 */
typedef uint16_t isis_antenna_temp_t;

/**
 * \brief Antennas number.
 */
typedef enum
{
    ISIS_ANTENNA_ANT_1 = 1,                                 /**< Antenna number 1. */
    ISIS_ANTENNA_ANT_2,                                     /**< Antenna number 2. */
    ISIS_ANTENNA_ANT_3,                                     /**< Antenna number 3. */
    ISIS_ANTENNA_ANT_4                                      /**< Antenna number 4. */
} isis_antenna_ant_t;

/**
 * \brief ISIS antenna independent deployment override options.
 */
typedef enum
{
    ISIS_ANTENNA_INDEPENDENT_DEPLOY_WITHOUT_OVERRIDE = 0,   /**< Independent deployment without override. */
    ISIS_ANTENNA_INDEPENDENT_DEPLOY_WITH_OVERRIDE           /**< Independent deployment with override. */
} isis_antenna_override_t;

/**
 * \brief Deployment status.
 */
typedef enum
{
    ISIS_ANTENNA_STATUS_DEPLOYED = 0,                       /**< The antenna is deployed. */
    ISIS_ANTENNA_STATUS_NOT_DEPLOYED                        /**< The antenna is not deployed. */
} isis_antenna_deployment_status_e;

/**
 * \brief Deployment stop causes.
 */
typedef enum
{
    ISIS_ANTENNA_OTHER_CAUSE = 0,                           /**< Deployment stopped by timeout. */
    ISIS_ANTENNA_TIMEOUT_CAUSE                              /**< Deployment stopped by other cause. */
} isis_antenna_stop_cause_e;

/**
 * \brief Burn system status.
 */
typedef enum
{
    ISIS_ANTENNA_BURN_INACTIVE = 0,                         /**< Burn system inactive. */
    ISIS_ANTENNA_BURN_ACTIVE                                /**< Burn system active. */
} isis_antenna_burn_system_status_e;

/**
 * \brief Data of a single antenna.
 */
typedef struct
{
    uint8_t status;                                         /**< Antenna status. */
    uint8_t timeout;                                        /**< Antenna timeout flag. */
    uint8_t burning;                                        /**< Antenna burning flag. */
} isis_antenna_single_antenna_data_t;

/**
 * \brief ISIS antenna status bits.
 */
typedef struct
{
    uint16_t code;                                          /**< Status code. */
    isis_antenna_single_antenna_data_t antenna_1;           /**< Antenna 1 status. */
    isis_antenna_single_antenna_data_t antenna_2;           /**< Antenna 2 status. */
    isis_antenna_single_antenna_data_t antenna_3;           /**< Antenna 3 status. */
    isis_antenna_single_antenna_data_t antenna_4;           /**< Antenna 4 status. */
    uint8_t ignoring_switches;                              /**< Ignoring switches. */
    uint8_t independent_burn;                               /**< Independent burn. */
    uint8_t armed;                                          /**< Armed. */
} isis_antenna_status_t;

/**
 * \brief ISIS Antenna data.
 */
typedef struct
{
    isis_antenna_status_t status;                           /**< Status data. */
    isis_antenna_temp_t temperature;                        /**< Temperature in K. */
} isis_antenna_data_t;

/**
 * \brief Driver initialization.
 *
 * \return The status/error code.
 */
int isis_antenna_init(void);

/**
 * \brief Arm the antenna module.
 *
 * \return The status/error code.
 */
int isis_antenna_arm(void);

/**
 * \brief Disarm the antenna module.
 *
 * \return The status/error code.
 */
int isis_antenna_disarm(void);

/**
 * \brief Executes a sequential deployment.
 *
 * \param[in] sec is the sequential deployment total time in seconds.
 *
 * \return The status/error code.
 */
int isis_antenna_start_sequential_deploy(uint8_t sec);

/**
 * \brief 
 *
 * \param[in] ant is the antenna number:
 * \parblock
 *      -\b ISIS_ANTENNA_ANT_1
 *      -\b ISIS_ANTENNA_ANT_2
 *      -\b ISIS_ANTENNA_ANT_3
 *      -\b ISIS_ANTENNA_ANT_4
 *      .
 * \endparblock
 *
 * \param[in] sec
 *
 * \param[in] ovr is the override option:
 * \parblock
 *      -\b ISIS_ANTENNA_INDEPENDENT_DEPLOYMENT_WITHOUT_OVERRIDE
 *      -\b ISIS_ANTENNA_INDEPENDENT_DEPLOYMENT_WITH_OVERRIDE
 *      .
 * \endparblock
 *
 * \return None.
 */
int isis_antenna_start_independent_deploy(isis_antenna_ant_t ant, uint8_t sec, isis_antenna_override_t ovr);

/**
 * \brief Reads the deployment status code.
 *
 * Report the deployment status of the antenna system. This status contains
 * information for each antenna as well as system level information.
 *
 * \param[in,out] status is a pointer to store the deployment status code (2 bytes).
 *
 * \return The status/error code.
 */
int isis_antenna_read_deployment_status_code(uint16_t *status);

/**
 * \brief Reads the deployment status.
 *
 * Send a command through I2C to read the deploy switches to know if the antennas were deployed.
 *
 * \param[in,out] status is a pointer to store the deployment status structure.
 *
 * \return The status/error code.
 */
int isis_antenna_read_deployment_status(isis_antenna_status_t *status);

/**
 * \brief Gets the data from the antenna.
 *
 * \param[in,out] data is a pointer to store the read data.
 *
 * \return The status/errpr code.
 */
int isis_antenna_get_data(isis_antenna_data_t *data);

/**
 * \brief Gets the status of antenna.
 *
 * \param[in] ant is the antenna to get the status. It can be:
 * \parblock
 *      -\b ISIS_ANTENNA_ANT_1
 *      -\b ISIS_ANTENNA_ANT_2
 *      -\b ISIS_ANTENNA_ANT_3
 *      -\b ISIS_ANTENNA_ANT_4
 *      .
 * \endparblock
 *
 * \param[in,out] ant_status is a pointer to store the given antenna status. It can be:
 * \parblock
 *      -\b ISIS_ANTENNA_STATUS_NOT_DEPLOYED
 *      -\b ISIS_ANTENNA_STATUS_DEPLOYED
 *      .
 * \endparblock
 *
 * \return The status/error code.
 */
int isis_antenna_get_antenna_status(isis_antenna_ant_t ant, uint8_t *ant_status);

/**
 * \brief Gets the timeout status of an antenna.
 *
 * \param[in] ant is the antenna to get the timeout flag. It can be:
 * \parblock
 *      -\b ISIS_ANTENNA_ANT_1
 *      -\b ISIS_ANTENNA_ANT_2
 *      -\b ISIS_ANTENNA_ANT_3
 *      -\b ISIS_ANTENNA_ANT_4
 *      .
 * \endparblock
 *
 * \param[in,out] is a pointer to store the timeout flag. It can be:
 * \parblock
 *      -\b ISIS_ANTENNA_TIMEOUT_CAUSE
 *      -\b ISIS_ANTENNA_OTHER_CAUSE
 *      .
 * \endparblock
 *
 * \return The status/error code.
 */
int isis_antenna_get_antenna_timeout(isis_antenna_ant_t ant, uint8_t *ant_timeout);

/**
 * \brief Gets the burn system status.
 *
 * \param[in] ant is the antenna to get the burn system status. It can be:
 * \parblock
 *      -\b ISIS_ANTENNA_ANT_1
 *      -\b ISIS_ANTENNA_ANT_2
 *      -\b ISIS_ANTENNA_ANT_3
 *      -\b ISIS_ANTENNA_ANT_4
 *      .
 * \endparblock
 *
 * \param[in,out] ant_burn is a pointer to store the antenna burning flag. It can be:
 * \parblock
 *      -\b ISIS_ANTENNA_BURN_ACTIVE
 *      -\b ISIS_ANTENNA_BURN_INACTIVE
 *      .
 * \endparblock
 *
 * \return The status/error code.
 */
int isis_antenna_get_burning(isis_antenna_ant_t ant, uint8_t *ant_burn);

/**
 * \brief Gets the arming status of the antennas.
 *
 * \param[in,out] armed is a pointer to store the antenna is armed flag.
 *
 * \return The status/error code.
 */
int isis_antenna_get_arming_status(bool *armed);

/**
 * \brief Gets the current raw temperature value.
 *
 * \param[in,out] temp is a pointer to store the raw temperature.
 *
 * \return The status/error code.
 */
int isis_antenna_get_raw_temperature(uint16_t *temp);

/**
 * \brief Converts a raw temperature reading to a real temperature.
 *
 * \param[in] raw is the raw temperature to convert.
 *
 * \return The converted temperature in Celsius.
 */
int16_t isis_antenna_raw_to_temp_c(uint16_t raw);

/**
 * \brief Gets the temperature of the antenna module in Celsius.
 *
 * \param[in,out] temp is a pointer to store the temperature value of the antenna system in Celsius.
 *
 * \return The status/error code.
 */
int isis_antenna_get_temperature_c(int16_t *temp);

/**
 * \brief Gets the temperature of the antenna module in Kelvin.
 *
 * \param[in,out] temp is a pointer to store the temperature value of the antenna system in Kelvin.
 *
 * \return The status/error code
 */
int isis_antenna_get_temperature_k(isis_antenna_temp_t *temp);

/**
 * \brief Initializes the I2C port.
 *
 * \return The status/error code.
 */
int isis_antenna_i2c_init(void);

/**
 * \brief Writes a given sequence of bytes to the I2C bus.
 *
 * \param[in] data is array of bytes to write.
 *
 * \param[in] len is the number of bytes to write .
 *
 * \return The status/error code.
 */
int isis_antenna_i2c_write(uint8_t *data, uint16_t len);

/**
 * \brief Reads data from the I2C bus.
 *
 * \param[in] data is a pointer to store the read bytes.
 *
 * \param[in] len is the number of bytes to read.
 *
 * \return The status/error code.
 */
int isis_antenna_i2c_read(uint8_t *data, uint16_t len);

/**
 * \brief Seconds delay.
 *
 * \param[in] s is the delay in seconds.
 *
 * \return None.
 */
void isis_antenna_delay_s(uint8_t s);

/**
 * \brief Milliseconds delay.
 *
 * \param[in] ms is the delay in milliseconds.
 *
 * \return None.
 */
void isis_antenna_delay_ms(uint16_t ms);

#endif /* ANTENNA_H_ */

/** \} End of isis_antenna group */
