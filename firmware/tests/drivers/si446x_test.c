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
 * \version 0.7.16
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
#include <string.h>

#include <config/radio_config_Si4463.h>
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
    uint8_t boot_options    = generate_random(0, UINT8_MAX);
    uint8_t xtal_options    = generate_random(0, UINT8_MAX);
    uint32_t xo_freq        = generate_random(0, UINT32_MAX-1);

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
    uint8_t p_set_prop_cmd[] = RADIO_CONFIGURATION_DATA_ARRAY;
    uint16_t p_set_prop_cmd_len = sizeof(p_set_prop_cmd)-1;

    uint8_t cmd[20] = {SI446X_CMD_NOP};
    uint8_t cmd_len = 0;
    uint16_t cmd_start_pos = 0;

    while(cmd_start_pos < p_set_prop_cmd_len)
    {
        cmd_len = p_set_prop_cmd[cmd_start_pos];

        memcpy(cmd, p_set_prop_cmd + cmd_start_pos + 1, cmd_len);

        set_cmd(cmd, cmd_len);

        cmd_start_pos += cmd_len + 1;
    }

    assert_return_code(si446x_configuration_init(p_set_prop_cmd, p_set_prop_cmd_len), 0);
}

static void si446x_part_info_test(void **state)
{
    uint8_t cmd[10] = {0};

    cmd[0] = SI446X_CMD_PART_INFO;

    uint8_t res[10] = {0};

    get_cmd(cmd, 1, res, 9);

    si446x_part_info_t part_info = {0};

    assert_return_code(si446x_part_info(&part_info), 0);

    assert_int_equal(part_info.chiprev,     res[1]);
    assert_int_equal(part_info.part,        ((uint16_t)res[2] << 8) | res[3]);
    assert_int_equal(part_info.pbuild,      res[4]);
    assert_int_equal(part_info.id,          ((uint16_t)res[5] << 8) | res[6]);
    assert_int_equal(part_info.customer,    res[7]);
    assert_int_equal(part_info.romid,       res[8]);
}

static void si446x_start_tx_test(void **state)
{
    uint8_t channel     = generate_random(0, UINT8_MAX);
    uint8_t condition   = generate_random(0, UINT8_MAX);
    uint16_t tx_len     = generate_random(1, UINT16_MAX);

    uint8_t cmd[5] = {SI446X_CMD_NOP};

    cmd[0] = SI446X_CMD_START_TX;
    cmd[1] = channel;
    cmd[2] = condition;
    cmd[3] = (uint8_t)(tx_len >> 8);
    cmd[4] = (uint8_t)(tx_len & 0xFF);

    set_cmd(cmd, 5);

    assert_return_code(si446x_start_tx(channel, condition, tx_len), 0);
}

static void si446x_start_rx_test(void **state)
{
    uint8_t channel     = generate_random(0, UINT8_MAX);
    uint8_t condition   = generate_random(0, UINT8_MAX);
    uint16_t rx_len     = generate_random(1, UINT16_MAX);
    uint8_t next_state1 = generate_random(0, UINT8_MAX);
    uint8_t next_state2 = generate_random(0, UINT8_MAX);
    uint8_t next_state3 = generate_random(0, UINT8_MAX);

    uint8_t cmd[8] = {SI446X_CMD_NOP};

    cmd[0] = SI446X_CMD_START_RX;
    cmd[1] = channel;
    cmd[2] = condition;
    cmd[3] = (uint8_t)(rx_len >> 8);
    cmd[4] = (uint8_t)(rx_len & 0xFF);
    cmd[5] = next_state1;
    cmd[6] = next_state2;
    cmd[7] = next_state3;

    set_cmd(cmd, 8);

    assert_return_code(si446x_start_rx(channel, condition, rx_len, next_state1, next_state2, next_state3), 0);
}

