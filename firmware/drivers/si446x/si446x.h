/*
 * si446x.h
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
 * \brief Si446x driver definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.9
 * 
 * \date 2017/06/01
 * 
 * \defgroup si446x Si446x
 * \ingroup drivers
 * \{
 */

#ifndef SI446X_H_
#define SI446X_H_

#include <stdint.h>
#include <stdbool.h>

#include "si446x_cmd.h"
#include "si446x_prop.h"

#define SI446X_MODULE_NAME      "Si446x"

#define SI4460_PART_INFO        0x4460      /**< Si4460 part info value. */
#define SI4461_PART_INFO        0x4461      /**< Si4461 part info value. */
#define SI4463_PART_INFO        0x4463      /**< Si4463 part info value. */
#define SI4464_PART_INFO        0x4464      /**< Si4464 part info value. */
#define SI4467_PART_INFO        0x4467      /**< Si4467 part info value. */
#define SI4468_PART_INFO        0x4468      /**< Si4468 part info value. */

/* Error codes */
#define SI446X_INVAL_PAR_ERROR  (-3)        /**< Invalid parameter error. */
#define SI446X_SPI_ERROR        (-2)        /**< Command error. */
#define SI446X_CTS_TIMEOUT      (-1)        /**< CTS timeout. */
#define SI446X_SUCCESS          0           /**< No errors. */

/**
 * \brief Si446x states.
 */
typedef enum
{
    SI446X_NO_CHANGE_STATE=0,               /**< No change. */
    SI446X_SLEEP_STATE,                     /**< Sleep state. */
    SI446X_SPI_ACTIVE_STATE,                /**< SPI Active state. */
    SI446X_READY_STATE,                     /**< Ready state. */
    SI446X_ANOTHER_READY_STATE,             /**< Another enumeration for ready state. */
    SI446X_TUNE_STATE_FOR_TX,               /**< Tune state for TX. */
    SI446X_TUNE_STATE_FOR_RX,               /**< Tune state for RX. */
    SI446X_TX_STATE,                        /**< TX state. */
    SI446X_RX_STATE                         /**< RX state. */
} si446x_states_e;

/**
 * \brief Basic information about the device.
 */
typedef struct
{
    uint8_t chiprev;                        /**< Chip mask revision. */
    uint16_t part;                          /**< Part number. */
    uint8_t pbuild;                         /**< Part build. */
    uint16_t id;                            /**< ID. */
    uint8_t customer;                       /**< Customer ID. */
    uint8_t romid;                          /**< ROM ID. */
} si446x_part_info_t;

/**
 * \brief FIFO info.
 */
typedef struct
{
    uint8_t rx_fifo_count;                  /**< Amount of space currently occupied in receive FIFO. */
    uint8_t tx_fifo_space;                  /**< Amount of space currently available in transmit FIFO. */
} si446x_fifo_info_t;

/**
 * \brief Function revision information of the device.
 */
typedef struct
{
    uint8_t revext;                         /**< External revision number. */
    uint8_t revbranch;                      /**< Branch revision number. */
    uint8_t revint;                         /**< Internal revision number. */
    uint16_t patch;                         /**< ID of applied patch. */
    uint8_t func;                           /**< Current functional mode. */
} si446x_func_info_t;

/**
 * \brief ADC conversion result.
 */
typedef struct
{
    uint16_t gpio_adc;                      /**< ADC value of voltage on GPIO. */
    uint16_t battery_adc;                   /**< ADC value of battery voltage. */
    uint16_t temp_adc;                      /**< ADC value of temperature sensor voltage of the chip in degrees kelvin. */
    uint8_t temp_slope;                     /**< Slope in the formula of Vtempadc - Temperature. */
    uint8_t temp_intercept;                 /**< Intercept in the fromula of Vtempadc - Temperature. */
} si446x_adc_reading_t;

/**
 * \brief Modem status.
 */
typedef struct
{
    uint8_t modem_pend;                     /**< Modem pending. */
    uint8_t modem_status;                   /**< Modem status. */
    uint8_t curr_rssi;                      /**< Current RSSI reading from the modem. */
    uint8_t latch_rssi;                     /**< Latched RSSI reading from the modem as configured by MODEM_RSSI_CONTROL. */
    uint8_t ant1_rssi;                      /**< RSSI of ANT1 while antenna diversity. */
    uint8_t ant2_rssi;                      /**< RSSI of ANT2 while antenna diversity. */
    uint16_t afc_freq_offset;               /**< The AFC value that is generated by the AFC loop during receive mode. */
} si446x_modem_status_t;

