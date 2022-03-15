/*
 * edc.h
 * 
 * Copyright The OBDH 2.0 Contributors.
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
 * \brief EDC driver definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Bruno Benedetti <brunobenedetti45@gmail.com>
 * 
 * \version 0.9.2
 * 
 * \date 2019/10/27
 * 
 * \defgroup edc EDC
 * \ingroup drivers
 * \{
 */

#ifndef EDC_H_
#define EDC_H_

#include <stdint.h>
#include <stdbool.h>

#include <drivers/i2c/i2c.h>
#include <drivers/gpio/gpio.h>

#define EDC_MODULE_NAME             "EDC"

#define EDC_SLAVE_ADDRESS           0x13    /**< 7-bit slave address. */

/* Commands IDs */
#define EDC_CMD_RTC_SET             0x01U   /**< Sets the RTC time. The parameters is the number of seconds elapsed since J2000 epoch. */
#define EDC_CMD_PTT_POP             0x06U   /**< Removes current PTT package from the PTT Package FIFO Buffer, allowing the reading of the next PTT package, if existent. */
#define EDC_CMD_PTT_PAUSE           0x08U   /**< Pauses the PTT Decoding Task. At initialization the PTT Task is paused. */
#define EDC_CMD_PTT_RESUME          0x09U   /**< Resumes the PTT Decoding Task. */
#define EDC_CMD_SAMPLER_START       0x0AU   /**< Start ADC Sample task. This trigger the sampling of a sequence of 2048 I&Q samples from the RF Front End. */
#define EDC_CMD_GET_STATE           0x30U   /**< Causes the transmission of System State Frame through RS-485 interface. */
#define EDC_CMD_GET_PTT_PKG         0x31U   /**< Causes the transmission of current PTT Decoder Frame through RS-485 interface. */
#define EDC_CMD_GET_HK_PKG          0x32U   /**< Updates HK Frame information, and transmit it through the RS-485 interface. */
#define EDC_CMD_GET_ADC_SEQ         0x34U   /**< Causes the transmission of current ADC Sampler Frame through RS-485 interface. */
#define EDC_CMD_ECHO                0xF0U   /**< Cause the transmission of the string "ECHO" in the debug interface. */

/* Frames IDs */
#define EDC_FRAME_ID_STATE          0x11U   /**< State frame. */
#define EDC_FRAME_ID_PTT            0x22U   /**< PTT frame. */
#define EDC_FRAME_ID_ADC_SEQ        0x33U   /**< ADC sequence frame. */
#define EDC_FRAME_ID_HK             0x44U   /**< Housekeeping frame. */

/* Frames length */
#define EDC_FRAME_STATE_LEN         9       /**< State frame length. */
#define EDC_FRAME_PTT_LEN           49      /**< PTT frame length. */
#define EDC_FRAME_ADC_SEQ_LEN       8199    /**< ADC sequence frame length. */
#define EDC_FRAME_HK_LEN            21      /**< Housekeeping frame length. */

/**
 * \brief EDC interfaces.
 */
typedef enum
{
    EDC_IF_UART=0,                          /**< UART interface */
    EDC_IF_I2C                              /**< I2C interface */
} edc_if_t;

/**
 * \brief EDC configuration parameters.
 */
typedef struct
{
    edc_if_t interface;                     /**< Interface option (EDC_IF_UART or EDC_IF_I2C). */
    i2c_port_t i2c_port;                    /**< I2C port. */
    uint32_t i2c_bitrate;                   /**< I2C bitrate in bps. */
    uart_port_t uart_port;                  /**< UART port. */
    gpio_pin_t en_pin;                      /**< Enable pin. */
} edc_config_t;

/**
 * \brief EDC command.
 */
typedef struct
{
    uint8_t id;         /**< Command ID. */
    uint32_t param;     /**< Command param. */
} edc_cmd_t;

/**
 * \brief State data.
 */
typedef struct
{
    uint32_t current_time;                  /**< Current time in number of seconds elapsed since J2000 epoch. */
    uint8_t ptt_available;                  /**< Number of PTT Package available for reading. */
    bool ptt_is_paused;                     /**< PTT Decoder task status. */
    uint8_t sampler_state;                  /**< ADC Sampler state. */
} edc_state_t;

