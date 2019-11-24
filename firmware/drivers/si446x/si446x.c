/*
 * si446x.c
 * 
 * Copyright (C) 2019, SpaceLab.
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
 * \version 0.1.0
 * 
 * \date 01/06/2017
 * 
 * \addtogroup si446x
 * \{
 */

#include "../interface/debug/debug.h"

#include "si446x.h"
#include "../hal/obdh_hal.h"
#include "si446x_config.h"
#include "si446x_spi.h"
#include "si446x_reg.h"
#include "radio_config_Si4463.h"

const uint8_t SI446X_CONFIGURATION_DATA[] = RADIO_CONFIGURATION_DATA_ARRAY;

uint8_t si446x_mode = 0xFF;

uint8_t si446x_init(void)
{
    debug_print_event_from_module(DEBUG_INFO, SI446X_MODULE_NAME, "Initializing device...");
    debug_new_line();

    // Power-on reset
    si446x_power_on_reset();

    // Registers configuration
    si446x_reg_config();

    // Set max. TX power
    si446x_set_tx_power(127);

    // Check if the device is working
    if (si446x_check_device())
    {
        return STATUS_SUCCESS;
    }
    else
    {
        return STATUS_FAIL;
    }
}

void si446x_shutdown(void)
{
    BIT_SET(TTC_RESETn_MAIN_OUT, TTC_RESETn_MAIN_PIN);
}

void si446x_power_up(void)
{
    BIT_CLEAR(TTC_RESETn_MAIN_OUT, TTC_RESETn_MAIN_PIN);
}

void si446x_slave_enable(void)
{
    BIT_CLEAR(SPI0_CSn_OUT, SPI0_CSn_PIN);
}

void si446x_slave_disable(void)
{
    BIT_SET(SPI0_CSn_OUT, SPI0_CSn_PIN);
}

void si446x_gpio_init(void)
{
    debug_print_event_from_module(DEBUG_INFO, SI446X_MODULE_NAME, "Initializing GPIO pins...");
    debug_new_line();

    BIT_SET(TTC_RESETn_MAIN_DIR, TTC_RESETn_MAIN_PIN);
    BIT_CLEAR(TTC_GPIO2_MAIN_DIR, TTC_GPIO2_MAIN_PIN);

    BIT_CLEAR(TTC_GPIO1_MAIN_DIR, TTC_GPIO1_MAIN_PIN);

    BIT_SET(SPI0_CSn_DIR, SPI0_CSn_PIN);

    si446x_slave_disable();
    si446x_shutdown();
}

void si446x_reg_config(void)
{
    debug_print_event_from_module(DEBUG_INFO, SI446X_MODULE_NAME, "Loading registers values...");
    debug_new_line();

    // Set RF parameter like frequency, data rate, etc.
    si446x_set_config(SI446X_CONFIGURATION_DATA, sizeof(SI446X_CONFIGURATION_DATA));

    uint8_t buf[2];

    // Frequency adjust (Tested manually)
    buf[0] = SI446X_XO_TUNE_REG_VALUE;
    si446x_set_properties(SI446X_PROPERTY_GLOBAL_XO_TUNE, buf, 1);

    // TX/RX shares 128 bytes FIFO
    buf[0] = 0x10;
    si446x_set_properties(SI446X_PROPERTY_GLOBAL_CONFIG, buf, 1);
    si446x_fifo_reset();    // The TX/RX FIFO sharing configuration will only take effect after FIFO reset.
}

void si446x_power_on_reset(void)
{
    debug_print_event_from_module(DEBUG_INFO, SI446X_MODULE_NAME, "Power-on reset...");
    debug_new_line();

    uint8_t buffer[8] = {RF_POWER_UP};

    si446x_shutdown();
    __delay_cycles(DELAY_100_MS_IN_CYCLES);
    si446x_power_up();
    __delay_cycles(2 * DELAY_10_MS_IN_CYCLES);           // Wait for stabilization

    // Send power-up command
    si446x_slave_enable();
    si446x_spi_write(buffer, 7);
    si446x_slave_disable();

    __delay_cycles(2 * DELAY_100_MS_IN_CYCLES);
}

