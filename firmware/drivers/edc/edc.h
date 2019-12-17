/*
 * edc.h
 * 
 * Copyright (C) 2019, SpaceLab.
 * 
 * This file is part of OBDH 2.0.
 * 
 * OBDH 2.0 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * OBDH 2.0 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with OBDH 2.0. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief EDC driver definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.0
 * 
 * \date 27/10/2019
 * 
 * \defgroup edc EDC
 * \ingroup drivers
 * \{
 */

#ifndef EDC_H_
#define EDC_H_

#include <stdint.h>

// Commands IDs
#define EDC_CMD_RTC_SET             0x01    /**< Sets the RTC time. The parameters is the number of seconds elapsed since J2000 epoch. */
#define EDC_CMD_PTT_POP             0x06    /**< Removes current PTT package from the PTT Package FIFO Buffer, allowing the reading of the next PTT package, if existent. */
#define EDC_CMD_PTT_PAUSE           0x08    /**< Pauses the PTT Decoding Task. At initialization the PTT Task is paused. */
#define EDC_CMD_PTT_RESUME          0x09    /**< Resumes the PTT Decoding Task. */
#define EDC_CMD_SAMPLER_START       0x0A    /**< Start ADC Sample task. This trigger the sampling of a sequence of 2048 I&Q samples from the RF Front End. */
#define EDC_CMD_GET_STATE           0x30    /**< Causes the transmission of System State Frame through RS-485 interface. */
#define EDC_CMD_GET_PTT_PKG         0x31    /**< Causes the transmission of current PTT Decoder Frame through RS-485 interface. */
#define EDC_CMD_GET_HK_PKG          0x32    /**< Updates HK Frame information, and transmit it through the RS-485 interface. */
#define EDC_CMD_GET_ADC_SEQ         0x34    /**< Causes the transmission of current ADC Sampler Frame through RS-485 interface. */
#define EDC_CMD_ECHO                0xF0    /**< Cause the transmission of the string "ECHO" in the debug interface. */

/**
 * \brief EDC command.
 */
typedef struct
{
    uint8_t id;         /**< Command ID. */
    uint32_t param;     /**< Command param. */
} edc_cmd_t;

/**
 * \brief Device initialization.
 *
 * \return The status/error code.
 */
int edc_init();

/**
 * \brief Writes a command to the EDC module.
 *
 * \param[in] cmd is the command to be written.
 *
 * \return The status/error code.
 */
int edc_write_cmd(edc_cmd_t cmd);

/**
 * \brief Reads data from the EDC module.
 *
 * \param[in,out] data is an array to store the read bytes.
 *
 * \param[in] len is the number of bytes to read.
 *
 * \return The status/error code.
 */
int edc_read(uint8_t *data, uint16_t len);

/**
 * \brief UART interface initialization.
 *
 * \return The status/error code.
 */
int edc_uart_init();

/**
 * \brief Writes data to the UART interface.
 *
 * \param[in,out] data is an array with the bytes to be written in the UART interface.
 *
 * \param[in] len is the number of bytes to write.
 *
 * \return The status/error code.
 */
int edc_uart_write(uint8_t *data, uint16_t len);

/**
 * \brief Reads data from the UART interface.
 *
 * \param[in,out] data is an array to store the read bytes.
 *
 * \param[in] len is the number of bytes to read.
 *
 * \return The status/error code.
 */
int edc_uart_read(uint8_t *data, uint16_t len);

#endif // EDC_H_

//! \} End of edc group
