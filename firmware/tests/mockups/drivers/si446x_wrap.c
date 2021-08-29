/*
 * si446x_wrap.c
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
 * \brief Si446x driver wrap implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.8
 * 
 * \date 2021/04/27
 * 
 * \addtogroup si446x_wrap
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include "si446x_wrap.h"

int __wrap_si446x_reset(void)
{
    return mock_type(int);
}

int __wrap_si446x_power_up(uint8_t boot_options, uint8_t xtal_options, uint32_t xo_freq)
{
    check_expected(boot_options);
    check_expected(xtal_options);
    check_expected(xo_freq);

    return mock_type(int);
}

int __wrap_si446x_configuration_init(uint8_t *p_set_prop_cmd, uint16_t p_set_prop_cmd_len)
{
    check_expected_ptr(p_set_prop_cmd);
    check_expected(p_set_prop_cmd_len);

    return mock_type(int);
}

int __wrap_si446x_part_info(si446x_part_info_t *part_info)
{
    if (part_info != NULL)
    {
        part_info->chiprev      = mock_type(uint8_t);
        part_info->part         = mock_type(uint16_t);
        part_info->pbuild       = mock_type(uint8_t);
        part_info->id           = mock_type(uint16_t);
        part_info->customer     = mock_type(uint8_t);
        part_info->romid        = mock_type(uint8_t);
    }

    return mock_type(int);
}

int __wrap_si446x_start_tx(uint8_t channel, uint8_t condition, uint16_t tx_len)
{
    check_expected(channel);
    check_expected(condition);
    check_expected(tx_len);

    return mock_type(int);
}

int __wrap_si446x_start_rx(uint8_t channel, uint8_t condition, uint16_t rx_len, si446x_state_t next_state1, si446x_state_t next_state2, si446x_state_t next_state3)
{
    check_expected(channel);
    check_expected(condition);
    check_expected(rx_len);
    check_expected(next_state1);
    check_expected(next_state2);
    check_expected(next_state3);

    return mock_type(int);
}

int __wrap_si446x_get_int_status(uint8_t ph_clr_pend, uint8_t modem_clr_pend, uint8_t chip_clr_pend, si446x_int_status_t *int_status)
{
    check_expected(ph_clr_pend);
    check_expected(modem_clr_pend);
    check_expected(chip_clr_pend);

    if (int_status != NULL)
    {
        int_status->int_pend        = mock_type(uint8_t);
        int_status->int_status      = mock_type(uint8_t);
        int_status->ph_pend         = mock_type(uint8_t);
        int_status->ph_status       = mock_type(uint8_t);
        int_status->modem_pend      = mock_type(uint8_t);
        int_status->modem_status    = mock_type(uint8_t);
        int_status->chip_pend       = mock_type(uint8_t);
        int_status->chip_status     = mock_type(uint8_t);
    }

    return mock_type(int);
}

int __wrap_si446x_gpio_pin_cfg(uint8_t gpio0, uint8_t gpio1, uint8_t gpio2, uint8_t gpio3, uint8_t nirq, uint8_t sdo, uint8_t gen_config)
{
    check_expected(gpio0);
    check_expected(gpio1);
    check_expected(gpio2);
    check_expected(gpio3);
    check_expected(nirq);
    check_expected(sdo);
    check_expected(gen_config);

    return mock_type(int);
}

int __wrap_si446x_set_property(uint8_t group, uint8_t num_props, uint8_t start_prop, uint8_t *data, uint16_t len)
{
    check_expected(group);
    check_expected(num_props);
    check_expected(start_prop);
    check_expected_ptr(data);
    check_expected(len);

    return mock_type(int);
}

int __wrap_si446x_change_state(si446x_state_t next_state)
{
    check_expected(next_state);

    return mock_type(int);
}

int __wrap_si446x_nop(void)
{
    return mock_type(int);
}

int __wrap_si446x_fifo_info(bool rst_rx, bool rst_tx, si446x_fifo_info_t *fifo_info)
{
    check_expected(rst_rx);
    check_expected(rst_tx);

    if (fifo_info != NULL)
    {
        fifo_info->rx_fifo_count = mock_type(uint8_t);
        fifo_info->tx_fifo_space = mock_type(uint8_t);
    }

    return mock_type(int);
}

int __wrap_si446x_write_tx_fifo(uint8_t num_bytes, uint8_t *p_tx_data)
{
    check_expected(num_bytes);
    check_expected_ptr(p_tx_data);

    return mock_type(int);
}

int __wrap_si446x_read_rx_fifo(uint8_t num_bytes, uint8_t *p_rx_data)
{
    check_expected(num_bytes);

    if (p_rx_data != NULL)
    {
        uint8_t i = 0;
        for(i=0; i<num_bytes; i++)
        {
            p_rx_data[i] = mock_type(uint8_t);
        }
    }

    return mock_type(int);
}

int __wrap_si446x_get_property(uint8_t group, uint8_t num_props, uint8_t start_prop, uint8_t *data)
{
    check_expected(group);
    check_expected(num_props);
    check_expected(start_prop);

    if (data != NULL)
    {
        uint8_t i = 0;
        for(i=0; i<num_props; i++)
        {
            data[i] = mock_type(uint8_t);
        }
    }

    return mock_type(int);
}

int __wrap_si446x_func_info(si446x_func_info_t *func_info)
{
    if (func_info != NULL)
    {
        func_info->revext       = mock_type(uint8_t);
        func_info->revbranch    = mock_type(uint8_t);
        func_info->revint       = mock_type(uint8_t);
        func_info->patch        = mock_type(uint16_t);
        func_info->func         = mock_type(uint8_t);
    }

    return mock_type(int);
}

int __wrap_si446x_frr_a_read(uint8_t resp_byte_count, uint8_t *frr_a_val)
{
    check_expected(resp_byte_count);

    if (frr_a_val != NULL)
    {
        uint8_t i = 0;
        for(i=0; i<resp_byte_count; i++)
        {
            frr_a_val[i] = mock_type(uint8_t);
        }
    }

    return mock_type(int);
}

int __wrap_si446x_frr_b_read(uint8_t resp_byte_count, uint8_t *frr_b_val)
{
    check_expected(resp_byte_count);

    if (frr_b_val != NULL)
    {
        uint8_t i = 0;
        for(i=0; i<resp_byte_count; i++)
        {
            frr_b_val[i] = mock_type(uint8_t);
        }
    }

    return mock_type(int);
}

int __wrap_si446x_frr_c_read(uint8_t resp_byte_count, uint8_t *frr_c_val)
{
    check_expected(resp_byte_count);

    if (frr_c_val != NULL)
    {
        uint8_t i = 0;
        for(i=0; i<resp_byte_count; i++)
        {
            frr_c_val[i] = mock_type(uint8_t);
        }
    }

    return mock_type(int);
}

int __wrap_si446x_frr_d_read(uint8_t resp_byte_count, uint8_t *frr_d_val)
{
    check_expected(resp_byte_count);

    if (frr_d_val != NULL)
    {
        uint8_t i = 0;
        for(i=0; i<resp_byte_count; i++)
        {
            frr_d_val[i] = mock_type(uint8_t);
        }
    }

    return mock_type(int);
}

int __wrap_si446x_get_adc_reading(uint8_t temp_en, bool bat_volt_en, bool adc_gpio_en, uint8_t adc_gpio_pin, si446x_adc_reading_t *adc_reading)
{
    check_expected(temp_en);
    check_expected(bat_volt_en);
    check_expected(adc_gpio_en);
    check_expected(adc_gpio_pin);

    if (adc_reading != NULL)
    {
        adc_reading->gpio_adc       = mock_type(uint16_t);
        adc_reading->battery_adc    = mock_type(uint16_t);
        adc_reading->temp_adc       = mock_type(uint16_t);
        adc_reading->temp_slope     = mock_type(uint8_t);
        adc_reading->temp_intercept = mock_type(uint8_t);
    }

    return mock_type(int);
}

int __wrap_si446x_get_packet_info(uint8_t field_number_mask, uint16_t len, int16_t diff_len, uint16_t *last_len)
{
    check_expected(field_number_mask);
    check_expected(len);
    check_expected(diff_len);

    if (last_len != NULL)
    {
        *last_len = mock_type(int);
    }

    return mock_type(int);
}

int __wrap_si446x_get_ph_status(si446x_ph_status_t *ph_status)
{
    if (ph_status != NULL)
    {
        ph_status->ph_pend      = mock_type(uint8_t);
        ph_status->ph_status    = mock_type(uint8_t);
    }

    return mock_type(int);
}

int __wrap_si446x_get_modem_status(uint8_t modem_clr_pend, si446x_modem_status_t *modem_status)
{
    check_expected(modem_clr_pend);

    if (modem_status != NULL)
    {
        modem_status->modem_pend        = mock_type(uint8_t);
        modem_status->modem_status      = mock_type(uint8_t);
        modem_status->curr_rssi         = mock_type(uint8_t);
        modem_status->latch_rssi        = mock_type(uint8_t);
        modem_status->ant1_rssi         = mock_type(uint8_t);
        modem_status->ant2_rssi         = mock_type(uint8_t);
        modem_status->afc_freq_offset   = mock_type(uint16_t);
    }

    return mock_type(int);
}

int __wrap_si446x_get_chip_status(uint8_t chip_clr_pend, si446x_chip_status_t *chip_status)
{
    check_expected(chip_clr_pend);

    if (chip_status != NULL)
    {
        chip_status->chip_pend          = mock_type(uint8_t);
        chip_status->chip_status        = mock_type(uint8_t);
        chip_status->chip_err_status    = mock_type(uint8_t);
    }

    return mock_type(int);
}

int __wrap_si446x_ircal(uint8_t searching_step_size, uint8_t searching_rssi_avg, uint8_t rx_chain_setting1, uint8_t rx_chain_setting2)
{
    check_expected(searching_step_size);
    check_expected(searching_rssi_avg);
    check_expected(rx_chain_setting1);
    check_expected(rx_chain_setting2);

    return mock_type(int);
}

int __wrap_si446x_protocol_cfg(uint8_t protocol)
{
    check_expected(protocol);

    return mock_type(int);
}

int __wrap_si446x_request_device_state(si446x_device_state_t *dev_state)
{
    if (dev_state != NULL)
    {
        dev_state->curr_state       = mock_type(uint8_t);
        dev_state->current_channel  = mock_type(uint8_t);
    }

    return mock_type(int);
}

int __wrap_si446x_rx_hop(uint8_t inte, uint8_t frac2, uint8_t frac1, uint8_t frac0, uint8_t vco_cnt1, uint8_t vco_cnt0)
{
    check_expected(inte);
    check_expected(frac2);
    check_expected(frac1);
    check_expected(frac0);
    check_expected(vco_cnt1);
    check_expected(vco_cnt0);

    return mock_type(int);
}

int __wrap_si446x_shutdown(void)
{
    return mock_type(int);
}

int __wrap_si446x_power_on(void)
{
    return mock_type(int);
}

bool __wrap_si446x_check_cts(uint32_t timeout_ms)
{
    check_expected(timeout_ms);

    return mock_type(bool);
}

int __wrap_si446x_set_cmd(uint8_t *cmd, uint16_t cmd_len)
{
    check_expected_ptr(cmd);
    check_expected(cmd_len);

    return mock_type(int);
}

int __wrap_si446x_get_cmd(uint8_t *cmd, uint16_t cmd_len, uint8_t *result, uint16_t result_len)
{
    check_expected(cmd_len);

    if (cmd != NULL)
    {
        uint16_t i = 0;
        for(i=0; i<cmd_len; i++)
        {
            cmd[i] = mock_type(uint8_t);
        }
    }

    check_expected(result_len);

    if (cmd != NULL)
    {
        uint16_t i = 0;
        for(i=0; i<result_len; i++)
        {
            result[i] = mock_type(uint8_t);
        }
    }

    return mock_type(int);
}

int __wrap_si446x_spi_init(void)
{
    return mock_type(int);
}

int __wrap_si446x_spi_transfer(uint8_t *wd, uint8_t *rd, uint16_t len)
{
    check_expected_ptr(wd);
    check_expected(len);

    if (rd != NULL)
    {
        uint16_t i = 0;
        for(i=0; i<len; i++)
        {
            rd[i] = mock_type(uint8_t);
        }
    }

    return mock_type(int);
}

int __wrap_si446x_spi_write_byte(uint8_t byte)
{
    check_expected(byte);

    return mock_type(int);
}

int __wrap_si446x_spi_write(uint8_t *data, uint16_t len)
{
    check_expected_ptr(data);
    check_expected(len);

    return mock_type(int);
}

int __wrap_si446x_spi_read(uint8_t *data, uint16_t len)
{
    check_expected(len);

    if (data != NULL)
    {
        uint16_t i = 0;
        for(i=0; i<len; i++)
        {
            data[i] = mock_type(uint8_t);
        }
    }

    return mock_type(int);
}

int __wrap_si446x_gpio_init(void)
{
    return mock_type(int);
}

int __wrap_si446x_gpio_write_sdn(bool state)
{
    check_expected(state);

    return mock_type(int);
}

int __wrap_si446x_gpio_read_nirq(void)
{
    return mock_type(int);
}

void __wrap_si446x_delay_ms(uint32_t ms)
{
    function_called();

    return;
}

/** \} End of si446x_wrap group */
