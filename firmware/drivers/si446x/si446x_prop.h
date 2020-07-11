/*
 * si446x_prop.h
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
 * \brief Si446x properties definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.3.20
 * 
 * \date 10/05/2020
 * 
 * \defgroup si446x_prop Properties
 * \ingroup si446x
 * \{
 */

#ifndef SI446X_PROP_H_
#define SI446X_PROP_H_

/* Properties */
#define SI446X_PROP_GLOBAL_XO_TUNE                          0x0000  /**< Configure crystal oscillator frequency tuning bank. */
#define SI446X_PROP_GLOBAL_CLK_CFG                          0x0001  /**< Clock configuration options. */
#define SI446X_PROP_GLOBAL_LOW_BATT_THRESH                  0x0002  /**< Low battery threshold. */
#define SI446X_PROP_GLOBAL_CONFIG                           0x0003  /**< Global configuration settings. */
#define SI446X_PROP_GLOBAL_WUT_CONFIG                       0x0004  /**< GLOBAL WUT configuation. */
#define SI446X_PROP_GLOBAL_WUT_M_15_8                       0x0005  /**< Configure WUT_M_15_8. */
#define SI446X_PROP_GLOBAL_WUT_M_7_0                        0x0006  /**< Configure WUT_M_7_0. */
#define SI446X_PROP_GLOBAL_WUT_R                            0x0007  /**< Configure WUT_R. */
#define SI446X_PROP_GLOBAL_WUT_LDC                          0x0008  /**< Configure WUT_LDC. */
#define SI446X_PROP_INT_CTL_ENABLE                          0x0100  /**< Interrupt enable property. */
#define SI446X_PROP_INT_CTL_PH_ENABLE                       0x0101  /**< Packet handler interrupt enable property. */
#define SI446X_PROP_INT_CTL_MODEM_ENABLE                    0x0102  /**< Modem interrupt enable property. */
#define SI446X_PROP_INT_CTL_CHIP_ENABLE                     0x0103  /**< Chip interrupt enable property. */
#define SI446X_PROP_FRR_CTL_A_MODE                          0x0200  /**< Fast Response Register A Configuration. */
#define SI446X_PROP_FRR_CTL_B_MODE                          0x0201  /**< Fast Response Register B Configuration. */
#define SI446X_PROP_FRR_CTL_C_MODE                          0x0202  /**< Fast Response Register C Configuration. */
#define SI446X_PROP_FRR_CTL_D_MODE                          0x0203  /**< Fast Response Register D Configuration. */
#define SI446X_PROP_PREAMBLE_TX_LENGTH                      0x1000  /**< Preamble length. */
#define SI446X_PROP_PREAMBLE_CONFIG_STD_1                   0x1001  /**< Standard preamble configuration. */
#define SI446X_PROP_PREAMBLE_CONFIG_NSTD                    0x1002  /**< Non-standard preamble configuation. */
#define SI446X_PROP_PREAMBLE_CONFIG_STD_2                   0x1003  /**< Standard preamble configuration. */
#define SI446X_PROP_PREAMBLE_CONFIG                         0x1004  /**< Preamble configuration bits. */
#define SI446X_PROP_PREAMBLE_PATTERN_31_24                  0x1005  /**< Preamble pattern. */
#define SI446X_PROP_PREAMBLE_PATTERN_23_16                  0x1006  /**< Preamble pattern. */
#define SI446X_PROP_PREAMBLE_PATTERN_15_8                   0x1007  /**< Preamble pattern. */
#define SI446X_PROP_PREAMBLE_PATTERN_7_0                    0x1008  /**< Preamble pattern. */
#define SI446X_PROP_SYNC_CONFIG                             0x1100  /**< Sync configuration bits. */
#define SI446X_PROP_SYNC_BITS_31_24                         0x1101  /**< Byte 3 of sync word. */
#define SI446X_PROP_SYNC_BITS_23_16                         0x1102  /**< Byte 2 of sync word. */
#define SI446X_PROP_SYNC_BITS_15_8                          0x1103  /**< Byte 1 of sync word. */
#define SI446X_PROP_SYNC_BITS_7_0                           0x1104  /**< Byte 0 of sync word. */
#define SI446X_PROP_PKT_CRC_CONFIG                          0x1200  /**< Select a CRC polynomial and seed. */
#define SI446X_PROP_PKT_CONFIG1                             0x1206  /**< General packet configuration bits. */
#define SI446X_PROP_PKT_LEN                                 0x1208  /**< Provides information regarding how to use the length from the received packet. */
#define SI446X_PROP_PKT_LEN_FIELD_SOURCE                    0x1209  /**< Field number containing the embedded length field. */
#define SI446X_PROP_PKT_LEN_ADJUST                          0x120A  /**< Adjust length field by this amount to derive the byte count of the variable length field. */
#define SI446X_PROP_PKT_TX_THRESHOLD                        0x120B  /**< TX almost empty threshold. */
#define SI446X_PROP_PKT_RX_THRESHOLD                        0x120C  /**< RX almost full threshold. */
#define SI446X_PROP_PKT_FIELD_1_LENGTH_12_8                 0x120D  /**< Byte 1 of field length. */
#define SI446X_PROP_PKT_FIELD_1_LENGTH_7_0                  0x120E  /**< Byte 0 of field length. */
#define SI446X_PROP_PKT_FIELD_1_CONFIG                      0x120F  /**< Field 1 configuration bits. */
#define SI446X_PROP_PKT_FIELD_1_CRC_CONFIG                  0x1210  /**< Field 1 CRC configuration bits. */
#define SI446X_PROP_PKT_FIELD_2_LENGTH_12_8                 0x1211  /**< Byte 1 of field length. */
#define SI446X_PROP_PKT_FIELD_2_LENGTH_7_0                  0x1212  /**< Byte 0 of field length. */
#define SI446X_PROP_PKT_FIELD_2_CONFIG                      0x1213  /**< Field 2 configuration bits. */
#define SI446X_PROP_PKT_FIELD_2_CRC_CONFIG                  0x1214  /**< Field 2 CRC configuration bits. */
#define SI446X_PROP_PKT_FIELD_3_LENGTH_12_8                 0x1215  /**< Byte 1 of field length. */
#define SI446X_PROP_PKT_FIELD_3_LENGTH_7_0                  0x1216  /**< Byte 0 of field length. */
#define SI446X_PROP_PKT_FIELD_3_CONFIG                      0x1217  /**< Field 3 configuration bits. */
#define SI446X_PROP_PKT_FIELD_3_CRC_CONFIG                  0x1218  /**< Field 3 CRC configuration bits. */
#define SI446X_PROP_PKT_FIELD_4_LENGTH_12_8                 0x1219  /**< Byte 1 of field length. */
#define SI446X_PROP_PKT_FIELD_4_LENGTH_7_0                  0x121A  /**< Byte 0 of field length. */
#define SI446X_PROP_PKT_FIELD_4_CONFIG                      0x121B  /**< Field 4 configuration bits. */
#define SI446X_PROP_PKT_FIELD_4_CRC_CONFIG                  0x121C  /**< Field 4 CRC configuration bits. */
#define SI446X_PROP_PKT_FIELD_5_LENGTH_12_8                 0x121D  /**< Byte 1 of field length. */
#define SI446X_PROP_PKT_FIELD_5_LENGTH_7_0                  0x121E  /**< Byte 0 of field length. */
#define SI446X_PROP_PKT_FIELD_5_CONFIG                      0x121F  /**< Field 5 configuration bits. */
#define SI446X_PROP_PKT_FIELD_5_CRC_CONFIG                  0x1220  /**< Field 5 CRC configuration bits. */
#define SI446X_PROP_PKT_RX_FIELD_1_LENGTH_12_8              0x1221  /**< Byte 1 of field length for RX. */
#define SI446X_PROP_PKT_RX_FIELD_1_LENGTH_7_0               0x1222  /**< Byte 0 of field length for RX. */
#define SI446X_PROP_PKT_RX_FIELD_1_CONFIG                   0x1223  /**< Field 1 configuration bits for RX. */
#define SI446X_PROP_PKT_RX_FIELD_1_CRC_CONFIG               0x1224  /**< Field 1 CRC configuration bits for RX. */
#define SI446X_PROP_PKT_RX_FIELD_2_LENGTH_12_8              0x1225  /**< Byte 1 of field length for RX. */
#define SI446X_PROP_PKT_RX_FIELD_2_LENGTH_7_0               0x1226  /**< Byte 0 of field length for RX. */
#define SI446X_PROP_PKT_RX_FIELD_2_CONFIG                   0x1227  /**< Field 2 configuration bits for RX. */
#define SI446X_PROP_PKT_RX_FIELD_2_CRC_CONFIG               0x1228  /**< Field 2 CRC configuration bits for RX. */
#define SI446X_PROP_PKT_RX_FIELD_3_LENGTH_12_8              0x1229  /**< Byte 1 of field length for RX. */
#define SI446X_PROP_PKT_RX_FIELD_3_LENGTH_7_0               0x122A  /**< Byte 0 of field length for RX. */
#define SI446X_PROP_PKT_RX_FIELD_3_CONFIG                   0x122B  /**< Field 3 configuration bits for RX. */
#define SI446X_PROP_PKT_RX_FIELD_3_CRC_CONFIG               0x122C  /**< Field 3 CRC configuration bits for RX. */
#define SI446X_PROP_PKT_RX_FIELD_4_LENGTH_12_8              0x122D  /**< Byte 1 of field length for RX. */
#define SI446X_PROP_PKT_RX_FIELD_4_LENGTH_7_0               0x122E  /**< Byte 0 of field length for RX. */
#define SI446X_PROP_PKT_RX_FIELD_4_CONFIG                   0x122F  /**< Field 4 configuration bits for RX. */
#define SI446X_PROP_PKT_RX_FIELD_4_CRC_CONFIG               0x1230  /**< Field 4 CRC configuration bits for RX. */
#define SI446X_PROP_PKT_RX_FIELD_5_LENGTH_12_8              0x1231  /**< Byte 1 of field length for RX. */
#define SI446X_PROP_PKT_RX_FIELD_5_LENGTH_7_0               0x1232  /**< Byte 0 of field length for RX. */
#define SI446X_PROP_PKT_RX_FIELD_5_CONFIG                   0x1233  /**< Field 5 configuration bits for RX. */
#define SI446X_PROP_PKT_RX_FIELD_5_CRC_CONFIG               0x1234  /**< Field 5 CRC configuration bits for RX. */
#define SI446X_PROP_MODEM_MOD_TYPE                          0x2000  /**< Modulation Type. */
#define SI446X_PROP_MODEM_MAP_CONTROL                       0x2001  /**< Controls bit mapping. */
#define SI446X_PROP_MODEM_DSM_CTRL                          0x2002  /**< . */
#define SI446X_PROP_MODEM_DATA_RATE_2                       0x2003  /**< Byte 2 of TX data rate in bps (bits per second). */
#define SI446X_PROP_MODEM_DATA_RATE_1                       0x2004  /**< Byte 1 of TX data rate in bps (bits per second). */
#define SI446X_PROP_MODEM_DATA_RATE_0                       0x2005  /**< Byte 0 of TX data rate in bps (bits per second). */
#define SI446X_PROP_MODEM_TX_NCO_MODE_3                     0x2006  /**< . */
#define SI446X_PROP_MODEM_TX_NCO_MODE_2                     0x2007  /**< . */
#define SI446X_PROP_MODEM_TX_NCO_MODE_1                     0x2008  /**< . */
#define SI446X_PROP_MODEM_TX_NCO_MODE_0                     0x2009  /**< . */
#define SI446X_PROP_MODEM_FREQ_DEV_2                        0x200A  /**< Byte 2 of TX frequency deviation (a 17-bit unsigned number). This only programs the MSB of TX frequency deviation. */
#define SI446X_PROP_MODEM_FREQ_DEV_1                        0x200B  /**< Byte 1 of frequency deviation. */
#define SI446X_PROP_MODEM_FREQ_DEV_0                        0x200C  /**< Byte 0 of frequency deviation. */
#define SI446X_PROP_MODEM_TX_RAMP_DELAY                     0x2018  /**< . */
#define SI446X_PROP_MODEM_MDM_CTRL                          0x2019  /**< . */
#define SI446X_PROP_MODEM_IF_CONTROL                        0x201A  /**< . */
#define SI446X_PROP_MODEM_IF_FREQ_2                         0x201B  /**< . */
#define SI446X_PROP_MODEM_IF_FREQ_1                         0x201C  /**< . */
#define SI446X_PROP_MODEM_IF_FREQ_0                         0x201D  /**< . */
#define SI446X_PROP_MODEM_DECIMATION_CFG1                   0x201E  /**< . */
#define SI446X_PROP_MODEM_DECIMATION_CFG0                   0x201F  /**< . */
#define SI446X_PROP_MODEM_BCR_OSR_1                         0x2022  /**< . */
#define SI446X_PROP_MODEM_BCR_OSR_0                         0x2023  /**< . */
#define SI446X_PROP_MODEM_BCR_NCO_OFFSET_2                  0x2024  /**< . */
#define SI446X_PROP_MODEM_BCR_NCO_OFFSET_1                  0x2025  /**< . */
#define SI446X_PROP_MODEM_BCR_NCO_OFFSET_0                  0x2026  /**< . */
#define SI446X_PROP_MODEM_BCR_GAIN_1                        0x2027  /**< . */
#define SI446X_PROP_MODEM_BCR_GAIN_0                        0x2028  /**< . */
#define SI446X_PROP_MODEM_BCR_GEAR                          0x2029  /**< . */
#define SI446X_PROP_MODEM_BCR_MISC1                         0x202A  /**< . */
#define SI446X_PROP_MODEM_AFC_GEAR                          0x202C  /**< . */
#define SI446X_PROP_MODEM_AFC_WAIT                          0x202D  /**< . */
#define SI446X_PROP_MODEM_AFC_GAIN_1                        0x202E  /**< . */
#define SI446X_PROP_MODEM_AFC_GAIN_0                        0x202F  /**< . */
#define SI446X_PROP_MODEM_AFC_LIMITER_1                     0x2030  /**< . */
#define SI446X_PROP_MODEM_AFC_LIMITER_0                     0x2031  /**< . */
#define SI446X_PROP_MODEM_AFC_MISC                          0x2032  /**< . */
#define SI446X_PROP_MODEM_AGC_CONTROL                       0x2035  /**< . */
#define SI446X_PROP_MODEM_AGC_WINDOW_SIZE                   0x2038  /**< . */
#define SI446X_PROP_MODEM_AGC_RFPD_DECAY                    0x2039  /**< . */
#define SI446X_PROP_MODEM_AGC_IFPD_DECAY                    0x203A  /**< . */
#define SI446X_PROP_MODEM_FSK4_GAIN1                        0x203B  /**< . */
#define SI446X_PROP_MODEM_FSK4_GAIN0                        0x203C  /**< . */
#define SI446X_PROP_MODEM_FSK4_TH1                          0x203D  /**< . */
#define SI446X_PROP_MODEM_FSK4_TH0                          0x203E  /**< . */
#define SI446X_PROP_MODEM_FSK4_MAP                          0x203F  /**< . */
#define SI446X_PROP_MODEM_OOK_PDTC                          0x2040  /**< . */
#define SI446X_PROP_MODEM_OOK_CNT1                          0x2042  /**< . */
#define SI446X_PROP_MODEM_OOK_MISC                          0x2043  /**< . */
#define SI446X_PROP_MODEM_RAW_SEARCH                        0x2044  /**< . */
#define SI446X_PROP_MODEM_RAW_CONTROL                       0x2045  /**< . */
#define SI446X_PROP_MODEM_RAW_EYE_1                         0x2046  /**< . */
#define SI446X_PROP_MODEM_RAW_EYE_0                         0x2047  /**< . */
#define SI446X_PROP_MODEM_ANT_DIV_MODE                      0x2048  /**< . */
#define SI446X_PROP_MODEM_ANT_DIV_CONTROL                   0x2049  /**< . */
#define SI446X_PROP_MODEM_RSSI_THRESH                       0x204A  /**< . */
#define SI446X_PROP_MODEM_RSSI_JUMP_THRESH                  0x204B  /**< . */
#define SI446X_PROP_MODEM_RSSI_CONTROL                      0x204C  /**< . */
#define SI446X_PROP_MODEM_RSSI_CONTROL2                     0x204D  /**< . */
#define SI446X_PROP_MODEM_RSSI_COMP                         0x204E  /**< . */
#define SI446X_PROP_MODEM_ANT_DIV_CONT                      0x2049  /**< . */
#define SI446X_PROP_MODEM_CLKGEN_BAND                       0x2051  /**< . */
#define SI446X_PROP_MODEM_CHFLT_RX1_CHFLT_COE13_7_0         0x2100  /**< . */
#define SI446X_PROP_MODEM_CHFLT_RX1_CHFLT_COE12_7_0         0x2101  /**< . */
#define SI446X_PROP_MODEM_CHFLT_RX1_CHFLT_COE11_7_0         0x2102  /**< . */
#define SI446X_PROP_MODEM_CHFLT_RX1_CHFLT_COE10_7_0         0x2103  /**< . */
#define SI446X_PROP_MODEM_CHFLT_RX1_CHFLT_COE9_7_0          0x2104  /**< . */
#define SI446X_PROP_MODEM_CHFLT_RX1_CHFLT_COE8_7_0          0x2105  /**< . */
#define SI446X_PROP_MODEM_CHFLT_RX1_CHFLT_COE7_7_0          0x2106  /**< . */
#define SI446X_PROP_MODEM_CHFLT_RX1_CHFLT_COE6_7_0          0x2107  /**< . */
#define SI446X_PROP_MODEM_CHFLT_RX1_CHFLT_COE5_7_0          0x2108  /**< . */
#define SI446X_PROP_MODEM_CHFLT_RX1_CHFLT_COE4_7_0          0x2109  /**< . */
#define SI446X_PROP_MODEM_CHFLT_RX1_CHFLT_COE3_7_0          0x210A  /**< . */
#define SI446X_PROP_MODEM_CHFLT_RX1_CHFLT_COE2_7_0          0x210B  /**< . */
#define SI446X_PROP_MODEM_CHFLT_RX1_CHFLT_COE1_7_0          0x210C  /**< . */
#define SI446X_PROP_MODEM_CHFLT_RX1_CHFLT_COE0_7_0          0x210D  /**< . */
#define SI446X_PROP_MODEM_CHFLT_RX1_CHFLT_COEM0             0x210E  /**< . */
#define SI446X_PROP_MODEM_CHFLT_RX1_CHFLT_COEM1             0x210F  /**< . */
#define SI446X_PROP_MODEM_CHFLT_RX1_CHFLT_COEM2             0x2110  /**< . */
#define SI446X_PROP_MODEM_CHFLT_RX1_CHFLT_COEM3             0x2111  /**< . */
#define SI446X_PROP_MODEM_CHFLT_RX2_CHFLT_COE13_7_0         0x2112  /**< . */
#define SI446X_PROP_MODEM_CHFLT_RX2_CHFLT_COE12_7_0         0x2113  /**< . */
#define SI446X_PROP_MODEM_CHFLT_RX2_CHFLT_COE11_7_0         0x2114  /**< . */
#define SI446X_PROP_MODEM_CHFLT_RX2_CHFLT_COE10_7_0         0x2115  /**< . */
#define SI446X_PROP_MODEM_CHFLT_RX2_CHFLT_COE9_7_0          0x2116  /**< . */
#define SI446X_PROP_MODEM_CHFLT_RX2_CHFLT_COE8_7_0          0x2117  /**< . */
#define SI446X_PROP_MODEM_CHFLT_RX2_CHFLT_COE7_7_0          0x2118  /**< . */
#define SI446X_PROP_MODEM_CHFLT_RX2_CHFLT_COE6_7_0          0x2119  /**< . */
#define SI446X_PROP_MODEM_CHFLT_RX2_CHFLT_COE5_7_0          0x211A  /**< . */
#define SI446X_PROP_MODEM_CHFLT_RX2_CHFLT_COE4_7_0          0x211B  /**< . */
#define SI446X_PROP_MODEM_CHFLT_RX2_CHFLT_COE3_7_0          0x211C  /**< . */
#define SI446X_PROP_MODEM_CHFLT_RX2_CHFLT_COE2_7_0          0x211D  /**< . */
#define SI446X_PROP_MODEM_CHFLT_RX2_CHFLT_COE1_7_0          0x211E  /**< . */
#define SI446X_PROP_MODEM_CHFLT_RX2_CHFLT_COE0_7_0          0x211F  /**< . */
#define SI446X_PROP_MODEM_CHFLT_RX2_CHFLT_COEM0             0x2120  /**< . */
#define SI446X_PROP_MODEM_CHFLT_RX2_CHFLT_COEM1             0x2121  /**< . */
#define SI446X_PROP_MODEM_CHFLT_RX2_CHFLT_COEM2             0x2122  /**< . */
#define SI446X_PROP_MODEM_CHFLT_RX2_CHFLT_COEM3             0x2123  /**< . */
#define SI446X_PROP_PA_MODE                                 0x2200  /**< PA operating mode and groups. */
#define SI446X_PROP_PA_PWR_LVL                              0x2201  /**< PA Level Configuration. */
#define SI446X_PROP_PA_BIAS_CLKDUTY                         0x2202  /**< PA Bias and TX clock duty cycle configuration. */
#define SI446X_PROP_PA_TC                                   0x2203  /**< PA cascode ramping Configuration. */
#define SI446X_PROP_SYNTH_PFDCP_CPFF                        0x2300  /**< . */
#define SI446X_PROP_SYNTH_PFDCP_CPINT                       0x2301  /**< . */
#define SI446X_PROP_SYNTH_VCO_KV                            0x2302  /**< . */
#define SI446X_PROP_SYNTH_LPFILT3                           0x2303  /**< . */
#define SI446X_PROP_SYNTH_LPFILT2                           0x2304  /**< . */
#define SI446X_PROP_SYNTH_LPFILT1                           0x2305  /**< . */
#define SI446X_PROP_SYNTH_LPFILT0                           0x2306  /**< . */
#define SI446X_PROP_MATCH_VALUE_1                           0x3000  /**< Match 1 value. */
#define SI446X_PROP_MATCH_MASK_1                            0x3001  /**< Match 1 mask. */
#define SI446X_PROP_MATCH_CTRL_1                            0x3002  /**< Packet match enable and match 1 configuration. */
#define SI446X_PROP_MATCH_VALUE_2                           0x3003  /**< Match 2 value. */
#define SI446X_PROP_MATCH_MASK_2                            0x3004  /**< Match 2 mask. */
#define SI446X_PROP_MATCH_CTRL_2                            0x3005  /**< Match 2 configuration. */
#define SI446X_PROP_MATCH_VALUE_3                           0x3006  /**< Match 3 value. */
#define SI446X_PROP_MATCH_MASK_3                            0x3007  /**< Match 3 mask. */
#define SI446X_PROP_MATCH_CTRL_3                            0x3008  /**< Match 3 configuration. */
#define SI446X_PROP_MATCH_VALUE_4                           0x3009  /**< Match 4 value. */
#define SI446X_PROP_MATCH_MASK_4                            0x300A  /**< Match 4 mask. */
#define SI446X_PROP_MATCH_CTRL_4                            0x300B  /**< Match 4 configuration. */
#define SI446X_PROP_FREQ_CONTROL_INTE                       0x4000  /**< Frac-N PLL integer number. */
#define SI446X_PROP_FREQ_CONTROL_FRAC_2                     0x4001  /**< Byte 2 of Frac-N PLL fraction number. */
#define SI446X_PROP_FREQ_CONTROL_FRAC_1                     0x4002  /**< Byte 1 of Frac-N PLL fraction number. */
#define SI446X_PROP_FREQ_CONTROL_FRAC_0                     0x4003  /**< Byte 0 of Frac-N PLL fraction number. */
#define SI446X_PROP_FREQ_CONTROL_CHANNEL_STEP_SIZE_1        0x4004  /**< Byte 1 of channel step size. */
#define SI446X_PROP_FREQ_CONTROL_CHANNEL_STEP_SIZE_0        0x4005  /**< Byte 0 of channel step size. */
#define SI446X_PROP_FREQ_CONTROL_VCOCNT_RX_ADJ              0x4007  /**< VCO target count adjustment for RX. */
#define SI446X_PROP_RX_HOP_CONTROL                          0x5000  /**< RX hop control. */
#define SI446X_PROP_RX_HOP_TABLE_SIZE                       0x5001  /**< Number of entries in the RX hop table. */
#define SI446X_PROP_RX_HOP_TABLE_ENTRY_0                    0x5002  /**< No.1 entry in RX hopping table. */


