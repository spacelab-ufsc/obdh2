/*
 * sl_eps2.h
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
 * \brief SpaceLab EPS 2.0 driver definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.17
 * 
 * \date 01/02/2020
 * 
 * \defgroup sl_eps2 SpaceLab EPS 2.0
 * \ingroup drivers
 * \{
 */

#ifndef SL_EPS2_H_
#define SL_EPS2_H_

#include <stdint.h>

#define SL_EPS2_SLAVE_ADR                   0x36    /**< 7-bit slave address. */

#define SL_EPS2_DEVICE_ID                   0x43    /**< EPS 2.0 device ID. */

// EPS 2.0 registers
#define SL_EPS2_REG_DEVICE_ID               0x00    /**< Device ID register address. */
#define SL_EPS2_REG_BATTERY_0_VOLTAGE       0x01    /**< Battery 0 voltage. */
#define SL_EPS2_REG_BATTERY_1_VOLTAGE       0x02    /**< Battery 1 voltage. */
#define SL_EPS2_REG_BATTERY_CHARGE          0x03    /**< Battery charge. */
#define SL_EPS2_REG_SOLAR_PANEL_0_CUR       0x04    /**< Solar panel 0 (+X) current. */
#define SL_EPS2_REG_SOLAR_PANEL_1_CUR       0x05    /**< Solar panel 1 (-X) current. */
#define SL_EPS2_REG_SOLAR_PANEL_2_CUR       0x06    /**< Solar panel 2 (+Y) current. */
#define SL_EPS2_REG_SOLAR_PANEL_3_CUR       0x07    /**< Solar panel 3 (-Y) current. */
#define SL_EPS2_REG_SOLAR_PANEL_4_CUR       0x08    /**< Solar panel 4 (+Z) current. */
#define SL_EPS2_REG_SOLAR_PANEL_5_CUR       0x09    /**< Solar panel 5 (-Z) current. */
#define SL_EPS2_REG_SOLAR_PANEL_30_VOLT     0x0A    /**< Solar panel -Y +X voltage. */
#define SL_EPS2_REG_SOLAR_PANEL_14_VOLT     0x0B    /**< Solar panel -X +Z voltage. */
#define SL_EPS2_REG_SOLAR_PANEL_52_VOLT     0x0C    /**< Solar panel -Z +Y voltage. */

/**
 * \brief Battery cells.
 */
typedef enum
{
    SL_EPS2_BATTERY_CELL_0=0,   /**< Cell 0. */
    SL_EPS2_BATTERY_CELL_1      /**< Cell 1. */
} sl_eps2_bat_cells_e;

/**
 * \brief Solar panels.
 */
typedef enum
{
    SL_EPS2_SOLAR_PANEL_0=0,    /**< Panel 0. */
    SL_EPS2_SOLAR_PANEL_1,      /**< Panel 1. */
    SL_EPS2_SOLAR_PANEL_2,      /**< Panel 2. */
    SL_EPS2_SOLAR_PANEL_3,      /**< Panel 3. */
    SL_EPS2_SOLAR_PANEL_4,      /**< Panel 4. */
    SL_EPS2_SOLAR_PANEL_5       /**< Panel 5. */
} sl_eps2_solar_panels_e;

/**
 * \brief Solar panels sets.
 */
typedef enum
{
    SL_EPS2_SOLAR_PANEL_30=0,   /**< Solar panel -Y +X. */
    SL_EPS2_SOLAR_PANEL_14,     /**< Solar panel -X +Z. */
    SL_EPS2_SOLAR_PANEL_52      /**< Solar panel -Z +Y. */
} sl_eps2_solar_panels_sets_e;

/**
 * \brief Initialization of the EPS module driver.
 *
 * \return The status/error code.
 */
int sl_eps2_init();

/**
 * \brief Verifies the EPS ID code.
 *
 * This function is useful to test the connection with the EPS module.
 *
 * \return The status/error code.
 */
int sl_eps2_check_device();

/**
 * \brief Writes a value to a register of the EPS module.
 *
 * \param[in] adr is the register address to write.
 *
 * \param[in,out] val is the value to write to the given register.
 *
 * \return The status/error code.
 */
int sl_eps2_write_reg(uint8_t adr, uint32_t val);

/**
 * \brief Reads a register from the EPS module.
 *
 * \param[in] adr is the register address to read.
 *
 * \param[in,out] val is a pointer to store the read value from the given register.
 *
 * \return The status/error code.
 */
int sl_eps2_read_reg(uint8_t adr, uint32_t *val);

/**
 * \brief Reads the battery voltage.
 *
 * \param[in] bat is the battery cell to read. It can be:
 * \parblock
 *      -\b SL_EPS2_BATTERY_CELL_0
 *      -\b SL_EPS2_BATTERY_CELL_1
 *      .
 * \endparblock
 *
 * \param[in,out] val is a pointer to store the raw value of the given battery voltage.
 *
 * \return The status/error code.
 */
int sl_eps2_read_battery_voltage(uint8_t bat, uint32_t *val);

/**
 * \brief Reads the battery charge.
 *
 * \param[in,out] val is a pointer to store the raw value of the battery charge.
 *
 * \return The status/error code.
 */
int sl_eps2_read_battery_charge(uint32_t *val);

/**
 * \brief Reads the current of a given solar panel.
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
 * \param[in,out] val is a pointer to store the raw value of the given solar panel current.
 *
 * \return The status/error code.
 */
int sl_eps2_read_solar_panel_current(uint8_t sp, uint32_t *val);

/**
 * \brief Reads the voltage of a given solar panel set.
 *
 * \param[sp] sp is the solar panel set to read the voltage. It can be:
 * \parblock
 *      -\b SL_EPS2_SOLAR_PANEL_30
 *      -\b SL_EPS2_SOLAR_PANEL_14
 *      -\b SL_EPS2_SOLAR_PANEL_52
 *      .
 * \endparblock
 *
 * \param[in,out] val is a pointer to store the raw value of the given solar panel voltage.
 *
 * \return The status/error code.
 */
int sl_eps2_read_solar_panel_voltage(uint8_t sp, uint32_t *val);

#endif // SL_EPS2_H_

//! \} End of sl_eps2 group