bool si446x_tx_packet(uint8_t *data, uint8_t len)
{
    debug_print_event_from_module(DEBUG_INFO, SI446X_MODULE_NAME, "Trying to transmit a packet...");
    debug_new_line();

    // Setting packet size
    si446x_set_properties(SI446X_PROPERTY_PKT_FIELD_1_LENGTH_7_0, &len, 1);

    si446x_fifo_reset();        // Clear FIFO
    si446x_write_tx_fifo(data, len);
    si446x_clear_interrupts();

    uint16_t tx_timer = SI446X_TX_TIMEOUT;

    si446x_enter_tx_mode();

    debug_print_event_from_module(DEBUG_INFO, SI446X_MODULE_NAME, "Transmitting a packet: ");
    uint8_t i = 0;
    for(i=0; i<len; i++)
    {
        debug_print_hex(data[i]);
        if (i < len-1)
        {
            debug_print_msg(",");
        }
    }
    debug_new_line();

    while(tx_timer--)
    {
        if (si446x_wait_packet_sent())         // Wait packet sent interruption
        {
            return true;
        }

        __delay_cycles(DELAY_100_uS_IN_CYCLES);
    }

    // If the packet transmission takes longer than expected, resets the radio.
//    si446x_init();

    return false;
}

bool si446x_tx_long_packet(uint8_t *packet, uint16_t len)
{
    if (len <= SI446X_TX_FIFO_LEN)
    {
        return si446x_tx_packet(packet, (uint8_t)(len));
    }

    debug_print_event_from_module(DEBUG_INFO, SI446X_MODULE_NAME, "Trying to transmit a long packet...");
    debug_new_line();

    // Setting packet size
    uint8_t buf[2];
    buf[0] = (uint8_t)(len);
    buf[1] = (uint8_t)(len >> 8);
    if (len > 255)
    {
        si446x_set_properties(SI446X_PROPERTY_PKT_FIELD_1_LENGTH_12_8, &buf[1], 1);
    }
    si446x_set_properties(SI446X_PROPERTY_PKT_FIELD_1_LENGTH_7_0, &buf[0], 1);
    
    si446x_fifo_reset();        // Clear FIFO
    si446x_write_tx_fifo(packet, SI446X_TX_FIFO_LEN);
    uint16_t long_pkt_pos = SI446X_TX_FIFO_LEN;
    
    si446x_clear_interrupts();
    
    uint8_t fifo_buffer[SI446X_TX_FIFO_ALMOST_EMPTY_THRESHOLD];
    uint16_t tx_timer = SI446X_TX_TIMEOUT;
    uint16_t i = 0;

    si446x_enter_tx_mode();

    debug_print_event_from_module(DEBUG_INFO, SI446X_MODULE_NAME, "Transmitting data: ");
    for(i=0; i<len; i++)
    {
        debug_print_hex(packet[i]);
        if (i < len-1)
        {
            debug_print_msg(",");
        }
    }
    debug_new_line();

    while(tx_timer--)
    {
        if (si446x_wait_gpio1())
        {            
            uint16_t bytes_to_transfer = len - long_pkt_pos;

            if (bytes_to_transfer <= SI446X_TX_FIFO_ALMOST_EMPTY_THRESHOLD)
            {
                for(i=0; i<bytes_to_transfer; i++)
                {
                    fifo_buffer[i] = packet[long_pkt_pos++];
                }

                si446x_write_tx_fifo(fifo_buffer, bytes_to_transfer);
                tx_timer = SI446X_TX_TIMEOUT;

                while(tx_timer--)
                {
                    if (si446x_wait_packet_sent())         // Wait packet sent interruption
                    {
                        return true;
                    }

                    __delay_cycles(DELAY_100_uS_IN_CYCLES);
                }
                break;
            }
            else
            {
                for(i=0; i<SI446X_TX_FIFO_ALMOST_EMPTY_THRESHOLD; i++)
                {
                    fifo_buffer[i] = packet[long_pkt_pos++];
                }

                si446x_write_tx_fifo(fifo_buffer, SI446X_TX_FIFO_ALMOST_EMPTY_THRESHOLD);
                tx_timer = SI446X_TX_TIMEOUT;
            }
        }
        __delay_cycles(160); //10us
    }

    // If the packet transmission takes longer than expected, resets the radio.
//    si446x_init();

    return false;
}

