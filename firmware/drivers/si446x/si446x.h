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
 * \version 0.3.15
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

#define SI446X_MODULE_NAME                                  "Si446x"

#define SI446X_PART_INFO                                    0x4463
#define SI446X_TX_FIFO_LEN                                  128
#define SI446X_RX_FIFO_LEN                                  128
#define SI446X_TX_FIFO_ALMOST_EMPTY_THRESHOLD               48
#define SI446X_RX_FIFO_ALMOST_FULL_THRESHOLD                48
#define SI446X_CTS_REPLY                                    0xFF
#define SI446X_CTS_TIMEOUT                                  2500    /* Waiting time for a valid FFh CTS reading. The typical time is 20 us. */
#define SI446X_CTS_TIMEOUT_MS                               5       /* Waiting time in milliseconds for a valid FFh CTS reading. The typical time is 20 us. */
#define SI446X_TX_TIMEOUT                                   20000   /* Waiting time for packet send interrupt. this time is depended on tx length and data rate of wireless. */
#define SI446X_FREQ_CHANNEL                                 0       /* Frequency channel. */

/* This value must be obtained measuring the output signal with a frequency analyzer */
#define SI446X_XO_TUNE_REG_VALUE                            97

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

/* Properties */
#define SI446X_PROPERTY_GLOBAL_XO_TUNE                      0x0000  /**< Configure crystal oscillator frequency tuning bank. */
#define SI446X_PROPERTY_GLOBAL_CLK_CFG                      0x0001  /**< Clock configuration options. */
#define SI446X_PROPERTY_GLOBAL_LOW_BATT_THRESH              0x0002  /**< Low battery threshold. */
#define SI446X_PROPERTY_GLOBAL_CONFIG                       0x0003  /**< Global configuration settings. */
#define SI446X_PROPERTY_GLOBAL_WUT_CONFIG                   0x0004  /**< GLOBAL WUT configuation. */
#define SI446X_PROPERTY_GLOBAL_WUT_M_15_8                   0x0005  /**< Configure WUT_M_15_8. */
#define SI446X_PROPERTY_GLOBAL_WUT_M_7_0                    0x0006  /**< Configure WUT_M_7_0. */
#define SI446X_PROPERTY_GLOBAL_WUT_R                        0x0007  /**< Configure WUT_R. */
#define SI446X_PROPERTY_GLOBAL_WUT_LDC                      0x0008  /**< Configure WUT_LDC. */
#define SI446X_PROPERTY_INT_CTL_ENABLE                      0x0100  /**< Interrupt enable property. */
#define SI446X_PROPERTY_INT_CTL_PH_ENABLE                   0x0101  /**< Packet handler interrupt enable property. */
#define SI446X_PROPERTY_INT_CTL_MODEM_ENABLE                0x0102  /**< Modem interrupt enable property. */
#define SI446X_PROPERTY_INT_CTL_CHIP_ENABLE                 0x0103  /**< Chip interrupt enable property. */
#define SI446X_PROPERTY_FRR_CTL_A_MODE                      0x0200  /**< Fast Response Register A Configuration. */
#define SI446X_PROPERTY_FRR_CTL_B_MODE                      0x0201  /**< Fast Response Register B Configuration. */
#define SI446X_PROPERTY_FRR_CTL_C_MODE                      0x0202  /**< Fast Response Register C Configuration. */
#define SI446X_PROPERTY_FRR_CTL_D_MODE                      0x0203  /**< Fast Response Register D Configuration. */
#define SI446X_PROPERTY_PREAMBLE_TX_LENGTH                  0x1000  /**< Preamble length. */
#define SI446X_PROPERTY_PREAMBLE_CONFIG_STD_1               0x1001  /**< Standard preamble configuration. */
#define SI446X_PROPERTY_PREAMBLE_CONFIG_NSTD                0x1002  /**< Non-standard preamble configuation. */
#define SI446X_PROPERTY_PREAMBLE_CONFIG_STD_2               0x1003  /**< Standard preamble configuration. */
#define SI446X_PROPERTY_PREAMBLE_CONFIG                     0x1004  /**< Preamble configuration bits. */
#define SI446X_PROPERTY_PREAMBLE_PATTERN_31_24              0x1005  /**< Preamble pattern. */
#define SI446X_PROPERTY_PREAMBLE_PATTERN_23_16              0x1006  /**< Preamble pattern. */
#define SI446X_PROPERTY_PREAMBLE_PATTERN_15_8               0x1007  /**< Preamble pattern. */
#define SI446X_PROPERTY_PREAMBLE_PATTERN_7_0                0x1008  /**< Preamble pattern. */
#define SI446X_PROPERTY_SYNC_CONFIG                         0x1100  /**< Sync configuration bits. */
#define SI446X_PROPERTY_SYNC_BITS_31_24                     0x1101  /**< Byte 3 of sync word. */
#define SI446X_PROPERTY_SYNC_BITS_23_16                     0x1102  /**< Byte 2 of sync word. */
#define SI446X_PROPERTY_SYNC_BITS_15_8                      0x1103  /**< Byte 1 of sync word. */
#define SI446X_PROPERTY_SYNC_BITS_7_0                       0x1104  /**< Byte 0 of sync word. */
#define SI446X_PROPERTY_PKT_CRC_CONFIG                      0x1200  /**< Select a CRC polynomial and seed. */
#define SI446X_PROPERTY_PKT_CONFIG1                         0x1206  /**< General packet configuration bits. */
#define SI446X_PROPERTY_PKT_LEN                             0x1208  /**< Provides information regarding how to use the length from the received packet. */
#define SI446X_PROPERTY_PKT_LEN_FIELD_SOURCE                0x1209  /**< Field number containing the embedded length field. */
#define SI446X_PROPERTY_PKT_LEN_ADJUST                      0x120A  /**< Adjust length field by this amount to derive the byte count of the variable length field. */
#define SI446X_PROPERTY_PKT_TX_THRESHOLD                    0x120B  /**< TX almost empty threshold. */
#define SI446X_PROPERTY_PKT_RX_THRESHOLD                    0x120C  /**< RX almost full threshold. */
#define SI446X_PROPERTY_PKT_FIELD_1_LENGTH_12_8             0x120D  /**< . */
#define SI446X_PROPERTY_PKT_FIELD_1_LENGTH_7_0              0x120E  /**< . */
#define SI446X_PROPERTY_PKT_FIELD_1_CONFIG                  0x120F  /**< . */
#define SI446X_PROPERTY_PKT_FIELD_1_CRC_CONFIG              0x1210  /**< . */
#define SI446X_PROPERTY_PKT_FIELD_2_LENGTH_12_8             0x1211  /**< . */
#define SI446X_PROPERTY_PKT_FIELD_2_LENGTH_7_0              0x1212  /**< . */
#define SI446X_PROPERTY_PKT_FIELD_2_CONFIG                  0x1213  /**< . */
#define SI446X_PROPERTY_PKT_FIELD_2_CRC_CONFIG              0x1214  /**< . */
#define SI446X_PROPERTY_PKT_FIELD_3_LENGTH_12_8             0x1215  /**< . */
#define SI446X_PROPERTY_PKT_FIELD_3_LENGTH_7_0              0x1216  /**< . */
#define SI446X_PROPERTY_PKT_FIELD_3_CONFIG                  0x1217  /**< . */
#define SI446X_PROPERTY_PKT_FIELD_3_CRC_CONFIG              0x1218  /**< . */
#define SI446X_PROPERTY_PKT_FIELD_4_LENGTH_12_8             0x1219  /**< . */
#define SI446X_PROPERTY_PKT_FIELD_4_LENGTH_7_0              0x121A  /**< . */
#define SI446X_PROPERTY_PKT_FIELD_4_CONFIG                  0x121B  /**< . */
#define SI446X_PROPERTY_PKT_FIELD_4_CRC_CONFIG              0x121C  /**< . */
#define SI446X_PROPERTY_PKT_FIELD_5_LENGTH_12_8             0x121D  /**< . */
#define SI446X_PROPERTY_PKT_FIELD_5_LENGTH_7_0              0x121E  /**< . */
#define SI446X_PROPERTY_PKT_FIELD_5_CONFIG                  0x121F  /**< . */
#define SI446X_PROPERTY_PKT_FIELD_5_CRC_CONFIG              0x1220  /**< . */
#define SI446X_PROPERTY_PKT_RX_FIELD_1_LENGTH_12_8          0x1221  /**< . */
#define SI446X_PROPERTY_PKT_RX_FIELD_1_LENGTH_7_0           0x1222  /**< . */
#define SI446X_PROPERTY_PKT_RX_FIELD_1_CONFIG               0x1223  /**< . */
#define SI446X_PROPERTY_PKT_RX_FIELD_1_CRC_CONFIG           0x1224  /**< . */
#define SI446X_PROPERTY_PKT_RX_FIELD_2_LENGTH_12_8          0x1225  /**< . */
#define SI446X_PROPERTY_PKT_RX_FIELD_2_LENGTH_7_0           0x1226  /**< . */
#define SI446X_PROPERTY_PKT_RX_FIELD_2_CONFIG               0x1227  /**< . */
#define SI446X_PROPERTY_PKT_RX_FIELD_2_CRC_CONFIG           0x1228  /**< . */
#define SI446X_PROPERTY_PKT_RX_FIELD_3_LENGTH_12_8          0x1229  /**< . */
#define SI446X_PROPERTY_PKT_RX_FIELD_3_LENGTH_7_0           0x122A  /**< . */
#define SI446X_PROPERTY_PKT_RX_FIELD_3_CONFIG               0x122B  /**< . */
#define SI446X_PROPERTY_PKT_RX_FIELD_3_CRC_CONFIG           0x122C  /**< . */
#define SI446X_PROPERTY_PKT_RX_FIELD_4_LENGTH_12_8          0x122D  /**< . */
#define SI446X_PROPERTY_PKT_RX_FIELD_4_LENGTH_7_0           0x122E  /**< . */
#define SI446X_PROPERTY_PKT_RX_FIELD_4_CONFIG               0x122F  /**< . */
#define SI446X_PROPERTY_PKT_RX_FIELD_4_CRC_CONFIG           0x1230  /**< . */
#define SI446X_PROPERTY_PKT_RX_FIELD_5_LENGTH_12_8          0x1231  /**< . */
#define SI446X_PROPERTY_PKT_RX_FIELD_5_LENGTH_7_0           0x1232  /**< . */
#define SI446X_PROPERTY_PKT_RX_FIELD_5_CONFIG               0x1233  /**< . */
#define SI446X_PROPERTY_PKT_RX_FIELD_5_CRC_CONFIG           0x1234  /**< . */
#define SI446X_PROPERTY_MODEM_MOD_TYPE                      0x2000  /**< . */
#define SI446X_PROPERTY_MODEM_MAP_CONTROL                   0x2001  /**< . */
#define SI446X_PROPERTY_MODEM_DSM_CTRL                      0x2002  /**< . */
#define SI446X_PROPERTY_MODEM_DATA_RATE_2                   0x2003  /**< . */
#define SI446X_PROPERTY_MODEM_DATA_RATE_1                   0x2004  /**< . */
#define SI446X_PROPERTY_MODEM_DATA_RATE_0                   0x2005  /**< . */
#define SI446X_PROPERTY_MODEM_TX_NCO_MODE_3                 0x2006  /**< . */
#define SI446X_PROPERTY_MODEM_TX_NCO_MODE_2                 0x2007  /**< . */
#define SI446X_PROPERTY_MODEM_TX_NCO_MODE_1                 0x2008  /**< . */
#define SI446X_PROPERTY_MODEM_TX_NCO_MODE_0                 0x2009  /**< . */
#define SI446X_PROPERTY_MODEM_FREQ_DEV_2                    0x200A  /**< . */
#define SI446X_PROPERTY_MODEM_FREQ_DEV_1                    0x200B  /**< . */
#define SI446X_PROPERTY_MODEM_FREQ_DEV_0                    0x200C  /**< . */
#define SI446X_PROPERTY_MODEM_TX_RAMP_DELAY                 0x2018  /**< . */
#define SI446X_PROPERTY_MODEM_MDM_CTRL                      0x2019  /**< . */
#define SI446X_PROPERTY_MODEM_IF_CONTROL                    0x201A  /**< . */
#define SI446X_PROPERTY_MODEM_IF_FREQ_2                     0x201B  /**< . */
#define SI446X_PROPERTY_MODEM_IF_FREQ_1                     0x201C  /**< . */
#define SI446X_PROPERTY_MODEM_IF_FREQ_0                     0x201D  /**< . */
#define SI446X_PROPERTY_MODEM_DECIMATION_CFG1               0x201E  /**< . */
#define SI446X_PROPERTY_MODEM_DECIMATION_CFG0               0x201F  /**< . */
#define SI446X_PROPERTY_MODEM_BCR_OSR_1                     0x2022  /**< . */
#define SI446X_PROPERTY_MODEM_BCR_OSR_0                     0x2023  /**< . */
#define SI446X_PROPERTY_MODEM_BCR_NCO_OFFSET_2              0x2024  /**< . */
#define SI446X_PROPERTY_MODEM_BCR_NCO_OFFSET_1              0x2025  /**< . */
#define SI446X_PROPERTY_MODEM_BCR_NCO_OFFSET_0              0x2026  /**< . */
#define SI446X_PROPERTY_MODEM_BCR_GAIN_1                    0x2027  /**< . */
#define SI446X_PROPERTY_MODEM_BCR_GAIN_0                    0x2028  /**< . */
#define SI446X_PROPERTY_MODEM_BCR_GEAR                      0x2029  /**< . */
#define SI446X_PROPERTY_MODEM_BCR_MISC1                     0x202A  /**< . */
#define SI446X_PROPERTY_MODEM_AFC_GEAR                      0x202C  /**< . */
#define SI446X_PROPERTY_MODEM_AFC_WAIT                      0x202D  /**< . */
#define SI446X_PROPERTY_MODEM_AFC_GAIN_1                    0x202E  /**< . */
#define SI446X_PROPERTY_MODEM_AFC_GAIN_0                    0x202F  /**< . */
#define SI446X_PROPERTY_MODEM_AFC_LIMITER_1                 0x2030  /**< . */
#define SI446X_PROPERTY_MODEM_AFC_LIMITER_0                 0x2031  /**< . */
#define SI446X_PROPERTY_MODEM_AFC_MISC                      0x2032  /**< . */
#define SI446X_PROPERTY_MODEM_AGC_CONTROL                   0x2035  /**< . */
#define SI446X_PROPERTY_MODEM_AGC_WINDOW_SIZE               0x2038  /**< . */
#define SI446X_PROPERTY_MODEM_AGC_RFPD_DECAY                0x2039  /**< . */
#define SI446X_PROPERTY_MODEM_AGC_IFPD_DECAY                0x203A  /**< . */
#define SI446X_PROPERTY_MODEM_FSK4_GAIN1                    0x203B  /**< . */
#define SI446X_PROPERTY_MODEM_FSK4_GAIN0                    0x203C  /**< . */
#define SI446X_PROPERTY_MODEM_FSK4_TH1                      0x203D  /**< . */
#define SI446X_PROPERTY_MODEM_FSK4_TH0                      0x203E  /**< . */
#define SI446X_PROPERTY_MODEM_FSK4_MAP                      0x203F  /**< . */
#define SI446X_PROPERTY_MODEM_OOK_PDTC                      0x2040  /**< . */
#define SI446X_PROPERTY_MODEM_OOK_CNT1                      0x2042  /**< . */
#define SI446X_PROPERTY_MODEM_OOK_MISC                      0x2043  /**< . */
#define SI446X_PROPERTY_MODEM_RAW_SEARCH                    0x2044  /**< . */
#define SI446X_PROPERTY_MODEM_RAW_CONTROL                   0x2045  /**< . */
#define SI446X_PROPERTY_MODEM_RAW_EYE_1                     0x2046  /**< . */
#define SI446X_PROPERTY_MODEM_RAW_EYE_0                     0x2047  /**< . */
#define SI446X_PROPERTY_MODEM_ANT_DIV_MODE                  0x2048  /**< . */
#define SI446X_PROPERTY_MODEM_ANT_DIV_CONTROL               0x2049  /**< . */
#define SI446X_PROPERTY_MODEM_RSSI_THRESH                   0x204A  /**< . */
#define SI446X_PROPERTY_MODEM_RSSI_JUMP_THRESH              0x204B  /**< . */
#define SI446X_PROPERTY_MODEM_RSSI_CONTROL                  0x204C  /**< . */
#define SI446X_PROPERTY_MODEM_RSSI_CONTROL2                 0x204D  /**< . */
#define SI446X_PROPERTY_MODEM_RSSI_COMP                     0x204E  /**< . */
#define SI446X_PROPERTY_MODEM_ANT_DIV_CONT                  0x2049  /**< . */
#define SI446X_PROPERTY_MODEM_CLKGEN_BAND                   0x2051  /**< . */
#define SI446X_PROPERTY_MODEM_CHFLT_RX1_CHFLT_COE13_7_0     0x2100  /**< . */
#define SI446X_PROPERTY_MODEM_CHFLT_RX1_CHFLT_COE12_7_0     0x2101  /**< . */
#define SI446X_PROPERTY_MODEM_CHFLT_RX1_CHFLT_COE11_7_0     0x2102  /**< . */
#define SI446X_PROPERTY_MODEM_CHFLT_RX1_CHFLT_COE10_7_0     0x2103  /**< . */
#define SI446X_PROPERTY_MODEM_CHFLT_RX1_CHFLT_COE9_7_0      0x2104  /**< . */
#define SI446X_PROPERTY_MODEM_CHFLT_RX1_CHFLT_COE8_7_0      0x2105  /**< . */
#define SI446X_PROPERTY_MODEM_CHFLT_RX1_CHFLT_COE7_7_0      0x2106  /**< . */
#define SI446X_PROPERTY_MODEM_CHFLT_RX1_CHFLT_COE6_7_0      0x2107  /**< . */
#define SI446X_PROPERTY_MODEM_CHFLT_RX1_CHFLT_COE5_7_0      0x2108  /**< . */
#define SI446X_PROPERTY_MODEM_CHFLT_RX1_CHFLT_COE4_7_0      0x2109  /**< . */
#define SI446X_PROPERTY_MODEM_CHFLT_RX1_CHFLT_COE3_7_0      0x210A  /**< . */
#define SI446X_PROPERTY_MODEM_CHFLT_RX1_CHFLT_COE2_7_0      0x210B  /**< . */
#define SI446X_PROPERTY_MODEM_CHFLT_RX1_CHFLT_COE1_7_0      0x210C  /**< . */
#define SI446X_PROPERTY_MODEM_CHFLT_RX1_CHFLT_COE0_7_0      0x210D  /**< . */
#define SI446X_PROPERTY_MODEM_CHFLT_RX1_CHFLT_COEM0         0x210E  /**< . */
#define SI446X_PROPERTY_MODEM_CHFLT_RX1_CHFLT_COEM1         0x210F  /**< . */
#define SI446X_PROPERTY_MODEM_CHFLT_RX1_CHFLT_COEM2         0x2110  /**< . */
#define SI446X_PROPERTY_MODEM_CHFLT_RX1_CHFLT_COEM3         0x2111  /**< . */
#define SI446X_PROPERTY_MODEM_CHFLT_RX2_CHFLT_COE13_7_0     0x2112  /**< . */
#define SI446X_PROPERTY_MODEM_CHFLT_RX2_CHFLT_COE12_7_0     0x2113  /**< . */
#define SI446X_PROPERTY_MODEM_CHFLT_RX2_CHFLT_COE11_7_0     0x2114  /**< . */
#define SI446X_PROPERTY_MODEM_CHFLT_RX2_CHFLT_COE10_7_0     0x2115  /**< . */
#define SI446X_PROPERTY_MODEM_CHFLT_RX2_CHFLT_COE9_7_0      0x2116  /**< . */
#define SI446X_PROPERTY_MODEM_CHFLT_RX2_CHFLT_COE8_7_0      0x2117  /**< . */
#define SI446X_PROPERTY_MODEM_CHFLT_RX2_CHFLT_COE7_7_0      0x2118  /**< . */
#define SI446X_PROPERTY_MODEM_CHFLT_RX2_CHFLT_COE6_7_0      0x2119  /**< . */
#define SI446X_PROPERTY_MODEM_CHFLT_RX2_CHFLT_COE5_7_0      0x211A  /**< . */
#define SI446X_PROPERTY_MODEM_CHFLT_RX2_CHFLT_COE4_7_0      0x211B  /**< . */
#define SI446X_PROPERTY_MODEM_CHFLT_RX2_CHFLT_COE3_7_0      0x211C  /**< . */
#define SI446X_PROPERTY_MODEM_CHFLT_RX2_CHFLT_COE2_7_0      0x211D  /**< . */
#define SI446X_PROPERTY_MODEM_CHFLT_RX2_CHFLT_COE1_7_0      0x211E  /**< . */
#define SI446X_PROPERTY_MODEM_CHFLT_RX2_CHFLT_COE0_7_0      0x211F  /**< . */
#define SI446X_PROPERTY_MODEM_CHFLT_RX2_CHFLT_COEM0         0x2120  /**< . */
#define SI446X_PROPERTY_MODEM_CHFLT_RX2_CHFLT_COEM1         0x2121  /**< . */
#define SI446X_PROPERTY_MODEM_CHFLT_RX2_CHFLT_COEM2         0x2122  /**< . */
#define SI446X_PROPERTY_MODEM_CHFLT_RX2_CHFLT_COEM3         0x2123  /**< . */
#define SI446X_PROPERTY_PA_MODE                             0x2200  /**< PA operating mode and groups. */
#define SI446X_PROPERTY_PA_PWR_LVL                          0x2201  /**< PA Level Configuration. */
#define SI446X_PROPERTY_PA_BIAS_CLKDUTY                     0x2202  /**< . */
#define SI446X_PROPERTY_PA_TC                               0x2203  /**< PA cascode ramping Configuration. */
#define SI446X_PROPERTY_SYNTH_PFDCP_CPFF                    0x2300  /**< . */
#define SI446X_PROPERTY_SYNTH_PFDCP_CPINT                   0x2301  /**< . */
#define SI446X_PROPERTY_SYNTH_VCO_KV                        0x2302  /**< . */
#define SI446X_PROPERTY_SYNTH_LPFILT3                       0x2303  /**< . */
#define SI446X_PROPERTY_SYNTH_LPFILT2                       0x2304  /**< . */
#define SI446X_PROPERTY_SYNTH_LPFILT1                       0x2305  /**< . */
#define SI446X_PROPERTY_SYNTH_LPFILT0                       0x2306  /**< . */
#define SI446X_PROPERTY_MATCH_VALUE_1                       0x3000  /**< Match 1 value. */
#define SI446X_PROPERTY_MATCH_MASK_1                        0x3001  /**< Match 1 mask. */
#define SI446X_PROPERTY_MATCH_CTRL_1                        0x3002  /**< Packet match enable and match 1 configuration. */
#define SI446X_PROPERTY_MATCH_VALUE_2                       0x3003  /**< Match 2 value. */
#define SI446X_PROPERTY_MATCH_MASK_2                        0x3004  /**< Match 2 mask. */
#define SI446X_PROPERTY_MATCH_CTRL_2                        0x3005  /**< Match 2 configuration. */
#define SI446X_PROPERTY_MATCH_VALUE_3                       0x3006  /**< Match 3 value. */
#define SI446X_PROPERTY_MATCH_MASK_3                        0x3007  /**< Match 3 mask. */
#define SI446X_PROPERTY_MATCH_CTRL_3                        0x3008  /**< Match 3 configuration. */
#define SI446X_PROPERTY_MATCH_VALUE_4                       0x3009  /**< Match 4 value. */
#define SI446X_PROPERTY_MATCH_MASK_4                        0x300A  /**< Match 4 mask. */
#define SI446X_PROPERTY_MATCH_CTRL_4                        0x300B  /**< Match 4 configuration. */
#define SI446X_PROPERTY_FREQ_CONTROL_INTE                   0x4000  /**< Frac-N PLL integer number. */
#define SI446X_PROPERTY_FREQ_CONTROL_FRAC_2                 0x4001  /**< Byte 2 of Frac-N PLL fraction number. */
#define SI446X_PROPERTY_FREQ_CONTROL_FRAC_1                 0x4002  /**< Byte 1 of Frac-N PLL fraction number. */
#define SI446X_PROPERTY_FREQ_CONTROL_FRAC_0                 0x4003  /**< Byte 0 of Frac-N PLL fraction number. */
#define SI446X_PROPERTY_FREQ_CONTROL_CHANNEL_STEP_SIZE_1    0x4004  /**< Byte 1 of channel step size. */
#define SI446X_PROPERTY_FREQ_CONTROL_CHANNEL_STEP_SIZE_0    0x4005  /**< Byte 0 of channel step size. */
#define SI446X_PROPERTY_FREQ_CONTROL_VCOCNT_RX_ADJ          0x4007  /**< VCO target count adjustment for RX. */
#define SI446X_PROPERTY_RX_HOP_CONTROL                      0x5000  /**< RX hop control. */
#define SI446X_PROPERTY_RX_HOP_TABLE_SIZE                   0x5001  /**< Number of entries in the RX hop table. */
#define SI446X_PROPERTY_RX_HOP_TABLE_ENTRY_0                0x5002  /**< No.1 entry in RX hopping table. */

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