/**
 * \brief Interrupt status.
 */
typedef struct
{
    uint8_t int_pend;                       /**< Interrupt pending. */
    uint8_t int_status;                     /**< Interrupt status. */
    uint8_t ph_pend;                        /**< Packet handler pending. */
    uint8_t ph_status;                      /**< Packet handler status. */
    uint8_t modem_pend;                     /**< Modem pending. */
    uint8_t modem_status;                   /**< Modem status. */
    uint8_t chip_pend;                      /**< Chip pending. */
    uint8_t chip_status;                    /**< Chip status. */
} si446x_int_status_t;

/**
 * \brief Device state.
 */
typedef struct
{
    uint8_t curr_state;                     /**< Current state. */
    uint8_t current_channel;                /**< Current channel. */
} si446x_device_state_t;

/**
 * \brief Chip status.
 */
typedef struct
{
    uint8_t chip_pend;                      /**< Chip pending flags. */
    uint8_t chip_status;                    /**< Chip status flags. */
    uint8_t chip_err_status;                /**< Last command error cause (Only valid if CMD_ERROR status bit is set). */
} si446x_chip_status_t;

/**
 * \brief Packet Handler status.
 */
typedef struct
{
    uint8_t ph_pend;                        /**< Packet Handler pendings flags. */
    uint8_t ph_status;                      /**< Packet Handler status flags. */
} si446x_ph_status_t;

/**
 * \brief Si446x state.
 */
typedef uint8_t si446x_state_t;

/**
 * \brief This functions is used to reset the si446x radio by applying shutdown and releasing it.
 *
 * After this function si446x_boot should be called. You can check if POR has completed by waiting 4 ms or by
 * polling GPIO 0, 2, or 3. When these GPIOs are high, it is safe to call si446x_boot.
 *
 * \return The status/error code.
 */
int si446x_reset(void);

/**
 * \brief This function is used to initialize after power-up the radio chip.
 *
 * \param[in] boot_options: Patch mode selector.
 *
 * \param[in] xtal_option: Select if TCXO is in use.
 *
 * \param[in] xo_freq: Frequency of TCXO or external crystal oscillator in Hz.
 *
 * \note Before this function si446x_reset should be called.
 *
 * \return The status/error code.
 */
int si446x_power_up(uint8_t boot_options, uint8_t xtal_options, uint32_t xo_freq);

/**
 * \brief This function is used to load all properties with a list of NULL terminated set property commands.
 *
 * \param[in] p_set_prop_cmd: First element of the list to be loaded.
 *
 * \param[in] p_set_prop_cmd_len: Length in bytes of the commands list.
 *
 * \note Before this function si446x_reset should be called.
 *
 * \return The status/error code.
 */
int si446x_configuration_init(uint8_t *p_set_prop_cmd, uint16_t p_set_prop_cmd_len);

/**
 * \brief This function sends the PART_INFO command to the radio and receives the answer Si446xCmd union.
 *
 * \param[in] part_info is a pointer to a si446x_part_info_t struct to store the result.
 *
 * \return The status/error code.
 */
int si446x_part_info(si446x_part_info_t *part_info);

/**
 * \brief Sends START_TX command to the radio.
 *
 * \param[in] channel: Channel number.
 *
 * \param[in] condition: Start TX condition.
 *
 * \param[in] tx_len: Payload length (exclude the PH generated CRC).
 *
 * \return The status/error code.
 */
int si446x_start_tx(uint8_t channel, uint8_t condition, uint16_t tx_len);

/**
 * \brief Sends START_RX command to the radio.
 *
 * \param[in] channel: Channel number.
 *
 * \param[in] condition: Start RX condition.
 *
 * \param[in] rx_len: Payload length (exclude the PH generated CRC).
 *
 * \param[in] next_state1: Next state when Preamble Timeout occurs.
 *
 * \param[in] next_state2: Next state when a valid packet received.
 *
 * \param[in] next_state3: Next state when invalid packet received (e.g., CRC error).
 *
 * \return The status/error code.
 */
int si446x_start_rx(uint8_t channel, uint8_t condition, uint16_t rx_len, si446x_state_t next_state1, si446x_state_t next_state2, si446x_state_t next_state3);