static void si446x_get_int_status_test(void **state)
{
    uint8_t ph_clr_pend     = generate_random(0, UINT8_MAX);
    uint8_t modem_clr_pend  = generate_random(0, UINT8_MAX);
    uint8_t chip_clr_pend   = generate_random(0, UINT8_MAX);

    check_cts();

    uint8_t cmd[10] = {SI446X_CMD_NOP};
    uint8_t result[10] = {0};

    cmd[0] = SI446X_CMD_GET_INT_STATUS;
    cmd[1] = ph_clr_pend;
    cmd[2] = modem_clr_pend;
    cmd[3] = chip_clr_pend;

    expect_value(__wrap_spi_write, port, SI446X_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SI446X_SPI_CS);
    expect_memory(__wrap_spi_write, data, (void*)cmd, 2);
    expect_value(__wrap_spi_write, len, 2);

    will_return(__wrap_spi_write, 0);

    check_cts();

    cmd[0] = SI446X_CMD_READ_CMD_BUF;
    cmd[1] = SI446X_CMD_NOP;
    cmd[2] = SI446X_CMD_NOP;
    cmd[3] = SI446X_CMD_NOP;

    expect_value(__wrap_spi_transfer, port, SI446X_SPI_PORT);
    expect_value(__wrap_spi_transfer, cs, SI446X_SPI_CS);
    expect_memory(__wrap_spi_transfer, wd, (void*)cmd, 10);
    expect_value(__wrap_spi_transfer, len, 10);

    uint16_t i = 0;
    for(i=0; i<10; i++)
    {
        result[i] = generate_random(0, UINT8_MAX);
        will_return(__wrap_spi_transfer, result[i]);
    }

    will_return(__wrap_spi_transfer, 0);

    si446x_int_status_t int_status = {0};

    assert_return_code(si446x_get_int_status(ph_clr_pend, modem_clr_pend, chip_clr_pend, &int_status), 0);

    assert_int_equal(int_status.int_pend,       result[2]);
    assert_int_equal(int_status.int_status,     result[3]);
    assert_int_equal(int_status.ph_pend,        result[4]);
    assert_int_equal(int_status.ph_status,      result[5]);
    assert_int_equal(int_status.modem_pend,     result[6]);
    assert_int_equal(int_status.modem_status,   result[7]);
    assert_int_equal(int_status.chip_pend,      result[8]);
    assert_int_equal(int_status.chip_status,    result[9]);
}

static void si446x_gpio_pin_cfg_test(void **state)
{
    uint8_t gpio0       = generate_random(0, UINT8_MAX);
    uint8_t gpio1       = generate_random(0, UINT8_MAX);
    uint8_t gpio2       = generate_random(0, UINT8_MAX);
    uint8_t gpio3       = generate_random(0, UINT8_MAX);
    uint8_t nirq        = generate_random(0, UINT8_MAX);
    uint8_t sdo         = generate_random(0, UINT8_MAX);
    uint8_t gen_config  = generate_random(0, UINT8_MAX);

    uint8_t cmd[8] = {SI446X_CMD_NOP};

    cmd[0] = SI446X_CMD_GPIO_PIN_CFG;
    cmd[1] = gpio0;
    cmd[2] = gpio1;
    cmd[3] = gpio2;
    cmd[4] = gpio3;
    cmd[5] = nirq;
    cmd[6] = sdo;
    cmd[7] = gen_config;

    set_cmd(cmd, 8);

    assert_return_code(si446x_gpio_pin_cfg(gpio0, gpio1, gpio2, gpio3, nirq, sdo, gen_config), 0);
}

static void si446x_set_property_test(void **state)
{
    uint8_t group       = generate_random(0, UINT8_MAX);
    uint8_t num_props   = generate_random(1, UINT8_MAX);
    uint8_t start_prop  = generate_random(0, UINT8_MAX);

    uint8_t cmd[256] = {0};

    cmd[0] = SI446X_CMD_SET_PROPERTY;
    cmd[1] = group;
    cmd[2] = num_props;
    cmd[3] = start_prop;

    uint8_t data[256] = {UINT8_MAX};
    uint16_t len = generate_random(1, 16-4);

    uint16_t i = 0;
    for(i=0; i<len; i++)
    {
        data[i] = generate_random(0, UINT8_MAX);
        cmd[i+4] = data[i];
    }

    set_cmd(cmd, 4+len);

    assert_return_code(si446x_set_property(group, num_props, start_prop, data, len), 0);
}

