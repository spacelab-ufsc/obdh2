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
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 1.0.0
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
#include <utils/mem_mng.h>

#include "startup.h"
#include "mission_manager.h"

#define MEDIA_INIT_MAX_RETRY    3 

xTaskHandle xTaskStartupHandle;

EventGroupHandle_t task_startup_status;

static int media_nor_clean(void)
{
    int err = 0;
    int die0_err = -1;
    int die1_err = -1;
    uint8_t retries = 5U;

    do 
    {
        if (die0_err != 0)
        {
            die0_err = media_erase(MEDIA_NOR, MEDIA_ERASE_DIE, 0U);
        }

        if (die1_err != 0)
        {
             die1_err = media_erase(MEDIA_NOR, MEDIA_ERASE_DIE, 1U);
        }

        if ((die0_err != 0) || (die0_err != 0))
        {
            err = -1;
        }
        else
        {
            err = 0;
        }

        --retries;
    } while((err != 0) && (retries > 0U));

    return err;
}

static void sys_log_print_obdh_parameters(obdh_telemetry_t *params)
{
    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_STARTUP_NAME, "Operation Mode: ");
    sys_log_print_hex((uint32_t)params->data.mode);
    sys_log_new_line();

    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_STARTUP_NAME, "Antenna Deployment Params: deploy_counter (");
    sys_log_print_uint((uint32_t)params->data.ant_deployment_counter);
    sys_log_print_msg("), deploy_exec (");
    sys_log_print_uint((uint32_t)params->data.ant_deployment_executed);
    sys_log_print_msg("), init_hib_count (");
    sys_log_print_uint((uint32_t)params->data.initial_hib_time_count);
    sys_log_print_msg("), init_hib_exec (");
    sys_log_print_uint((uint32_t)params->data.initial_hib_executed);
    sys_log_print_msg(")");
    sys_log_new_line();

    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_STARTUP_NAME, "Used flash pages: ");
    sys_log_print_uint(params->data.media.last_page_obdh_data);
    sys_log_new_line();

    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_STARTUP_NAME, "Last flash update timestamp: ");
    sys_log_print_uint(params->timestamp);
    sys_log_new_line();

    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_STARTUP_NAME, "Main EDC: ");
    sys_log_print_hex((uint32_t)params->data.main_edc);
    sys_log_new_line();

    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_STARTUP_NAME, "Main payload state: ");
    sys_log_print_hex((uint32_t)params->data.main_payload_state);
    sys_log_new_line();

    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_STARTUP_NAME, "Secondary payload state: ");
    sys_log_print_hex((uint32_t)params->data.sec_payload_state);
    sys_log_new_line();

    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_STARTUP_NAME, "Battery Critical Level: ");
    sys_log_print_uint((uint32_t)params->data.batt_crit_level_mv);
    sys_log_print_msg(" mV");
    sys_log_new_line();

    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_STARTUP_NAME, "Operation flags: hibernation_on (");
    sys_log_print_uint((uint32_t)params->data.hibernation_on);
    sys_log_print_msg("), general_telemetry_on (");
    sys_log_print_uint((uint32_t)params->data.general_telemetry_on);
    sys_log_print_msg("), eps_beacon (");
    sys_log_print_uint((uint32_t)params->data.eps_beacon_on);
    sys_log_print_msg("), manual_experiments (");
    sys_log_print_uint((uint32_t)params->data.manual_experiments);
    sys_log_print_msg(")");
    sys_log_new_line();
}

