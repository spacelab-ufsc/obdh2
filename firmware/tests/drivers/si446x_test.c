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
 * \version 0.7.14
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

#include <stdlib.h>

#include <drivers/gpio/gpio.h>
#include <drivers/spi/spi.h>
#include <drivers/si446x/si446x.h>

#define SI446X_GPIO_SDN         GPIO_PIN_4
#define SI446X_GPIO_NIRQ        GPIO_PIN_3
#define SI446X_SPI_PORT         SPI_PORT_0
#define SI446X_SPI_CS           GPIO_PIN_5

unsigned int generate_random(unsigned int l, unsigned int r);

void check_cts(void);

void set_cmd(uint8_t *cmd, uint16_t cmd_len);

void get_cmd(uint8_t *cmd, uint16_t cmd_len, uint8_t *res, uint16_t len);

static void si446x_reset_test(void **state)
{
    /* Shutdown */
    expect_value(__wrap_gpio_set_state, pin, SI446X_GPIO_SDN);
    expect_value(__wrap_gpio_set_state, level, true);

    will_return(__wrap_gpio_set_state, 0);

    /* Power on */
    expect_value(__wrap_gpio_set_state, pin, SI446X_GPIO_SDN);
    expect_value(__wrap_gpio_set_state, level, false);

    will_return(__wrap_gpio_set_state, 0);

    assert_return_code(si446x_reset(), 0);
}

static void si446x_power_up_test(void **state)
{
    uint8_t boot_options = generate_random(0, UINT8_MAX);
    uint8_t xtal_options = generate_random(0, UINT8_MAX);
    uint32_t xo_freq = generate_random(0, UINT32_MAX-1);

    uint8_t cmd[7] = {0};

    cmd[0] = SI446X_CMD_POWER_UP;
    cmd[1] = boot_options;
    cmd[2] = xtal_options;
    cmd[3] = (uint8_t)(xo_freq >> 24);
    cmd[4] = (uint8_t)((xo_freq >> 16) & 0xFF);
    cmd[5] = (uint8_t)((xo_freq >> 8) & 0xFF);
    cmd[6] = (uint8_t)(xo_freq & 0xFF);

    set_cmd(cmd, 7);

    assert_return_code(si446x_power_up(boot_options, xtal_options, xo_freq), 0);
}

static void si446x_configuration_init_test(void **state)
{
}

static void si446x_part_info_test(void **state)
{
    uint8_t cmd[10] = {0};

    cmd[0] = SI446X_CMD_PART_INFO;

    uint8_t res[10] = {0};

    uint16_t i = 0;
    for(i=0; i<9; i++)
    {
        res[i] = generate_random(0, UINT8_MAX);
    }

    get_cmd(cmd, 1, res, 9);

    si446x_part_info_t part_info = {0};

    assert_return_code(si446x_part_info(&part_info), 0);

    assert_int_equal(part_info.chiprev, res[1]);
    assert_int_equal(part_info.part, ((uint16_t)res[2] << 8) | res[3]);
    assert_int_equal(part_info.pbuild, res[4]);
    assert_int_equal(part_info.id, ((uint16_t)res[5] << 8) | res[6]);
    assert_int_equal(part_info.customer, res[7]);
    assert_int_equal(part_info.romid, res[8]);
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
    uint32_t timeout_ms = generate_random(1, 1000);

    uint8_t cmd[2] = {SI446X_CMD_READ_CMD_BUF, SI446X_CMD_NOP};

    expect_value(__wrap_spi_transfer, port, SI446X_SPI_PORT);
    expect_value(__wrap_spi_transfer, cs, SI446X_SPI_CS);
    expect_memory(__wrap_spi_transfer, wd, (void*)cmd, 2);
    expect_value(__wrap_spi_transfer, len, 2);

    will_return(__wrap_spi_transfer, 0);
    will_return(__wrap_spi_transfer, SI446X_CMD_COMPLETE);

    will_return(__wrap_spi_transfer, 0);

    assert_return_code(si446x_check_cts(timeout_ms), 0);
}

static void si446x_set_cmd_test(void **state)
{
    uint8_t cmd[256] = {UINT8_MAX};

    cmd[0] = SI446X_CMD_READ_CMD_BUF;
    cmd[1] = SI446X_CMD_NOP;

    expect_value(__wrap_spi_transfer, port, SI446X_SPI_PORT);
    expect_value(__wrap_spi_transfer, cs, SI446X_SPI_CS);
    expect_memory(__wrap_spi_transfer, wd, (void*)cmd, 2);
    expect_value(__wrap_spi_transfer, len, 2);

    will_return(__wrap_spi_transfer, 0);
    will_return(__wrap_spi_transfer, SI446X_CMD_COMPLETE);

    will_return(__wrap_spi_transfer, 0);

    uint16_t cmd_len = generate_random(1, UINT8_MAX);

    uint16_t i = 0;
    for(i=0; i<cmd_len; i++)
    {
        cmd[i] = generate_random(0, UINT8_MAX);
    }

    expect_value(__wrap_spi_write, port, SI446X_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SI446X_SPI_CS);
    expect_memory(__wrap_spi_write, data, (void*)cmd, cmd_len);
    expect_value(__wrap_spi_write, len, cmd_len);

    will_return(__wrap_spi_write, 0);

    assert_return_code(si446x_set_cmd(cmd, cmd_len), 0);
}

