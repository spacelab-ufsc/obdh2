/*
 * startup.c
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
 * \brief Startup task implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.10.17
 * 
 * \date 2019/12/04
 * 
 * \addtogroup startup
 * \{
 */

#include <stdbool.h>

#include <config/config.h>
#include <system/system.h>
#include <system/sys_log/sys_log.h>
#include <system/clocks.h>
#include <devices/watchdog/watchdog.h>
#include <devices/leds/leds.h>
#include <devices/eps/eps.h>
#include <devices/current_sensor/current_sensor.h>
#include <devices/voltage_sensor/voltage_sensor.h>
#include <devices/temp_sensor/temp_sensor.h>
#include <devices/antenna/antenna.h>
#include <devices/media/media.h>
#include <devices/payload/payload.h>
#include <app/structs/satellite.h>

#include "startup.h"

xTaskHandle xTaskStartupHandle;

EventGroupHandle_t task_startup_status;

void vTaskStartup(void)
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

#if defined(CONFIG_DEV_MEDIA_INT_ENABLED) && (CONFIG_DEV_MEDIA_INT_ENABLED == 1)
    /* Internal non-volatile memory initialization */
    if (media_init(MEDIA_INT_FLASH) != 0)
    {
        error_counter++;
    }
#endif /* CONFIG_DEV_MEDIA_INT_ENABLED */

#if defined(CONFIG_DEV_MEDIA_FRAM_ENABLED) && (CONFIG_DEV_MEDIA_FRAM_ENABLED == 1)
    /* FRAM memory initialization */
    if (system_get_hw_version() >= HW_VERSION_1)
    {
        if (media_init(MEDIA_FRAM) == 0)
        {
            /* Check if FRAM is initialized */
            if (mem_mng_check_fram() == 0)
            {
                /* Load last saved OBDH data from FRAM */
                if (mem_mng_load_obdh_data_from_fram(&sat_data_buf.obdh) != 0)
                {
                    error_counter++;
                }
            }
            else
            {
                sys_log_print_event_from_module(SYS_LOG_WARNING, TASK_STARTUP_NAME, "FRAM was not initialized in previous cycles");
                sys_log_new_line();

                /* Initialize FRAM */
                if (mem_mng_init_fram() == 0)
                {
                    /* Load default values to the OBDH data buffer */
                    mem_mng_load_obdh_data_from_default_values(&sat_data_buf.obdh);

                    sys_log_print_event_from_module(SYS_LOG_WARNING, TASK_STARTUP_NAME, "Loading default values to FRAM...");
                    sys_log_new_line();

                    /* Write the OBDH data to the FRAM memory */
                    if (mem_mng_save_obdh_data_to_fram(&sat_data_buf.obdh) != 0)
                    {
                        error_counter++;
                    }
                }
                else
                {
                    error_counter++;
                }
            }
        }
        else
        {
            error_counter++;
        }
    }
#endif /* CONFIG_DEV_MEDIA_FRAM_ENABLED */

#if defined(CONFIG_DEV_MEDIA_NOR_ENABLED) && (CONFIG_DEV_MEDIA_NOR_ENABLED == 1)
    /* NOR memory initialization */
    if (media_init(MEDIA_NOR) != 0)
    {
        error_counter++;
    }
#endif /* CONFIG_DEV_MEDIA_NOR_ENABLED */

#if defined(CONFIG_DEV_LEDS_ENABLED) && (CONFIG_DEV_LEDS_ENABLED == 1)
    /* LEDs device initialization */
    if (leds_init() != 0)
    {
        error_counter++;
    }
#endif /* CONFIG_DEV_LEDS_ENABLED */

#if defined(CONFIG_DEV_CURRENT_SENSOR_ENABLED) && (CONFIG_DEV_CURRENT_SENSOR_ENABLED == 1)
    /* Current sensor device initialization */
    if (current_sensor_init() != 0)
    {
        error_counter++;
    }
#endif /* CONFIG_DEV_CURRENT_SENSOR_ENABLED */

#if defined(CONFIG_DEV_VOLTAGE_SENSOR_ENABLED) && (CONFIG_DEV_VOLTAGE_SENSOR_ENABLED == 1)
    /* Voltage sensor device initialization */
    if (voltage_sensor_init() != 0)
    {
        error_counter++;
    }
#endif /* CONFIG_DEV_VOLTAGE_SENSOR_ENABLED */

#if defined(CONFIG_DEV_TEMP_SENSOR_ENABLED) && (CONFIG_DEV_TEMP_SENSOR_ENABLED == 1)
    /* Temperature sensor device initialization */
    if (temp_sensor_init() != 0)
    {
        error_counter++;
    }
#endif /* CONFIG_DEV_TEMP_SENSOR_ENABLED */

#if defined(CONFIG_DEV_EPS_ENABLED) && (CONFIG_DEV_EPS_ENABLED == 1)
    /* EPS device initialization */
    if (eps_init() != 0)
    {
        error_counter++;
    }
#endif /* CONFIG_DEV_EPS_ENABLED */

#if defined(CONFIG_DEV_PAYLOAD_EDC_ENABLED) && (CONFIG_DEV_PAYLOAD_EDC_ENABLED == 1)
    /* Payload EDC device initialization */
    if (payload_init(PAYLOAD_EDC_1) != 0)
    {
        error_counter++;
    }

    if (payload_disable(PAYLOAD_EDC_1) != 0)
    {
        error_counter++;
    }

    if (payload_init(PAYLOAD_EDC_0) != 0)
    {
        error_counter++;
    }
#endif /* CONFIG_DEV_PAYLOAD_EDC_ENABLED */

#if defined(CONFIG_DEV_ANTENNA_ENABLED) && (CONFIG_DEV_ANTENNA_ENABLED == 1)
    /* Antenna device initialization */
    if (antenna_init() != 0)
    {
        error_counter++;
    }
#endif /* CONFIG_DEV_ANTENNA_ENABLED */

#if defined(CONFIG_DEV_PAYLOAD_X_ENABLED) && (CONFIG_DEV_PAYLOAD_X_ENABLED == 1)
    /* Payload X device initialization */
    if (payload_init(PAYLOAD_X) != 0)
    {
        error_counter++;
    }
#endif /* CONFIG_DEV_PAYLOAD_X_ENABLED */

    if (error_counter > 0U)
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

/** \} End of startup group */
