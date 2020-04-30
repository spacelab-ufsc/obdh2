/*
 * si446x.h
 * 
 * Copyright (C) 2019, SpaceLab.
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
 * \brief Si446x driver definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.3.13
 * 
 * \date 01/06/2017
 * 
 * \defgroup si446x Si446x
 * \ingroup drivers
 * \{
 */

#ifndef SI446X_H_
#define SI446X_H_

#include <stdint.h>
#include <stdbool.h>

#define SI446X_MODULE_NAME                      "Si446x"

#define SI446X_PART_INFO                        0x4463
#define SI446X_TX_FIFO_LEN                      128
#define SI446X_RX_FIFO_LEN                      128
#define SI446X_TX_FIFO_ALMOST_EMPTY_THRESHOLD   48
#define SI446X_RX_FIFO_ALMOST_FULL_THRESHOLD    48
#define SI446X_CTS_REPLY                        0xFF
#define SI446X_CTS_TIMEOUT                      2500    /* Waiting time for a valid FFh CTS reading. The typical time is 20 us. */
#define SI446X_TX_TIMEOUT                       20000   /* Waiting time for packet send interrupt. this time is depended on tx length and data rate of wireless. */
#define SI446X_FREQ_CHANNEL                     0       /* Frequency channel. */

/* This value must be obtained measuring the output signal with a frequency analyzer */
/* The register value is tuned according to the desired output frequency */
#define SI446X_XO_TUNE_REG_VALUE                97

/**
 * \brief Si446x modes.
 */
typedef enum
{
    SI446X_MODE_STANDBY=0,      /**< Standby mode. */
    SI446X_MODE_TX,             /**< TX mode. */
    SI446X_MODE_RX              /**< RX mode. */
} si446x_modes_e;

/**
 * \brief Si446x initialization.
 * 
 * Initializes the Si446x module with the configuration parameters from
 * "si446x_reg_config.h".
 * 
 * \return The status/error code.
 */
int8_t si446x_init(void);

/**
 * \brief GPIO initialization.
 * 
 * \return The status/error code.
 */
int8_t si446x_gpio_init(void);

/**
 * \brief Configures the registers of the SI446X device.
 * 
 * All the configuration parameters are defined in the "si446x_reg_config.h" file.
 * 
 * \return None
 */
void si446x_reg_config(void);

/**
 * \brief Power on reset procedure of the SI446X module.
 * 
 * \return None
 */
void si446x_power_on_reset(void);

/**
 * \brief Transmit a packet through RF.
 * 
 * \param data is the data with the packet to send.
 * \param len is the length of the data to send.
 * 
 * \return It can return:
 *              -\b true if the packet was sent.
 *              -\b false if the packet was not sent.
 *              .
 */
bool si446x_tx_packet(uint8_t *data, uint8_t len);

/**
 * \brief Transmit a long packet (> FIFO buffer, or 64 bytes) through RF.
 * 
 * Reference: AN633, Figure 62.
 * 
 * \param packet is the packet to send.
 * \param len is the length of the packet.
 * 
 * \return None
 */
bool si446x_tx_long_packet(uint8_t *packet, uint16_t len);

/**
 * \brief 
 * 
 * \param rx_buf
 * 
 * \param read_len
 *
 * \return 
 */
uint8_t si446x_rx_packet(uint8_t *rx_buf, uint8_t read_len);

/**
 * \brief 
 * 
 * \return It can return:
 *              -\b true 
 *              -\b false 
 *              .
 */
bool si446x_rx_init(void);

/**
 * \brief Checks if the device is working.
 * 
 * \return It can return:
 *              -\b true if the device is working.
 *              -\b false if the device is not working.
 *              .
 */
bool si446x_check_device(void);

/**
 * \brief 
 * 
 * \return It can return:
 *              -\b true 
 *              -\b false 
 *              .
 */
bool si446x_check_cts(void);

/**
 * \brief 
 * 
 * \param pwr
 * 
 * \return It can return:
 *              -\b true 
 *              -\b false 
 *              .
 */
bool si446x_set_tx_power(uint8_t pwr);

/**
 * \brief 
 * 
 * \param start_property
 * \param para_buf
 * \param length
 * 
 * \return It can return:
 *              -\b true 
 *              -\b false 
 *              .
 */
bool si446x_set_properties(uint16_t start_property, uint8_t *para_buf, uint8_t length);

/**
 * \brief 
 * 
 * \return It can return:
 *              -\b true 
 *              -\b false 
 *              .
 */
bool si446x_get_properties(uint16_t start_property, uint8_t length, uint8_t *para_buf);

/**
 * \brief 
 * 
 * \param 
 * \param 
 * 
 * \return None
 */
void si446x_set_config(const uint8_t *parameters, uint16_t para_len);