/* Interruption status */
#define SI446X_INT_STATUS_CHIP_INT_STATUS                   0x04
#define SI446X_INT_STATUS_MODEM_INT_STATUS                  0x02
#define SI446X_INT_STATUS_PH_INT_STATUS                     0x01
#define SI446X_INT_STATUS_FILTER_MATCH                      0x80
#define SI446X_INT_STATUS_FILTER_MISS                       0x40
#define SI446X_INT_STATUS_PACKET_SENT                       0x20
#define SI446X_INT_STATUS_PACKET_RX                         0x10
#define SI446X_INT_STATUS_CRC_ERROR                         0x08
#define SI446X_INT_STATUS_TX_FIFO_ALMOST_EMPTY              0x02
#define SI446X_INT_STATUS_RX_FIFO_ALMOST_FULL               0x01
#define SI446X_INT_STATUS_INVALID_SYNC                      0x20
#define SI446X_INT_STATUS_RSSI_JUMP                         0x10
#define SI446X_INT_STATUS_RSSI                              0x08
#define SI446X_INT_STATUS_INVALID_PREAMBLE                  0x04
#define SI446X_INT_STATUS_PREAMBLE_DETECT                   0x02
#define SI446X_INT_STATUS_SYNC_DETECT                       0x01
#define SI446X_INT_STATUS_CAL                               0x40
#define SI446X_INT_STATUS_FIFO_UNDERFLOW_OVERFLOW_ERROR     0x20
#define SI446X_INT_STATUS_STATE_CHANGE                      0x10
#define SI446X_INT_STATUS_CMD_ERROR                         0x08
#define SI446X_INT_STATUS_CHIP_READY                        0x04
#define SI446X_INT_STATUS_LOW_BATT                          0x02
#define SI446X_INT_STATUS_WUT                               0x01

