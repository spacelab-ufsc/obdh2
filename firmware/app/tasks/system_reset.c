/*
 * system_reset.c
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
 * along with OBDH 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Periodic system reset task implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.47
 * 
 * \date 2020/01/12
 * 
 * \addtogroup system_reset
 * \{
 */

#include <system/system.h>
#include <system/sys_log/sys_log.h>

#include "system_reset.h"

xTaskHandle xTaskSystemResetHandle;

void vTaskSystemReset(void)
{
    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS_64(TASK_SYSTEM_RESET_PERIOD_MS));

        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_SYSTEM_RESET_NAME, "Restarting the system...");
        sys_log_new_line();

        system_reset();
    }
}

/** \} End of system_reset group */
