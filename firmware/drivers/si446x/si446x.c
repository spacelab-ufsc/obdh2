/*
 * si446x.c
 * 
 * Copyright (C) 2020, SpaceLab.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with OBDH 2.0. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Si446x driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.3.23
 * 
 * \date 01/06/2017
 * 
 * \addtogroup si446x
 * \{
 */

#include <drivers/gpio/gpio.h>

#include "si446x.h"

int si446x_reset(void)
{
    if (si446x_shutdown() != 0)
    {
        return -1;
    }

    si446x_delay_ms(10);

    if (si446x_power_on() != 0)
    {
        return -1;
    }

    return SI446X_SUCCESS;
}

int si446x_power_up(uint8_t boot_options, uint8_t xtal_options, uint32_t xo_freq)
{
    uint8_t cmd[7] = {SI446X_CMD_NOP};

    cmd[0] = SI446X_CMD_POWER_UP;
    cmd[1] = boot_options;
    cmd[2] = xtal_options;
    cmd[3] = (uint8_t)(xo_freq >> 24);
    cmd[4] = (uint8_t)((xo_freq >> 16) & 0xFF);
    cmd[5] = (uint8_t)((xo_freq >> 8) & 0xFF);
    cmd[6] = (uint8_t)(xo_freq & 0xFF);

    return si446x_set_cmd(cmd, 7);
}

int si446x_configuration_init(uint8_t *p_set_prop_cmd, uint16_t p_set_prop_cmd_len)
{
    uint8_t cmd[20] = {SI446X_CMD_NOP};
    uint8_t cmd_len = 0;
    uint16_t cmd_start_pos = 0;

    while(cmd_start_pos < p_set_prop_cmd_len)
    {
        cmd_len = p_set_prop_cmd[cmd_start_pos];

        memcpy(cmd, p_set_prop_cmd + cmd_start_pos + 1, cmd_len);

        si446x_set_cmd(cmd, cmd_len);

        cmd_start_pos += cmd_len + 1;
    }
}

int si446x_part_info(si446x_part_info_t *part_info)
{
    uint8_t cmd[10] = {SI446X_CMD_NOP};
    uint8_t result[10] = {0};

    cmd[0] = SI446X_CMD_PART_INFO;

    int error = si446x_get_cmd(cmd, 1, result, 9);

    if (error != SI446X_SUCCESS)
    {
        return error;
    }

    part_info->chiprev      = result[1];
    part_info->part         = ((uint16_t)result[2] << 8) | result[3];
    part_info->pbuild       = result[4];
    part_info->id           = ((uint16_t)result[5] << 8) | result[6];
    part_info->customer     = result[7];
    part_info->romid        = result[8];

    return SI446X_SUCCESS;
}

int si446x_start_tx(uint8_t channel, uint8_t condition, uint16_t tx_len)
{
    uint8_t cmd[5] = {SI446X_CMD_NOP};

    cmd[0] = SI446X_CMD_START_TX;
    cmd[1] = channel;
    cmd[2] = condition;
    cmd[3] = (uint8_t)(tx_len >> 8);
    cmd[4] = (uint8_t)(tx_len & 0xFF);

    return si446x_set_cmd(cmd, 5);
}

int si446x_start_rx(uint8_t channel, uint8_t condition, uint16_t rx_len, si446x_state_t next_state1, si446x_state_t next_state2, si446x_state_t next_state3)
{
    uint8_t cmd[8] = {SI446X_CMD_NOP};

    cmd[0] = SI446X_CMD_START_RX;
    cmd[1] = channel;
    cmd[2] = condition;
    cmd[3] = (uint8_t)(rx_len >> 8);
    cmd[4] = (uint8_t)(rx_len & 0xFF);
    cmd[5] = next_state1;
    cmd[6] = next_state2;
    cmd[7] = next_state3;

    return si446x_set_cmd(cmd, 8);
}

