/*
 * op_ctrl.h
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
 * \brief Operation Control Task definition
 * 
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 0.10.19
 * 
 * \date 2024/08/04
 * 
 * \defgroup op_ctrl Operation Control
 * \ingroup tasks
 * \{
 */

#ifndef OP_CTRL_H_
#define OP_CTRL_H_

#include <stdint.h>

#include <FreeRTOS.h>
#include <task.h>

#include <structs/satellite.h>
#include <system/system.h>

#define TASK_OP_CTRL_NAME                      "Operation Control"    /**< Task name. */
#define TASK_OP_CTRL_STACK_SIZE                256U                   /**< Stack size in bytes. */
#define TASK_OP_CTRL_PRIORITY                  4U                     /**< Task priority. */
#define TASK_OP_CTRL_PERIOD_MS                 30000UL                /**< Task period in milliseconds. */
#define TASK_OP_CTRL_INIT_TIMEOUT_MS           2000U                  /**< Task priority. */

#define PAYLOAD_X_EXPERIMENT_PERIOD_MS         (600000UL)             /**< Payload X default experiment time in milliseconds. */
#define PAYLOAD_X_CANCEL_EXPERIMENT_FLAG       (1UL << 31UL)          /**< Flag used as a notification to cancel running experiment */

#define SAT_NOTIFY_IN_BRAZIL                   (1UL << 0UL)           /**< In Brazil notification flag */   
#define SAT_NOTIFY_OUT_OF_BRAZIL               (1UL << 1UL)           /**< Outside Brazil notification flag */
#define SAT_NOTIFY_PX_FINISHED                 (1UL << 2UL)           /**< Payload X experiment period finished */ 

/**
 * \brief Changes satellite's operation mode.
 *
 * \param[in] mode is the satellite mode to enter.
 *
 * \return None.
 */
static inline void satellite_change_mode(uint8_t mode) 
{
    sat_data_buf.obdh.data.mode = mode;
    sat_data_buf.obdh.data.ts_last_mode_change = system_get_time();
}

/**
 * \brief Operation Control Task Handle
 */
extern TaskHandle_t xTaskOpCtrlHandle;

/**
 * \brief Operation Control Task 
 *
 * \return None.
 */
void vTaskOpCtrl(void);

/**
 * \brief Sends a notification to the Operation Control Task.
 *
 * \param[in] notification_flag is the notification flag to send 
 * to the Operation Control Task. A example of a flag would be 
 * SAT_NOTIFY_IN_BRAZIL.
 *
 * \return None.
 */
void notify_op_ctrl(uint32_t notification_flag);

#endif 

/** \} End of op_ctrl group */
