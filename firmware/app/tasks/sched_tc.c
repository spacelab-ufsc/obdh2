/*
 * sched_tc.c
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
 * \brief Scheduled TC Execution Task definition.
 * 
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 1.0.0
 * 
 * \date 2025/04/12
 * 
 * \addtogroup sched_tc
 * \{
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include <projdefs.h>
#include <structs/satellite.h>
#include <structs/obdh_data.h>
#include <devices/payload/payload.h>
#include <conops/conops.h>
#include <conops/util/cmd_queue.h>
#include <system/sys_log/sys_log.h>
#include <system/system.h>
#include <utils/mem_mng.h>
#include <config/config.h>

#include "sched_tc.h"
#include "process_tc.h"
#include "startup.h"

TaskHandle_t xTaskSchedTCHandle;

static struct conops_cmd_queue tc_queue;

static void tc_queue_lock(void *lock)
{
    SemaphoreHandle_t sem = *(SemaphoreHandle_t*)lock; // cppcheck-suppress misra-c2012-11.2

    if (xSemaphoreTake(sem, pdMS_TO_TICKS(TC_QUEUE_MUTEX_TIMEOUT_MS)) == pdFALSE)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_SCHED_TC_NAME, "Failed to acquire TC Queue lock!");
        sys_log_new_line();
    }
}

static void tc_queue_unlock(void *lock)
{
    SemaphoreHandle_t sem = *(SemaphoreHandle_t*)lock; // cppcheck-suppress misra-c2012-11.2

    (void)xSemaphoreGive(sem);
}

void reset_sched_tc_queue(void)
{
    cmd_queue_reset(&tc_queue);
}

int save_sched_tc_queue_to_fram(void)
{
    return mem_mng_save_tc_queue_to_fram(&tc_queue);
}

int schedule_tc(uint8_t *pkt, uint16_t pkt_size)
{
    struct conops_cmd tc = {0};

    tc.timestamp = ((uint32_t)pkt[8] << 24U) |
                   ((uint32_t)pkt[9] << 16U) |
                   ((uint32_t)pkt[10] << 8U)  |
                   ((uint32_t)pkt[11] << 0U);

    /* Remove packet ID, callsign, timestamp and hmac hash from the packet size before copying */
    (void)memcpy(tc.payload, &pkt[12], pkt_size - 1U - 7U - 4U - 20U); 

    return cmd_queue_enqueue(&tc_queue, &tc);
}

void vTaskSchedTC(void* p)
{
    (void)p;

    SemaphoreHandle_t xSemQueuelock = NULL;
    uint16_t qsize = 0U;
    int err = 0;

    (void)xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_SCHED_TC_STARTUP_TIMEOUT_MS));

    if (mem_mng_load_tc_queue_from_fram(&tc_queue) == 0)
    {
        qsize = tc_queue.size;
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_SCHED_TC_NAME, "Failed to load TC queue from FRAM!");
        sys_log_new_line();
    }

    xSemQueuelock = xSemaphoreCreateMutex();

    if (xSemQueuelock != NULL)
    {
        cmd_queue_init(&tc_queue, &xSemQueuelock, tc_queue_lock, tc_queue_unlock);
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_WARNING, TASK_SCHED_TC_NAME, "Failed to create TC Queue lock! Using lockless impl...");
        sys_log_new_line();

        cmd_queue_init(&tc_queue, NULL, NULL, NULL);
    }

    /* The cmd_queue_init function resets the queue size, thus this step is needed to preserve its size */
    if (qsize != 0U)
    {
        tc_queue.size = qsize;
    }

    TickType_t last_cycle = xTaskGetTickCount();

    while (1)
    {
        struct conops_cmd tc = {0};

        uint32_t now = system_get_time(); 

        while (cmd_queue_dequeue(&tc_queue, now, &tc) == 1)
        {
            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_SCHED_TC_NAME, "Executing TC scheduled for: ");
            sys_log_print_uint(tc.timestamp);
            sys_log_new_line();

            err = execute_tc(tc.payload, sizeof(tc.payload), true);

            if (err != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_SCHED_TC_NAME, "Invalid TC was scheduled!");
                sys_log_new_line();
            }
        }

        sat_data_buf.obdh.data.tc_queue_size = (uint8_t)tc_queue.size;
        sat_data_buf.obdh.data.ts_next_sched_tc = (tc_queue.size > 0U) ? ((uint32_t)tc_queue.buffer[0].timestamp) : (0U);

        vTaskDelayUntil(&last_cycle, pdMS_TO_TICKS(TASK_SCHED_TC_PERIOD_MS));
    }
}

/** \} End of sched_tc group */
