/*
 * startup.c
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
 * \brief Startup task implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.5.28
 * 
 * \date 2019/12/04
 * 
 * \addtogroup startup
 * \{
 */

#include <stdbool.h>

#include <config/config.h>
#include <system/sys_log/sys_log.h>
#include <system/clocks.h>
#include <devices/watchdog/watchdog.h>
#include <devices/leds/leds.h>
#include <devices/eps/eps.h>
#include <devices/radio/radio.h>
#include <devices/payload_edc/payload_edc.h>
#include <devices/current_sensor/current_sensor.h>
#include <devices/voltage_sensor/voltage_sensor.h>
#include <devices/temp_sensor/temp_sensor.h>
#include <devices/antenna/antenna.h>
#include <devices/media/media.h>

#include <ngham/ngham.h>

#include <csp/csp.h>
#include <csp/interfaces/csp_if_spi.h>
#include <csp/interfaces/csp_if_i2c.h>

#include "startup.h"

xTaskHandle xTaskStartupHandle;

EventGroupHandle_t task_startup_status;

void vTaskStartup(void *pvParameters)
{
    unsigned int error_counter = 0;

    /* Logger device initialization */
    sys_log_init();

    /* Print the FreeRTOS version */
    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_STARTUP_NAME, "FreeRTOS ");
    sys_log_print_msg(tskKERNEL_VERSION_NUMBER);
    sys_log_new_line();

    /* Print the hardware version */
    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_STARTUP_NAME, "Hardware revision is ");
    sys_log_print_uint(system_get_hw_version());
    sys_log_new_line();

    /* Print the system clocks */
    clocks_config_t clks = clocks_read();
    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_STARTUP_NAME, "System clocks: MCLK=");
    sys_log_print_uint(clks.mclk_hz);
    sys_log_print_msg(" Hz, SMCLK=");
    sys_log_print_uint(clks.smclk_hz);
    sys_log_print_msg(" Hz, ACLK=");
    sys_log_print_uint(clks.aclk_hz);
    sys_log_print_msg(" Hz");
    sys_log_new_line();

    /* Print last reset cause (code) */
    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_STARTUP_NAME, "Last reset cause: ");
    sys_log_print_hex(system_get_reset_cause());
    sys_log_new_line();

    /* Internal non-volatile memory initialization */
    if (media_init(MEDIA_INT_FLASH) != 0)
    {
        error_counter++;
    }

    /* LEDs device initialization */
    if (leds_init() != 0)
    {
        error_counter++;
    }

    /* Current sensor device initialization */
    if (current_sensor_init() != 0)
    {
        error_counter++;
    }

    /* Voltage sensor device initialization */
    if (voltage_sensor_init() != 0)
    {
        error_counter++;
    }

    /* Temperature sensor device initialization */
    if (temp_sensor_init() != 0)
    {
        error_counter++;
    }

    /* External NOR memory initialization */
    if (media_init(MEDIA_NOR) != 0)
    {
        error_counter++;
    }

    /* EPS device initialization */
    if (eps_init() != 0)
    {
        error_counter++;
    }

    /* Radio device initialization */
    if (radio_init() != 0)
    {
        error_counter++;
    }

    /* NGHam initialization */
    ngham_init_arrays();
    ngham_init();

    /* CSP initialization */
    if (startup_init_csp() != CSP_ERR_NONE)
    {
        error_counter++;
    }

    /* Payload EDC device initialization */
    if (payload_edc_init() != 0)
    {
        error_counter++;
    }

    /* Antenna device initialization */
    if (antenna_init() != 0)
    {
        error_counter++;
    }

    if (error_counter > 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_STARTUP_NAME, "Boot completed with ");
        sys_log_print_uint(error_counter);
        sys_log_print_msg(" ERROR(S)!");
        sys_log_new_line();

        led_set(LED_FAULT);
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_STARTUP_NAME, "Boot completed with SUCCESS!");
        sys_log_new_line();

        led_clear(LED_FAULT);
    }

    /* Startup task status = Done */
    xEventGroupSetBits(task_startup_status, TASK_STARTUP_DONE);

    vTaskSuspend(xTaskStartupHandle);
}

int startup_init_csp()
{
#if CONFIG_CSP_ENABLED == 1
    /* CSP initialization */
    if (csp_init(CONFIG_CSP_MY_ADDRESS) != CSP_ERR_NONE)
    {
        return -1;  /* Error during CSP initialization */
    }

    /* Buffer initialization */
    if (csp_buffer_init(CONFIG_CSP_BUFFER_MAX_PKTS, CONFIG_CSP_BUFFER_MAX_SIZE) != CSP_ERR_NONE)
    {
        return -1;  /* Error during the CSP buffer initialization */
    }

    if (csp_route_set(CONFIG_CSP_TTC_ADDRESS, &csp_if_spi, CSP_NODE_MAC) != CSP_ERR_NONE)
    {
        return -1;
    }

    if (csp_route_set(CONFIG_CSP_EPS_ADDRESS, &csp_if_i2c, CSP_NODE_MAC) != CSP_ERR_NONE)
    {
        return -1;
    }

    /* CSP router task initialization */
	if (csp_route_start_task(CONFIG_CSP_ROUTER_WORD_STACK, CONFIG_CSP_ROUTER_TASK_PRIORITY) != CSP_ERR_NONE)
    {
        return -1;  /* Error during CSP router task initialization! */
    }

    return CSP_ERR_NONE;
#else
    sys_log_print_event_from_module(SYS_LOG_WARNING, TASK_STARTUP_NAME, "libcsp disabled!");
    sys_log_new_line();

    return CSP_ERR_NONE;
#endif /* CONFIG_CSP_ENABLED */
}

/** \} End of startup group */