static void si446x_get_cmd_test(void **state)
{
    check_cts();

    uint8_t cmd[256] = {SI446X_CMD_NOP};
    uint16_t cmd_len = generate_random(1, UINT8_MAX);

    uint16_t i = 0;
    for(i=0; i<cmd_len; i++)
    {
        cmd[i] = generate_random(0, UINT8_MAX);
    }

    expect_value(__wrap_spi_write, port, SI446X_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SI446X_SPI_CS);
    expect_memory(__wrap_spi_write, data, (void*)cmd, cmd_len);
    expect_value(__wrap_spi_write, len, cmd_len);

    will_return(__wrap_spi_write, 0);

    check_cts();

    uint8_t cmd2[256] = {SI446X_CMD_NOP};
    cmd2[0] = SI446X_CMD_READ_CMD_BUF;

    uint8_t res[256] = {UINT8_MAX};
    uint16_t res_len = generate_random(1, UINT8_MAX);

    for(i=0; i<res_len; i++)
    {
        res[i] = generate_random(0, UINT8_MAX);
    }

    expect_value(__wrap_spi_transfer, port, SI446X_SPI_PORT);
    expect_value(__wrap_spi_transfer, cs, SI446X_SPI_CS);
    expect_memory(__wrap_spi_transfer, wd, (void*)cmd2, res_len+1);
    expect_value(__wrap_spi_transfer, len, res_len+1);

    for(i=0; i<res_len+1; i++)
    {
        will_return(__wrap_spi_transfer, res[i]);
    }

    will_return(__wrap_spi_transfer, 0);

    assert_return_code(si446x_get_cmd(cmd, cmd_len, res, res_len), 0);
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

unsigned int generate_random(unsigned int l, unsigned int r)
{
    return (rand() % (r - l + 1)) + l;
}

void check_cts(void)
{
    uint8_t cmd[2] = {SI446X_CMD_READ_CMD_BUF, SI446X_CMD_NOP};

    expect_value(__wrap_spi_transfer, port, SI446X_SPI_PORT);
    expect_value(__wrap_spi_transfer, cs, SI446X_SPI_CS);
    expect_memory(__wrap_spi_transfer, wd, (void*)cmd, 2);
    expect_value(__wrap_spi_transfer, len, 2);

    will_return(__wrap_spi_transfer, 0);
    will_return(__wrap_spi_transfer, SI446X_CMD_COMPLETE);

    will_return(__wrap_spi_transfer, 0);
}

void set_cmd(uint8_t *cmd, uint16_t cmd_len)
{
    check_cts();

    expect_value(__wrap_spi_write, port, SI446X_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SI446X_SPI_CS);
    expect_memory(__wrap_spi_write, data, (void*)cmd, cmd_len);
    expect_value(__wrap_spi_write, len, cmd_len);

    will_return(__wrap_spi_write, 0);
}

void get_cmd(uint8_t *cmd, uint16_t cmd_len, uint8_t *res, uint16_t data_len)
{
    check_cts();

    expect_value(__wrap_spi_write, port, SI446X_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SI446X_SPI_CS);
    expect_memory(__wrap_spi_write, data, (void*)cmd, cmd_len);
    expect_value(__wrap_spi_write, len, cmd_len);

    will_return(__wrap_spi_write, 0);

    check_cts();

    uint8_t cmd2[256] = {SI446X_CMD_NOP};
    cmd2[0] = SI446X_CMD_READ_CMD_BUF;

    uint16_t i = 0;
    for(i=0; i<data_len; i++)
    {
        res[i] = generate_random(0, UINT8_MAX);
    }

    expect_value(__wrap_spi_transfer, port, SI446X_SPI_PORT);
    expect_value(__wrap_spi_transfer, cs, SI446X_SPI_CS);
    expect_memory(__wrap_spi_transfer, wd, (void*)cmd2, data_len+1);
    expect_value(__wrap_spi_transfer, len, data_len+1);

    for(i=0; i<data_len+1; i++)
    {
        will_return(__wrap_spi_transfer, res[i]);
    }

    will_return(__wrap_spi_transfer, 0);
}

/** \} End of si446x_test group */