/* Interruption control */
#define SI446X_CHIP_INT_STATUS_EN                           0x04
#define SI446X_MODEM_INT_STATUS_EN                          0x02
#define SI446X_PH_INT_STATUS_EN                             0x01

/* FRR control */
#define SI446X_FRR_MODE_DISABLED                            0
#define SI446X_FRR_MODE_GLOBAL_STATUS                       1
#define SI446X_FRR_MODE_GLOBAL_INTERRUPT_PENDING            2
#define SI446X_FRR_MODE_PACKET_HANDLER_STATUS               3
#define SI446X_FRR_MODE_PACKET_HANDLER_INTERRUPT_PENDING    4
#define SI446X_FRR_MODE_MODEM_STATUS                        5
#define SI446X_FRR_MODE_MODEM_INTERRUPT_PENDING             6
#define SI446X_FRR_MODE_CHIP_STATUS                         7
#define SI446X_FRR_MODE_CHIP_INTERRUPT_PENDING              8
#define SI446X_FRR_MODE_CURRENT_STATE                       9
#define SI446X_FRR_MODE_LATCHED_RSSI                        10

/* Preamble configuration */
#define SI446X_PREAMBLE_STANDARD_PREAM                      0x00
#define SI446X_PREAMBLE_DSA_ONLY                            0x80
#define SI446X_PREAMBLE_FIRST_1                             0x20
#define SI446X_PREAMBLE_FIRST_0                             0x00
#define SI446X_PREAMBLE_LENGTH_NIBBLES                      0x00
#define SI446X_PREAMBLE_LENGTH_BYTES                        0x10
#define SI446X_PREAMBLE_MAN_CONST                           0x08
#define SI446X_PREAMBLE_MAN_ENABLE                          0x02
#define SI446X_PREAMBLE_NON_STANDARD                        0x00
#define SI446X_PREAMBLE_STANDARD_1010                       0x01
#define SI446X_PREAMBLE_STANDARD_0101                       0x02