uint8_t si446x_rx_packet(uint8_t *rx_buf, uint8_t read_len)
{
    uint8_t rx_len = si446x_read_rx_fifo(rx_buf, read_len);     // Read data from the FIFO
    si446x_fifo_reset();                                        // Clear FIFO

    return rx_len;
}

bool si446x_rx_init(void)
{
    uint8_t length = 50;

    si446x_set_properties(SI446X_PROPERTY_PKT_FIELD_2_LENGTH_7_0, &length, 1);  // Reload RX FIFO size
    si446x_fifo_reset();                                                        // Clear FIFO
    si446x_set_rx_interrupt();
    si446x_clear_interrupts();
    si446x_enter_rx_mode();

    return true;
}

bool si446x_check_device(void)
{
    uint8_t buffer[10];
    uint16_t part_info;

    if (!si446x_get_cmd(SI446X_CMD_PART_INFO, buffer, 9))
    {
        debug_print_event_from_module(DEBUG_ERROR, SI446X_MODULE_NAME, "Error reading the device ID!");
        debug_new_line();

        return false;
    }

    part_info = (buffer[2] << 8) | buffer[3];
    if (part_info != SI446X_PART_INFO)
    {
        debug_print_event_from_module(DEBUG_ERROR, SI446X_MODULE_NAME, "Error checking the device ID! (read=");
        debug_print_hex(part_info);
        debug_print_msg(", expected=");
        debug_print_hex(SI446X_PART_INFO);
        debug_print_msg(")");
        debug_new_line();

        return false;
    }
    else
    {
        return true;
    }
}

bool si446x_check_cts(void)
{
    uint16_t timeout_counter = SI446X_CTS_TIMEOUT;

    while(timeout_counter--)
    {
        si446x_slave_enable();
        
        si446x_spi_transfer(SI446X_CMD_READ_BUF);
        
        if (si446x_spi_transfer(SI446X_CMD_NOP) == SI446X_CTS_REPLY) // Read CTS
        {
            si446x_slave_disable();
            
            return true;
        }
        
        si446x_slave_disable();
    }

    return  false;
}

bool si446x_get_cmd(uint8_t cmd, uint8_t *para_buf, uint8_t length)
{
    if (!si446x_check_cts())
    {
        return false;
    }
    
    si446x_slave_enable();
    si446x_spi_transfer(cmd);                   // Send the command
    si446x_slave_disable();
    
    if (!si446x_check_cts())
    {
        return false;
    }
    
    si446x_slave_enable();
    si446x_spi_transfer(SI446X_CMD_READ_BUF);   // Send READ_BUF command to grab the command parameters
    si446x_spi_read(para_buf, length);          // Read the parameters
    si446x_slave_disable();
    
    return true;
}

bool si446x_set_tx_power(uint8_t pwr)
{
    if (pwr > 127)      // Max. value is 127
    {
        pwr = 127;
    }

    debug_print_event_from_module(DEBUG_INFO, SI446X_MODULE_NAME, "Configuring TX power to ");
    debug_print_hex(pwr);
    debug_print_msg("...");
    debug_new_line();

    uint8_t buffer[5];
    buffer[0] = 0x08;
    buffer[1] = pwr;
    buffer[2] = 0x00;
    buffer[3] = 0x3D;

    return si446x_set_properties(SI446X_PROPERTY_PA_MODE, buffer, 4);
}

bool si446x_set_properties(uint16_t start_property, uint8_t *para_buf, uint8_t length)
{
    if (!si446x_check_cts())
    {
        return false;
    }
    
    uint8_t buffer[5];
    buffer[0] = SI446X_CMD_SET_PROPERTY;    // CMD
    buffer[1] = start_property >> 8;        // GROUP
    buffer[2] = length;                     // NUM_PROPS
    buffer[3] = start_property & 0xFF;      // START_PROP
    
    si446x_slave_enable();
    si446x_spi_write(buffer, 4);            // Set start property and read length
    si446x_spi_write(para_buf, length);     // Set parameters
    si446x_slave_disable();
    
    return true;
}