/**
 * \brief Get the Interrupt status/pending flags form the radio and clear flags if requested.
 *
 * \param[in] ph_clr_pend: Packet Handler pending flags clear.
 *
 * \param[in] modem_clr_pend: Modem Status pending flags clear.
 *
 * \param[in] chip_clr_pend: Chip State pending flags clear.
 *
 * \param[in,out] int_status: Is a pointer to store the int_status data.
 *
 * \return The status/error code.
 */
int si446x_get_int_status(uint8_t ph_clr_pend, uint8_t modem_clr_pend, uint8_t chip_clr_pend, si446x_int_status_t *int_status);

/**
 * \brief Send GPIO pin config command to the radio and reads the answer into Si446xCmd union.
 *
 * \param[in] gpio0: GPIO0 configuration.
 *
 * \param[in] gpio1: GPIO1 configuration.
 *
 * \param[in] gpio2: GPIO2 configuration.
 *
 * \param[in] gpio3: GPIO3 configuration.
 *
 * \param[in] nirq: NIRQ configuration.
 *
 * \param[in] sdo: SDO configuration.
 *
 * \param[in] gen_config: General pin configuration.
 *
 * \return The status/error code.
 */
int si446x_gpio_pin_cfg(uint8_t gpio0, uint8_t gpio1, uint8_t gpio2, uint8_t gpio3, uint8_t nirq, uint8_t sdo, uint8_t gen_config);

/**
 * \brief Send SET_PROPERTY command to the radio.
 *
 * \param[in] group: Property group.
 *
 * \param[in] num_props: Number of property to be set. The properties must be in ascending order in their sub-property aspect. Max. 12 properties can be set in one command
 *
 * \param[in] start_group: Start sub-property address.
 *
 * \param[in] data: Properties values
 *
 * \param[in] len: Lenght in bytes of the data array.
 *
 * \return The status/error code.
 */
int si446x_set_property(uint8_t group, uint8_t num_props, uint8_t start_prop, uint8_t *data, uint16_t len);

/**
 * \brief Issue a change state command to the radio.
 *
 * \param[in] next_state: Next state. It can be:
 * \parblock
 *      -\b SI446X_NO_CHANGE_STATE
 *      -\b SI446X_SLEEP_STATE
 *      -\b SI446X_SPI_ACTIVE_STATE
 *      -\b SI446X_READY_STATE
 *      -\b SI446X_ANOTHER_READY_STATE
 *      -\b SI446X_TUNE_STATE_FOR_TX
 *      -\b SI446X_TUNE_STATE_FOR_RX
 *      -\b SI446X_TX_STATE
 *      -\b SI446X_RX_STATE
 *      .
 * \endparblock
 *
 * \return The status/error code.
 */
int si446x_change_state(si446x_state_t next_state);

/**
 * \brief Sends NOP command to the radio. Can be used to maintain SPI communication.
 *
 * \return The status/error code.
 */
int si446x_nop(void);

/**
 * \brief Send the FIFO_INFO command to the radio.
 *
 * Optionally resets the TX/RX FIFO. Reads the radio response back Si446xCmd uniom.
 *
 * \param[in] rst_rx: RX FIFO reset flag.
 *
 * \param[in] rst_tx: TX FIFO reset flag.
 *
 * \param[in,out] fifo_info: Is a pointer to store the FIFO info.
 *
 * \return The status/error code.
 */
int si446x_fifo_info(bool rst_rx, bool rst_tx, si446x_fifo_info_t *fifo_info);

/**
 * \brief The function can be used to load data into TX FIFO.
 *
 * \param[in] num_bytes: Data length to be load.
 *
 * \param[in] p_tx_data: Pointer to the data.
 *
 * \return The status/error code.
 */
int si446x_write_tx_fifo(uint8_t num_bytes, uint8_t *p_tx_data);

/**
 * \brief Reads the RX FIFO content from the radio.
 *
 * \param[in] num_bytes: Data length to be read.
 *
 * \param[in] p_rx_data: Pointer to the buffer location.
 *
 * \return The status/error code.
 */
int si446x_read_rx_fifo(uint8_t num_bytes, uint8_t *p_rx_data);

/**
 * \brief Get property values from the radio. Reads them into Si446xCmd union.
 *
 * \param[in] group: Property group number.
 *
 * \param[in] num_props: Number of properties to be read.
 *
 * \param[in] start_props: Starting sub-property number.
 *
 * \return The status/error code.
 */
int si446x_get_property(uint8_t group, uint8_t num_props, uint8_t start_prop, uint8_t *data);

/**
 * \brief Sends the FUNC_INFO command to the radio, then reads the response into Si446xCmd union.
 *
 * \param[in,out] func_info is a pointer to a si446x_func_info_t struct to store the result.
 *
 * \return The status/error code.
 */