/* Sync. configuration */
#define SI446X_SYNC_CONFIG_SKIP_TX                          0x80
#define SI446X_SYNC_CONFIG_RX_ERRORS_MASK                   0x70
#define SI446X_SYNC_CONFIG_4FSK                             0x08
#define SI446X_SYNC_CONFIG_MANCH                            0x04
#define SI446X_SYNC_CONFIG_LENGTH_MASK                      0x03

/* Packet CRC configuration */
#define SI446X_CRC_SEED_ALL_0S                              0x00
#define SI446X_CRC_SEED_ALL_1S                              0x80
#define SI446X_CRC_MASK                                     0x0F
#define SI446X_CRC_NONE                                     0x00
#define SI446X_CRC_ITU_T                                    0x01
#define SI446X_CRC_IEC_16                                   0x02
#define SI446X_CRC_BIACHEVA                                 0x03
#define SI446X_CRC_16_IBM                                   0x04
#define SI446X_CRC_CCITT                                    0x05
#define SI446X_CRC_KOOPMAN                                  0x06
#define SI446X_CRC_IEEE_802_3                               0x07
#define SI446X_CRC_CASTAGNOLI                               0x08

/* Packet config1 */
#define SI446X_PH_FIELD_SPLIT                               0x80
#define SI446X_PH_RX_DISABLE                                0x40
#define SI446X_4FSK_EN                                      0x20
#define SI446X_RX_MULTI_PKT                                 0x10
#define SI446X_MANCH_POL                                    0x08
#define SI446X_CRC_INVERT                                   0x04
#define SI446X_CRC_ENDIAN                                   0x02
#define SI446X_BIT_ORDER                                    0x01

