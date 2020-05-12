/*
 * si446x_cmd.h
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with OBDH 2.0. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Si446x commands definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.3.20
 * 
 * \date 10/05/2020
 * 
 * \defgroup si446x_cmd Commands
 * \ingroup si446x
 * \{
 */

#ifndef SI446X_CMD_H_
#define SI446X_CMD_H_

#define SI446X_CMD_COMPLETE                                 0xFF    /**< CTS value. */

/* Boot commands */
#define SI446X_CMD_POWER_UP                                 0x02    /**< Command to power-up the device and select the operational mode and functionality. */

/* Common commands */
#define SI446X_CMD_NOP                                      0x00    /**< No operation command */
#define SI446X_CMD_PART_INFO                                0x01    /**< Reports basic information about the device. */
#define SI446X_CMD_FUNC_INFO                                0x10    /**< Returns the Function revision information of the device. */
#define SI446X_CMD_SET_PROPERTY                             0x11    /**< Sets the value of one or more properties. */
#define SI446X_CMD_GET_PROPERTY                             0x12    /**< Retrieves the value of one or more properties. */
#define SI446X_CMD_GPIO_PIN_CFG                             0x13    /**< Configures the GPIO pins. */
#define SI446X_CMD_FIFO_INFO                                0x15    /**< Access the current byte counts in the TX and RX FIFOs and provide for resetting the FIFOs. */
#define SI446X_CMD_GET_INT_STATUS                           0x20    /**< Returns the interrupt status of ALL the possible interrupt events (both STATUS and PENDING). Optionally, it may be used to clear latched (PENDING) interrupt events. */
#define SI446X_CMD_REQUEST_DEVICE_STATE                     0x33    /**< Request current device state and channel. */
#define SI446X_CMD_CHANGE_STATE                             0x34    /**< Manually switch the chip to a desired operating state. */
#define SI446X_CMD_OFFLINE_RECAL                            0x38    /**< Recalibrates due to temperature change. */
#define SI446X_CMD_READ_CMD_BUF                             0x44    /**< Used to read CTS and the command response. */
#define SI446X_CMD_FRR_A_READ                               0x50    /**< Reads the fast response registers (FRR) starting with FRR_A. */
#define SI446X_CMD_FRR_B_READ                               0x51    /**< Reads the fast response registers (FRR) starting with FRR_B. */
#define SI446X_CMD_FRR_C_READ                               0x53    /**< Reads the fast response registers (FRR) starting with FRR_C. */
#define SI446X_CMD_FRR_D_READ                               0x57    /**< Reads the fast response registers (FRR) starting with FRR_D. */

/* IR CAL commands */
#define SI446X_CMD_IRCAL                                    0x17    /**< Image rejection calibration. */
#define SI446X_CMD_IRCAL_MANUAL                             0x19    /**< Image rejection calibration. */

/* TX commands */
#define SI446X_CMD_START_TX                                 0x31    /**< Switches to TX state and starts transmission of a packet. */
#define SI446X_CMD_TX_HOP                                   0x37    /**< Hop to a new frequency while in TX. */
#define SI446X_CMD_WRITE_TX_FIFO                            0x66    /**< Writes data byte(s) to the TX FIFO. */

/* RX commands */
#define SI446X_CMD_PACKET_INFO                              0x16    /**< Returns information about the length of the variable field in the last packet received and (optionally) overrides field length. */
#define SI446X_CMD_GET_MODEM_STATUS                         0x22    /**< Returns the interrupt status of the Modem Interrupt Group (both STATUS and PENDING). Optionally, it may be used to clear latched (PENDING) interrupt events. */
#define SI446X_CMD_START_RX                                 0x32    /**< Switches to RX state and starts reception of a packet. */
#define SI446X_CMD_RX_HOP                                   0x36    /**< Manually hop to a new frequency while in RX mode. */
#define SI446X_CMD_READ_RX_FIFO                             0x77    /**< Reads data byte(s) from the RX FIFO. */