static void si446x_change_state_test(void **state)
{
    uint8_t next_state = 0;
    for(next_state=0; next_state<UINT8_MAX; next_state++)
    {
        switch(next_state)
        {
            case SI446X_NO_CHANGE_STATE:        break;
            case SI446X_SLEEP_STATE:            break;
            case SI446X_SPI_ACTIVE_STATE:       break;
            case SI446X_READY_STATE:            break;
            case SI446X_ANOTHER_READY_STATE:    break;
            case SI446X_TUNE_STATE_FOR_TX:      break;
            case SI446X_TUNE_STATE_FOR_RX:      break;
            case SI446X_TX_STATE:               break;
            case SI446X_RX_STATE:               break;
            default:
                assert_int_equal(si446x_change_state(next_state), -1);
                continue;
        }

        uint8_t cmd[2] = {SI446X_CMD_NOP};

        cmd[0] = SI446X_CMD_CHANGE_STATE;
        cmd[1] = next_state;

        set_cmd(cmd, 2);

        assert_return_code(si446x_change_state(next_state), 0);
    }
}

static void si446x_nop_test(void **state)
{
    uint8_t cmd = SI446X_CMD_NOP;

    expect_value(__wrap_spi_write, port, SI446X_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SI446X_SPI_CS);
    expect_memory(__wrap_spi_write, data, (void*)&cmd, 1);
    expect_value(__wrap_spi_write, len, 1);

    will_return(__wrap_spi_write, 0);

    assert_return_code(si446x_nop(), 0);
}

static void si446x_fifo_info_test(void **state)
{
    bool rst_rx = (bool)generate_random(0, 1);
    bool rst_tx = (bool)generate_random(0, 1);

    uint8_t cmd[4] = {SI446X_CMD_NOP};
    uint8_t result[4] = {0};

    cmd[0] = SI446X_CMD_FIFO_INFO;
    cmd[1] = ((uint8_t)rst_rx << 1) | (uint8_t)rst_tx;

    get_cmd(cmd, 2, result, 3);

    si446x_fifo_info_t fifo_info = {0};

    assert_return_code(si446x_fifo_info(rst_rx, rst_tx, &fifo_info), 0);

    assert_int_equal(fifo_info.rx_fifo_count, result[2]);
    assert_int_equal(fifo_info.tx_fifo_space, result[3]);
}

static void si446x_write_tx_fifo_test(void **state)
{
    uint8_t cmd[129] = {SI446X_CMD_NOP};
    uint8_t p_tx_data[129] = {UINT8_MAX};
    uint8_t num_bytes = generate_random(1, 129);

    uint8_t i = 0;
    for(i=0; i<num_bytes; i++)
    {
        p_tx_data[i] = generate_random(0, UINT8_MAX);
        cmd[i+1] = p_tx_data[i];
    }

    cmd[0] = SI446X_CMD_WRITE_TX_FIFO;

    set_cmd(cmd, 1+num_bytes);

    assert_return_code(si446x_write_tx_fifo(num_bytes, p_tx_data), 0);
}

static void si446x_read_rx_fifo_test(void **state)
{
    uint8_t res[129] = {UINT8_MAX};
    uint8_t num_bytes = generate_random(1, 129);

    check_cts();

    uint8_t cmd[129] = {SI446X_CMD_NOP};

    cmd[0] = SI446X_CMD_READ_RX_FIFO;

    expect_value(__wrap_spi_transfer, port, SI446X_SPI_PORT);
    expect_value(__wrap_spi_transfer, cs, SI446X_SPI_CS);
    expect_memory(__wrap_spi_transfer, wd, (void*)cmd, 1+num_bytes);
    expect_value(__wrap_spi_transfer, len, 1+num_bytes);

    uint16_t i = 0;
    for(i=0; i<1+num_bytes; i++)
    {
        res[i] = generate_random(0, UINT8_MAX);
        will_return(__wrap_spi_transfer, res[i]);
    }

    will_return(__wrap_spi_transfer, 0);

    uint8_t p_rx_data[129] = {UINT8_MAX};

    assert_return_code(si446x_read_rx_fifo(num_bytes, p_rx_data), 0);

    assert_memory_equal((void*)p_rx_data, (void*)res, 1+num_bytes);
}

