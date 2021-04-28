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
 * \version 0.6.4
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
    return -1;
}

int __wrap_si446x_power_up(uint8_t boot_options, uint8_t xtal_options, uint32_t xo_freq)
{
    return -1;
}

int __wrap_si446x_configuration_init(uint8_t *p_set_prop_cmd, uint16_t p_set_prop_cmd_len)
{
    return -1;
}

int __wrap_si446x_part_info(si446x_part_info_t *part_info)
{
    return -1;
}

int __wrap_si446x_start_tx(uint8_t channel, uint8_t condition, uint16_t tx_len)
{
    return -1;
}

int __wrap_si446x_start_rx(uint8_t channel, uint8_t condition, uint16_t rx_len, si446x_state_t next_state1, si446x_state_t next_state2, si446x_state_t next_state3)
{
    return -1;
}

int __wrap_si446x_get_int_status(uint8_t ph_clr_pend, uint8_t modem_clr_pend, uint8_t chip_clr_pend, si446x_int_status_t *int_status)
{
    return -1;
}

int __wrap_si446x_gpio_pin_cfg(uint8_t gpio0, uint8_t gpio1, uint8_t gpio2, uint8_t gpio3, uint8_t nirq, uint8_t sdo, uint8_t gen_config)
{
    return -1;
}

int __wrap_si446x_set_property(uint8_t group, uint8_t num_props, uint8_t start_prop, uint8_t *data, uint16_t len)
{
    return -1;
}

int __wrap_si446x_change_state(si446x_state_t next_state)
{
    return -1;
}

int __wrap_si446x_nop(void)
{
    return -1;
}

int __wrap_si446x_fifo_info(bool rst_rx, bool rst_tx, si446x_fifo_info_t *fifo_info)
{
    return -1;
}

int __wrap_si446x_write_tx_fifo(uint8_t num_bytes, uint8_t *p_tx_data)
{
    return -1;
}

int __wrap_si446x_read_rx_fifo(uint8_t num_bytes, uint8_t *p_rx_data)
{
    return -1;
}

int __wrap_si446x_get_property(uint8_t group, uint8_t num_props, uint8_t start_prop, uint8_t *data)
{
    return -1;
}

int __wrap_si446x_func_info(si446x_func_info_t *func_info)
{
    return -1;
}

int __wrap_si446x_frr_a_read(uint8_t resp_byte_count, uint8_t *frr_a_val)
{
    return -1;
}

int __wrap_si446x_frr_b_read(uint8_t resp_byte_count, uint8_t *frr_b_val)
{
    return -1;
}

int __wrap_si446x_frr_c_read(uint8_t resp_byte_count, uint8_t *frr_c_val)
{
    return -1;
}

int __wrap_si446x_frr_d_read(uint8_t resp_byte_count, uint8_t *frr_d_val)
{
    return -1;
}

int __wrap_si446x_get_adc_reading(uint8_t temp_en, bool bat_volt_en, bool adc_gpio_en, uint8_t adc_gpio_pin, si446x_adc_reading_t *adc_reading)
{
    return -1;
}

int __wrap_si446x_get_packet_info(uint8_t field_number_mask, uint16_t len, int16_t diff_len, uint16_t *last_len)
{
    return -1;
}

int __wrap_si446x_get_ph_status(si446x_ph_status_t *ph_status)
{
    return -1;
}

int __wrap_si446x_get_modem_status(uint8_t modem_clr_pend, si446x_modem_status_t *modem_status)
{
    return -1;
}

int __wrap_si446x_get_chip_status(uint8_t chip_clr_pend, si446x_chip_status_t *chip_status)
{
    return -1;
}

int __wrap_si446x_ircal(uint8_t searching_step_size, uint8_t searching_rssi_avg, uint8_t rx_chain_setting1, uint8_t rx_chain_setting2)
{
    return -1;
}

int __wrap_si446x_protocol_cfg(uint8_t protocol)
{
    return -1;
}

int __wrap_si446x_request_device_state(si446x_device_state_t *dev_state)
{
    return -1;
}

int __wrap_si446x_rx_hop(uint8_t inte, uint8_t frac2, uint8_t frac1, uint8_t frac0, uint8_t vco_cnt1, uint8_t vco_cnt0)
{
    return -1;
}

int __wrap_si446x_shutdown(void)
{
    return -1;
}

int __wrap_si446x_power_on(void)
{
    return -1;
}

bool __wrap_si446x_check_cts(uint32_t timeout_ms)
{
    return false;
}

int __wrap_si446x_set_cmd(uint8_t *cmd, uint16_t cmd_len)
{
    return -1;
}

int __wrap_si446x_get_cmd(uint8_t *cmd, uint16_t cmd_len, uint8_t *result, uint16_t result_len)
{
    return -1;
}

int __wrap_si446x_spi_init()
{
    return -1;
}

int __wrap_si446x_spi_transfer(uint8_t *wd, uint8_t *rd, uint16_t len)
{
    return -1;
}

int __wrap_si446x_spi_write_byte(uint8_t byte)
{
    return -1;
}

int __wrap_si446x_spi_write(uint8_t *data, uint16_t len)
{
    return -1;
}

int __wrap_si446x_spi_read(uint8_t *data, uint16_t len)
{
    return -1;
}

int __wrap_si446x_gpio_init()
{
    return -1;
}

int __wrap_si446x_gpio_write_sdn(bool state)
{
    return -1;
}

int __wrap_si446x_gpio_read_nirq()
{
    return -1;
}

void __wrap_si446x_delay_ms(uint32_t ms)
{
    return;
}

/** \} End of si446x_wrap group */