int si446x_get_int_status(uint8_t ph_clr_pend, uint8_t modem_clr_pend, uint8_t chip_clr_pend, si446x_int_status_t *int_status)
{
    if (!si446x_check_cts(100))
    {
        return SI446X_CTS_TIMEOUT;
    }

    uint8_t cmd[10] = {SI446X_CMD_NOP};
    uint8_t result[10] = {0};

    cmd[0] = SI446X_CMD_GET_INT_STATUS;
    cmd[1] = ph_clr_pend;
    cmd[2] = modem_clr_pend;
    cmd[3] = chip_clr_pend;

    /*  Writes the command */
    si446x_spi_write(cmd, 2);

    if (!si446x_check_cts(100))
    {
        return SI446X_CTS_TIMEOUT;
    }

    cmd[0] = SI446X_CMD_READ_CMD_BUF;
    cmd[1] = SI446X_CMD_NOP;
    cmd[2] = SI446X_CMD_NOP;
    cmd[3] = SI446X_CMD_NOP;

    /* Reads the result */
    si446x_spi_transfer(cmd, result, 10);

    int_status->int_pend        = result[2];
    int_status->int_status      = result[3];
    int_status->ph_pend         = result[4];
    int_status->ph_status       = result[5];
    int_status->modem_pend      = result[6];
    int_status->modem_status    = result[7];
    int_status->chip_pend       = result[8];
    int_status->chip_status     = result[9];

    return SI446X_SUCCESS;
}

int si446x_gpio_pin_cfg(uint8_t gpio0, uint8_t gpio1, uint8_t gpio2, uint8_t gpio3, uint8_t nirq, uint8_t sdo, uint8_t gen_config)
{
    uint8_t cmd[8] = {SI446X_CMD_NOP};

    cmd[0] = SI446X_CMD_GPIO_PIN_CFG;
    cmd[1] = gpio0;
    cmd[2] = gpio1;
    cmd[3] = gpio2;
    cmd[4] = gpio3;
    cmd[5] = nirq;
    cmd[6] = sdo;
    cmd[7] = gen_config;

    return si446x_set_cmd(cmd, 8);
}

int si446x_set_property(uint8_t group, uint8_t num_props, uint8_t start_prop, uint8_t *data, uint16_t len)
{
    uint8_t cmd[16] = {SI446X_CMD_NOP};

    cmd[0] = SI446X_CMD_SET_PROPERTY;
    cmd[1] = group;
    cmd[2] = num_props;
    cmd[3] = start_prop;

    memcpy(cmd+4, data, len);

    return si446x_set_cmd(cmd, 4+len);
}

int si446x_change_state(si446x_state_t next_state)
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
            return -1;  /* Invalid state */
    }

    uint8_t cmd[2] = {SI446X_CMD_NOP};

    cmd[0] = SI446X_CMD_CHANGE_STATE;
    cmd[1] = next_state;

    return si446x_set_cmd(cmd, 2);
}

int si446x_nop(void)
{
    return si446x_spi_write_byte(SI446X_CMD_NOP);
}

int si446x_fifo_info(bool rst_rx, bool rst_tx, si446x_fifo_info_t *fifo_info)
{
    uint8_t cmd[4] = {SI446X_CMD_NOP};
    uint8_t result[4] = {0};

    cmd[0] = SI446X_CMD_FIFO_INFO;
    cmd[1] = ((uint8_t)rst_rx << 1) | (uint8_t)rst_tx;

    int error = si446x_get_cmd(cmd, 2, result, 3);

    if (error != SI446X_SUCCESS)
    {
        return error;
    }

    fifo_info->rx_fifo_count = result[2];
    fifo_info->tx_fifo_space = result[3];

    return SI446X_SUCCESS;
}

int si446x_write_tx_fifo(uint8_t num_bytes, uint8_t *p_tx_data)
{
    uint8_t cmd[129] = {SI446X_CMD_NOP};

    cmd[0] = SI446X_CMD_WRITE_TX_FIFO;

    memcpy(cmd+1, p_tx_data, num_bytes);

    return si446x_set_cmd(cmd, 1+num_bytes);
}