/* Packet length */
#define SI446X_INFINITE_LEN                                 0x40
#define SI446X_ENDIAN_LITTLE                                0x00
#define SI446X_ENDIAN_BIG                                   0x20
#define SI446X_SIZE_ENUM_0                                  0x00
#define SI446X_SIZE_ENUM_1                                  0x01
#define SI446X_IN_FIFO                                      0x08
#define SI446X_DST_FIELD_ENUM_0                             0x00
#define SI446X_DST_FIELD_ENUM_1                             0x01
#define SI446X_DST_FIELD_ENUM_2                             0x02
#define SI446X_DST_FIELD_ENUM_3                             0x03
#define SI446X_DST_FIELD_ENUM_4                             0x04
#define SI446X_DST_FIELD_ENUM_5                             0x05
#define SI446X_DST_FIELD_ENUM_6                             0x06
#define SI446X_DST_FIELD_ENUM_7                             0x07

/* Packet length field source */
#define SI446X_SRC_FIELD_ENUM_0                             0x00
#define SI446X_SRC_FIELD_ENUM_1                             0x01
#define SI446X_SRC_FIELD_ENUM_2                             0x02
#define SI446X_SRC_FIELD_ENUM_3                             0x03
#define SI446X_SRC_FIELD_ENUM_4                             0x04
#define SI446X_SRC_FIELD_ENUM_5                             0x05

