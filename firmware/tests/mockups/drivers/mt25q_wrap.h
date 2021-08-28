/*
 * mt25q_wrap.h
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
 * \brief MT25Q driver wrap definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.2
 * 
 * \date 2021/08/08
 * 
 * \defgroup mt25q_wrap MT25q Wrap
 * \ingroup tests
 * \{
 */

#ifndef MT25Q_WRAP_H_
#define MT25Q_WRAP_H_

#include <stdint.h>
#include <stdbool.h>

#include <drivers/mt25q/mt25q.h>

int __wrap_mt25q_init(void);

int __wrap_mt25q_reset(void);

int __wrap_mt25q_read_device_id(mt25q_dev_id_t *dev_id);

int __wrap_mt25q_read_flash_description(flash_description_t *fdo);

int __wrap_mt25q_clear_flag_status_register(void);

int __wrap_mt25q_read_status(mt25q_status_t *status);

int __wrap_mt25q_enter_deep_power_down(void);

int __wrap_mt25q_release_from_deep_power_down(void);

int __wrap_mt25q_write_enable(void);

int __wrap_mt25q_write_disable(void);

bool __wrap_mt25q_is_busy(void);

int __wrap_mt25q_die_erase(mt25q_sector_t die);

int __wrap_mt25q_sector_erase(mt25q_sector_t sector);

int __wrap_mt25q_sub_sector_erase(mt25q_sector_t sub);

int __wrap_mt25q_write(uint32_t adr, uint8_t *data, uint16_t len);

int __wrap_mt25q_read(uint32_t adr, uint8_t *data, uint16_t len);

uint32_t __wrap_mt25q_get_max_address(void);

int __wrap_mt25q_enter_4_byte_address_mode(void);

int __wrap_mt25q_read_flag_status_register(uint8_t *flag);

flash_description_t __wrap_mt25q_get_flash_description(void);

int __wrap_mt25q_spi_init(void);

int __wrap_mt25q_spi_write(uint8_t *data, uint16_t len);

int __wrap_mt25q_spi_read(uint8_t *data, uint16_t len);

int __wrap_mt25q_spi_transfer(uint8_t *wdata, uint8_t *rdata, uint16_t len);

int __wrap_mt25q_spi_select(void);

int __wrap_mt25q_spi_unselect(void);

int __wrap_mt25q_spi_write_only(uint8_t *data, uint16_t len);

int __wrap_mt25q_spi_read_only(uint8_t *data, uint16_t len);

int __wrap_mt25q_spi_transfer_only(uint8_t *wdata, uint8_t *rdata, uint16_t len);

int __wrap_mt25q_gpio_init(void);

int __wrap_mt25q_gpio_set_hold(bool state);

int __wrap_mt25q_gpio_set_reset(bool state);

void __wrap_mt25q_delay_ms(uint32_t ms);

#endif /* MT25Q_WRAP_H_ */

/** \} End of mt25q_wrap group */