static void si446x_get_property_test(void **state)
{
    uint8_t group       = generate_random(0, UINT8_MAX);
    uint8_t num_props   = generate_random(1, 16);
    uint8_t start_prop  = generate_random(0, UINT8_MAX);

    uint8_t cmd[256] = {SI446X_CMD_NOP};
    uint8_t res[256] = {0};

    cmd[0] = SI446X_CMD_GET_PROPERTY;
    cmd[1] = group;
    cmd[2] = num_props;
    cmd[3] = start_prop;

    get_cmd(cmd, 4, res, 1+16);

    uint8_t data[256] = {0};

    assert_return_code(si446x_get_property(group, num_props, start_prop, data), 0);

    assert_memory_equal((void*)(res+2), (void*)data, num_props);
}

static void si446x_func_info_test(void **state)
{
    uint8_t cmd = SI446X_CMD_FUNC_INFO;
    uint8_t res[256] = {UINT8_MAX};

    get_cmd(&cmd, 1, res, 7);

    si446x_func_info_t func_info = {0};

    assert_return_code(si446x_func_info(&func_info), 0);

    assert_int_equal(func_info.revext,      res[2]);
    assert_int_equal(func_info.revbranch,   res[3]);
    assert_int_equal(func_info.revint,      res[4]);
    assert_int_equal(func_info.patch,       ((uint16_t)res[5] << 8) | res[6]);
    assert_int_equal(func_info.func,        res[7]);
}

static void si446x_frr_a_read_test(void **state)
{
    uint8_t resp_byte_count = generate_random(1, UINT8_MAX);

    check_cts();

    uint8_t cmd[2] = {SI446X_CMD_NOP};
    uint8_t res[2] = {UINT8_MAX};
    cmd[0] = SI446X_CMD_FRR_A_READ;

    expect_value(__wrap_spi_transfer, port, SI446X_SPI_PORT);
    expect_value(__wrap_spi_transfer, cs, SI446X_SPI_CS);
    expect_memory(__wrap_spi_transfer, wd, (void*)cmd, 2);
    expect_value(__wrap_spi_transfer, len, 2);

    res[0] = generate_random(0, UINT8_MAX);
    res[1] = generate_random(0, UINT8_MAX);

    will_return(__wrap_spi_transfer, res[0]);
    will_return(__wrap_spi_transfer, res[1]);

    will_return(__wrap_spi_transfer, 0);

    uint8_t frr_a_val[2] = {UINT8_MAX};

    assert_return_code(si446x_frr_a_read(resp_byte_count, frr_a_val), 0);

    assert_int_equal(res[1], *frr_a_val);
}

static void si446x_frr_b_read_test(void **state)
{
    uint8_t resp_byte_count = generate_random(1, UINT8_MAX);

    check_cts();

    uint8_t cmd[2] = {SI446X_CMD_NOP};
    uint8_t res[2] = {UINT8_MAX};
    cmd[0] = SI446X_CMD_FRR_B_READ;

    expect_value(__wrap_spi_transfer, port, SI446X_SPI_PORT);
    expect_value(__wrap_spi_transfer, cs, SI446X_SPI_CS);
    expect_memory(__wrap_spi_transfer, wd, (void*)cmd, 2);
    expect_value(__wrap_spi_transfer, len, 2);

    res[0] = generate_random(0, UINT8_MAX);
    res[1] = generate_random(0, UINT8_MAX);

    will_return(__wrap_spi_transfer, res[0]);
    will_return(__wrap_spi_transfer, res[1]);

    will_return(__wrap_spi_transfer, 0);

    uint8_t frr_b_val[2] = {UINT8_MAX};

    assert_return_code(si446x_frr_b_read(resp_byte_count, frr_b_val), 0);

    assert_int_equal(res[1], *frr_b_val);
}