/**
 * \brief PTT data.
 */
typedef struct
{
    uint32_t time_tag;                      /**< PTT signal receiving time in number of seconds elapsed since J2000 epoch. */
    uint8_t error_code;                     /**< PTT error code. */
    int32_t carrier_freq;                   /**< Carrier frequency. */
    uint16_t carrier_abs;                   /**< Carrier amplitude at ADC interface output. */
    uint8_t msg_byte_length;                /**< user_msg length in number of bytes. */
    uint8_t user_msg[36];                   /**< User Message as specified in ARGOS-2 PTT-A2 signal specification. */
} edc_ptt_t;

/**
 * \brief Housekeeping data.
 */
typedef struct
{
    uint32_t current_time;                  /**< Current time in number of seconds elapsed since J2000 epoch. */
    uint32_t elapsed_time;                  /**< Elapsed time since last system initialization in seconds. */
    uint16_t current_supply;                /**< System current supply in mA. */
    uint16_t voltage_supply;                /**< System voltage supply in mV. */
    int8_t temp;                            /**< EDC board temperature in Celsius. */
    uint8_t pll_sync_bit;                   /**< RF Front End LO frequency synthesizer indicator of PLL synchronization. */
    int16_t adc_rms;                        /**< RMS level at front-end output. */
    uint8_t num_rx_ptt;                     /**< Number of generated PTT package since last system initialization. */
    uint8_t max_parl_decod;                 /**< Maximum number of active PTT decoder channels registered since last system initialization. */
    uint8_t mem_err_count;                  /**< Number of double bit errors detected by MSS data memory controller since last system initialization. */
} edc_hk_t;

/**
 * \brief Device initialization.
 *
 * \param[in] config is the configuration parameters of the EDC driver.
 *
 * \return The status/error code.
 */
int edc_init(edc_config_t config);

/**
 * \brief Enables the EDC module.
 *
 * \param[in] config is the configuration parameters of the EDC driver.
 *
 * \return The status/error code.
 */
int edc_enable(edc_config_t config);

/**
 * \brief Disables the EDC module.
 *
 * \param[in] config is the configuration parameters of the EDC driver.
 *
 * \return The status/error code.
 */
int edc_disable(edc_config_t config);

/**
 * \brief Writes a command to the EDC module.
 *
 * \param[in] config is the configuration parameters of the EDC driver.
 *
 * \param[in] cmd is the command to be written.
 *
 * \return The status/error code.
 */
int edc_write_cmd(edc_config_t config, edc_cmd_t cmd);

/**
 * \brief Reads data from the EDC module.
 *
 * \param[in] config is the configuration parameters of the EDC driver.
 *
 * \param[in,out] data is an array to store the read bytes.
 *
 * \param[in] len is the number of bytes to read.
 *
 * \return The status/error code.
 */
int edc_read(edc_config_t config, uint8_t *data, uint16_t len);

/**
 * \brief Verifies if the EDC is connected.
 *
 * \param[in] config is the configuration parameters of the EDC driver.
 *
 * \return The status/error code.
 */
int edc_check_device(edc_config_t config);

/**
 * \brief Sets the RTC time.
 *
 * \param[in] config is the configuration parameters of the EDC driver.
 *
 * \param[in] time is the number of seconds elapsed since J2000 epoch.
 *
 * \return The status/error code.
 */
int edc_set_rtc_time(edc_config_t config, uint32_t time);

/**
 * \brief Removes current PTT package from the PTT Package FIFO buffer.
 *
 * It allows the reading of the next PTT package, if existent.
 *
 * \param[in] config is the configuration parameters of the EDC driver.
 *
 * \return The status/error code.
 */
int edc_pop_ptt_pkg(edc_config_t config);

/**
 * \brief Pauses the PTT decoding task.
 *
 * \note At initialization the PTT task is paused
 *
 * \param[in] config is the configuration parameters of the EDC driver.
 *
 * \return The status/error code.
 */
int edc_pause_ptt_task(edc_config_t config);