int si446x_func_info(si446x_func_info_t *func_info);

/**
 * \brief Reads the Fast Response Registers starting with A register into Si446xCmd union.
 *
 * \param[in] resp_byte_count: Number of Fast Response Registers to be read.
 *
 * \param[in,out] frr_a_val: Is a pointer to store the FRR A value.
 *
 * \return The status/error code.
 */
int si446x_frr_a_read(uint8_t resp_byte_count, uint8_t *frr_a_val);

/**
 * \brief Reads the Fast Response Registers starting with B register into Si446xCmd union.
 *
 * \param[in] resp_byte_count: Number of Fast Response Registers to be read.
 *
 * \param[in,out] frr_a_val: Is a pointer to store the FRR B value.
 *
 * \return The status/error code.
 */
int si446x_frr_b_read(uint8_t resp_byte_count, uint8_t *frr_b_val);

/**
 * \brief Reads the Fast Response Registers starting with C register into Si446xCmd union.
 *
 * \param[in] resp_byte_count: Number of Fast Response Registers to be read.
 *
 * \param[in,out] frr_a_val: Is a pointer to store the FRR C value.
 *
 * \return The status/error code.
 */
int si446x_frr_c_read(uint8_t resp_byte_count, uint8_t *frr_c_val);

/**
 * \brief Reads the Fast Response Registers starting with D register into Si446xCmd union.
 *
 * \param[in] resp_byte_count: Number of Fast Response Registers to be read.
 *
 * \param[in,out] frr_a_val: Is a pointer to store the FRR D value.
 *
 * \return The status/error code.
 */
int si446x_frr_d_read(uint8_t resp_byte_count, uint8_t *frr_d_val);

/**
 * \brief Reads the ADC values from the radio into Si446xCmd union.
 *
 * \param[in] temp_en: Enables the temperature ADC.
 *
 * \param[in] bat_volt_en: Enables the battery voltage ADC.
 *
 * \parma[in] adc_gpio_en: Enables the ADC GPIO pin.
 *
 * \param[in] adc_gpio_pin: Defines the ADC GPIO pin.
 *
 * \param[in,out] adc_reading: A pointer to a si446x_adc_reading_t struct to store the result.
 *
 * \return The status/error code.
 */
int si446x_get_adc_reading(uint8_t temp_en, bool bat_volt_en, bool adc_gpio_en, uint8_t adc_gpio_pin, si446x_adc_reading_t *adc_reading);

/**
 * \brief Receives information from the radio of the current packet.
 *
 * Optionally can be used to modify the Packet Handler properties during packet reception.
 *
 * \param[in] field_number_mask: Packet Field number mask value.
 *
 * \param[in] len: Length value.
 *
 * \param[in] diff_len: Difference length.
 *
 * \return The status/error code.
 */
int si446x_get_packet_info(uint8_t field_number_mask, uint16_t len, int16_t diff_len, uint16_t *last_len);

/**
 * \brief Gets the Packet Handler status flags. Optionally clears them.
 *
 * \param[in] ph_status: A pointer to store the packet handler status.
 *
 * \return The status/error code.
 */
int si446x_get_ph_status(si446x_ph_status_t *ph_status);

/**
 * \brief Gets the Modem status flags. Optionally clears them.
 *
 * \param[in] modem_clr_pend: Flags to clear.
 *
 * \param[in,out] modem_status is a pointer to store the read modem status.
 *
 * \return The status/error code.
 */
int si446x_get_modem_status(uint8_t modem_clr_pend, si446x_modem_status_t *modem_status);

/**
 * \brief Gets the Chip status flags. Optionally clears them.
 *
 * \param[in] chip_clr_pend: Flags to clear.
 *
 * \param[in,out] chip_status: Is a pointer to store the chip status values .
 *
 * \return The status/error code.
 */
int si446x_get_chip_status(uint8_t chip_clr_pend, si446x_chip_status_t *chip_status);

/**
 * \brief Performs image rejection calibration.
 *
 * Completion can be monitored by polling CTS or waiting for CHIP_READY interrupt source.
 *
 * \param[in] searching_step_size
 *
 * \param[in] searching_rssi_avg
 *
 * \param[in] rx_chain_setting1
 *
 * \param[in] rx_chain_setting2
 *
 * \return The status/error code.
 */
int si446x_ircal(uint8_t searching_step_size, uint8_t searching_rssi_avg, uint8_t rx_chain_setting1, uint8_t rx_chain_setting2);