/* Packet fiels configuration */
#define SI446X_FIELD_CONFIG_4FSK                            0x10
#define SI446X_FIELD_CONFIG_PN_START                        0x04
#define SI446X_FIELD_CONFIG_WHITEN                          0x02
#define SI446X_FIELD_CONFIG_MANCH                           0x01

/* Packet RX fields CRC configuration */
#define SI446X_FIELD_CONFIG_CRC_START                       0x80
#define SI446X_FIELD_CONFIG_ALT_CRC_START                   0x40
#define SI446X_FIELD_CONFIG_SEND_CRC                        0x20
#define SI446X_FIELD_CONFIG_SEND_ALT_CRC                    0x10
#define SI446X_FIELD_CONFIG_CHECK_CRC                       0x08
#define SI446X_FIELD_CONFIG_CHECK_ALT_CRC                   0x04
#define SI446X_FIELD_CONFIG_CRC_ENABLE                      0x02
#define SI446X_FIELD_CONFIG_ALT_CRC_ENABLE                  0x01

/* Property modem mod type */
#define SI446X_TX_DIRECT_MODE_TYPE_SYNCHRONOUS              0x00
#define SI446X_TX_DIRECT_MODE_TYPE_ASYNCHRONOUS             0x80
#define SI446X_TX_DIRECT_MODE_GPIO0                         0x00
#define SI446X_TX_DIRECT_MODE_GPIO1                         0x20
#define SI446X_TX_DIRECT_MODE_GPIO2                         0x40
#define SI446X_TX_DIRECT_MODE_GPIO3                         0x60
#define SI446X_MOD_SOURCE_PACKET_HANDLER                    0x00
#define SI446X_MOD_SOURCE_DIRECT_MODE                       0x08
#define SI446X_MOD_SOURCE_RANDOM_GENERATOR                  0x10
#define SI446X_MOD_TYPE_CW                                  0x00
#define SI446X_MOD_TYPE_OOK                                 0x01
#define SI446X_MOD_TYPE_2FSK                                0x02
#define SI446X_MOD_TYPE_2GFSK                               0x03
#define SI446X_MOD_TYPE_4FSK                                0x04
#define SI446X_MOD_TYPE_4GFSK                               0x05

