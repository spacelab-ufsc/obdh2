/*
 * sched_tc.h
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
 * \defgroup sched_tc  Scheduled TC
 * \ingroup tasks
 * \{
 */

#ifndef SCHED_TC_H_
#define SCHED_TC_H_

#include <stdint.h>

#include <FreeRTOS.h>
#include <task.h>

#define TASK_SCHED_TC_NAME                      "Sched-TC"             /**< Task name. */
#define TASK_SCHED_TC_STACK_SIZE                1024U                  /**< Stack size in bytes. */
#define TASK_SCHED_TC_PRIORITY                  2U                     /**< Task priority. */
#define TASK_SCHED_TC_PERIOD_MS                 1000U                  /**< Task period. */
#define TASK_SCHED_TC_STARTUP_TIMEOUT_MS        (5000UL)               /**< Task startup event group notification timeout */

#define TC_QUEUE_MUTEX_TIMEOUT_MS               10000UL                /**< TC Queue mutex max wait time */

/**
 * \brief Scheduled TC Execution Task Handle.
 */
extern TaskHandle_t xTaskSchedTCHandle;

/**
 * \brief Scheduled TC Execution Task.
 *
 * \return None.
 */
void vTaskSchedTC(void* p);

/**
 * \brief Resets TC Queue.
 *
 * \return None.
 */
void reset_sched_tc_queue(void);

/**
 * \brief Saves TC Queue to FRAM.
 *
 * \return The status/error code.
 */
int save_sched_tc_queue_to_fram(void);

/**
 * \brief Schedule TC.
 *
 * \param[in] pkt is the received 'Schedule TC' telecommand packet.
 *
 * \param[in] pkt_len is the packet size.
 *
 * \return The status/error code.
 */
int schedule_tc(uint8_t *pkt, uint16_t pkt_size);

#endif 

/** \} End of sched_tc group */
