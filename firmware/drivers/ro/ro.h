/*
 * ro.h
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
 * \brief RO driver definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Bruno Benedetti <brunobenedetti45@gmail.com>
 * \author Renato Augusto Schenkel Meneghin Marchiori
 * 
 * \version 0.0.1
 * 
 * \date 2019/10/27
 * 
 * \defgroup ro RO
 * \ingroup drivers
 * \{
 */


#ifndef RO_H_
#define RO_H_

#include <stdint.h>
#include <stdbool.h>

#include <drivers/i2c/i2c.h>
#include <drivers/gpio/gpio.h>
#include <drivers/uart/uart.h>

#define RO_MODULE_NAME          "RO"

#define RO_SLAVE_ADDRESS        0x15
#define RO_UART_BAUDRATE         115200U

/*============================================================================*/
/* Command IDs                                                                */
/*============================================================================*/

/* General Commands */
#define RO_CMD_RTC_SET              0x01U   /**< Set RTC time (seconds since J2000). */
#define RO_CMD_RESET                0x02U   /**< Reset the RO instrument. */
#define RO_CMD_ECHO                 0x03U   /**< Echo command for communication test. */

/* Acquisition and Tracking */
#define RO_CMD_START_TRACKING       0x10U   /**< Start GNSS tracking task. */
#define RO_CMD_STOP_TRACKING        0x11U   /**< Stop GNSS tracking task. */
#define RO_CMD_START_CAPTURE        0x12U   /**< Start Radio Occultation acquisition. */
#define RO_CMD_ABORT_CAPTURE        0x13U   /**< Abort current acquisition. */

/* Telemetry */
#define RO_CMD_GET_STATE            0x30U   /**< Get current instrument state. */
#define RO_CMD_GET_HK               0x31U   /**< Get housekeeping frame. */
#define RO_CMD_GET_EVENT            0x32U   /**< Get current occultation event metadata. */
#define RO_CMD_GET_NAVIGATION       0x33U   /**< Get navigation solution. */
#define RO_CMD_GET_OBSERVABLES      0x34U   /**< Get scientific observables. */
#define RO_CMD_GET_IQ               0x35U   /**< Get raw I/Q samples. */
#define RO_CMD_GET_HK_PKG          0x36U   /**< Updates HK Frame information, and transmit it through the RS-485 interface. */

/*============================================================================*/
/* Frame IDs                                                                  */
/*============================================================================*/

#define RO_FRAME_ID_STATE           0x11U
#define RO_FRAME_ID_HK              0x22U
#define RO_FRAME_ID_EVENT           0x33U
#define RO_FRAME_ID_NAVIGATION      0x44U
#define RO_FRAME_ID_OBSERVABLES     0x55U
#define RO_FRAME_ID_IQ              0x66U
#define RO_FRAME_ID_ACK             0xF0U
#define RO_FRAME_ID_ERROR           0xFFU


/*============================================================================*/
/* Frame Lengths                                                              */
/*============================================================================*/

#define RO_FRAME_STATE_LEN          sizeof(ro_state_t)
#define RO_FRAME_HK_LEN             sizeof(ro_hk_t)
#define RO_FRAME_EVENT_LEN          sizeof(ro_event_t)
#define RO_FRAME_NAV_LEN            sizeof(ro_navigation_t)
#define RO_FRAME_OBS_LEN            sizeof(ro_observables_t)
#define RO_FRAME_ACK_LEN            2

/**
 * \brief RO interfaces.
 **/
typedef enum
{
    RO_IF_UART=0,
    RO_IF_I2C

} ro_if_t;

/**
 * \brief RO configuration parameters.
 **/
typedef struct
{
    ro_if_t interface;
    i2c_port_t i2c_port;
    uint32_t i2c_bitrate;
    uart_port_t uart_port;
    gpio_pin_t en_pin;
} ro_config_t;

/**
 * \brief RO command.
 **/
typedef struct
{
    uint8_t id;
    uint32_t param;
} ro_cmd_t;

/**
 * \brief RO state data.
 **/
typedef struct
{
    uint32_t current_time;
    uint8_t operational_state;
    uint8_t tracking_state;
    uint8_t event_available;
    uint8_t reserved;
} ro_state_t;

/**
 * \brief RO housekeeping data.
 **/
typedef struct
{
    uint32_t current_time;
    uint32_t elapsed_time;
    uint16_t fpga_temperature;
    uint16_t rf_temperature;
    uint16_t voltage;
    uint16_t current;
    uint8_t operational_state;
    uint8_t reserved[7];
} ro_hk_t;

/**
 * \brief RO event data.
 **/
typedef struct
{
    uint32_t event_id;
    uint32_t timestamp_start;
    uint32_t timestamp_end;
    uint8_t prn;
    uint32_t duration_ms;
    uint32_t num_samples;
    uint8_t event_status;
} ro_event_t;

/**
 * \brief RO navigation data.
 **/
