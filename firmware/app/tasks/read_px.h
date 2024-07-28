/*
 * read_px.h
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
 * \brief Read PX task implementation.
 *
 * \author Augusto Cezar Boldori Vassoler <augustovassoler@gmail.com>
 *
 * \version 0.0.1
 *
 * \date 2023/08/28
 *
 * \addtogroup read_px
 * \{
 */

#ifndef READ_PX_H_
#define READ_PX_H_

#include <FreeRTOS.h>
#include <task.h>

#define TASK_READ_PX_NAME                  "PX Task"           /**< Task name. */
#define TASK_READ_PX_STACK_SIZE            300                 /**< Stack size in bytes. */
#define TASK_READ_PX_PRIORITY              3                   /**< Task priority. */
#define TASK_READ_PX_PERIOD_MS             (60000)             /**< Task period in milliseconds. */
#define TASK_READ_PX_INIT_TIMEOUT_MS       2000                /**< Wait time to initialize the task in milliseconds. */

#define PX_PONG_BUF_SIZE                   4;                 /**< Size of pong response message. */
/**
 * \brief PX data type.
 */
typedef struct
{
    uint8_t buffer[4];
    int32_t length;
} pl_px_buf_t;

/**
 * \brief PX read data buffer.
 */
extern pl_px_buf_t px_buf;

/**
 * \brief Read PX handle.
 */
extern xTaskHandle xTaskReadPXHandle;

/**
 * \brief Read PX task.
 *
 * \return None.
 */
void vTaskReadPX(void);

#endif /* READ_PX_H_ */