int si446x_read_rx_fifo(uint8_t num_bytes, uint8_t *p_rx_data)
{
    if (!si446x_check_cts(100))
    {
        return SI446X_CTS_TIMEOUT;
    }

    uint8_t cmd[129] = {SI446X_CMD_NOP};

    cmd[0] = SI446X_CMD_READ_RX_FIFO;

    return si446x_spi_transfer(cmd, p_rx_data, 1+num_bytes);
}

int si446x_get_property(uint8_t group, uint8_t num_props, uint8_t start_prop, uint8_t *data)
{
    uint8_t cmd[18] = {SI446X_CMD_NOP};
    uint8_t result[18] = {0};

    cmd[0] = SI446X_CMD_GET_PROPERTY;
    cmd[1] = group;
    cmd[2] = num_props;
    cmd[3] = start_prop;

    int error = si446x_get_cmd(cmd, 4, result, 17);

    if (error != SI446X_SUCCESS)
    {
        return error;
    }

    memcpy(data, result+2, num_props);

    return SI446X_SUCCESS;
}

int si446x_func_info(si446x_func_info_t *func_info)
{
    uint8_t cmd[8] = {SI446X_CMD_NOP};
    uint8_t result[8] = {0};

    cmd[0] = SI446X_CMD_FUNC_INFO;

    int error = si446x_get_cmd(cmd, 1, result, 7);

    if (error != SI446X_SUCCESS)
    {
        return error;
    }

    func_info->revext       = result[2];
    func_info->revbranch    = result[3];
    func_info->revint       = result[4];
    func_info->patch        = ((uint16_t)result[5] << 8) | result[6];
    func_info->func         = result[7];

    return SI446X_SUCCESS;
}

int si446x_frr_a_read(uint8_t resp_byte_count, uint8_t *frr_a_val)
{
    if (!si446x_check_cts(100))
    {
        return SI446X_CTS_TIMEOUT;
    }

    uint8_t cmd[2] = {SI446X_CMD_NOP};
    uint8_t result[2] = {0};

    cmd[0] = SI446X_CMD_FRR_A_READ;

    si446x_spi_transfer(cmd, result, 2);

    *frr_a_val = result[1];

    return SI446X_SUCCESS;
}

int si446x_frr_b_read(uint8_t resp_byte_count, uint8_t *frr_b_val)
{
    if (!si446x_check_cts(100))
    {
        return SI446X_CTS_TIMEOUT;
    }

    uint8_t cmd[2] = {SI446X_CMD_NOP};
    uint8_t result[2] = {0};

    cmd[0] = SI446X_CMD_FRR_B_READ;

    si446x_spi_transfer(cmd, result, 2);

    *frr_b_val = result[1];

    return SI446X_SUCCESS;
}

int si446x_frr_c_read(uint8_t resp_byte_count, uint8_t *frr_c_val)
{
    if (!si446x_check_cts(100))
    {
        return SI446X_CTS_TIMEOUT;
    }

    uint8_t cmd[2] = {SI446X_CMD_NOP};
    uint8_t result[2] = {0};

    cmd[0] = SI446X_CMD_FRR_C_READ;

    si446x_spi_transfer(cmd, result, 2);

    *frr_c_val = result[1];

    return SI446X_SUCCESS;
}

int si446x_frr_d_read(uint8_t resp_byte_count, uint8_t *frr_d_val)
{
    if (!si446x_check_cts(100))
    {
        return SI446X_CTS_TIMEOUT;
    }

    uint8_t cmd[2] = {SI446X_CMD_NOP};
    uint8_t result[2] = {0};

    cmd[0] = SI446X_CMD_FRR_D_READ;

    si446x_spi_transfer(cmd, result, 2);

    *frr_d_val = result[1];

    return SI446X_SUCCESS;
}

