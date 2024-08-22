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
 * \version 0.10.19
 * 
 * \date 2021/04/27
 * 
 * \addtogroup housekeeping
 * \{
 */

#include <system/sys_log/sys_log.h>

#include <devices/current_sensor/current_sensor.h>
#include <devices/voltage_sensor/voltage_sensor.h>
#include <devices/temp_sensor/temp_sensor.h>
#include <utils/mem_mng.h>

#include <structs/satellite.h>

#include "housekeeping.h"
#include "op_ctrl.h"
#include "startup.h"

xTaskHandle xTaskHousekeepingHandle;

void vTaskHousekeeping(void)
{
    /* Wait startup task to finish */
    xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_HOUSEKEEPING_INIT_TIMEOUT_MS));

    TickType_t last_cycle = xTaskGetTickCount();

    while(1)
    {
        /* Hibernation mode check */
        if (sat_data_buf.obdh.data.mode == OBDH_MODE_HIBERNATION)
        {
            if ((sat_data_buf.obdh.data.ts_last_mode_change + sat_data_buf.obdh.data.mode_duration) <= system_get_time())
            {
                notify_op_ctrl(SAT_NOTIFY_LEAVE_MANUAL_MODE);
            }
        }

        /* Save the last available OBDH data at every minute */
        if (mem_mng_save_obdh_data_to_fram(&sat_data_buf.obdh) != 0)
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HOUSEKEEPING_NAME, "Error writing data to the FRAM memory!");
            sys_log_new_line();
        }
        else 
        {
            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_HOUSEKEEPING_NAME, "Saving obdh data to fram...");
            sys_log_new_line();
        }
        
        vTaskDelayUntil(&last_cycle, pdMS_TO_TICKS(TASK_HOUSEKEEPING_PERIOD_MS));
    }
}

/** \} End of housekeeping group */
