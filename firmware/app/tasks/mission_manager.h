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
 * \version 1.0.0
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
#include <structs/obdh_data.h>
#include <system/system.h>
#include <conops/conops.h>

#define TASK_MISSION_MANAGER_NAME                      "Mission Manager"      /**< Task name. */
#define TASK_MISSION_MANAGER_STACK_SIZE                1024U                  /**< Stack size in bytes. */
#define TASK_MISSION_MANAGER_PRIORITY                  4U                     /**< Task priority. */
#define TASK_MISSION_MANAGER_EV_NOTIFICATION_TIMEOUT   (3600000UL)            /**< Task maximum block time waiting for event notifications */
#define TASK_MISSION_MANAGER_STARTUP_TIMEOUT_MS        (5000UL)               /**< Task startup event group notification timeout */

/* Mission Specific Macros */
#if defined (CONFIG_MISSION_GOLDS_UFSC) && (CONFIG_MISSION_GOLDS_UFSC == 1)
    #define MISSION_OPERATION_MODES                        7U

    /* Mission Operation modes */
    #define OBDH_MODE_DEPLOYMENT                           0x00U
    #define OBDH_MODE_COMMISSION                           0x01U
    #define OBDH_MODE_NORMAL                               0x02U
    #define OBDH_MODE_STAND_BY                             0x03U
    #define OBDH_MODE_EXPERIMENT                           0x04U
    #define OBDH_MODE_FDIR                                 0x05U
    #define OBDH_MODE_MANUAL                               0x06U
    
    /* Mission Event Masking */
    #define EV_ADD_CRIT_BITMASK(ev)                        ((ev) | 0x8000U)
    #define EV_CHECK_CRIT_BITMASK(ev)                      (((ev) & 0x8000U) != 0U)
    
    /* Mission Events */
    #define EV_PERSIST_STATE_ON_INIT                       (0x0000U)                                  /**< Used for the state machine initialization */   
    #define EV_IN_BRAZIL                                   (0x0001U)                                  /**< In Brazil notification flag */   
    #define EV_OUT_OF_BRAZIL                               (0x0002U)                                  /**< Outside Brazil notification flag */
    #define EV_PX_FINISHED                                 (0x0003U)                                  /**< Payload X experiment period finished flag */ 
    #define EV_HIBERNATION_TIMEOUT                         (0x0004U)                                  /**< Notifies that hibernation time has ended */  
    #define EV_BATTERY_LEVEL_CRITICAL                      (0x0005U)                                  /**< Satellite battery is on critical levels */  
    #define EV_FDIR_RESOLVED                               (0x0006U)                                  /**< FDIR condition was resolved */  
    #define EV_COMISSION_TIMEOUT                           (0x0007U)                                  /**< Commission Mode timedout */  
    #define EV_DEPLOYMENT_COMPLETE                         (0x0008U)                                  /**< Antenna Deployment complete */  
    #define EV_TC_GOTO_MANUAL_MODE                         (EV_ADD_CRIT_BITMASK(OBDH_MODE_MANUAL))
    #define EV_TC_GOTO_STAND_BY_MODE                       (EV_ADD_CRIT_BITMASK(OBDH_MODE_STAND_BY))
    #define EV_TC_GOTO_NORMAL_MODE                         (EV_ADD_CRIT_BITMASK(OBDH_MODE_NORMAL))
    #define EV_TC_GOTO_FDIR_MODE                           (EV_ADD_CRIT_BITMASK(OBDH_MODE_FDIR))
    #define EV_TC_GOTO_DEPLOYMENT_MODE                     (EV_ADD_CRIT_BITMASK(OBDH_MODE_DEPLOYMENT))
    #define EV_TC_GOTO_COMMISSION_MODE                     (EV_ADD_CRIT_BITMASK(OBDH_MODE_COMMISSION))
    #define EV_TC_GOTO_EXPERIMENT_MODE                     (EV_ADD_CRIT_BITMASK(OBDH_MODE_EXPERIMENT))
    #define EV_TC_ENABLE_PAYLOAD                           (EV_ADD_CRIT_BITMASK(0x0010U))             /**< Enables payload specified on 'src' field from event */
    #define EV_TC_DISABLE_PAYLOAD                          (EV_ADD_CRIT_BITMASK(0x0020U))             /**< Disables payload specified on 'src' field from event */
    #define EV_TC_ENTER_HIBERNATION                        (EV_ADD_CRIT_BITMASK(0x0030U))             /**< Enter hibernation for a number of hours specified on 'src' field from event */
    #define EV_TC_LEAVE_HIBERNATION                        (EV_ADD_CRIT_BITMASK(0x0040U))

    /* Payload X defines */
    #define PAYLOAD_X_EXPERIMENT_PERIOD_MS                 (600000UL)             /**< Payload X default experiment time in milliseconds. */
    #define PAYLOAD_X_CANCEL_EXPERIMENT_FLAG               (1UL << 31UL)          /**< Flag used as a notification to cancel running experiment */

    /* Commission Mode defines */
    #define COMMISSION_MODE_TIMEOUT_SEC                    (1209600UL)            /**< Max time allowed on commission (Can be overwritten through system parameters) */

#endif

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
void vTaskMissionManager(void* p);

/**
 * \brief Sends a event notification to the Mission Manager Task.
 *
 * \param[in] ev is the event to send to the Mission Manager Task.
 *
 * \return The status/error code.
 */
int8_t notify_event_to_mission_manager(const struct conops_event *ev);

#endif 

/** \} End of mission_man group */