/* FRR mode property */
#define SI446X_FRR_MODE_DISABLED                            0       /**< Disabled (Will always read back 0). */
#define SI446X_FRR_MODE_GLOBAL_STATUS                       1       /**< Global status. */
#define SI446X_FRR_MODE_GLOBAL_INTERRUPT_PENDING            2       /**< Global interrupt pending. */
#define SI446X_FRR_MODE_PACKET_HANDLER_STATUS               3       /**< Packet Handler status. */
#define SI446X_FRR_MODE_PACKET_HANDLER_INTERRUPT_PENDING    4       /**< Packet Handler interrupt pending. */
#define SI446X_FRR_MODE_MODEM_STATUS                        5       /**< Modem status. */
#define SI446X_FRR_MODE_MODEM_INTERRUPT_PENDING             6       /**< Modem interrupt pending. */
#define SI446X_FRR_MODE_CHIP_STATUS                         7       /**< Chip status. */
#define SI446X_FRR_MODE_CHIP_INTERRUPT_PENDING              8       /**< Chip status interrupt pending. */
#define SI446X_FRR_MODE_CURRENT_STATE                       9       /**< Current state. */
#define SI446X_FRR_MODE_LATCHED_RSSI                        10      /**< Latched RSSI value as defined in MODEM_RSSI_CONTROL:LATCH. */

#endif /* SI446X_PROP_H_ */

/** \} End of si446x_prop group */