bool si446x_get_properties(uint16_t start_property, uint8_t length, uint8_t *para_buf)
{
    if (!si446x_check_cts())
    {
        return false;
    }
    
    uint8_t buffer[5];
    buffer[0] = SI446X_CMD_GET_PROPERTY;
    buffer[1] = start_property >> 8;    // GROUP
    buffer[2] = length;                 // NUM_PROPS
    buffer[3] = start_property & 0xFF;  // START_PROP
    
    si446x_slave_enable();
    si446x_spi_write(buffer, 4);        // Set start property and read length
    si446x_slave_disable();
    
    if (!si446x_check_cts())
    {
        return false;
    }
    
    si446x_slave_enable();
    si446x_spi_transfer(SI446X_CMD_READ_BUF);   // Turn to read command mode
    si446x_spi_write(para_buf, length);         // Read parameters
    si446x_slave_disable();
    
    return true;
}

void si446x_set_config(const uint8_t *parameters, uint16_t para_len)
{
    // Command buffer starts with the length of the command in RADIO_CONFIGURATION_DATA_ARRAY
    uint8_t cmd_len;
    uint16_t cmd;
    uint16_t pos;
    uint8_t buffer[30];
    
    para_len--;
    cmd_len = parameters[0];
    pos = cmd_len + 1;
    
    while(pos < para_len)
    {
        cmd_len = parameters[pos++] - 1;            // Get command len
        cmd = parameters[pos++];                    // Get command
        memcpy(buffer, parameters + pos, cmd_len);  // Get parameters
        
        si446x_set_cmd(cmd, buffer, cmd_len);
        pos += cmd_len;
    }
}

bool si446x_set_preamble_len(uint8_t len)
{
    debug_print_event_from_module(DEBUG_INFO, SI446X_MODULE_NAME, "Configuring preamble length to ");
    debug_print_dec(len);
    debug_print_msg("...");
    debug_new_line();

    return si446x_set_properties(SI446X_PROPERTY_PREAMBLE_TX_LENGTH, &len, 1);
}

bool si446x_set_sync_word(uint8_t *sync_word, uint8_t len)
{
    if ((len == 0) || (len > 3))
    {
        debug_print_event_from_module(DEBUG_ERROR, SI446X_MODULE_NAME, "Error configuring sync word! Length greater than 4 bytes!");
        debug_new_line();

        return false;
    }

    debug_print_event_from_module(DEBUG_INFO, SI446X_MODULE_NAME, "Configuring sync word as ");
    uint8_t i = 0;
    for(i=0; i<len; i++)
    {
        debug_print_hex(sync_word[i]);
        if (i < len-1)
        {
            debug_print_msg(",");
        }
    }
    debug_new_line();

    uint8_t buffer[6];
    buffer[0] = len - 1;
    memcpy(buffer + 1,sync_word, len);

    return si446x_set_properties(SI446X_PROPERTY_SYNC_CONFIG, buffer, len);
}

bool si446x_set_gpio_mode(uint8_t gpio0_mode, uint8_t gpio1_mode)
{
    uint8_t buffer[7];
    buffer[0] = gpio0_mode;
    buffer[1] = gpio1_mode;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    
    return si446x_set_cmd(SI446X_CMD_GPIO_PIN_CFG, buffer, 6);
}

bool si446x_set_cmd(uint8_t cmd, uint8_t *para_buf, uint8_t len)
{
    if (!si446x_check_cts())
    {
        return false;
    }
    
    si446x_slave_enable();
    si446x_spi_transfer(cmd);           // Send the command
    si446x_spi_write(para_buf, len);    // Send the parameters
    si446x_slave_disable();
    
    return true;
}

bool si446x_set_tx_interrupt(void)
{
    uint8_t buffer[4];      // Enable PACKET_SENT interruption
    
    buffer[0] = 0x01;
    buffer[1] = 0x20;
    buffer[2] = 0x00;
    
    return si446x_set_properties(SI446X_PROPERTY_INT_CTL_ENABLE, buffer, 3);
}