static void si446x_frr_c_read_test(void **state)
{
    uint8_t resp_byte_count = generate_random(1, UINT8_MAX);

    check_cts();

    uint8_t cmd[2] = {SI446X_CMD_NOP};
    uint8_t res[2] = {UINT8_MAX};
    cmd[0] = SI446X_CMD_FRR_C_READ;

    expect_value(__wrap_spi_transfer, port, SI446X_SPI_PORT);
    expect_value(__wrap_spi_transfer, cs, SI446X_SPI_CS);
    expect_memory(__wrap_spi_transfer, wd, (void*)cmd, 2);
    expect_value(__wrap_spi_transfer, len, 2);

    res[0] = generate_random(0, UINT8_MAX);
    res[1] = generate_random(0, UINT8_MAX);

    will_return(__wrap_spi_transfer, res[0]);
    will_return(__wrap_spi_transfer, res[1]);

    will_return(__wrap_spi_transfer, 0);

    uint8_t frr_c_val[2] = {UINT8_MAX};

    assert_return_code(si446x_frr_c_read(resp_byte_count, frr_c_val), 0);

    assert_int_equal(res[1], *frr_c_val);
}

static void si446x_frr_d_read_test(void **state)
{
    uint8_t resp_byte_count = generate_random(1, UINT8_MAX);

    check_cts();

    uint8_t cmd[2] = {SI446X_CMD_NOP};
    uint8_t res[2] = {UINT8_MAX};
    cmd[0] = SI446X_CMD_FRR_D_READ;

    expect_value(__wrap_spi_transfer, port, SI446X_SPI_PORT);
    expect_value(__wrap_spi_transfer, cs, SI446X_SPI_CS);
    expect_memory(__wrap_spi_transfer, wd, (void*)cmd, 2);
    expect_value(__wrap_spi_transfer, len, 2);

    res[0] = generate_random(0, UINT8_MAX);
    res[1] = generate_random(0, UINT8_MAX);

    will_return(__wrap_spi_transfer, res[0]);
    will_return(__wrap_spi_transfer, res[1]);

    will_return(__wrap_spi_transfer, 0);

    uint8_t frr_d_val[2] = {UINT8_MAX};

    assert_return_code(si446x_frr_d_read(resp_byte_count, frr_d_val), 0);

    assert_int_equal(res[1], *frr_d_val);
}

static void si446x_get_adc_reading_test(void **state)
{
    uint8_t temp_en         = generate_random(0, UINT8_MAX);
    bool bat_volt_en        = (bool)generate_random(0, 1);
    bool adc_gpio_en        = (bool)generate_random(0, 1);
    uint8_t adc_gpio_pin    = generate_random(0, UINT8_MAX);

    uint8_t cmd[10] = {SI446X_CMD_NOP};
    uint8_t res[10] = {UINT8_MAX};

    cmd[0] = SI446X_CMD_GET_ADC_READING;
    cmd[1] = ((uint8_t)temp_en << 4) | ((uint8_t)bat_volt_en << 3) | ((uint8_t)adc_gpio_en << 2) | (adc_gpio_pin & 0x03);

    get_cmd(cmd, 2, res, 9);

    si446x_adc_reading_t adc_reading = {0};

    assert_return_code(si446x_get_adc_reading(temp_en, bat_volt_en, adc_gpio_en, adc_gpio_pin, &adc_reading), 0);

    assert_int_equal(adc_reading.gpio_adc,          ((uint16_t)res[2] << 8) | res[3]);
    assert_int_equal(adc_reading.battery_adc,       ((uint16_t)res[4] << 8) | res[5]);
    assert_int_equal(adc_reading.temp_adc,          ((uint16_t)res[6] << 8) | res[7]);
    assert_int_equal(adc_reading.temp_slope,        res[8]);
    assert_int_equal(adc_reading.temp_intercept,    res[9]);
}