/* Property PA mode */
#define SI446X_PA_MODE_1_GROUP                              0x04
#define SI446X_PA_MODE_2_GROUPS                             0x08
#define SI446X_PA_MODE_CLASS_E                              0x00
#define SI446X_PA_MODE_SWITCH_CURRENT                       0x01


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
 * Initializes the Si446x module with the configuration parameters from "radio_config_Si4463.h".
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
 * All the configuration parameters are defined in the "radio_config_Si4463.h" file.
 * 
 * \return None.
 */
void si446x_reg_config(void);

/**
 * \brief Power on reset procedure of the SI446X module.
 * 
 * \return None.
 */
void si446x_power_on_reset(void);

/**
 * \brief Transmit a packet through RF.
 * 
 * \param[in] data is the data with the packet to send.
 *
 * \param[in] len is the length of the data to send.
 * 
 * \return TRUE/FALSE if successful or not.
 */
bool si446x_tx_packet(uint8_t *data, uint8_t len);

/**
 * \brief Transmit a long packet (> FIFO buffer, or 64 bytes) through RF.
 * 
 * \see AN633, Figure 62.
 * 
 * \param[in] packet is the packet to send.
 *
 * \param[in] len is the length of the packet.
 * 
 * \return None.
 */
bool si446x_tx_long_packet(uint8_t *packet, uint16_t len);

