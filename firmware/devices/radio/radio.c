/*
 * radio.c
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with OBDH 2.0. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Radio device implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.3.22
 * 
 * \date 27/10/2019
 * 
 * \addtogroup radio
 * \{
 */

#include <FreeRTOS.h>
#include <task.h>

#include <system/sys_log/sys_log.h>

#include <drivers/si446x/si446x.h>
#include <config/radio_config_Si4463.h>
#include <config/config.h>

#include "radio.h"

int radio_init()
{
    sys_log_print_event_from_module(SYS_LOG_INFO, RADIO_MODULE_NAME, "Initializing radio device...");
    sys_log_new_line();

    /* SPI interface initialization */
    if (si446x_spi_init() != SI446X_SUCCESS)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, RADIO_MODULE_NAME, "Error initializing the SPI interface!");
        sys_log_new_line();

        return -1;
    }

    /* Radio reset */
    if (si446x_reset() != SI446X_SUCCESS)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, RADIO_MODULE_NAME, "Error during the reset!");
        sys_log_new_line();

        return -1;
    }

    /* Loading configuration parameters from WDS */
    uint8_t si446x_config[] = {RADIO_CONFIGURATION_DATA_ARRAY};

    if (si446x_configuration_init(si446x_config , sizeof(si446x_config )-1) != SI446X_SUCCESS)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, RADIO_MODULE_NAME, "Error loading the configuration parameters!");
        sys_log_new_line();

        return -1;
    }

    /* Frequency calibration */
    uint16_t prop = SI446X_PROP_GLOBAL_XO_TUNE;
    uint8_t prop_param = SI446X_XO_TUNE_REG_VALUE;

    if (si446x_set_property((uint8_t)(prop >> 8), 1, (uint8_t)(prop & 0xFF), &prop_param, 1) != SI446X_SUCCESS)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, RADIO_MODULE_NAME, "Error calibrating the XO frequency!");
        sys_log_new_line();

        return -1;
    }

    /* FIFO configuration (128 bytes shared between TX and RX) */
    prop = SI446X_PROP_GLOBAL_CONFIG;
    prop_param = 0x10;  /* FIFO_MODE = TX/RX FIFO are sharing with 128-byte size buffer */

    if (si446x_set_property((uint8_t)(prop >> 8), 1, (uint8_t)(prop & 0xFF), &prop_param, 1) != SI446X_SUCCESS)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, RADIO_MODULE_NAME, "Error configuring the FIFO!");
        sys_log_new_line();

        return -1;
    }

    /* FIFO reset */
    si446x_fifo_info_t fifo_info;

    if (si446x_fifo_info(true, true, &fifo_info) != SI446X_SUCCESS)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, RADIO_MODULE_NAME, "Error reseting the FIFO!");
        sys_log_new_line();

        return -1;
    }

    return 0;
}

int radio_send(uint8_t *data, uint16_t len, uint32_t timeout_ms)
{
    sys_log_print_event_from_module(SYS_LOG_INFO, RADIO_MODULE_NAME, "Transmmiting ");
    sys_log_print_uint(len);
    sys_log_print_msg(" byte(s)...");
    sys_log_new_line();

    uint16_t prop = SI446X_PROP_PKT_FIELD_1_LENGTH_7_0;
    uint8_t prop_param = (uint8_t)len;

    /* Configuring the length of the packet */
    if (si446x_set_property((uint8_t)(prop >> 8), 1, (uint8_t)(prop & 0xFF), &prop_param, 1) != SI446X_SUCCESS)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, RADIO_MODULE_NAME, "Error transmitting a packet!");
        sys_log_new_line();

        return -1;
    }

    /* FIFO reset */
    si446x_fifo_info_t fifo_info;

    if (si446x_fifo_info(true, true, &fifo_info) != SI446X_SUCCESS)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, RADIO_MODULE_NAME, "Error reseting the FIFO!");
        sys_log_new_line();

        return -1;
    }

    /* Writing the packet to the TX FIFO */
    if (si446x_write_tx_fifo(len, data) != SI446X_SUCCESS)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, RADIO_MODULE_NAME, "Error writing data to FIFO!");
        sys_log_new_line();

        return -1;
    }

    /* Clear interrupts */
    si446x_int_status_t int_status;

    if (si446x_get_int_status(0, 0, 0, &int_status) != SI446X_SUCCESS)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, RADIO_MODULE_NAME, "Error clearing the interrupts!");
        sys_log_new_line();

        return -1;
    }

    /* Start TX */
    if (si446x_start_tx(0, (SI446X_RX_STATE << 4) | SI446X_START_TX_DO_NOT_RETRANSMIT | SI446X_START_TX_START_TX_IMMEDIATELY, 0) != SI446X_SUCCESS)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, RADIO_MODULE_NAME, "Error starting the TX!");
        sys_log_new_line();

        return -1;
    }

    /* Wait the packet transmission */
    timeout_ms /= 10;

    while(timeout_ms--)
    {
        si446x_get_int_status(0, 0, 0, &int_status);

        if (int_status.ph_status & SI446X_INT_STATUS_PACKET_SENT)
        {
            break;
        }

        si446x_delay_ms(10);
    }

    return 0;
}

