/*
 * si446x_test.c
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
 * \brief Unit test of the Si446x driver.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.12
 * 
 * \date 2021/09/04
 * 
 * \defgroup si446x_unit_test Si446x
 * \ingroup tests
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include <drivers/gpio/gpio.h>
#include <drivers/spi/spi.h>
#include <drivers/si446x/si446x.h>

static void si446x_reset_test(void **state)
{
}

static void si446x_power_up_test(void **state)
{
}

static void si446x_configuration_init_test(void **state)
{
}

static void si446x_part_info_test(void **state)
{
}

static void si446x_start_tx_test(void **state)
{
}

static void si446x_start_rx_test(void **state)
{
}

static void si446x_get_int_status_test(void **state)
{
}

static void si446x_gpio_pin_cfg_test(void **state)
{
}

static void si446x_set_property_test(void **state)
{
}

static void si446x_change_state_test(void **state)
{
}

static void si446x_nop_test(void **state)
{
}

static void si446x_fifo_info_test(void **state)
{
}

static void si446x_write_tx_fifo_test(void **state)
{
}

static void si446x_read_rx_fifo_test(void **state)
{
}

static void si446x_get_property_test(void **state)
{
}

static void si446x_func_info_test(void **state)
{
}

static void si446x_frr_a_read_test(void **state)
{
}

static void si446x_frr_b_read_test(void **state)
{
}

static void si446x_frr_c_read_test(void **state)
{
}

static void si446x_frr_d_read_test(void **state)
{
}

static void si446x_get_adc_reading_test(void **state)
{
}

static void si446x_get_packet_info_test(void **state)
{
}

static void si446x_get_ph_status_test(void **state)
{
}

static void si446x_get_modem_status_test(void **state)
{
}

static void si446x_get_chip_status_test(void **state)
{
}

static void si446x_ircal_test(void **state)
{
}

static void si446x_protocol_cfg_test(void **state)
{
}

static void si446x_request_device_state_test(void **state)
{
}

static void si446x_rx_hop_test(void **state)
{
}

static void si446x_shutdown_test(void **state)
{
}

static void si446x_power_on_test(void **state)
{
}

static void si446x_check_cts_test(void **state)
{
}

static void si446x_set_cmd_test(void **state)
{
}

static void si446x_get_cmd_test(void **state)
{
}

static void si446x_spi_init_test(void **state)
{
}

static void si446x_spi_transfer_test(void **state)
{
}

static void si446x_spi_write_byte_test(void **state)
{
}

static void si446x_spi_write_test(void **state)
{
}

static void si446x_spi_read_test(void **state)
{
}

static void si446x_gpio_init_test(void **state)
{
}

static void si446x_gpio_write_sdn_test(void **state)
{
}

static void si446x_gpio_read_nirq_test(void **state)
{
}

static void si446x_delay_ms_test(void **state)
{
}

int main(void)
{
    const struct CMUnitTest si446x_tests[] = {
        cmocka_unit_test(si446x_reset_test),
        cmocka_unit_test(si446x_power_up_test),
        cmocka_unit_test(si446x_configuration_init_test),
        cmocka_unit_test(si446x_part_info_test),
        cmocka_unit_test(si446x_start_tx_test),
        cmocka_unit_test(si446x_start_rx_test),
        cmocka_unit_test(si446x_get_int_status_test),
        cmocka_unit_test(si446x_gpio_pin_cfg_test),
        cmocka_unit_test(si446x_set_property_test),
        cmocka_unit_test(si446x_change_state_test),
        cmocka_unit_test(si446x_nop_test),
        cmocka_unit_test(si446x_fifo_info_test),
        cmocka_unit_test(si446x_write_tx_fifo_test),
        cmocka_unit_test(si446x_read_rx_fifo_test),
        cmocka_unit_test(si446x_get_property_test),
        cmocka_unit_test(si446x_func_info_test),
        cmocka_unit_test(si446x_frr_a_read_test),
        cmocka_unit_test(si446x_frr_b_read_test),
        cmocka_unit_test(si446x_frr_c_read_test),
        cmocka_unit_test(si446x_frr_d_read_test),
        cmocka_unit_test(si446x_get_adc_reading_test),
        cmocka_unit_test(si446x_get_packet_info_test),
        cmocka_unit_test(si446x_get_ph_status_test),
        cmocka_unit_test(si446x_get_modem_status_test),
        cmocka_unit_test(si446x_get_chip_status_test),
        cmocka_unit_test(si446x_ircal_test),
        cmocka_unit_test(si446x_protocol_cfg_test),
        cmocka_unit_test(si446x_request_device_state_test),
        cmocka_unit_test(si446x_rx_hop_test),
        cmocka_unit_test(si446x_shutdown_test),
        cmocka_unit_test(si446x_power_on_test),
        cmocka_unit_test(si446x_check_cts_test),
        cmocka_unit_test(si446x_set_cmd_test),
        cmocka_unit_test(si446x_get_cmd_test),
        cmocka_unit_test(si446x_spi_init_test),
        cmocka_unit_test(si446x_spi_transfer_test),
        cmocka_unit_test(si446x_spi_write_byte_test),
        cmocka_unit_test(si446x_spi_write_test),
        cmocka_unit_test(si446x_spi_read_test),
        cmocka_unit_test(si446x_gpio_init_test),
        cmocka_unit_test(si446x_gpio_write_sdn_test),
        cmocka_unit_test(si446x_gpio_read_nirq_test),
        cmocka_unit_test(si446x_delay_ms_test),
    };

    return cmocka_run_group_tests(si446x_tests, NULL, NULL);
}

/** \} End of si446x_test group */