static void si446x_get_packet_info_test(void **state)
{
    uint8_t field_number_mask   = generate_random(0, UINT8_MAX);
    uint16_t len                = generate_random(1, UINT16_MAX);
    int16_t diff_len            = generate_random(0, INT16_MAX);

    uint8_t cmd[6] = {SI446X_CMD_NOP};
    uint8_t res[6] = {UINT8_MAX};

    cmd[0] = SI446X_CMD_PACKET_INFO;
    cmd[1] = field_number_mask;
    cmd[2] = (uint8_t)(len >> 8);
    cmd[3] = (uint8_t)(len & 0xFF);
    cmd[4] = (uint8_t)(diff_len >> 8);
    cmd[5] = (uint8_t)(diff_len & 0xFF);

    get_cmd(cmd, 6, res, 3);

    uint16_t last_len = UINT16_MAX;

    assert_return_code(si446x_get_packet_info(field_number_mask, len, diff_len, &last_len), 0);

    assert_int_equal(last_len, ((uint16_t)res[2] << 8) | res[3]);
}

static void si446x_get_ph_status_test(void **state)
{
    uint8_t cmd[4] = {SI446X_CMD_NOP};
    uint8_t res[4] = {UINT8_MAX};

    cmd[0] = SI446X_CMD_GET_PH_STATUS;

    res[0] = generate_random(0, UINT8_MAX);
    res[1] = generate_random(0, UINT8_MAX);
    res[2] = generate_random(0, UINT8_MAX);
    res[3] = generate_random(0, UINT8_MAX);

    get_cmd(cmd, 1, res, 3);

    si446x_ph_status_t ph_status = {0};

    assert_return_code(si446x_get_ph_status(&ph_status), 0);

    assert_int_equal(ph_status.ph_pend,     res[2]);
    assert_int_equal(ph_status.ph_status,   res[3]);
}

static void si446x_get_modem_status_test(void **state)
{
    uint8_t modem_clr_pend = generate_random(0, UINT8_MAX);

    uint8_t cmd[10] = {SI446X_CMD_NOP};
    uint8_t res[10] = {UINT8_MAX};

    cmd[0] = SI446X_CMD_GET_MODEM_STATUS;

    get_cmd(cmd, 1, res, 9);

    si446x_modem_status_t modem_status = {0};

    assert_return_code(si446x_get_modem_status(modem_clr_pend, &modem_status), 0);

    assert_int_equal(modem_status.modem_pend,       res[2]);
    assert_int_equal(modem_status.modem_status,     res[3]);
    assert_int_equal(modem_status.curr_rssi,        res[4]);
    assert_int_equal(modem_status.latch_rssi,       res[5]);
    assert_int_equal(modem_status.ant1_rssi,        res[6]);
    assert_int_equal(modem_status.ant2_rssi,        res[7]);
    assert_int_equal(modem_status.afc_freq_offset,  ((uint16_t)res[8] << 8) | res[9]);
}

static void si446x_get_chip_status_test(void **state)
{
    uint8_t chip_clr_pend = generate_random(0, UINT8_MAX);

    uint8_t cmd[5] = {SI446X_CMD_NOP};
    uint8_t res[5] = {UINT8_MAX};

    cmd[0] = SI446X_CMD_GET_CHIP_STATUS;

    res[0] = generate_random(0, UINT8_MAX);
    res[1] = generate_random(0, UINT8_MAX);
    res[2] = generate_random(0, UINT8_MAX);
    res[3] = generate_random(0, UINT8_MAX);

    get_cmd(cmd, 1, res, 4);

    si446x_chip_status_t chip_status = {0};

    assert_return_code(si446x_get_chip_status(chip_clr_pend, &chip_status), 0);

    assert_int_equal(chip_status.chip_pend,         res[2]);
    assert_int_equal(chip_status.chip_status,       res[3]);
    assert_int_equal(chip_status.chip_err_status,   res[4]);
}

static void si446x_ircal_test(void **state)
{
    uint8_t searching_step_size = generate_random(0, UINT8_MAX);
    uint8_t searching_rssi_avg  = generate_random(0, UINT8_MAX);
    uint8_t rx_chain_setting1   = generate_random(0, UINT8_MAX);
    uint8_t rx_chain_setting2   = generate_random(0, UINT8_MAX);

    uint8_t cmd[5] = {SI446X_CMD_NOP};

    cmd[0] = SI446X_CMD_IRCAL;
    cmd[1] = searching_step_size;
    cmd[2] = searching_rssi_avg;
    cmd[3] = rx_chain_setting1;
    cmd[4] = rx_chain_setting2;

    set_cmd(cmd, 5);

    assert_return_code(si446x_ircal(searching_step_size, searching_rssi_avg, rx_chain_setting1, rx_chain_setting2), 0);
}