int radio_recv(uint8_t *data, uint16_t len, uint32_t timeout_ms)
{
    if (si446x_change_state(SI446X_RX_STATE) != SI446X_SUCCESS)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, RADIO_MODULE_NAME, "Error starting reception!");
        sys_log_new_line();

        return -1;
    }

    uint16_t prop = SI446X_PROP_INT_CTL_ENABLE;
    uint8_t prop_param = 0x03;

    if (si446x_set_property((uint8_t)(prop >> 8), 1, (uint8_t)(prop & 0xFF), prop_param, 1) != SI446X_SUCCESS)
    {
        return -1;
    }

    prop = SI446X_PROP_INT_CTL_PH_ENABLE;
    prop_param = 0x18;

    if (si446x_set_property((uint8_t)(prop >> 8), 1, (uint8_t)(prop & 0xFF), prop_param, 1) != SI446X_SUCCESS)
    {
        return -1;
    }

    prop = SI446X_PROP_INT_CTL_MODEM_ENABLE;
    prop_param = 0x00;

    if (si446x_set_property((uint8_t)(prop >> 8), 1, (uint8_t)(prop & 0xFF), prop_param, 1) != SI446X_SUCCESS)
    {
        return -1;
    }

    if (si446x_start_rx(0, SI446X_START_RX_START_RX_IMMEDIATELY, 0, SI446X_RX_STATE, SI446X_RX_STATE, SI446X_RX_STATE) != SI446X_SUCCESS)
    {
        return -1;
    }

    TickType_t timeout_tick = pdMS_TO_TICKS(timeout_ms);

    TickType_t start_time_tick = xTaskGetTickCount();

    while((start_time_tick + timeout_tick) <= xTaskGetTickCount())
    {
        int avail = radio_available();

        if (avail > 0)
        {
            si446x_read_rx_fifo(avail, data);

            sys_log_print_event_from_module(SYS_LOG_INFO, RADIO_MODULE_NAME, "");
            sys_log_print_uint(avail);
            sys_log_print_msg(" byte(s) received!");
            sys_log_new_line();

            /* Clear interrupts */
            si446x_int_status_t int_status;

            si446x_get_int_status(0, 0, 0, &int_status);

            return avail;
        }

        vTaskDelay(pdMS_TO_TICKS(250));
    }

    return 0;
}

int radio_available()
{
    si446x_int_status_t int_status;

    si446x_get_int_status(0, 0, 0, &int_status);

    if ((int_status.ph_status | SI446X_INT_STATUS_PACKET_RX) > 0)
    {
        si446x_fifo_info_t fifo_info;

        si446x_fifo_info(false, false, &fifo_info);

        return fifo_info.rx_fifo_count;
    }
    else
    {
        return 0;
    }
}

int radio_sleep()
{
    if (si446x_change_state(SI446X_SLEEP_STATE) != SI446X_SUCCESS)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, RADIO_MODULE_NAME, "Error configuring sleep mode!");
        sys_log_new_line();

        return -1;
    }

    return 0;
}

/** \} End of radio group */