/**
 * \brief Resumes the PTT decoding task.
 *
 * \param[in] config is the configuration parameters of the EDC driver.
 *
 * \return The status/error code.
 */
int edc_resume_ptt_task(edc_config_t config);

/**
 * \brief Starts the ADC sampling task.
 *
 * The ADC sampling task stores a sequence composed of 2048 IQ RF front-end samples.
 *
 * \param[in] config is the configuration parameters of the EDC driver.
 *
 * \return The status/error code.
 */
int edc_start_adc_task(edc_config_t config);

/**
 * \brief Gets the system state frame.
 *
 * The System State Frame provides EDC current state information.
 *
 * | Byte | Name            | Data | Description                                                 |
 * | :--- | :-------------- | :--- | :---------------------------------------------------------- |
 * | 0    | Frame type      | u8   | 0x11                                                        |
 * | 1-4  | Current time    | u32  | Current time in number of seconds elapsed since J2000 epoch |
 * | 5    | PTT available   | u8   | Number of PTT packages available for reading                |
 * | 6    | PTT task status | u8   | PTT decoder task status (0 = on, 1 = paused)                |
 * | 7    | Sampler state   | u8   | ADC sampler state (0 = empty, 1 = busy, 2 = ready)          |
 * | 8    | Checksum        | u8   | Bitwise XOR operation between all transmitted bytes         |
 *
 * \see EDC - Environmental Data Collector User Guide. Section 3.6 - System State Frame. Page 9.
 *
 * \param[in] config is the configuration parameters of the EDC driver.
 *
 * \return The number of read bytes (-1 on error).
 */
int16_t edc_get_state_pkg(edc_config_t config, uint8_t *status);

/**
 * \brief Gets the current PTT decoder frame.
 *
 * The MSS has an internal task for handling signal decoding, the PTT Decoder Task. When this task is active,
 * one PTT Data Package is generated for every identified PTT signal at the receiver signal. Information on
 * those packages include signal receiving time, frequency and power, along with the decoded message or decoding
 * error code. Generated packages are stored in a FIFO Buffer, with a maximum depth of 64 PTT Packages. The
 * oldest PTT Package in the buffer is provided for reading encapsulated by the PTT Decoder Frame.
 *
 * | Byte  | Name         | Data | Description                                                              |
 * | :---- | :----------- | :--- | :----------------------------------------------------------------------- |
 * | 0     | Frame type   | u8   | 0x22 if the buffer contains data, 0xFF otherwise (empty frame)           |
 * | 1-4   | Time tag     | u32  | PTT signal receiving time in number of seconds elapsed since J2000 epoch |
 * | 5     | Error code   | u8   | 0=no error, 1=parity error at PTT msg len code, 2=synch pattern timeout  |
 * | 6-9   | Carrier freq | s32  | Carrier frequency. Convert to kHz = carrierFreq*128/2^11 + 401635        |
 * | 10-11 | Carrier abs  | u16  | Carrier amplitude at ADC interface output                                |
 * | 12    | Msg byte len | u8   | User message length in number of bytes                                   |
 * | 13-47 | User message | u8   | As specified in ARGOS-2 PTT -A2 signal specification                     |
 * | 48    | Checksum     | u8   | Bitwise XOR operation between all transmitted bytes                      |
 *
 * The on-board computer can issue the number of ready-to-read PTT Packages checking the PTT_AV value at System
 * Status Frame. It is also possible to check if the PTT Decoder Buffer is empty by trying to read it and
 * checking if an Empty Frame is returned. After reading a PTT Decoder Frame, the on-board computer must send a
 * PTT_POP command, which removes the oldest package from the buffer.
 *
 * When the PTT Decoder Buffer is full, new incoming packages are discarded, and when empty, an Empty Frame is
 * provided for reading.
 *
 * To prevent generation of PTT packages with invalid time tags, the PTT Decoder Task is off at initialization.
 *
 * \see EDC - Environmental Data Collector User Guide. Section 3.3 - PTT Decoder Frames. Page 7.
 *
 * \param[in] config is the configuration parameters of the EDC driver.
 *
 * \return The number of read bytes (-1 on error).
 */
