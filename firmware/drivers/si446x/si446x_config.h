/*
 * si446x_config.h
 * 
 * Copyright (C) 2017-2019, SpaceLab.
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
 * \brief Si446x configuration parameters.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.0
 * 
 * \date 16/06/2017
 * 
 * \addtogroup si446x
 * \{
 */

#ifndef SI446X_CONFIG_H_
#define SI446X_CONFIG_H_

#define SI446X_MODULE_NAME                      "SI446X"

#define SI446X_SPI_CLK                          BEACON_RADIO_SPI_CLK

#define SI4463_PART_INFO                        0x4463
#define SI446X_TX_FIFO_LEN                      128
#define SI446X_RX_FIFO_LEN                      128
#define SI446X_TX_FIFO_ALMOST_EMPTY_THRESHOLD   48
#define SI446X_RX_FIFO_ALMOST_FULL_THRESHOLD    48

#define SI446X_CTS_REPLY                        0xFF
#define SI446X_CTS_TIMEOUT                      2500    // Waiting time for a valid FFh CTS reading. The typical time is 20 us.
#define SI446X_TX_TIMEOUT                       20000   // Waiting time for packet send interrupt. this time is depended on tx length and data rate of wireless.
#define SI446X_FREQ_CHANNEL                     0       // Frequency channel.

// This value must be obtained measuring the output signal with a frequency analyzer
// The register value is tuned according to the desired output frequency
#define SI446X_XO_TUNE_REG_VALUE                97

#endif // SI446X_CONFIG_H_

//! \} End of si446x group
