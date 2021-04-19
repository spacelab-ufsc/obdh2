/*
 * mt25q.h
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
 * \brief MT25Q driver definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.5.26
 * 
 * \date 2019/11/15
 * 
 * \defgroup mt25q MT25Q
 * \ingroup drivers
 * \{
 */

#ifndef MT25Q_H_
#define MT25Q_H_

#include <stdint.h>
#include <stdbool.h>

#define MT25Q_MODULE_NAME       "MT25Q"

/**
 * \brief Device ID structure.
 */
typedef struct
{
    uint8_t manufacturer_id;
    uint8_t memory_type;
    uint8_t memory_capacity;
} mt25q_dev_id_t;

/**
 * \brief Driver initialization.
 *
 * \return The error/status code.
 */
int mt25q_init(void);

/**
 * \brief Reads the device ID information.
 *
 * \param[in,out] dev_id is a pointer to store the read device ID.
 *
 * \return The error/status code.
 */
int mt25q_read_device_id(mt25q_dev_id_t *dev_id);

/**
 * \brief Writes data to a given address.
 *
 * \param[in] adr is the address to write.
 *
 * \param[in] data is an array of bytes to write.
 *
 * \param[in] len is the number of bytes to write.
 *
 * \return Error/status code.
 */
int mt25q_write(uint32_t adr, uint8_t *data, uint32_t len);

/**
 * \brief Reads data from a given address.
 *
 * \param[in] adr is the address to write.
 *
 * \param[in] data is an array of bytes to write.
 *
 * \param[in] len is the number of bytes to write.
 *
 * \return Error/status code.
 */
int mt25q_read(uint32_t adr, uint8_t *data, uint32_t len);

/**
 * \brief SPI interface initialization.
 *
 * \return Error/status code.
 */
int mt25q_spi_init(void);

/**
 * \brief Writes the device using the SPI interface.
 *
 * \param[in] data is an array of bytes to write.
 *
 * \param[in] len is the number of bytes to write.
 *
 * \return Error/status code.
 */
int mt25q_spi_write(uint8_t *data, uint16_t len);

/**
 * \brief Reads the device using the SPI interface.
 *
 * \param[in] data is an array to store the read bytes.
 *
 * \param[in] len is the number of bytes to write.
 *
 * \return Error/status code.
 */
int mt25q_spi_read(uint8_t *data, uint16_t len);

/**
 * \brief SPI transfer operation (write and/or read).
 *
 * \param[in,out] wdata is a pointer to the data to be written during the SPI transfer.
 *
 * \param[in,out] rdata is a pointer to store the read data during the SPI transfer.
 *
 * \param[in] len is the number of bytes of the transfer operation.
 *
 * \return Error/status code.
 */
int mt25q_spi_transfer(uint8_t *wdata, uint8_t *rdata, uint16_t len);

/**
 * \brief Initialization of the GPIO pins.
 *
 * This function initializes the pins "HOLD" and "RESET".
 *
 * \return The error/status code.
 */
int mt25q_gpio_init(void);

/**
 * \brief Sets the state of the hold pin.
 *
 * \param[in] state is the state to set the hold pin (high/low or true/false).
 *
 * \return The error/status code.
 */
int mt25q_gpio_set_hold(bool state);

/**
 * \brief Sets the state of the reset pin.
 *
 * \param[in] state is the state to set the reset pin (high/low or true/false).
 *
 * \return The error/status code.
 */
int mt25q_gpio_set_reset(bool state);

/**
 * \brief Milliseconds delay.
 *
 * \param[in] ms is the delay period in milliseconds.
 *
 * \return None.
 */
void mt25q_delay_ms(uint32_t ms);

#endif /* MT25Q_H_ */

/** \} End of mt25q group */
