/*
 * lpl.c
 * 
 * Copyright The OBDH 2.0 Contributors.
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
 * along with OBDH 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief LPL device implementation.
 * 
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 1.0.0
 * 
 * \date 2025/09/28
 * 
 * \addtogroup lpl
 * \{
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <FreeRTOS.h>
#include <task.h>

#include <system/sys_log/sys_log.h>
#include <system/system.h>

#include <drivers/uart/uart.h>
#include <drivers/gpio/gpio.h>

#include <devices/media/media.h>

#include "lpl.h"

static lpl_t *handle = NULL;

int lpl_init(lpl_t *dev) 
{
    gpio_config_t conf = {0};

    conf.mode = GPIO_MODE_OUTPUT;

    dev->uart_conf.baudrate  = 115200;
    dev->uart_conf.data_bits = 8;
    dev->uart_conf.parity    = UART_NO_PARITY;
    dev->uart_conf.stop_bits = UART_ONE_STOP_BIT;
    dev->uart_port = UART_PORT_1;
    dev->en_pin = GPIO_PIN_29;

    (void)memset(dev->packet, LPL_PACKET_PREAMBLE, sizeof(dev->packet));

    int err = -1;

    if (uart_init(dev->uart_port, dev->uart_conf) == 0)
    {
        err = uart_rx_enable(dev->uart_port);

        if (err == 0)
        {
            err = gpio_init(dev->en_pin, conf);

            if (err == 0)
            {
                err = lpl_reset(dev);
            }
        }
    }

    return err;
}

 
int lpl_enable(const lpl_t *dev)
{
    return gpio_set_state(dev->en_pin, true);
}

int lpl_disable(const lpl_t *dev)
{
    return gpio_set_state(dev->en_pin, false);
}

int lpl_available(const lpl_t *dev)
{
    return uart_read_available(dev->uart_port);
}

int lpl_read(lpl_t *dev, uint8_t *buf, const uint16_t size)
{
    return uart_read(dev->uart_port, buf, size);
}

int lpl_reset(const lpl_t *dev)
{
    int err = lpl_disable(dev);

    if (err == 0)
    {
        vTaskDelay(pdMS_TO_TICKS(100U));
        err = lpl_enable(dev);
    }

    return err;
}

int lpl_recv(lpl_t *dev, const uint32_t timeout_ms)
{
    int err = -1;
    uint32_t remaining_time = timeout_ms;

    int bytes = lpl_available(dev);

    if (bytes == 0)
    {
        sys_log_print_event_from_module(SYS_LOG_INFO, LPL_MODULE_NAME, "No data is available from LPL device");
        sys_log_new_line();
        err = 0;
    }
    else if (bytes > 0)
    {
        while ((lpl_available(dev) != (int)LPL_PACKET_SIZE) && (remaining_time > 0U))
        {
            uint32_t delay = (((int32_t)remaining_time - 100) < 0) ? remaining_time : 100U;
            vTaskDelay(pdMS_TO_TICKS(delay));
            remaining_time -= delay;
        }

        if (lpl_available(dev) >= (int)LPL_PACKET_SIZE)
        {
            uint8_t buf[LPL_PACKET_SIZE];
            lpl_read(dev, buf, LPL_PACKET_SIZE);

            if (buf[0U] == LPL_PACKET_PREAMBLE)
            {
                (void)memcpy(dev->packet, buf, LPL_PACKET_SIZE);
                err = 0;
                
                uint32_t now = system_get_time();
                (void)memcpy(&dev->packet[LPL_PACKET_SIZE], (void*)&now, sizeof(now));
            }
            else
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, LPL_MODULE_NAME, "The packet didn't contain a valid preamble!");
                sys_log_new_line();
            }
        }
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, LPL_MODULE_NAME, "Failed to get available bytes from LPL device!");
        sys_log_new_line();
    }

    return err;
}

int lpl_store_to_fram(lpl_t *dev)
{
    int err = -1;

    if (media_write(MEDIA_FRAM, LPL_FRAM_MEM_ADDR, dev->packet, sizeof(dev->packet)) == 0)
    {
        err = 0;
    }

    return err;
}

int lpl_load_from_fram(lpl_t *dev)
{
    int err = -1;

    if (media_read(MEDIA_FRAM, LPL_FRAM_MEM_ADDR, dev->packet, sizeof(dev->packet)) == 0)
    {
        err = 0;
    }

    return err;
}

void lpl_register_handle(lpl_t *dev)
{
    handle = dev;
}

lpl_t* lpl_get_handle(void)
{
    return handle;
}

/** \} End of payload group */