typedef struct
{
    int32_t sat_pos_x;
    int32_t sat_pos_y;
    int32_t sat_pos_z;
    int32_t sat_vel_x;
    int32_t sat_vel_y;
    int32_t sat_vel_z;
    uint32_t gnss_time;
} ro_navigation_t;

/**
 * \brief RO observation data.
 **/
typedef struct
{
    uint32_t observation_id;
    uint32_t gnss_time;
    uint8_t  prn;
    int32_t carrier_phase;
    int32_t code_phase;
    int32_t doppler;
    uint16_t snr;
    uint8_t tracking_state;
    uint8_t reserved[7];
} ro_observation_t;

/** 
 * \brief RO ADC sequence data.
 **/
typedef struct
{
    uint32_t timestamp;
    uint16_t sample_count;
} ro_iq_header_t;

/*
 * \brief RO operational state.
 */
typedef enum
{
    RO_STATE_OFF = 0,
    RO_STATE_BOOT,
    RO_STATE_IDLE,
    RO_STATE_ACQUISITION,
    RO_STATE_TRACKING,
    RO_STATE_OPEN_LOOP,
    RO_STATE_EVENT,
    RO_STATE_ERROR
} ro_operational_state_t;

/*
 * \brief RO tracking state.
 */
typedef enum
{
    RO_TRACK_IDLE = 0,
    RO_TRACK_SEARCH,
    RO_TRACK_PLL,
    RO_TRACK_DLL,
    RO_TRACK_ACQUISITION,
    RO_TRACK_OPEN_LOOP,
    RO_TRACK_LOST
} ro_tracking_state_t;


/*============================================================================*/
/* Initialization                                                             */
/*============================================================================*/

/// @brief 
/// @param config 
/// @return 
int ro_init(ro_config_t config);

/// @brief 
/// @param config 
/// @return 
int ro_enable(ro_config_t config);

/// @brief 
/// @param config 
/// @return 
int ro_disable(ro_config_t config);

int ro_check_device(ro_config_t config);

int ro_set_rtc_time(ro_config_t config, uint32_t time);

/*============================================================================*/
/* Communication                                                                                                                                 */
/*============================================================================*/

int ro_write_cmd(ro_config_t config, ro_cmd_t cmd);

int ro_read(ro_config_t config, uint8_t *data, uint16_t len);

int ro_echo(ro_config_t config);

uint16_t ro_calc_checksum(uint8_t *data, uint16_t len);


/*============================================================================*/
/* Instrument Control                                                         */
/*============================================================================*/

int ro_start_tracking(ro_config_t config);

int ro_stop_tracking(ro_config_t config);

int ro_start_capture(ro_config_t config);

int ro_abort_capture(ro_config_t config);

int ro_enable_open_loop(ro_config_t config);

int ro_disable_open_loop(ro_config_t config);

int ro_clear_event(ro_config_t config);

/*============================================================================*/
/* Data Acquisition                                                           */
/*============================================================================*/

int16_t ro_get_state_pkg(ro_config_t config, uint8_t *status);

int16_t ro_get_hk_pkg(ro_config_t config, uint8_t *hk);

int16_t ro_get_event_pkg(ro_config_t config, uint8_t *event);

int16_t ro_get_navigation_pkg(ro_config_t config, uint8_t *navigation);

int16_t ro_get_observation_pkg(ro_config_t config, uint8_t *observation);

/*============================================================================*/
/* Data Parsing                                                               */
/*============================================================================*/

int ro_get_state(ro_config_t config,
                 ro_state_t *state);

int ro_get_hk(ro_config_t config,
              ro_hk_t *hk);

int ro_get_event(ro_config_t config,
                 ro_event_t *event);

int ro_get_navigation(ro_config_t config,
                      ro_navigation_t *navigation);

int ro_get_observation(ro_config_t config,
                       ro_observation_t *observation);

/*============================================================================*/
/* I2C Driver                                                                 */
/*============================================================================*/

int ro_i2c_init(ro_config_t config);

int ro_i2c_write(ro_config_t config,
                 uint8_t *data,
                 uint16_t len);

int ro_i2c_read(ro_config_t config,
                uint8_t *data,
                uint16_t len);

/*============================================================================*/
/* GPIO Driver                                                                */
/*============================================================================*/

int ro_gpio_init(ro_config_t config);

int ro_gpio_set(ro_config_t config);

int ro_gpio_clear(ro_config_t config);

/*============================================================================*/
/* UART Driver                                                                */
/*============================================================================*/

int ro_uart_init(ro_config_t config);

int ro_uart_write(ro_config_t config,
                  uint8_t *data,
                  uint16_t len);

int ro_uart_read(ro_config_t config,
                 uint8_t *data,
                 uint16_t len);

int ro_uart_rx_available(ro_config_t config);

/*============================================================================*/
/* Delay                                                                      */
/*============================================================================*/

void ro_delay_ms(uint32_t ms);

#endif /* RO_H_ */