/*
 * process_tc.h
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
 * along with OBDH 2.0. If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * \brief Process TC task definition.
 *
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 *
 * \version 1.0.0
 *
 * \date 2021/07/06
 *
 * \defgroup process_tc Process TC
 * \ingroup tasks
 * \{
 */

#ifndef PROCESS_TC_H_
#define PROCESS_TC_H_

#include <stdbool.h>

#include <FreeRTOS.h>
#include <task.h>

#define TASK_PROCESS_TC_NAME                "Process TC"        /**< Task name. */
#define TASK_PROCESS_TC_STACK_SIZE          2048                /**< Stack size in bytes. */
#define TASK_PROCESS_TC_PRIORITY            4                   /**< Task priority. */
#define TASK_PROCESS_TC_PERIOD_MS           1000                /**< Task period in milliseconds. */
#define TASK_PROCESS_TC_INITIAL_DELAY_MS    6500                /**< Delay, in milliseconds, before the first execution. */
#define TASK_PROCESS_TC_INIT_TIMEOUT_MS     10000               /**< Wait time to initialize the task in milliseconds. */
#define TASK_PROCESS_TC_MAX_WAIT_TIME_MS    (5000)              /**< Max wait time to received a confirmation notification from mission manager in milliseconds. */

#define MISSION_MANAGER_NOTIFICATION_BIT    (1U << 2U)          /**< Mission Manager notification bit on event group. */

/**
 * \brief Process TC handle.
 */
extern xTaskHandle xTaskProcessTCHandle;

/**
 * \brief Process TC task.
 *
 * \return None.
 */
void vTaskProcessTC(void* p);

/**
 * \brief Executes/Processes a telecommand.
 *
 * \param[in] pkt is the packet to process.
 *
 * \param[in] pkt_len is the number of bytes of the given packet.
 *
 * \param[in] is_scheduled is a flag to mark if a telecommand packet
 * was scheduled instead of just received. This is needed because in a
 * scheduled TC there is no need to authenticate it, since it was
 * authenticated on queue insertion.
 *
 * \return The status/error code.
 */
int execute_tc(uint8_t *pkt, uint8_t pkt_len, bool is_scheduled);

#endif /* PROCESS_TC_H_ */

/** \} End of process_tc group */
