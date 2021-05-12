/*
 * sl_ttc2.c
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
 * \brief SpaceLab TTC 2.0 driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.6.13
 * 
 * \date 2021/05/12
 * 
 * \addtogroup sl_ttc2
 * \{
 */

#include <stdbool.h>

#include <drivers/tca4311a/tca4311a.h>
#include <drivers/i2c/i2c.h>
#include <drivers/gpio/gpio.h>

#include "sl_ttc2.h"

#define SL_TTC2_CRC8_INITIAL_VALUE          0       /**< CRC8-CCITT initial value. */
#define SL_TTC2_CRC8_POLYNOMIAL             0x07    /**< CRC8-CCITT polynomial. */

/**
 * \brief Computes the CRC-8 of a sequence of bytes.
 *
 * \param[in] data is an array of data to compute the CRC-8.
 *
 * \param[in] len is the number of bytes of the given array.
 *
 * \return The computed CRC-8 value of the given data.
 */
uint8_t sl_ttc2_crc8(uint8_t *data, uint8_t len);

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
bool sl_ttc2_check_crc(uint8_t *data, uint8_t len, uint8_t crc);

int sl_ttc2_init(sl_ttc2_config_t config)
{
    return -1;
}

int sl_ttc2_check_device(sl_ttc2_config_t config)
{
    return -1;
}

int sl_ttc2_write_reg(sl_ttc2_config_t config, uint8_t adr, uint32_t val)
{
    return -1;
}

int sl_ttc2_read_reg(sl_ttc2_config_t config, uint8_t adr, uint32_t *val)
{
    return -1;
}

int sl_ttc2_read_hk_data(sl_ttc2_config_t config, sl_ttc2_hk_data_t *data)
{
    return -1;
}

int sl_ttc2_read_time_counter(sl_ttc2_config_t config, uint32_t *val)
{
    return -1;
}

int sl_ttc2_read_voltage(sl_ttc2_config_t config, uint8_t volt, sl_ttc2_voltage_t *val)
{
    return -1;
}

int sl_ttc2_read_current(sl_ttc2_config_t config, uint8_t cur, sl_ttc2_current_t *val)
{
    return -1;
}

int sl_ttc2_read_temp(sl_ttc2_config_t config, uint8_t temp, sl_ttc2_temp_t *val)
{
    return -1;
}

int sl_ttc2_read_reset_cause(sl_ttc2_config_t config, uint8_t *val)
{
    return -1;
}

int sl_ttc2_read_reset_counter(sl_ttc2_config_t config, uint16_t *val)
{
    return -1;
}

int sl_ttc2_read_hardware_version(sl_ttc2_config_t config, uint8_t *val)
{
    return -1;
}

int sl_ttc2_read_firmware_version(sl_ttc2_config_t config, uint32_t *val)
{
    return -1;
}

uint8_t sl_ttc2_crc8(uint8_t *data, uint8_t len)
{
    uint8_t crc = SL_TTC2_CRC8_INITIAL_VALUE;

    while(len--)
    {
        crc ^= *data++;

        uint8_t j = 0;
        for (j=0; j<8; j++)
        {
            crc = (crc << 1) ^ ((crc & 0x80)? SL_TTC2_CRC8_POLYNOMIAL : 0);
        }

        crc &= 0xFF;
    }

    return crc;
}

bool sl_ttc2_check_crc(uint8_t *data, uint8_t len, uint8_t crc)
{
    if (crc != sl_ttc2_crc8(data, len))
    {
        return false;
    }

    return true;
}

/** \} End of sl_ttc2 group */