int16_t edc_get_ptt_pkg(edc_config_t config, uint8_t *pkg);

/**
 * \brief Gets the housekeeping (HK) frame.
 *
 * The HK Frame contains the most recent housekeeping information. Its contents are updated whenever a read
 * request is received.
 *
 * | Byte  | Name           | Data | Description                                                            |
 * | :---- | :------------- | :--- | :--------------------------------------------------------------------- |
 * | 0     | Frame type     | u8   | 0x44                                                                   |
 * | 1-4   | Current time   | u32  | Seconds since J2000 epoch                                              |
 * | 5-8   | Elapsed time   | u32  | Elapsed time since last boot in seconds                                |
 * | 9-10  | Current supply | u16  | System current supply in mA                                            |
 * | 11-12 | Voltage supply | u16  | System voltage supply in mV                                            |
 * | 13    | Temperature    | u8   | Board temperature (add -40 to get the result in Celsius)               |
 * | 14    | PLL sync bit   | u8   | RF front-end LO frequency synthesizer indicator of PLL synchronization |
 * | 15-16 | ADC RMS        | u16  | RMS level at front-end output (saturation point is 32768)              |
 * | 17    | Num of RX PTT  | u8   | Number of generated PTT packages since last system initialization      |
 * | 18    | Max parl decod | u8   | Max number of active PTT decoder channels registered since last boot   |
 * | 19    | Mem err count  | u8   | Number of double bit err detected by MSS data mem ctrl since last boot |
 * | 20    | Checksum       | u8   | Bitwise XOR operation between all transmitted bytes                    |
 *
 * \see EDC - Environmental Data Collector User Guide. Section 3.5 - HK Frame. Page 9.
 *
 * \param[in] config is the configuration parameters of the EDC driver.
 *
 * \return The number of read bytes (-1 on error).
 */
int16_t edc_get_hk_pkg(edc_config_t config, uint8_t *hk);

/**
 * \brief Gets the current ADC sample frame.
 *
 * The EDC can provide a sequence of 2048 consecutive samples from the RF front-end output upon OBC request.
 * The last captured sequence is available for reading at ADC Sample Frame.
 *
 * | Byte      | Name          | Data | Description                                                           |
 * | :-------- | :------------ | :--- | :---------------------------------------------------------------------|
 * | 0         | Frame type    | u8   | 0x33 when the buffer contains data. 0xFF when there is no data        |
 * | 1-4       | Time tag      | u32  | Sequence capture time in number of seconds elapsed since J2000 epoch. |
 * | 5-6       | I sample 0    | s16  | First ADC I sample                                                    |
 * | 7-8       | Q sample 0    | s16  | First ADC Q sample                                                    |
 * | ...       | ...           | ...  | ...                                                                   |
 * | 8193-8194 | I sample 2047 | s16  | Last ADC I sample                                                     |
 * | 8195-8196 | Q sample 2047 | s16  | Last ADC Q sample                                                     |
 * | 8197      | Checksum 0    | u8   | Bitwise XOR operation between all transmitted odd bytes               |
 * | 8198      | Checksum 1    | u8   | Bitwise XOR operation between all transmitted even bytes              |
 *
 * \param[in,out] is a poiter to store the ADC sample frame bytes.
 *
 * \see EDC - Environmental Data Collector User Guide. Section 3.4 - ADC Sample Frame. Page 8.
 *
 * \param[in] config is the configuration parameters of the EDC driver.
 *
 * \return The number of read bytes (-1 on error).
 */
int16_t edc_get_adc_seq(edc_config_t config, uint8_t *seq);

/**
 * \brief Writes the string "ECHO" in the debug interface.
 *
 * \see EDC - Environmental Data Collector User Guide. Table 10 - Command List. Page 11.
 *
 * \param[in] config is the configuration parameters of the EDC driver.
 *
 * \return The status/error code.
 */
int edc_echo(edc_config_t config);

/**
 * \brief Calculates the checksum of an array of bytes.
 *
 * This checksum value is obtained through a bitwise XOR operation between all bytes.
 *
 * \param[in] data is an array of bytes to obtain the checksum.
 *
 * \param[in] len is the number of bytes of the given array.
 *
 * \return The calculated checksum value.
 */
