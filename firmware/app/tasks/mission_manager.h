/*
 * mission_manager.h
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
 * \brief Mission Manager Task definition
 * 
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 0.10.19
 * 
 * \date 2024/08/04
 * 
 * \defgroup mission_man  Mission Manager
 * \ingroup tasks
 * \{
 */

#ifndef MISSION_MANAGER_H_
#define MISSION_MANAGER_H_

#include <stdint.h>

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

#include <structs/satellite.h>
#include <system/system.h>

#define TASK_MISSION_MANAGER_NAME                      "Mission Man"          /**< Task name. */
#define TASK_MISSION_MANAGER_STACK_SIZE                512U                   /**< Stack size in bytes. */
#define TASK_MISSION_MANAGER_PRIORITY                  4U                     /**< Task priority. */
#define TASK_MISSION_MANAGER_EV_NOTIFICATION_TIMEOUT   (3600000UL)            /**< Task maximum block time waiting for event notifications */

#define PAYLOAD_X_EXPERIMENT_PERIOD_MS                 (600000UL)             /**< Payload X default experiment time in milliseconds. */
#define PAYLOAD_X_CANCEL_EXPERIMENT_FLAG               (1UL << 31UL)          /**< Flag used as a notification to cancel running experiment */

#define HOUSEKEEPING_WAKE_UP_RQ                        0xAE                   /**< Used to check which task sent a wake up request */  

#define EV_NOTIFY_IN_BRAZIL                            0                      /**< In Brazil notification flag */   
#define EV_NOTIFY_OUT_OF_BRAZIL                        1                      /**< Outside Brazil notification flag */
#define EV_NOTIFY_PX_FINISHED                          2                      /**< Payload X experiment period finished flag */ 
#define EV_NOTIFY_MODE_CHANGE_RQ                       3                      /**< A request to change operation mode was made through TC */ 
#define EV_NOTIFY_ACTIVATE_PAYLOAD_RQ                  4                      /**< A request to activate payload was made through TC */ 
#define EV_NOTIFY_DEACTIVATE_PAYLOAD_RQ                5                      /**< A request to deactivate payload was made through TC */ 


/**
 * \brief Event type. Used to pass events to the Mission Manager Task 
 * from other tasks for processing.
 */
typedef struct 
{
    uint8_t event;      /**< Event to process. [Ex: EV_NOTIFY_IN_BRAZIL] */
    uint8_t args[3];    /**< Arguments used to describe event properties. */
} event_t;

/**
 * \brief Changes satellite's operation mode.
 *
 * \param[in] mode is the satellite mode to enter.
 *
 * \return None.
 */
void satellite_change_mode(const uint8_t mode);

/**
 * \brief Mission Manager Task Handle.
 */
extern TaskHandle_t xTaskMissionManagerHandle;

/**
 * \brief Operation Control event queue handle.
 */
extern QueueHandle_t event_queue;

/**
 * \brief Operation Control Task 
 *
 * \return None.
 */
void vTaskMissionManager(void);

/**
 * \brief Sends a event notification to the Mission Manager Task.
 *
 * \param[in] ev is the event to send to the Mission Manager Task.
 * A example of a event would be `{ .event = EV_NOTIFY_IN_BRAZIL, .args[0] = 0U, .args[1] = 0U, .args[2] = 0U }`.
 *
 * \return The status/error code.
 */
int8_t notify_event_to_mission_manager(const event_t *ev);

/**
 * \brief Overrides current operation mode.
 *
 * \param[in] mode is the operation mode to enter.
 *
 * \return The status/error code.
 */
int8_t override_op_mode(const uint8_t mode);

#endif 

/** \} End of mission_man group */
