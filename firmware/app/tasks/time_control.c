/*
 * time_control.h
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
 * \brief Time control task implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.4.14
 * 
 * \date 2020/08/09
 * 
 * \addtogroup time_control
 * \{
 */

#include <system/system.h>
#include <system/sys_log/sys_log.h>
#include <devices/media/media.h>
#include <config/config.h>

#include "time_control.h"
#include "startup.h"

xTaskHandle xTaskTimeControlHandle;

void vTaskTimeControl(void *pvParameters)
{
    /* Wait startup task to finish */
    xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_TIME_CONTROL_INIT_TIMEOUT_MS));

    /* Load the last saved system time */
    sys_time_t last_sys_time = 0;

    if (media_read(MEDIA_INT_FLASH, CONFIG_MEM_ADR_SYS_TIME, &last_sys_time, 1) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_TIME_CONTROL_NAME, "Error reading the system time from the internal flash memory!");
        sys_log_new_line();
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_TIME_CONTROL_NAME, "Last saved system time (epoch): ");
        sys_log_print_uint(last_sys_time);
        sys_log_print_msg(" sec");
        sys_log_new_line();
    }

    system_set_time(last_sys_time);

    vTaskDelay(pdMS_TO_TICKS(1000));

    while(1)
    {
        TickType_t last_cycle = xTaskGetTickCount();

        system_increment_time();

        /* Read the current system time */
        sys_time_t sys_tm = system_get_time();

        if (sys_tm % 60 == 0)
        {
            /* Save the current system time */
            if (media_write(MEDIA_INT_FLASH, CONFIG_MEM_ADR_SYS_TIME, &sys_tm, 1))
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_TIME_CONTROL_NAME, "Error writing the system time to the internal flash memory!");
                sys_log_new_line();
            }
        }

        vTaskDelayUntil(&last_cycle, pdMS_TO_TICKS(TASK_TIME_CONTROL_PERIOD_MS));
    }
}

/** \} End of time_control group */