/* Advanced commands */
#define SI446X_CMD_GET_ADC_READING                          0x14    /**< Performs conversions using the Auxiliary ADC and returns the results of those conversions. */
#define SI446X_CMD_GET_PH_STATUS                            0x21    /**< Returns the interrupt status of the Packet Handler Interrupt Group (both STATUS and PENDING). Optionally, it may be used to clear latched (PENDING) interrupt events. */
#define SI446X_CMD_GET_CHIP_STATUS                          0x23    /**< Returns the interrupt status of the Chip Interrupt Group (both STATUS and PENDING). Optionally, it may be used to clear latched (PENDING) interrupt events. */

/* Other commands */
#define SI446X_CMD_PATCH_IMAGE                              0x04    /**< . */
#define SI446X_CMD_PROTOCOL_CFG                             0x18    /**< . */
#define SI446X_CMD_START_MFSK                               0x35    /**< . */


/* GPIO configuration */
#define SI446X_GPIO_NO_CHANGE                               0       /**< Do not modify the behavior of this pin. */
#define SI446X_GPIO_DISABLED                                1       /**< Input and output drivers disabled. */
#define SI446X_GPIO_LOW                                     2       /**< CMOS output driven low. */
#define SI446X_GPIO_HIGH                                    3       /**< CMOS output driven high. */
#define SI446X_GPIO_INPUT                                   4       /**< This is used for all GPIO functions that require the pin to be an input. */
#define SI446X_GPIO_32_KHZ_CLOCK                            5       /**< 32 kHz clock. */
#define SI446X_GPIO_BOOT_CLOCK                              6       /**< Outputs boot clock. */
#define SI446X_GPIO_DIVIDED_MCU_CLOCK                       7       /**< Divided MCU clock. */
#define SI446X_GPIO_CTS                                     8       /**< Output High when clear to send (CTS) a new command, output low otherwise. */
#define SI446X_GPIO_INV_CTS                                 9       /**< Output low when clear to send a new command, output high otherwise. */
#define SI446X_GPIO_HIGH_ON_CMD_OVERLAP                     10      /**< Output low unless command overlap occurs. When command overlap occurs output goes high until the rising edge of CTS. */
#define SI446X_GPIO_SPI_DATA_OUT                            11      /**< SPI data out. */
#define SI446X_GPIO_HIGH_AFTER_RESET                        12      /**< Output low until power on reset is complete then output high. */
#define SI446X_GPIO_HIGH_AFTER_CALIBRATION                  13      /**< Pulses high when calibration timer expires. */
#define SI446X_GPIO_HIGH_AFTER_WUT                          14      /**< Pulses high when wakeup timer expires. */
#define SI446X_GPIO_UNUSED_0                                15      /**< Unused0. */
#define SI446X_GPIO_TX_DATA_CLOCK                           16      /**< TX data CLK output to be used in conjuction with TX Data pin. */
#define SI446X_GPIO_RX_DATA_CLOCK                           17      /**< RX data CLK output to be used in conjuction with RX Data pin. */
#define SI446X_GPIO_UNUSED_1                                18      /**< Unused1. */
#define SI446X_GPIO_TX_DATA                                 19      /**< TX data. This option is not to be used for Tx direct mode input. For Tx direct mode input, use option 4. */
#define SI446X_GPIO_RX_DATA                                 20      /**< RX data. */
#define SI446X_GPIO_RX_RAW_DATA                             21      /**< RX raw data. */
#define SI446X_GPIO_ANTENNA_1_SWITCH                        22      /**< Antenna 1 Switch used for antenna diversity. */
#define SI446X_GPIO_ANTENNA_2_SWITCH                        23      /**< Antenna 2 Switch used for antenna diversity. */
#define SI446X_GPIO_VALID_PREAMBLE                          24      /**< High when a valid preamble is detected. Returns to output low after a packet is received or sync word timeout occurs. */
#define SI446X_GPIO_INVALID_PREAMBLE                        25      /**< High when an invalid preamble is detected. */
#define SI446X_GPIO_SYNC_DETECTED                           26      /**< High when a sync word is detected. Returns to output low after the packet is received. */
#define SI446X_GPIO_RSSI_ABOVE_CAT                          27      /**< High when RSSI above clear channel assesment threshold, low when below threshold. */
#define SI446X_GPIO_TX_STATE                                32      /**< High while in the transmit state. */
#define SI446X_GPIO_RX_STATE                                33      /**< High while in the receive state. */
#define SI446X_GPIO_RX_FIFO_ALMOST_FULL                     34      /**< High while the rx fifo is almost full. */
#define SI446X_GPIO_TX_FIFO_ALMOST_EMPTY                    35      /**< High while the tx fifo is almost empty. */
#define SI446X_GPIO_BATT_LOW                                36      /**< High while the battery voltage is low. */
#define SI446X_GPIO_RSSI_ABOVE_CAT_LOW                      37      /**< High when RSSI above clear channel assesment threshold. Goes low on sync detect or exiting rx state. */
#define SI446X_GPIO_HOP                                     38      /**< Toggles when hop occurs. */
#define SI446X_GPIO_HOP_TABLE_WRAPPED                       39      /**< Toggles when the hop table wraps. */
#define SI446X_NIRQ_INTERRUPT_SIGNAL                        39      /**< Active low interrupt signal. */


