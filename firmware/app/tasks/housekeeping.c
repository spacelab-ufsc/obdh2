/*
 * housekeeping.c
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
 * \brief Housekeeping task implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 1.0.0
 * 
 * \date 2021/04/27
 * 
 * \addtogroup housekeeping
 * \{
 */

#include <system/system.h>
#include <conops/conops.h>
#include <devices/eps/eps.h>
#include <system/sys_log/sys_log.h>

#include <devices/current_sensor/current_sensor.h>
#include <devices/voltage_sensor/voltage_sensor.h>
#include <devices/temp_sensor/temp_sensor.h>
#include <utils/mem_mng.h>

#include <structs/satellite.h>

#include "housekeeping.h"
#include "mission_manager.h"
#include "startup.h"
#include "sched_tc.h"

xTaskHandle xTaskHousekeepingHandle;

void vTaskHousekeeping(void *p)
{
    (void)p;

    /* Wait startup task to finish */
    (void)xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_HOUSEKEEPING_INIT_TIMEOUT_MS));

    TickType_t last_cycle = xTaskGetTickCount();

    while(1)
    {
        /* Hibernation mode check */
        if ((sat_data_buf.obdh.data.hibernation_on) && (sat_data_buf.obdh.data.mode != OBDH_MODE_DEPLOYMENT))
        {
            uint32_t hib = sat_data_buf.obdh.data.hib_duration;

            if (hib > 0U) 
            {
                if (hib <= 60U) {
                    taskENTER_CRITICAL();
                    sat_data_buf.obdh.data.hib_duration = 0U;
                    taskEXIT_CRITICAL();
                } else {
                    hib -= 60U;
                    taskENTER_CRITICAL();
                    sat_data_buf.obdh.data.hib_duration = hib;
                    taskEXIT_CRITICAL();
                }
            }

            if (sat_data_buf.obdh.data.hib_duration == 0U)
            {
                const struct conops_event leave_hib = {
                    .callback = NULL,
                    .ev_name = "WAKE-UP",
                    .src = 0U,
                    .ev_id = EV_HIBERNATION_TIMEOUT,
                };

                if (notify_event_to_mission_manager(&leave_hib) != 0)
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HOUSEKEEPING_NAME, "Failed to notify WAKE UP event");
                    sys_log_new_line();
                }
            }
        }

        vTaskDelay(pdMS_TO_TICKS(100U));

        if (sat_data_buf.obdh.data.mode == OBDH_MODE_COMMISSION)
        {
            if (system_get_time() >= sat_data_buf.obdh.data.ts_commission_timeout)
            {
                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_HOUSEKEEPING_NAME, "Commission Mode timedout! Notifying Mission Manager...");
                sys_log_new_line();

                const struct conops_event commission_timeout = {
                    .callback = NULL,
                    .ev_name = "COMM-TIME",
                    .src = 0U,
                    .ev_id = EV_COMISSION_TIMEOUT,
                };

                if (notify_event_to_mission_manager(&commission_timeout) != 0)
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HOUSEKEEPING_NAME, "Failed to notify Commission Timeout event");
                    sys_log_new_line();
                }
            }
        }

        /* Save the last available OBDH data at every minute */
        if (mem_mng_save_obdh_data_to_fram(&sat_data_buf.obdh) == 0)
        {
            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_HOUSEKEEPING_NAME, "Saved OBDH data to FRAM!");
            sys_log_new_line();
        }
        else 
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HOUSEKEEPING_NAME, "Error writing data to FRAM!");
            sys_log_new_line();
        }

        vTaskDelay(pdMS_TO_TICKS(50U));

        uint32_t eps_beacon_state = UINT32_MAX;

        if (eps_get_param(SL_EPS2_REG_BEACON_ENABLE, &eps_beacon_state) == 0)
        {
            if (eps_beacon_state != (uint32_t)sat_data_buf.obdh.data.eps_beacon_on)
            {
                int err = 0;
                uint8_t retry_count = 5U;

                do 
                {
                    err = eps_set_param(SL_EPS2_REG_BEACON_ENABLE, (uint32_t)sat_data_buf.obdh.data.eps_beacon_on);
                    vTaskDelay(100U);
                    --retry_count;
                } while ((err < 0) && (retry_count > 0U));

                if (retry_count == 0U)
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HOUSEKEEPING_NAME, "Failed to update EPS beacon state!");
                    sys_log_new_line();
                }
            }
        }
         
        vTaskDelay(pdMS_TO_TICKS(50U));

        /* Save the last available TC Queue at every minute */
        if (save_sched_tc_queue_to_fram() == 0)
        {
            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_HOUSEKEEPING_NAME, "Saved TC Queue to FRAM!");
            sys_log_new_line();
        }
        else 
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HOUSEKEEPING_NAME, "Error saving TC Queue to FRAM!");
            sys_log_new_line();
        }

        vTaskDelayUntil(&last_cycle, pdMS_TO_TICKS(TASK_HOUSEKEEPING_PERIOD_MS));
    }
}

/** \} End of housekeeping group */