/**
 * \brief Sets the chip up for specified protocol.
 *
 * \param[in] protocol
 *
 * \return The status/error code.
 */
int si446x_protocol_cfg(uint8_t protocol);

/**
 * \brief Requests the current state of the device and lists pending TX and RX requests.
 *
 * \param[in,out] dev_state: Is a pointer to store the device state values.
 *
 * \return The status/error code.
 */
int si446x_request_device_state(si446x_device_state_t *dev_state);

/**
 * \brief While in RX state this will hop to the frequency specified by the parameters and start searching for a preamble.
 *
 * \param[in] inte: New INTE register value.
 *
 * \param[in] frac2: New FRAC2 register value.
 *
 * \param[in] frac1: New FRAC1 register value.
 *
 * \param[in] frac0: New FRAC0 register value.
 *
 * \param[in] vco_cnt1: New VCO_CNT1 register value.
 *
 * \param[in] vco_cnt0: New VCO_CNT0 register value.
 *
 * \return The status/error code.
 */
int si446x_rx_hop(uint8_t inte, uint8_t frac2, uint8_t frac1, uint8_t frac0, uint8_t vco_cnt1, uint8_t vco_cnt0);

/**
 * \brief Shutdown the device.
 *
 * \return The status/error code.
 */
int si446x_shutdown(void);

/**
 * \brief Power-on the device.
 *
 * \return The status/error code.
 */
int si446x_power_on(void);

/**
 * \brief Verifies if the device is clear to send (CTS).
 *
 * \param[in] timeout_ms: Is the timeout in milliseconds.
 *
 * \return TRUR/FALSE if clear to send or not.
 */
bool si446x_check_cts(uint32_t timeout_ms);

/**
 * \brief Writes a command with no response.
 *
 * \param[in] cmd: Command array.
 *
 * \param[in] cmd_len: Command array length in bytes.
 *
 * \return The status/error code.
 */
int si446x_set_cmd(uint8_t *cmd, uint16_t cmd_len);

/**
 * \brief Writes a command with response.
 *
 * \param[in] cmd: Command array.
 *
 * \param[in] cmd_len: Command array length in bytes.
 *
 * \param[in,out] result: A pointer to store the command response.
 *
 * \param[in] result_len: The number of bytes of the response.
 *
 * \return The status/error code.
 */
int si446x_get_cmd(uint8_t *cmd, uint16_t cmd_len, uint8_t *result, uint16_t result_len);

/**
 * \brief SPI interface initialization.
 *
 * \return The status/error code.
 */
int si446x_spi_init(void);

/**
 * \brief SPI transfer routine (write and read at the same time).
 *
 * \param[in] wd is an array ot bytes to write during the transfer.
 *
 * \param[in,out] rd is an array to store the read bytes during the transfer.
 *
 * \param[in] len is the number of bytes to transfer.
 *
 * \return The status/error code.
 */
int si446x_spi_transfer(uint8_t *wd, uint8_t *rd, uint16_t len);

/**
 * \brief Writes a byte over the SPI interface.
 *
 * \param[in] byte is the byte to be written to the SPI interface.
 *
 * \return The status/error code.
 */
int si446x_spi_write_byte(uint8_t byte);

/**
 * \brief Write an array of bytes over the SPI interface.
 *
 * \param[in] data is the array of bytes to write to the SPI interface.
 *
 * \param[in] len is the number of bytes to be written.
 *
 * \return The status/error code.
 */
int si446x_spi_write(uint8_t *data, uint16_t len);

/**
 * \brief Reads N bytes from the SPI interface.
 *
 * \param[in] data is an array to store the read bytes.
 *
 * \param[in] len is the number of bytes to read.
 *
 * \return The status/error code.
 */
int si446x_spi_read(uint8_t *data, uint16_t len);

/**
 * \brief GPIO pins initialization.
 *
 * \return The status/error code.
 */
int si446x_gpio_init(void);

/**
 * \brief Write the state of the SDN pin.
 *
 * \param[in] state is new state of the SDN pin.
 *
 * \return The status/error code.
 */
int si446x_gpio_write_sdn(bool state);

/**
 * \brief Reads the state of the nIRQ pin.
 *
 * \return The state of the nIRQ pin. It can be:
 * \parblock
 *      -\b GPIO_STATE_HIGH
 *      -\b GPIO_STATE_LOW
 *      .
 * \endparblock
 */
int si446x_gpio_read_nirq(void);

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
