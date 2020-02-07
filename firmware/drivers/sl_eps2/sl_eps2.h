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
 * \version 0.1.16
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

#define SL_EPS2_SLAVE_ADR           0x36    /**< 7-bit slave address. */

#define SL_EPS2_DEVICE_ID           0x43    /**< EPS 2.0 device ID. */

// EPS 2.0 registers
#define SL_EPS2_REG_DEVICE_ID       0x00    /**< Device ID register address. */

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
 * \param[in] val is the value to write to the given register.
 *
 * \return The status/error code.
 */
int sl_eps2_write_reg(uint8_t adr, uint32_t val);

/**
 * \brief Reads a register from the EPS module.
 *
 * \param[in] adr is the register address to read.
 *
 * \param[in] val is a pointer to store the read value from the given register.
 *
 * \return The status/error code.
 */
int sl_eps2_read_reg(uint8_t adr, uint32_t *val);

#endif // SL_EPS2_H_

//! \} End of sl_eps2 group