/**
 * \brief 
 * 
 * \param len
 * 
 * \return It can return:
 *              -\b true 
 *              -\b false 
 *              .
 */
bool si446x_set_preamble_len(uint8_t len);

/**
 * \brief 
 * 
 * \param sync_word
 * \param len
 * 
 * \return It can return:
 *              -\b true 
 *              -\b false 
 *              .
 */
bool si446x_set_sync_word(uint8_t *sync_word, uint8_t len);

/**
 * \brief 
 * 
 * \param gpio0_mode
 * \param gpio1_mode
 * 
 * \return It can return:
 *              -\b true 
 *              -\b false 
 *              .
 */
bool si446x_set_gpio_mode(uint8_t gpio0_mode, uint8_t gpio1_mode);

/**
 * \brief 
 * 
 * \param cmd
 * \param para_buf
 * \param len
 * 
 * \return It can return:
 *              -\b true 
 *              -\b false 
 *              .
 */
bool si446x_set_cmd(uint8_t cmd, uint8_t *para_buf, uint8_t len);

/**
 * \brief Reads a command.
 * 
 * \param cmd is the command to read.
 * \param para_buf is buffer to store the parameters.
 * \param length is the length of the parameter to read.
 * 
 * \return It can return:
 *              -\b true if no error occurs.
 *              -\b false if an error occurs.
 *              .
 */
bool si446x_get_cmd(uint8_t cmd, uint8_t *para_buf, uint8_t length);

/**
 * \brief 
 * 
 * \return It can return:
 *              -\b true 
 *              -\b false 
 *              .
 */
bool si446x_set_tx_interrupt(void);

/**
 * \brief 
 * 
 * \return It can return:
 *              -\b true 
 *              -\b false 
 *              .
 */
bool si446x_set_rx_interrupt(void);

/**
 * \brief 
 * 
 * \return It can return:
 *              -\b true 
 *              -\b false 
 *              .
 */
bool si446x_clear_interrupts(void);

/**
 * \brief 
 * 
 * \param data
 * \param len
 * 
 * \return None
 */
void si446x_write_tx_fifo(uint8_t *data, uint8_t len);

/**
 * \brief 
 * 
 * \param data
 * 
 * \param read_len
 *
 * \return 
 */
uint8_t si446x_read_rx_fifo(uint8_t *data, uint8_t read_len);

/**
 * \brief 
 * 
 * \return None
 */
void si446x_fifo_reset(void);

/**
 * \brief 
 * 
 * \return None
 */
void si446x_enter_tx_mode(void);

/**
 * \brief 
 * 
 * \return None
 */
void si446x_enter_rx_mode(void);

/**
 * \brief 
 * 
 * return It can return:
 *              -\b true 
 *              -\b false 
 *              .
 */
bool si446x_enter_standby_mode(void);

/**
 * \brief .
 *
 * \return .
 */
bool si446x_wait_packet_sent(void);

/**
 * \brief SPI interface initialization.
 *
 * \return The status/error code.
 */
int si446x_spi_init();

/**
 * \brief SPI transfer routine (write and read at the same time).
 *
 * \param[in] byte is the byte to write during the data transfer.
 *
 * \return The read byte during the data transfer.
 */
uint8_t si446x_spi_transfer(uint8_t byte);

/**
 * \brief Writes a byte over the SPI interface.
 *
 * \param[in] byte is the byte to be written to the SPI interface.
 *
 * \return None.
 */
void si446x_spi_write_byte(uint8_t byte);

/**
 * \brief Write an array of bytes over the SPI interface.
 *
 * \param[in] data is the array to write to the SPI interface.
 *
 * \param[in] size is the number of bytes to be written.
 *
 * \return None.
 */
void si446x_spi_write(uint8_t *data, uint16_t size);

/**
 * \brief Reads a N bytes from the SPI interface.
 *
 * \param[in] data is an array to store the read bytes.
 *
 * \param[in] size is the number of bytes to read.
 *
 * \return None.
 */
void si446x_spi_read(uint8_t *data, uint16_t size);

/**
 * \brief Inquire interrupt.
 * 
 * \return It can return:
 *              -\b true if an interrupt occurs.
 *              -\b false if no interrupt occurs.
 *              .
 */
bool si446x_wait_niqr(void);

/**
 * \brief Checks GPIO1 interruption.
 * 
 * \return It can return:
 *              -\b true if an interrupt occurs.
 *              -\b false if no interrupt occurs.
 *              .
 */
bool si446x_wait_gpio1(void);

/**
 * \brief Milliseconds delay.
 *
 * \param[in] ms is the time to delay in milliseconds.
 *
 * \return None.
 */
void si446x_delay_ms(uint32_t ms);

#endif /* SI446X_H_ */

/** \} End of si446x group */
