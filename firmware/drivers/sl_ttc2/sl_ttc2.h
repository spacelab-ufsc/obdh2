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
 * \version 0.6.16
 * 
 * \date 2021/05/12
 * 
 * \defgroup sl_ttc2 SpaceLab TTC 2.0
 * \ingroup drivers
 * \{
 */

#ifndef SL_TTC2_H_
#define SL_TTC2_H_

#include <stdbool.h>
#include <stdint.h>

#include <drivers/spi/spi.h>

#define SL_TTC2_MODULE_NAME                     "SpaceLab TTC 2.0"

/* TTC 2.0 IDs */
#define SL_TTC2_DEVICE_ID_RADIO_1               0xCC2A  /**< TTC 2.0 device ID (radio 1). */
#define SL_TTC2_DEVICE_ID_RADIO_2               0xCC2B  /**< TTC 2.0 device ID (radio 2). */

/* TTC 2.0 Commands */
#define SL_TTC2_CMD_NOP                         0       /**< No operation. */
#define SL_TTC2_CMD_READ_REG                    1       /**< Read register command. */
#define SL_TTC2_CMD_WRITE_REG                   2       /**< Write register command. */
#define SL_TTC2_CMD_TRANSMIT_PKT                3       /**< Transmit packet command. */
#define SL_TTC2_CMD_RECEIVE_PKT                 4       /**< Receive packet command. */

/* TTC 2.0 Registers */
#define SL_TTC2_REG_DEVICE_ID                   0       /**< Device ID (0xCC2A or 0xCC2B). */
#define SL_TTC2_REG_HARDWARE_VERSION            1       /**< Hardware version. */
#define SL_TTC2_REG_FIRMWARE_VERSION            2       /**< Firmware version. */
#define SL_TTC2_REG_TIME_COUNTER                3       /**< Time counter in millseconds. */
#define SL_TTC2_REG_RESET_COUNTER               4       /**< Reset counter. */
#define SL_TTC2_REG_LAST_RESET_CAUSE            5       /**< Last reset cause. */
#define SL_TTC2_REG_INPUT_VOLTAGE_MCU           6       /**< Input voltage of the uC in mV. */
#define SL_TTC2_REG_INPUT_CURRENT_MCU           7       /**< Input current of the uC in mA. */
#define SL_TTC2_REG_TEMPERATURE_MCU             8       /**< Temperature of the uC in K. */
#define SL_TTC2_REG_INPUT_VOLTAGE_RADIO         9       /**< Input voltage of the radio in mV. */
#define SL_TTC2_REG_INPUT_CURRENT_RADIO         10      /**< Input current of the radio in mA. */
#define SL_TTC2_REG_TEMPERATURE_RADIO           11      /**< Temperature of the radio in K. */
#define SL_TTC2_REG_LAST_VALID_TC               12      /**< Last valid telecommand (uplink packet ID). */
#define SL_TTC2_REG_RSSI_LAST_VALID_TC          13      /**< RSSI of the last valid telecommand. */
#define SL_TTC2_REG_TEMPERATURE_ANTENNA         14      /**< Temperature of the antenna module in K. */
#define SL_TTC2_REG_ANTENNA_STATUS              15      /**< Antenna module status bits. */
#define SL_TTC2_REG_ANTENNA_DEPLOYMENT_STATUS   16      /**< Antenna deployment status (0=never executed, 1=executed). */
#define SL_TTC2_REG_ANTENNA_DEP_HIB_STATUS      17      /**< Antenna deployment hibernation status (0=never executed, 1=executed). */
#define SL_TTC2_REG_TX_ENABLE                   18      /**< TX enable (0=off, 1=on). */
#define SL_TTC2_REG_TX_PACKET_COUNTER           19      /**< TX packet counter. */
#define SL_TTC2_REG_RX_PACKET_COUNTER           20      /**< RX packet counter. */
#define SL_TTC2_REG_FIFO_TX_PACKET              21      /**< Number of packets in the TX FIFO. */
#define SL_TTC2_REG_FIFO_RX_PACKET              22      /**< Number of packets in the RX FIFO. */
#define SL_TTC2_REG_LEN_FIRST_RX_PACKET_IN_FIFO 23      /**< Number of bytes of the first available packet in the RX buffer. /

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
 * \brief RSSI type.
 */
typedef uint16_t sl_ttc2_rssi_t;

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
    SL_TTC2_TEMP_RADIO,                                 /**< Radio temperature. */
    SL_TTC2_TEMP_ANTENNA                                /**< Antenna temperature. */
} sl_ttc2_temp_e;

/**
 * \brief Packet types.
 */
typedef enum
{
    SL_TTC2_TX_PKT=0,                                   /**< TX packet. */
    SL_TTC2_RX_PKT                                      /**< RX packet. */
} sl_ttc2_pkt_e;

/**
 * \brief Data structure.
 */
