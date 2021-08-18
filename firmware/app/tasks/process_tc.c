/*
 * process_tc.c
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
 * \brief Process TC task implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.7
 * 
 * \date 2021/07/06
 * 
 * \addtogroup process_tc
 * \{
 */

#include <config/config.h>

#include <system/sys_log/sys_log.h>
#include <devices/ttc/ttc.h>

#include "process_tc.h"
#include "startup.h"

xTaskHandle xTaskProcessTCHandle;

void vTaskProcessTC(void *pvParameters)
{
    /* Wait startup task to finish */
    xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_PROCESS_TC_INIT_TIMEOUT_MS));

    /* Delay before the first cycle */
    vTaskDelay(pdMS_TO_TICKS(TASK_PROCESS_TC_INITIAL_DELAY_MS));

    while(1)
    {
        TickType_t last_cycle = xTaskGetTickCount();

        int pkts = ttc_avail(TTC_1);

        if (pkts > 0)
        {
            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "New ");
            sys_log_print_uint((unsigned int)pkts);
            sys_log_print_msg(" packet(s) available!");
            sys_log_new_line();

            uint8_t pkt[300] = {0xFF};
            uint16_t pkt_len = 0;
            if (ttc_recv(TTC_1, pkt, &pkt_len) != 0)
            {
                switch(pkt[0])
                {
                    case CONFIG_PKT_ID_UPLINK_PING_REQ:
                        break;
                    case CONFIG_PKT_ID_UPLINK_DATA_REQ:
                        break;
                    case CONFIG_PKT_ID_UPLINK_BROADCAST_MSG:
                        break;
                    case CONFIG_PKT_ID_UPLINK_ENTER_HIBERNATION:
                        break;
                    case CONFIG_PKT_ID_UPLINK_LEAVE_HIBERNATION:
                        break;
                    case CONFIG_PKT_ID_UPLINK_ACTIVATE_MODULE:
                        break;
                    case CONFIG_PKT_ID_UPLINK_DEACTIVATE_MODULE:
                        break;
                    case CONFIG_PKT_ID_UPLINK_ACTIVATE_PAYLOAD:
                        break;
                    case CONFIG_PKT_ID_UPLINK_DEACTIVATE_PAYLOAD:
                        break;
                    case CONFIG_PKT_ID_UPLINK_ERASE_MEMORY:
                        break;
                    case CONFIG_PKT_ID_UPLINK_FORCE_RESET:
                        break;
                    case CONFIG_PKT_ID_UPLINK_GET_PAYLOAD_DATA:
                        break;
                    default:
                        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Unknow packet received!");
                        sys_log_new_line();
                }
            }
        }

        vTaskDelayUntil(&last_cycle, pdMS_TO_TICKS(TASK_PROCESS_TC_PERIOD_MS));
    }
}

/** \} End of process_tc group */