/* Interruption status masks */
#define SI446X_INT_STATUS_CHIP_INT_STATUS_PEND              0x04    /**< If set, CHIP_INT_STATUS interrupt is pending. */
#define SI446X_INT_STATUS_MODEM_INT_STATUS_PEND             0x02    /**< If set, MODEM_INT_STATUS interrupt is pending. */
#define SI446X_INT_STATUS_PH_INT_STATUS_PEND                0x01    /**< If set, PH_INT_STATUS interrupt is pending. */
#define SI446X_INT_STATUS_CHIP_INT_STATUS                   0x04    /**< If set, chip status has interrupt pending. */
#define SI446X_INT_STATUS_MODEM_INT_STATUS                  0x02    /**< If set, modem status has interrupt pending. */
#define SI446X_INT_STATUS_PH_INT_STATUS                     0x01    /**< If set, packet handler status has interrupt pending. */
#define SI446X_INT_STATUS_FILTER_MATCH_PEND                 0x80    /**< If set, FILTER_MATCH interrupt is pending. */
#define SI446X_INT_STATUS_FILTER_MISS_PEND                  0x40    /**< If set, FILTER_MISS interrupt is pending. */
#define SI446X_INT_STATUS_PACKET_SENT_PEND                  0x20    /**< If set, PACKET_SENT interrupt is pending. */
#define SI446X_INT_STATUS_PACKET_RX_PEND                    0x10    /**< If set, PACKET_RX interrupt is pending. */
#define SI446X_INT_STATUS_CRC_ERROR_PEND                    0x08    /**< If set, CRC_ERROR interrupt is pending. */
#define SI446X_INT_STATUS_TX_FIFO_ALMOST_EMPTY_PEND         0x02    /**< If set, TX_FIFO_ALMOST_EMPTY interrupt is pending. */
#define SI446X_INT_STATUS_RX_FIFO_ALMOST_FULL_PEND          0x01    /**< If set, RX_FIFO_ALMOST_FULL interrupt is pending. */
#define SI446X_INT_STATUS_FILTER_MATCH                      0x80    /**< If set, incoming packet matched filter. */
#define SI446X_INT_STATUS_FILTER_MISS                       0x40    /**< If set, incoming packet was discarded because filter did not match. */
#define SI446X_INT_STATUS_PACKET_SENT                       0x20    /**< If set, Packet Sent. */
#define SI446X_INT_STATUS_PACKET_RX                         0x10    /**< If set, Packet Received. */
#define SI446X_INT_STATUS_CRC_ERROR                         0x08    /**< If set, CRC-32 error. */
#define SI446X_INT_STATUS_TX_FIFO_ALMOST_EMPTY              0x02    /**< If set, TX fifo is below watermark. */
#define SI446X_INT_STATUS_RX_FIFO_ALMOST_FULL               0x01    /**< If set, RX fifo is above watermark. */
#define SI446X_INT_STATUS_INVALID_SYNC_PEND                 0x20    /**< If set, INVALID_SYNC interrupt is pending. */
#define SI446X_INT_STATUS_RSSI_JUMP_PEND                    0x10    /**< If set, RSSI_JUMP interrupt is pending. */
#define SI446X_INT_STATUS_RSSI_PEND                         0x08    /**< If set, RSSI interrupt is pending. */
#define SI446X_INT_STATUS_INVALID_PREAMBLE_PEND             0x04    /**< If set, INVALID_PREAMBLE interrupt is pending. */
#define SI446X_INT_STATUS_PREAMBLE_DETECT_PEND              0x02    /**< If set, PREAMBLE_DETECT interrupt is pending. */
#define SI446X_INT_STATUS_SYNC_DETECT_PEND                  0x01    /**< If set, SYNC_DETECT interrupt is pending. */
#define SI446X_INT_STATUS_INVALID_SYNC                      0x20    /**< If set, invalid sync has been detected. */
#define SI446X_INT_STATUS_RSSI_JUMP                         0x10    /**< If set, RSSI jump above MODEM_RSSI_JUMP_THRESH has occured. */
#define SI446X_INT_STATUS_RSSI                              0x08    /**< If set, RSSI is above MODEM_RSSI_THRESH. */
#define SI446X_INT_STATUS_INVALID_PREAMBLE                  0x04    /**< If set, invalid preamble has been detected. */
#define SI446X_INT_STATUS_PREAMBLE_DETECT                   0x02    /**< If set, preamble has been detected. */
#define SI446X_INT_STATUS_SYNC_DETECT                       0x01    /**< If set, sync has been detected. */
#define SI446X_INT_STATUS_CAL_PEND                          0xC0    /**< If set, CAL interrupt is pending. */
#define SI446X_INT_STATUS_FIFO_UNDERFLOW_OVERFLOW_ERR_PEND  0x20    /**< If set, FIFO_UNDERFLOW_OVERFLOW_ERROR interrupt is pending. */
#define SI446X_INT_STATUS_STATE_CHANGE_PEND                 0x10    /**< If set, STATE_CHANGE interrupt is pending. */
#define SI446X_INT_STATUS_CMD_ERROR_PEND                    0x08    /**< If set, CMD_ERROR interrupt is pending. */
#define SI446X_INT_STATUS_CHIP_READY_PEND                   0x04    /**< If set, CHIP_READY interrupt is pending. */
#define SI446X_INT_STATUS_LOW_BATT_PEND                     0x02    /**< If set, LOW_BATT interrupt is pending. */
#define SI446X_INT_STATUS_WUT_PEND                          0x01    /**< If set, WUT interrupt is pending. */
#define SI446X_INT_STATUS_CAL                               0xC0    /**< If set, calibration timer has expired. */
#define SI446X_INT_STATUS_FIFO_UNDERFLOW_OVERFLOW_ERR       0x20    /**< If set, fifo underflow or overflow occured. */
#define SI446X_INT_STATUS_STATE_CHANGE                      0x10    /**< If set, a state change has occured. */
#define SI446X_INT_STATUS_CMD_ERROR                         0x08    /**< If set, command error has occured. */
#define SI446X_INT_STATUS_CHIP_READY                        0x04    /**< If set, chip is ready to accept commands. */
#define SI446X_INT_STATUS_LOW_BATT                          0x02    /**< If set, low battery has been detected. */
#define SI446X_INT_STATUS_WUT                               0x01    /**< If set, wakeup timer has expired. */


/* Interruption enable control masks */
#define SI446X_INT_CTL_ENABLE_CHIP_INT_STATUS_EN            0x04    /**< If set, Enables CHIP_INT_STATUS interrupt. */
#define SI446X_INT_CTL_ENABLE_MODEM_INT_STATUS_EN           0x02    /**< If set, Enables MODEM_INT_STATUS interrupt. */
#define SI446X_INT_CTL_ENABLE_PH_INT_STATUS_EN              0x01    /**< If set, Enables PH_INT_STATUS interrupt. */


/* Start TX command flags */
#define SI446X_START_TX_RETRANSMIT                          0x04    /**< Send last packet again. */
#define SI446X_START_TX_DO_NOT_RETRANSMIT                   0x00    /**< Send data that has been written to fifo. */
#define SI446X_START_TX_START_TX_IMMEDIATELY                0x00    /**< Start TX immediately. */
#define SI446X_START_TX_START_TX_WHEN_WAKEUP_TIMER_EXPIRES  0x01    /**< Start TX when wake up timer expires. */


/* Interruption status flags */
#define SI446X_INT_STATUS_PACKET_SENT                       0x20    /**< Packet sent indicator. */

#endif /* SI446X_CMD_H_ */

/** \} End of si446x_cmd group */
