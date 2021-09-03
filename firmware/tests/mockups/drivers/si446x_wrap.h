/*
 * si446x_wrap.h
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
 * \brief Si446x driver wrap definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.8
 * 
 * \date 2021/04/27
 * 
 * \defgroup si446x_wrap Si446x Wrap
 * \ingroup tests
 * \{
 */

#ifndef SI446X_WRAP_H_
#define SI446X_WRAP_H_

#include <stdbool.h>

#include <drivers/si446x/si446x.h>

/**
 * \brief Mockup function of the si446x reset routine.
 *
 * \return The status/error code.
 */
int __wrap_si446x_reset(void);

/**
 * \brief Mockup function of the si446x power-up routine.
 *
 * \param[in] boot_options: Patch mode selector.
 *
 * \param[in] xtal_option: Select if TCXO is in use.
 *
 * \param[in] xo_freq: Frequency of TCXO or external crystal oscillator in Hz.
 *
 * \note Before this function si446x_reset should be called.
 *
 * \return The status/error code.
 */
int __wrap_si446x_power_up(uint8_t boot_options, uint8_t xtal_options, uint32_t xo_freq);

int __wrap_si446x_configuration_init(uint8_t *p_set_prop_cmd, uint16_t p_set_prop_cmd_len);

int __wrap_si446x_part_info(si446x_part_info_t *part_info);

int __wrap_si446x_start_tx(uint8_t channel, uint8_t condition, uint16_t tx_len);

int __wrap_si446x_start_rx(uint8_t channel, uint8_t condition, uint16_t rx_len, si446x_state_t next_state1, si446x_state_t next_state2, si446x_state_t next_state3);

int __wrap_si446x_get_int_status(uint8_t ph_clr_pend, uint8_t modem_clr_pend, uint8_t chip_clr_pend, si446x_int_status_t *int_status);

int __wrap_si446x_gpio_pin_cfg(uint8_t gpio0, uint8_t gpio1, uint8_t gpio2, uint8_t gpio3, uint8_t nirq, uint8_t sdo, uint8_t gen_config);

int __wrap_si446x_set_property(uint8_t group, uint8_t num_props, uint8_t start_prop, uint8_t *data, uint16_t len);

int __wrap_si446x_change_state(si446x_state_t next_state);

int __wrap_si446x_nop(void);

int __wrap_si446x_fifo_info(bool rst_rx, bool rst_tx, si446x_fifo_info_t *fifo_info);

int __wrap_si446x_write_tx_fifo(uint8_t num_bytes, uint8_t *p_tx_data);

int __wrap_si446x_read_rx_fifo(uint8_t num_bytes, uint8_t *p_rx_data);

int __wrap_si446x_get_property(uint8_t group, uint8_t num_props, uint8_t start_prop, uint8_t *data);

int __wrap_si446x_func_info(si446x_func_info_t *func_info);

int __wrap_si446x_frr_a_read(uint8_t resp_byte_count, uint8_t *frr_a_val);

int __wrap_si446x_frr_b_read(uint8_t resp_byte_count, uint8_t *frr_b_val);

int __wrap_si446x_frr_c_read(uint8_t resp_byte_count, uint8_t *frr_c_val);

int __wrap_si446x_frr_d_read(uint8_t resp_byte_count, uint8_t *frr_d_val);

int __wrap_si446x_get_adc_reading(uint8_t temp_en, bool bat_volt_en, bool adc_gpio_en, uint8_t adc_gpio_pin, si446x_adc_reading_t *adc_reading);

int __wrap_si446x_get_packet_info(uint8_t field_number_mask, uint16_t len, int16_t diff_len, uint16_t *last_len);

int __wrap_si446x_get_ph_status(si446x_ph_status_t *ph_status);

int __wrap_si446x_get_modem_status(uint8_t modem_clr_pend, si446x_modem_status_t *modem_status);

int __wrap_si446x_get_chip_status(uint8_t chip_clr_pend, si446x_chip_status_t *chip_status);

int __wrap_si446x_ircal(uint8_t searching_step_size, uint8_t searching_rssi_avg, uint8_t rx_chain_setting1, uint8_t rx_chain_setting2);

int __wrap_si446x_protocol_cfg(uint8_t protocol);

int __wrap_si446x_request_device_state(si446x_device_state_t *dev_state);

int __wrap_si446x_rx_hop(uint8_t inte, uint8_t frac2, uint8_t frac1, uint8_t frac0, uint8_t vco_cnt1, uint8_t vco_cnt0);

int __wrap_si446x_shutdown(void);

int __wrap_si446x_power_on(void);

bool __wrap_si446x_check_cts(uint32_t timeout_ms);

int __wrap_si446x_set_cmd(uint8_t *cmd, uint16_t cmd_len);

int __wrap_si446x_get_cmd(uint8_t *cmd, uint16_t cmd_len, uint8_t *result, uint16_t result_len);

int __wrap_si446x_spi_init(void);

int __wrap_si446x_spi_transfer(uint8_t *wd, uint8_t *rd, uint16_t len);

int __wrap_si446x_spi_write_byte(uint8_t byte);

int __wrap_si446x_spi_write(uint8_t *data, uint16_t len);

int __wrap_si446x_spi_read(uint8_t *data, uint16_t len);

int __wrap_si446x_gpio_init(void);

int __wrap_si446x_gpio_write_sdn(bool state);

int __wrap_si446x_gpio_read_nirq(void);

void __wrap_si446x_delay_ms(uint32_t ms);

#endif /* WDT_WRAP_H_ */

/** \} End of si446x_wrap group */