typedef struct
{
    uint32_t                time_counter;               /**< Time counter in milliseconds. */
    uint16_t                reset_counter;              /**< Reset counter. */
    uint8_t                 last_reset_cause;           /**< Last reset cause. */
    sl_ttc2_voltage_t       voltage_mcu;                /**< Input voltage of the uC in mV. */
    sl_ttc2_current_t       current_mcu;                /**< Input current of the uC in mA. */
    sl_ttc2_temp_t          temperature_mcu;            /**< Temperature of the uC in K. */
    sl_ttc2_voltage_t       voltage_radio;              /**< Input voltage of the radio in mV. */
    sl_ttc2_current_t       current_radio;              /**< Input current of the radio in mA. */
    sl_ttc2_temp_t          temperature_radio;          /**< Temperature of the radio in K. */
    uint8_t                 last_valid_tc;              /**< Last valid telecommand (uplink packet ID). */
    sl_ttc2_rssi_t          rssi_last_valid_tc;         /**< RSSI of the last valid telecommand. */
    sl_ttc2_temp_t          temperature_antenna;        /**< Temperature of the antenna module in K. */
    uint16_t                antenna_status;             /**< Antenna module status bits. */
    uint8_t                 deployment_status;          /**< Antenna deployment status (0=never executed, 1=executed). */
    uint8_t                 hibernation_status;         /**< Antenna deployment hibernation (0=never executed, 1=executed). */
    uint32_t                tx_packet_counter;          /**< TX packet counter. */
    uint32_t                rx_packet_counter;          /**< RX packet counter. */
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
 * \brief Reads the device ID of the TTC module.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in,out] val is a pointer to store the read device ID value.
 *
 * \return The status/error code.
 */
int sl_ttc2_read_device_id(sl_ttc2_config_t config, uint16_t *val);

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
 * \param[in,out] val is a pointer to store the read voltage value.
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
 *      -\b SL_TTC2_TEMP_ANTENNA
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
 * \brief Reads the ID of the last valid telecommand received by the TTC module
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in,out] val is a pointer to store the read ID of the last valid telecommand.
 *
 * \return The status/error code.
 */
int sl_ttc2_read_last_valid_tc(sl_ttc2_config_t config, uint8_t *val);

/**
 * \brief Reads the RSSI valur of the last valid telecommand of the TTC module.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in,out] val is a pointer to store the read RSSI value.
 *
 * \return The status/error code.
 */
int sl_ttc2_read_rssi(sl_ttc2_config_t config, sl_ttc2_rssi_t *val);

/**
 * \brief Reads the antenna status of the TTC module.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in,out] val is a pointer to store the read antenna status value.
 *
 * \return The status/error code.
 */
int sl_ttc2_read_antenna_status(sl_ttc2_config_t config, uint16_t *val);

/**
 * \brief Reads the antenna deployment status (executed or not executed).
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in,out] val is a pointer to store the read status value.
 *
 * \return The status/error code.
 */
int sl_ttc2_read_antenna_deployment_status(sl_ttc2_config_t config, uint8_t *val);

/**
 * \brief Reads the antenna deployment hibernation status (executed or not executed).
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in,out] val is a pointer to store the read status value.
 *
 * \return The status/error code.
 */
int sl_ttc2_read_antenna_deployment_hibernation_status(sl_ttc2_config_t config, uint8_t *val);

/**
 * \brief Reads the TX enable flag.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in,out] val is a pointer to store the read TX enable flag.
 *
 * \return The status/error code.
 */
int sl_ttc2_read_tx_enable(sl_ttc2_config_t config, uint8_t *val);

/**
 * \brief Sets the TX enable flag of the TTC.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in] en is TRUE/FALSE to enable/disable the transmitter.
 *
 * \return The status/error code.
 */
int sl_ttc2_set_tx_enable(sl_ttc2_config_t config, bool en);

/**
 * \brief Reads the packet counter value (transmitted or received).
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in] pkt is packet type counter to read. It can be:
 * \parblock
 *      -\b SL_TTC2_TX_PKT
 *      -\b SL_TTC2_RX_PKT
 *      .
 * \endparblock
 *
 * \param[in,out] val is a pointer to store the read packet counter value.
 *
 * \return The status/error code.
 */
int sl_ttc2_read_pkt_counter(sl_ttc2_config_t config, uint8_t pkt, uint32_t *val);

/**
 * \brief Reads the number of packets available in a given FIFO (TX or RX).
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in] pkt is packet type FIFO to read. It can be:
 * \parblock
 *      -\b SL_TTC2_TX_PKT
 *      -\b SL_TTC2_RX_PKT
 *      .
 * \endparblock
 *
 * \param[in,out] val is a pointer to store the read value.
 *
 * \return The status/error code.
 */
int sl_ttc2_read_fifo_pkts(sl_ttc2_config_t config, uint8_t pkt, uint8_t *val);

/**
 * \brief Reads the register with the number of bytes of the first available packet in the RX FIFO.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in,out] val is a pointer to store the read value.
 *
 * \return The status/error code.
 */
int sl_ttc2_read_len_rx_pkt_in_fifo(sl_ttc2_config_t config, uint16_t *val);

/**
 * \brief Verifies the number available RX packets to read.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \return The number of available packets to read (-1 on error).
 */
int sl_ttc2_check_pkt_avail(sl_ttc2_config_t config);

/**
 * \brief Transmit packet command.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in] data is the data to transmit.
 *
 * \param[in] len is the number of bytes to transmit.
 *
 * \return The status/error code.
 */
int sl_ttc2_transmit_packet(sl_ttc2_config_t config, uint8_t *data, uint16_t len);

/**
 * \brief Reads a received packet.
 *
 * \param[in] config is a structure with the configuration parameters of the driver.
 *
 * \param[in,out] data is a pointer to store the read packet.
 *
 * \param[in,out] len is a pointer to store the length of the read packet (in bytes).
 *
 * \return The status/error code.
 */
int sl_ttc2_read_packet(sl_ttc2_config_t config, uint8_t *data, uint16_t *len);

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