uint16_t edc_calc_checksum(uint8_t *data, uint16_t len);

/**
 * \brief Gets the state data.
 *
 * \param[in] config is the configuration parameters of the EDC driver.
 *
 * \param[in,out] state_data is a pointer to a state structure.
 *
 * \return The status/error code.
 */
int edc_get_state(edc_config_t config, edc_state_t *state_data);

/**
 * \brief Gets the PTT data.
 *
 * \param[in] config is the configuration parameters of the EDC driver.
 *
 * \param[in,out] ptt_data is a pointer to store the ptt data.
 *
 * \return The status/error code.
 */
int edc_get_ptt(edc_config_t config, edc_ptt_t *ptt_data);

/**
 * \brief Gets the housekeeping data.
 *
 * \param[in] config is the configuration parameters of the EDC driver.
 *
 * \param[in,out] hk_data is a pointer to store the hk data.
 *
 * \return The status/error code.
 */
int edc_get_hk(edc_config_t config, edc_hk_t *hk_data);

/**
 * \brief Initializes the I2C port.
 *
 * \param[in] config is the configuration parameters of the EDC driver.
 *
 * \return The status/error code.
 */
int edc_i2c_init(edc_config_t config);

/**
 * \brief Writes a given sequence of bytes to the I2C bus.
 *
 * \param[in] config is the configuration parameters of the EDC driver.
 *
 * \param[in] data is array of bytes to write.
 *
 * \param[in] len is the number of bytes to write .
 *
 * \return The status/error code.
 */
int edc_i2c_write(edc_config_t config, uint8_t *data, uint16_t len);

/**
 * \brief Reads data from the I2C bus.
 *
 * \param[in] config is the configuration parameters of the EDC driver.
 *
 * \param[in] data is a pointer to store the read bytes.
 *
 * \param[in] len is the number of bytes to read.
 *
 * \return The status/error code.
 */
int edc_i2c_read(edc_config_t config, uint8_t *data, uint16_t len);

/**
 * \brief GPIO pin initialization.
 *
 * \param[in] config is the configuration parameters of the EDC driver.
 *
 * \return The status/error code.
 */
int edc_gpio_init(edc_config_t config);

/**
 * \brief Set the GPIO pin state.
 *
 * \param[in] config is the configuration parameters of the EDC driver.
 *
 * \return The status/error code.
 */
int edc_gpio_set(edc_config_t config);

/**
 * \brief Clears the GPIO pin.
 *
 * \param[in] config is the configuration parameters of the EDC driver.
 *
 * \return The status/error code.
 */
int edc_gpio_clear(edc_config_t config);

/**
 * \brief Milliseconds delay.
 *
 * \param[in] ms is the period to delay in milliseconds.
 *
 * \return None.
 */
void edc_delay_ms(uint32_t ms);

/**
 * \brief Initializes the EDC UART port.
 *
 * \param[in] config is the configuration parameters of the EDC driver.
 *
 * \return The status/error code.
 */
int edc_uart_init(edc_config_t config);

/**
 * \brief Writes data to the UART port.
 *
 * \param[in] config is the configuration parameters of the EDC driver.
 *
 * \param[in] data is array of bytes to write.
 *
 * \param[in] len is the number of bytes to write .
 *
 * \return The status/error code.
 */
int edc_uart_write(edc_config_t config, uint8_t *data, uint16_t len);

/**
 * \brief Reads data from the UART port.
 *
 * \param[in] config is the configuration parameters of the EDC driver.
 *
 * \param[in] data is a pointer to store the read bytes.
 *
 * \param[in] len is the number of bytes to read.
 *
 * \return The status/error code.
 */
int edc_uart_read(edc_config_t config, uint8_t *data, uint16_t len);

/**
 * \brief Verifies if there is data available to received in the UART RX buffer.
 *
 * \param[in] config is the configuration parameters of the EDC driver.
 *
 * \return The number of available bytes, -1 on error.
 */
int edc_uart_rx_available(edc_config_t config);

#endif /* EDC_H_ */

/** \} End of edc group */