void vTaskStartup(void *p)
{
    (void)p;

    unsigned int error_counter = 0;
    int err = -1;

    /* Logger device initialization */
    (void)sys_log_init();

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

#if defined(CONFIG_DEV_MEDIA_NOR_ENABLED) && (CONFIG_DEV_MEDIA_NOR_ENABLED == 1)
    /* NOR memory initialization */
    for (int i = 0; i < MEDIA_INIT_MAX_RETRY; ++i)
    {
        if (media_init(MEDIA_NOR) == 0)
        {
            err = 0;
            break;
        }
    }

    if (err != 0) 
    {
        error_counter++;
    }
    else 
    {
        err = -1;
    }
#endif /* CONFIG_DEV_MEDIA_NOR_ENABLED */

#if defined(CONFIG_DEV_MEDIA_FRAM_ENABLED) && (CONFIG_DEV_MEDIA_FRAM_ENABLED == 1)
    /* FRAM memory initialization */
    if (system_get_hw_version() >= (uint8_t)HW_VERSION_1)
    {
        for (int i = 0; i < MEDIA_INIT_MAX_RETRY; ++i)
        { // cppcheck-suppress misra-c2012-15.4
            if (media_init(MEDIA_FRAM) == 0)
            {
                /* Check if FRAM is initialized */
                if (mem_mng_check_fram() == 0)
                {
                    /* Load last saved OBDH data from FRAM */
                    if (mem_mng_load_obdh_data_from_fram(&sat_data_buf.obdh) == 0)
                    {
                        err = 0;

                        if (system_reset_count() == 0)
                        {
                            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_STARTUP_NAME, "Reset counter: ");
                            sys_log_print_uint((uint32_t)(sat_data_buf.obdh.data.reset_counter));
                            sys_log_new_line();
                        }
                        else 
                        {
                            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_STARTUP_NAME, "Failed to save the reset counter param");
                            sys_log_new_line();
                        }

                        break;
                    }
                    else 
                    {
                        /* Failed to read FRAM data or CRC was not valid */
                        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_STARTUP_NAME, "Failed to load OBDH data correctly!");
                        sys_log_new_line();

                        sys_log_print_event_from_module(SYS_LOG_WARNING, TASK_STARTUP_NAME, "Cleaning NOR memory...");
                        sys_log_new_line();

                        (void)media_nor_clean();

                        sys_log_print_event_from_module(SYS_LOG_WARNING, TASK_STARTUP_NAME, "Loading default values to memory...");
                        sys_log_new_line();

                        /* Load default values to the OBDH data buffer */
                        mem_mng_load_obdh_data_from_default_values(&sat_data_buf.obdh);

                        sys_log_print_event_from_module(SYS_LOG_WARNING, TASK_STARTUP_NAME, "Saving default values to FRAM...");
                        sys_log_new_line();

                        /* Write the OBDH data to the FRAM memory */
                        if (mem_mng_save_obdh_data_to_fram(&sat_data_buf.obdh) == 0)
                        {
                            err = 0;
                            break;
                        }
                    }
                }
                else
                {
                    sys_log_print_event_from_module(SYS_LOG_WARNING, TASK_STARTUP_NAME, "FRAM was not initialized in previous cycles!");
                    sys_log_new_line();

                    sys_log_print_event_from_module(SYS_LOG_WARNING, TASK_STARTUP_NAME, "Trying to clean NOR memory!");
                    sys_log_new_line();

                    (void)media_nor_clean();

                    /* Initialize FRAM */
                    if (mem_mng_init_fram() == 0)
                    {
                        sys_log_print_event_from_module(SYS_LOG_WARNING, TASK_STARTUP_NAME, "Loading default values to memory...");
                        sys_log_new_line();

                        /* Load default values to the OBDH data buffer */
                        mem_mng_load_obdh_data_from_default_values(&sat_data_buf.obdh);

                        sys_log_print_event_from_module(SYS_LOG_WARNING, TASK_STARTUP_NAME, "Saving default values to FRAM...");
                        sys_log_new_line();

                        /* Write the OBDH data to the FRAM memory */
                        if (mem_mng_save_obdh_data_to_fram(&sat_data_buf.obdh) == 0)
                        {
                            err = 0;
                            break;
                        }
                    }
                }
            }
        }

        if (err != 0) 
        {
            error_counter++;
        }
    }
#endif /* CONFIG_DEV_MEDIA_FRAM_ENABLED */

    /* FRAM initialization status = Done */
    (void)xEventGroupSetBits(task_startup_status, FRAM_INIT_DONE);

    /* Print OBDH parameters */
#if defined(CONFIG_PRINT_OBDH_PARAMS) && (CONFIG_PRINT_OBDH_PARAMS == 1)
    sys_log_print_obdh_parameters(&sat_data_buf.obdh);
#endif

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

    /* Payload enables initialization */
    if (payload_init_gpio_enables() < 0)
    {
        error_counter++;
    }

#if defined(CONFIG_DEV_PAYLOAD_EDC_ENABLED) && (CONFIG_DEV_PAYLOAD_EDC_ENABLED == 1)
    /* Payload EDC device initialization */
    sat_data_buf.edc_0.id = PL_ID_EDC_1;
    sat_data_buf.edc_1.id = PL_ID_EDC_2;

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

    if (payload_disable(PAYLOAD_EDC_0) != 0)
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
    sat_data_buf.payload_x.id = PL_ID_PAYLOAD_X;
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

        (void)led_set(LED_FAULT);
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_STARTUP_NAME, "Boot completed with SUCCESS!");
        sys_log_new_line();

        (void)led_clear(LED_FAULT);
    }

    sat_data_buf.obdh.data.hw_version = system_get_hw_version();

    sat_data_buf.obdh.data.last_reset_cause = system_get_reset_cause();

    /* Startup task status = Done */
    (void)xEventGroupSetBits(task_startup_status, TASK_STARTUP_DONE);

    vTaskSuspend(xTaskStartupHandle);
}

/** \} End of startup group */
