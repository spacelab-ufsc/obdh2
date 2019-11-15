/*
 * rf4463_spi.c
 * 
 * Copyright (C) 2017, Federal University of Santa Catarina
 * 
 * This file is part of FloripaSat-Beacon.
 * 
 * FloripaSat-Beacon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * FloripaSat-Beacon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with FloripaSat-Beacon. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \file rf4463_spi.c
 * 
 * \brief NiceRF RF4463 driver (SPI communication functions implementation).
 * 
 * This library suits for RF4463PRO and RF4463F30 in FIFO mode.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 29/07/2017
 * 
 * \addtogroup rf4463
 * \{
 */

#include <stdint.h>
#include "../driver/spi.h"
#include "rf4463_spi.h"
#include "rf4463_config.h"
#include "rf4463_registers.h"

static void rf4463_spi_write_byte(uint8_t byte) {
    spi_tx(RADIO_BASE_ADDRESS, byte);
}

void rf4463_spi_write(uint8_t *data, uint16_t size) {
    spi_tx_multiple(RADIO_BASE_ADDRESS, data, size);
}

void rf4463_spi_read(uint8_t *data, uint16_t size) {
    spi_rx_multiple(RADIO_BASE_ADDRESS, data, size);
}

uint8_t rf4463_spi_transfer(uint8_t byte) {
    return spi_tx(RADIO_BASE_ADDRESS, byte);
}

//! \} End of rf4463 group
