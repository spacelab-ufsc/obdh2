/*
 * cy15x102qn_wrap.h
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
 * \brief CY15x102QN driver wrap definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.2
 * 
 * \date 2021/08/08
 * 
 * \defgroup cy15x102qn_wrap CY15x102QN Wrap
 * \ingroup tests
 * \{
 */

#ifndef CY15X102QN_WRAP_H_
#define CY15X102QN_WRAP_H_

#include <stdint.h>

#include <drivers/cy15x102qn/cy15x102qn.h>

int __wrap_cy15x102qn_init(cy15x102qn_config_t *conf);

int __wrap_cy15x102qn_set_write_enable(cy15x102qn_config_t *conf);

int __wrap_cy15x102qn_reset_write_enable(cy15x102qn_config_t *conf);

int __wrap_cy15x102qn_read_status_reg(cy15x102qn_config_t *conf, cy15x102qn_status_t *status);

int __wrap_cy15x102qn_write_status_reg(cy15x102qn_config_t *conf, cy15x102qn_status_t status);

int __wrap_cy15x102qn_write(cy15x102qn_config_t *conf, uint32_t adr, uint8_t *data, uint32_t len);

int __wrap_cy15x102qn_read(cy15x102qn_config_t *conf, uint32_t adr, uint8_t *data, uint32_t len);

int __wrap_cy15x102qn_fast_read(cy15x102qn_config_t *conf, uint32_t adr, uint8_t *data, uint32_t len);

int __wrap_cy15x102qn_special_sector_write(cy15x102qn_config_t *conf, uint8_t adr, uint8_t *data, uint16_t len);

int __wrap_cy15x102qn_special_sector_read(cy15x102qn_config_t *conf, uint8_t adr, uint8_t *data, uint16_t len);

int __wrap_cy15x102qn_read_device_id(cy15x102qn_config_t *conf, cy15x102qn_device_id_t *id);

int __wrap_cy15x102qn_read_unique_id(cy15x102qn_config_t *conf, cy15x102qn_uid_t *uid);

int __wrap_cy15x102qn_write_serial_number(cy15x102qn_config_t *conf, cy15x102qn_serial_number_t s_num);

int __wrap_cy15x102qn_read_serial_number(cy15x102qn_config_t *conf, cy15x102qn_serial_number_t *s_num);

int __wrap_cy15x102qn_deep_power_down_mode(cy15x102qn_config_t *conf);

int __wrap_cy15x102qn_hibernate_mode(cy15x102qn_config_t *conf);

int __wrap_cy15x102qn_spi_init(cy15x102qn_config_t *conf);

int __wrap_cy15x102qn_spi_write(cy15x102qn_config_t *conf, uint8_t *data, uint16_t len);

int __wrap_cy15x102qn_spi_read(cy15x102qn_config_t *conf, uint8_t *data, uint16_t len);

int __wrap_cy15x102qn_spi_transfer(cy15x102qn_config_t *conf, uint8_t *wdata, uint8_t *rdata, uint16_t len);

int __wrap_cy15x102qn_spi_select(cy15x102qn_config_t *conf);

int __wrap_cy15x102qn_spi_unselect(cy15x102qn_config_t *conf);

int __wrap_cy15x102qn_spi_write_only(cy15x102qn_config_t *conf, uint8_t *data, uint16_t len);

int __wrap_cy15x102qn_spi_read_only(cy15x102qn_config_t *conf, uint8_t *data, uint16_t len);

int __wrap_cy15x102qn_spi_transfer_only(cy15x102qn_config_t *conf, uint8_t *wdata, uint8_t *rdata, uint16_t len);

int __wrap_cy15x102qn_gpio_init(cy15x102qn_config_t *conf);

int __wrap_cy15x102qn_gpio_set_write_protect(cy15x102qn_config_t *conf);

int __wrap_cy15x102qn_gpio_clear_write_protect(cy15x102qn_config_t *conf);

#endif /* CY15X102QN_WRAP_H_ */

/** \} End of cy15x102qn_wrap group */
