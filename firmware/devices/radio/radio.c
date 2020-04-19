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
 * \version 0.3.13
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

#include "radio.h"

int radio_init()
{
    sys_log_print_event_from_module(SYS_LOG_INFO, RADIO_MODULE_NAME, "Initializing radio device...");
    sys_log_new_line();

    return si446x_init();
}

int radio_send(uint8_t *data, uint16_t len)
{
    sys_log_print_event_from_module(SYS_LOG_INFO, RADIO_MODULE_NAME, "Transmmiting ");
    sys_log_print_dec(len);
    sys_log_print_msg(" byte(s)...");
    sys_log_new_line();

    if (!si446x_tx_long_packet(uint8_t *packet, uint16_t len))
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, RADIO_MODULE_NAME, "Error transmitting a packet!");
        sys_log_new_line();

        return -1;
    }

    return 0;
}

int radio_recv(uint8_t *data, uint16_t len, uint32_t timeout_ms)
{
    if (!si446x_rx_init())
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, RADIO_MODULE_NAME, "Error starting RX mode!");
        sys_log_new_line();

        return -1;
    }

    int recv_len = 0;

    TickType_t timeout_tick = pdMS_TO_TICKS(timeout_ms);

    TickType_t start_time_tick = xTaskGetTickCount();

    while((start_time_tick + timeout_tick) <= xTaskGetTickCount())
    {
        if (si446x_wait_nirq())
        {
            recv_len = (int)si446x_rx_packet(data, len);

            si446x_clear_interrupts();

            sys_log_print_event_from_module(SYS_LOG_INFO, RADIO_MODULE_NAME, "");
            sys_log_print_dec(len);
            sys_log_print_msg(" byte(s) received!");
            sys_log_new_line();

            return recv_len;
        }

        vTaskDelay(pdMS_TO_TICKS(250));
    }

    return 0;
}

int radio_available()
{
    return (int)si446x_wait_nirq();
}

int radio_sleep()
{
    if (!si446x_enter_standby_mode())
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, RADIO_MODULE_NAME, "Error configuring sleep mode!");
        sys_log_new_line();

        return -1;
    }

    return 0;
}

/** \} End of radio group */