/**
 * \brief ;
 * 
 * \param rx_buf
 * 
 * \param read_len
 *
 * \return .
 */
uint8_t si446x_rx_packet(uint8_t *rx_buf, uint8_t read_len);

/**
 * \brief .
 * 
 * \return TRUE/FALSE if successful or not.
 */
bool si446x_rx_init(void);

/**
 * \brief Checks if the device is connected.
 * 
 * \return TRUE/FALSE if the device is connected or not.
 */
bool si446x_check_device(void);

/**
 * \brief Checks if the radio is ready to receive a command (CTS - Clear To Send).
 *
 * To ensure the radio is ready to receive the next command, the host MCU has to pull down the NSEL pin to
 * monitor the status of CTS over the SPI port. The 0x44 command ID has to be sent and eight clock pulses have
 * to be generated on the SCLK pin. During the additional eight clock cycles, the radio clocks out the CTS as a
 * byte on the SDO pin. When completed, the NSEL should be pulled back to high. If the CTS byte is 0xFF, it
 * means that the radio processed the last command successfully and is ready to receive the next command; in
 * any other case, the CTS read procedure has to be repeated from the beginning as long as the CTS byte is not
 * 0xFF.
 *
 * \see AN633. 7.2.1 - Checking the Radio is Ready to Receive Command. Page 15.
 *
 * \return TRUE/FALSE if the CTS value was read or not.
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
 * \param[in] start_property .
 *
 * \param[in,out] data .
 *
 * \param[in] len .
 * 
 * \return TRUE/FALSE if successful or not.
 */
bool si446x_set_properties(uint16_t start_property, uint8_t *data, uint8_t len);

/**
 * \brief 
 * 
 * \param[in] start_property .
 *
 * \param[in] len .
 *
 * \param[in,out] data .
 *
 * \return TRUE/FALSE if successful or not.
 */
bool si446x_get_properties(uint16_t start_property, uint8_t len, uint8_t *data);

/**
 * \brief 
 * 
 * \param 
 * \param 
 * 
 * \return None.
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
 * \brief Reads the response of a command.
 * 
 * \param[in] cmd is the command to read the response.
 *
 * \param[in,out] data is a pointer to store the command result.
 *
 * \param[in] len is the number of bytes to read.
 * 
 * \return TRUE/FALSE if successful or not.
 */
bool si446x_get_cmd(uint8_t cmd, uint8_t *data, uint8_t len);

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
 * \return None.
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
 * \return None.
 */
void si446x_fifo_reset(void);

/**
 * \brief 
 * 
 * \return None.
 */
void si446x_enter_tx_mode(void);

/**
 * \brief 
 * 
 * \return None.
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
 * \brief Reads the ADC value of temperature sensor voltage of the chip.
 *
 * \param[in,out] raw_temp is a pointer to store the raw value of the read temperature.
 *
 * \return TRUE/FALSE if successful or not.
 */
bool si446x_read_temp(uint32_t *raw_temp);

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
bool si446x_wait_nirq(void);

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