static void si446x_protocol_cfg_test(void **state)
{
    uint8_t protocol = generate_random(0, UINT8_MAX);

    uint8_t cmd[2] = {SI446X_CMD_NOP};

    cmd[0] = SI446X_CMD_PROTOCOL_CFG;
    cmd[1] = protocol;

    set_cmd(cmd, 2);

    assert_return_code(si446x_protocol_cfg(protocol), 0);
}

static void si446x_request_device_state_test(void **state)
{
    uint8_t cmd[4] = {SI446X_CMD_NOP};
    uint8_t res[4] = {UINT8_MAX};

    cmd[0] = SI446X_CMD_REQUEST_DEVICE_STATE;

    res[0] = generate_random(0, UINT8_MAX);
    res[1] = generate_random(0, UINT8_MAX);
    res[2] = generate_random(0, UINT8_MAX);
    res[3] = generate_random(0, UINT8_MAX);

    get_cmd(cmd, 1, res, 3);

    si446x_device_state_t dev_state = {0};

    assert_return_code(si446x_request_device_state(&dev_state), 0);

    assert_int_equal(dev_state.curr_state,      res[2]);
    assert_int_equal(dev_state.current_channel, res[3]);
}

static void si446x_rx_hop_test(void **state)
{
    uint8_t inte        = generate_random(0, UINT8_MAX);
    uint8_t frac2       = generate_random(0, UINT8_MAX);
    uint8_t frac1       = generate_random(0, UINT8_MAX);
    uint8_t frac0       = generate_random(0, UINT8_MAX);
    uint8_t vco_cnt1    = generate_random(0, UINT8_MAX);
    uint8_t vco_cnt0    = generate_random(0, UINT8_MAX);

    uint8_t cmd[7] = {SI446X_CMD_NOP};

    cmd[0] = SI446X_CMD_RX_HOP;
    cmd[1] = inte;
    cmd[2] = frac2;
    cmd[3] = frac1;
    cmd[4] = frac0;
    cmd[5] = vco_cnt1;
    cmd[6] = vco_cnt0;

    set_cmd(cmd, 7);

    assert_return_code(si446x_rx_hop(inte, frac2, frac1, frac0, vco_cnt1, vco_cnt0), 0);
}

static void si446x_shutdown_test(void **state)
{
    expect_value(__wrap_gpio_set_state, pin, SI446X_GPIO_SDN);
    expect_value(__wrap_gpio_set_state, level, true);

    will_return(__wrap_gpio_set_state, 0);

    assert_return_code(si446x_shutdown(), 0);
}

static void si446x_power_on_test(void **state)
{
    expect_value(__wrap_gpio_set_state, pin, SI446X_GPIO_SDN);
    expect_value(__wrap_gpio_set_state, level, false);

    will_return(__wrap_gpio_set_state, 0);

    assert_return_code(si446x_power_on(), 0);
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
    expect_value(__wrap_spi_init, port, SI446X_SPI_PORT);
    expect_value(__wrap_spi_init, config.speed_hz, 1000000UL);
    expect_value(__wrap_spi_init, config.mode, SPI_MODE_1);

    will_return(__wrap_spi_init, 0);

    assert_return_code(si446x_spi_init(), 0);
}

