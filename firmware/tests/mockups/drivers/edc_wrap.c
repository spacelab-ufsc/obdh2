/*
 * edc_wrap.c
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
 * \brief EDC driver wrap implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.4
 * 
 * \date 2021/08/16
 * 
 * \addtogroup edc_wrap
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include "edc_wrap.h"

int __wrap_edc_init(edc_config_t config)
{
    check_expected(config.port);
    check_expected(config.bitrate);

    return mock_type(int);
}

int __wrap_edc_write_cmd(edc_cmd_t cmd)
{
    check_expected(cmd.id);
    check_expected(cmd.param);

    return mock_type(int);
}

int __wrap_edc_read(uint8_t *data, uint16_t len)
{
    check_expected_ptr(data);
    check_expected(len);

    return mock_type(int);
}

int __wrap_edc_check_device(void)
{
    return mock_type(int);
}

int __wrap_edc_set_rtc_time(uint32_t time)
{
    check_expected(time);

    return mock_type(int);
}

int __wrap_edc_pop_ptt_pkg(void)
{
    return mock_type(int);
}

int __wrap_edc_pause_ptt_task(void)
{
    return mock_type(int);
}

int __wrap_edc_resume_ptt_task(void)
{
    return mock_type(int);
}

int __wrap_edc_start_adc_task(void)
{
    return mock_type(int);
}

int16_t __wrap_edc_get_state_pkg(uint8_t *status)
{
    if (status != NULL)
    {
        status = mock_ptr_type(uint8_t*);
    }

    return mock_type(int16_t);
}

int16_t __wrap_edc_get_ptt_pkg(uint8_t *pkg)
{
    if (pkg != NULL)
    {
        pkg = mock_ptr_type(uint8_t*);
    }

    return mock_type(int16_t);
}

int16_t __wrap_edc_get_hk_pkg(uint8_t *hk)
{
    if (hk != NULL)
    {
        hk = mock_ptr_type(uint8_t*);
    }

    return mock_type(int16_t);
}

int16_t __wrap_edc_get_adc_seq(uint8_t *seq)
{
    if (seq != NULL)
    {
        seq = mock_ptr_type(uint8_t*);
    }

    return mock_type(int16_t);
}

int __wrap_edc_echo(void)
{
    return mock_type(int);
}

uint16_t __wrap_edc_calc_checksum(uint8_t *data, uint16_t len)
{
    check_expected_ptr(data);
    check_expected(len);

    return mock_type(uint16_t);
}

int __wrap_edc_get_state(edc_state_t *state_data)
{
    if (state_data != NULL)
    {
        state_data->current_time    = mock_type(uint32_t);
        state_data->ptt_available   = mock_type(uint8_t);
        state_data->ptt_is_paused   = mock_type(bool);
        state_data->sampler_state   = mock_type(uint8_t);
    }

    return mock_type(int);
}

int __wrap_edc_get_hk(edc_hk_t *hk_data)
{
    if (hk_data != NULL)
    {
        hk_data->current_time       = mock_type(uint32_t);
        hk_data->elapsed_time       = mock_type(uint32_t);
        hk_data->current_supply     = mock_type(uint16_t);
        hk_data->voltage_supply     = mock_type(uint16_t);
        hk_data->temp               = mock_type(int8_t);
        hk_data->pll_sync_bit       = mock_type(uint8_t);
        hk_data->adc_rms            = mock_type(int16_t);
        hk_data->num_rx_ptt         = mock_type(uint8_t);
        hk_data->max_parl_decod     = mock_type(uint8_t);
        hk_data->mem_err_count      = mock_type(uint8_t);
    }

    return mock_type(int);
}

void __wrap_edc_delay_ms(uint32_t ms)
{
    check_expected(ms);

    return;
}


/** \} End of edc_wrap group */
