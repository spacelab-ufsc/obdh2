/*
 * rf4463_config.h
 * 
 * Copyright (C) 2017-2019, Universidade Federal de Santa Catarina
 * 
 * This file is part of FloripaSat-OBDH.
 * 
 * FloripaSat-OBDH is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * FloripaSat-OBDH is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with FloripaSat-OBDH. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief NiceRF RF4463 configuration.
 * 
 * This library suits for RF4463PRO and RF4463F30 in FIFO mode.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.2.8
 * 
 * \date 16/06/2017
 * 
 * \addtogroup rf4463
 * \{
 */

#ifndef RF4463_CONFIG_H_
#define RF4463_CONFIG_H_

#define RF4463_MODULE_NAME                      "RF4463"

#define RF4463_SPI_CLK                          BEACON_RADIO_SPI_CLK

#define RF4463_PART_INFO                        0x4463
#define RF4463_TX_FIFO_LEN                      128
#define RF4463_RX_FIFO_LEN                      128
#define RF4463_TX_FIFO_ALMOST_EMPTY_THRESHOLD   48
#define RF4463_RX_FIFO_ALMOST_FULL_THRESHOLD    48

#define RF4463_CTS_REPLY                        0xFF
#define RF4463_CTS_TIMEOUT                      2500    // Waiting time for a valid FFh CTS reading. The typical time is 20 us.
#define RF4463_TX_TIMEOUT                       20000   // Waiting time for packet send interrupt. this time is depended on tx length and data rate of wireless.
#define RF4463_FREQ_CHANNEL                     0       // Frequency channel.

// This value must be obtained measuring the output signal with a frequency analyzer
// The register value is tuned according to the desired output frequency
#define RF4463_XO_TUNE_REG_VALUE                97

#endif // RF4463_CONFIG_H_

//! \} End of rf4463 group