static void si446x_spi_transfer_test(void **state)
{
    uint8_t wdata[256] = {UINT8_MAX};
    uint8_t rdata[256] = {UINT8_MAX};
    uint16_t data_len = generate_random(1, UINT8_MAX);

    uint16_t i = 0;
    for(i=0; i<data_len; i++)
    {
        wdata[i] = generate_random(0, UINT8_MAX);
        rdata[i] = generate_random(0, UINT8_MAX);
        will_return(__wrap_spi_transfer, rdata[i]);
    }

    expect_value(__wrap_spi_transfer, port, SI446X_SPI_PORT);
    expect_value(__wrap_spi_transfer, cs, SI446X_SPI_CS);
    expect_memory(__wrap_spi_transfer, wd, (void*)wdata, data_len);
    expect_value(__wrap_spi_transfer, len, data_len);

    will_return(__wrap_spi_transfer, 0);

    assert_return_code(si446x_spi_transfer(wdata, rdata, data_len), 0);
}

static void si446x_spi_write_byte_test(void **state)
{
    uint8_t byte = generate_random(0, UINT8_MAX);

    expect_value(__wrap_spi_write, port, SI446X_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SI446X_SPI_CS);
    expect_memory(__wrap_spi_write, data, (void*)&byte, 1);
    expect_value(__wrap_spi_write, len, 1);

    will_return(__wrap_spi_write, 0);

    assert_return_code(si446x_spi_write_byte(byte), 0);
}

static void si446x_spi_write_test(void **state)
{
    uint16_t data_len = generate_random(1, UINT8_MAX);
    uint8_t data[256] = {UINT8_MAX};

    uint16_t i = 0;
    for(i=0; i<data_len; i++)
    {
        data[i] = generate_random(0, UINT8_MAX);
    }

    expect_value(__wrap_spi_write, port, SI446X_SPI_PORT);
    expect_value(__wrap_spi_write, cs, SI446X_SPI_CS);
    expect_memory(__wrap_spi_write, data, (void*)data, data_len);
    expect_value(__wrap_spi_write, len, data_len);

    will_return(__wrap_spi_write, 0);

    assert_return_code(si446x_spi_write(data, data_len), 0);
}

static void si446x_spi_read_test(void **state)
{
    uint16_t data_len = generate_random(1, UINT8_MAX);
    uint8_t data[256] = {UINT8_MAX};

    expect_value(__wrap_spi_read, port, SI446X_SPI_PORT);
    expect_value(__wrap_spi_read, cs, SI446X_SPI_CS);
    expect_value(__wrap_spi_read, len, data_len);

    uint16_t i = 0;
    for(i=0; i<data_len; i++)
    {
        data[i] = generate_random(0, UINT8_MAX);
        will_return(__wrap_spi_read, data[i]);
    }

    will_return(__wrap_spi_read, 0);

    assert_return_code(si446x_spi_read(data, data_len), 0);
}

static void si446x_gpio_init_test(void **state)
{
    /* SDN pin */
    expect_value(__wrap_gpio_init, pin, SI446X_GPIO_SDN);
    expect_value(__wrap_gpio_init, config.mode, GPIO_MODE_OUTPUT);

    will_return(__wrap_gpio_init, 0);

    /* nIRQ pin */
    expect_value(__wrap_gpio_init, pin, SI446X_GPIO_NIRQ);
    expect_value(__wrap_gpio_init, config.mode, GPIO_MODE_INPUT);

    will_return(__wrap_gpio_init, 0);

    assert_return_code(si446x_gpio_init(), 0);
}

static void si446x_gpio_write_sdn_test(void **state)
{
    bool sdn_state = (bool)generate_random(0, 1);

    expect_value(__wrap_gpio_set_state, pin, SI446X_GPIO_SDN);
    expect_value(__wrap_gpio_set_state, level, sdn_state);

    will_return(__wrap_gpio_set_state, 0);

    assert_return_code(si446x_gpio_write_sdn(sdn_state), 0);
}

static void si446x_gpio_read_nirq_test(void **state)
{
    uint8_t res = 0;
    for(res=0; res<UINT8_MAX; res++)
    {
        expect_value(__wrap_gpio_get_state, pin, SI446X_GPIO_NIRQ);

        will_return(__wrap_gpio_get_state, res);

        switch(res)
        {
            case 0:     assert_int_equal(si446x_gpio_read_nirq(), 0);   break;
            case 1:     assert_int_equal(si446x_gpio_read_nirq(), 1);   break;
            default:    assert_int_equal(si446x_gpio_read_nirq(), -1);
        }
    }
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