int si446x_get_adc_reading(uint8_t temp_en, bool bat_volt_en, bool adc_gpio_en, uint8_t adc_gpio_pin, si446x_adc_reading_t *adc_reading)
{
    uint8_t cmd[10] = {SI446X_CMD_NOP};
    uint8_t result[10] = {0};

    cmd[0] = SI446X_CMD_GET_ADC_READING;
    cmd[1] = ((uint8_t)temp_en << 4) | ((uint8_t)bat_volt_en << 3) | ((uint8_t)adc_gpio_en << 2) | (adc_gpio_pin & 0x03);

    int error = si446x_get_cmd(cmd, 2, result, 9);

    if (error != SI446X_SUCCESS)
    {
        return error;
    }

    adc_reading->gpio_adc       = ((uint16_t)result[2] << 8) | result[3];
    adc_reading->battery_adc    = ((uint16_t)result[4] << 8) | result[5];
    adc_reading->temp_adc       = ((uint16_t)result[6] << 8) | result[7];
    adc_reading->temp_slope     = result[8];
    adc_reading->temp_intercept = result[9];

    return SI446X_SUCCESS;
}

int si446x_get_packet_info(uint8_t field_number_mask, uint16_t len, int16_t diff_len, uint16_t *last_len)
{
    uint8_t cmd[6] = {SI446X_CMD_NOP};
    uint8_t result[6] = {0};

    cmd[0] = SI446X_CMD_PACKET_INFO;
    cmd[1] = field_number_mask;
    cmd[2] = (uint8_t)(len >> 8);
    cmd[3] = (uint8_t)(len & 0xFF);
    cmd[4] = (uint8_t)(diff_len >> 8);
    cmd[5] = (uint8_t)(diff_len & 0xFF);

    int error = si446x_get_cmd(cmd, 6, result, 3);

    if (error != SI446X_SUCCESS)
    {
        return error;
    }

    *last_len = ((uint16_t)result[2] << 8) | result[3];

    return SI446X_SUCCESS;
}

int si446x_get_ph_status(si446x_ph_status_t *ph_status)
{
    uint8_t cmd[4] = {SI446X_CMD_NOP};
    uint8_t result[4] = {0};

    cmd[0] = SI446X_CMD_GET_PH_STATUS;

    int error = si446x_get_cmd(cmd, 1, result, 3);

    if (error != SI446X_SUCCESS)
    {
        return error;
    }

    ph_status->ph_pend      = result[2];
    ph_status->ph_status    = result[3];

    return SI446X_SUCCESS;
}

int si446x_get_modem_status(uint8_t modem_clr_pend, si446x_modem_status_t *modem_status)
{
    uint8_t cmd[10] = {SI446X_CMD_NOP};
    uint8_t result[10] = {0};

    cmd[0] = SI446X_CMD_GET_MODEM_STATUS;

    int error = si446x_get_cmd(cmd, 1, result, 9);

    if (error != SI446X_SUCCESS)
    {
        return error;
    }

    modem_status->modem_pend        = result[2];
    modem_status->modem_status      = result[3];
    modem_status->curr_rssi         = result[4];
    modem_status->latch_rssi        = result[5];
    modem_status->ant1_rssi         = result[6];
    modem_status->ant2_rssi         = result[7];
    modem_status->afc_freq_offset   = ((uint16_t)result[8] << 8) | result[9];

    return SI446X_SUCCESS;
}

int si446x_get_chip_status(uint8_t chip_clr_pend, si446x_chip_status_t *chip_status)
{
    uint8_t cmd[5] = {SI446X_CMD_NOP};
    uint8_t result[5] = {0};

    cmd[0] = SI446X_CMD_GET_CHIP_STATUS;

    int error = si446x_get_cmd(cmd, 1, result, 4);

    if (error != SI446X_SUCCESS)
    {
        return error;
    }

    chip_status->chip_pend          = result[2];
    chip_status->chip_status        = result[3];
    chip_status->chip_err_status    = result[4];

    return SI446X_SUCCESS;
}

