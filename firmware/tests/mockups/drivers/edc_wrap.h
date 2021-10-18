/*
 * edc_wrap.h
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
 * \brief EDC driver wrap definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.8.14
 * 
 * \date 2021/08/16
 * 
 * \defgroup edc_wrap EDC Wrap
 * \ingroup tests
 * \{
 */

#ifndef EDC_WRAP_H_
#define EDC_WRAP_H_

#include <stdint.h>

#include <drivers/edc/edc.h>

int __wrap_edc_init(edc_config_t config);

int __wrap_edc_write_cmd(edc_config_t config, edc_cmd_t cmd);

int __wrap_edc_read(edc_config_t config, uint8_t *data, uint16_t len);

int __wrap_edc_check_device(edc_config_t config);

int __wrap_edc_set_rtc_time(edc_config_t config, uint32_t time);

int __wrap_edc_pop_ptt_pkg(edc_config_t config);

int __wrap_edc_pause_ptt_task(edc_config_t config);

int __wrap_edc_resume_ptt_task(edc_config_t config);

int __wrap_edc_start_adc_task(edc_config_t config);

int16_t __wrap_edc_get_state_pkg(edc_config_t config, uint8_t *status);

int16_t __wrap_edc_get_ptt_pkg(edc_config_t config, uint8_t *pkg);

int16_t __wrap_edc_get_hk_pkg(edc_config_t config, uint8_t *hk);

int16_t __wrap_edc_get_adc_seq(edc_config_t config, uint8_t *seq);

int __wrap_edc_echo(edc_config_t config);

uint16_t __wrap_edc_calc_checksum(uint8_t *data, uint16_t len);

int __wrap_edc_get_state(edc_config_t config, edc_state_t *state_data);

int __wrap_edc_get_hk(edc_config_t config, edc_hk_t *hk_data);

void __wrap_edc_delay_ms(uint32_t ms);

#endif /* EDC_WRAP_H_ */

/** \} End of edc_wrap group */