bool si446x_set_rx_interrupt(void)
{
    uint8_t buffer[4];      // Enable PACKET_RX interrution
    
    buffer[0] = 0x03;
    buffer[1] = 0x18;
    buffer[2] = 0x00;
    
    return si446x_set_properties(SI446X_PROPERTY_INT_CTL_ENABLE, buffer, 3);
}

bool si446x_clear_interrupts(void)
{
    uint8_t buffer[4];
    
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    
    return si446x_set_cmd(SI446X_CMD_GET_INT_STATUS, buffer, 4);
}

void si446x_write_tx_fifo(uint8_t *data, uint8_t len)
{
    uint8_t buffer[SI446X_TX_FIFO_LEN];
    memcpy(buffer, data, len);

    si446x_set_cmd(SI446X_CMD_TX_FIFO_WRITE, buffer, len);
}

uint8_t si446x_read_rx_fifo(uint8_t *data, uint8_t read_len)
{
    if (!si446x_check_cts())
    {
        return 0;
    }

    si446x_slave_enable();
    si446x_spi_transfer(SI446X_CMD_RX_FIFO_READ);
    si446x_spi_read(data, read_len);
    si446x_slave_disable();
    
    return read_len;
}

void si446x_fifo_reset(void)
{
    uint8_t data = 0x03;
    
    si446x_set_cmd(SI446X_CMD_FIFO_INFO, &data, 1);
}

void si446x_enter_tx_mode(void)
{
    if (si446x_mode == SI446X_MODE_TX)
    {
        return;
    }

    debug_print_event_from_module(DEBUG_INFO, SI446X_MODULE_NAME, "Entering TX mode...");
    debug_new_line();

    uint8_t buffer[5];
    
    buffer[0] = SI446X_FREQ_CHANNEL;
    buffer[1] = 0x30;                   // TXCOMPLETE_STATE = Ready State; RETRANSMIT = 0 = No re-transmition; START = 0 = Start TX immediately
    buffer[2] = 0x00;                   // TX packet length MSB (If equal zero, default length)
    buffer[3] = 0x00;                   // TX packet length LSB (If equal zero, default length)
    
    si446x_set_cmd(SI446X_CMD_START_TX, buffer, 4);

    si446x_mode = SI446X_MODE_TX;
}

void si446x_enter_rx_mode(void)
{
    if (si446x_mode == SI446X_MODE_RX)
    {
        return;
    }

    debug_print_event_from_module(DEBUG_INFO, SI446X_MODULE_NAME, "Entering RX mode...");
    debug_new_line();

    uint8_t buffer[8];
    
    buffer[0] = SI446X_FREQ_CHANNEL;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x08;
    buffer[6] = 0x08;
    
    si446x_set_cmd(SI446X_CMD_START_RX, buffer, 7);

    si446x_mode = SI446X_MODE_RX;
}

bool si446x_enter_standby_mode(void)
{
    if (si446x_mode == SI446X_MODE_STANDBY)
    {
        return;
    }

    debug_print_event_from_module(DEBUG_INFO, SI446X_MODULE_NAME, "Entering standby mode...");
    debug_new_line();

    uint8_t data = 0x01;

    si446x_mode = SI446X_MODE_STANDBY;

    return si446x_set_cmd(SI446X_CMD_CHANGE_STATE, &data, 1);
}

bool si446x_wait_nIRQ(void)
{
    if (BIT_READ(TTC_GPIO2_MAIN_IN, TTC_GPIO2_MAIN_PIN) == 1)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool si446x_wait_packet_sent(void) {
    uint8_t interrupt_status[5];
    si446x_get_cmd(SI446X_CMD_GET_INT_STATUS, interrupt_status, 5);

    if (interrupt_status[4] & 0x20)
    {
        si446x_clear_interrupts();
        return true;
    }
    else
    {
        return false;
    }
}

bool si446x_wait_gpio1(void)
{
    if (BIT_READ(TTC_GPIO1_MAIN_IN, TTC_GPIO1_MAIN_PIN) == 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//! \} End of si446x group