int si446x_ircal(uint8_t searching_step_size, uint8_t searching_rssi_avg, uint8_t rx_chain_setting1, uint8_t rx_chain_setting2)
{
    uint8_t cmd[5] = {SI446X_CMD_NOP};

    cmd[0] = SI446X_CMD_IRCAL;
    cmd[1] = searching_step_size;
    cmd[2] = searching_rssi_avg;
    cmd[3] = rx_chain_setting1;
    cmd[4] = rx_chain_setting2;

    return si446x_set_cmd(cmd, 5);
}

int si446x_protocol_cfg(uint8_t protocol)
{
    uint8_t cmd[2] = {SI446X_CMD_NOP};

    cmd[0] = SI446X_CMD_PROTOCOL_CFG;
    cmd[1] = protocol;

    return si446x_set_cmd(cmd, 2);
}

int si446x_request_device_state(si446x_device_state_t *dev_state)
{
    uint8_t cmd[4] = {SI446X_CMD_NOP};
    uint8_t result[4] = {0};

    cmd[0] = SI446X_CMD_REQUEST_DEVICE_STATE;

    int error = si446x_get_cmd(cmd, 1, result, 3);

    if (error != SI446X_SUCCESS)
    {
        return error;
    }

    dev_state->curr_state = result[2];
    dev_state->current_channel = result[3];

    return SI446X_SUCCESS;
}

int si446x_rx_hop(uint8_t inte, uint8_t frac2, uint8_t frac1, uint8_t frac0, uint8_t vco_cnt1, uint8_t vco_cnt0)
{
    uint8_t cmd[7] = {SI446X_CMD_NOP};

    cmd[0] = SI446X_CMD_RX_HOP;
    cmd[1] = inte;
    cmd[2] = frac2;
    cmd[3] = frac1;
    cmd[4] = frac0;
    cmd[5] = vco_cnt1;
    cmd[6] = vco_cnt0;

    return si446x_set_cmd(cmd, 7);
}

int si446x_shutdown(void)
{
    return si446x_gpio_write_sdn(true);
}

int si446x_power_on(void)
{
    return si446x_gpio_write_sdn(false);
}

bool si446x_check_cts(uint32_t timeout_ms)
{
    uint8_t cmd[] = {SI446X_CMD_READ_CMD_BUF, SI446X_CMD_NOP};
    uint8_t result[] = {0, 0};

    while(timeout_ms--)
    {
        si446x_spi_transfer(cmd, result, 2);

        if (result[1] == SI446X_CMD_COMPLETE)
        {
            return true;
        }

        si446x_delay_ms(1);
    }

    return false;   /* Timeout reached */
}

int si446x_set_cmd(uint8_t *cmd, uint16_t cmd_len)
{
    if (!si446x_check_cts(100))
    {
        return SI446X_CTS_TIMEOUT;
    }

    return si446x_spi_write(cmd, cmd_len);
}

int si446x_get_cmd(uint8_t *cmd, uint16_t cmd_len, uint8_t *result, uint16_t result_len)
{
    /* Writes the command */
    if (!si446x_check_cts(100))
    {
        return SI446X_CTS_TIMEOUT;
    }

    if (si446x_spi_write(cmd, cmd_len) != SI446X_SUCCESS)
    {
        return SI446X_SPI_ERROR;
    }

    /* Reads the result */
    cmd[0] = SI446X_CMD_READ_CMD_BUF;

    memcpy(cmd+1, (char*)SI446X_CMD_NOP, cmd_len-1);

    if (!si446x_check_cts(100))
    {
        return SI446X_CTS_TIMEOUT;
    }

    if (si446x_spi_transfer(cmd, result, result_len+1) != SI446X_SUCCESS)
    {
        return SI446X_SPI_ERROR;
    }

    return SI446X_SUCCESS;
}

/** \} End of si446x group */
