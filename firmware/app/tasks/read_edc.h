/*
 * read_edc.c
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
 * \brief Read EDC data task definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 0.10.19
 * 
 * \date 2020/08/16
 * 
 * \defgroup read_edc Read EDC
 * \ingroup tasks
 * \{
 */

#ifndef READ_EDC_H_
#define READ_EDC_H_

#include <FreeRTOS.h>
#include <task.h>

#define TASK_READ_EDC_NAME                  "EDC Task"          /**< Task name. */
#define TASK_READ_EDC_STACK_SIZE            300                 /**< Stack size in bytes. */
#define TASK_READ_EDC_PRIORITY              3                   /**< Task priority. */
#define TASK_READ_EDC_PERIOD_MS             (60000)             /**< Task period in milliseconds. */
#define TASK_READ_EDC_INIT_TIMEOUT_MS       2000                /**< Wait time to initialize the task in milliseconds. */
#define TASK_READ_EDC_MAX_WAIT_TIME_MS      (60000UL * 60UL)    /**< Max time to wait for notifications. (1 hour)*/

/**
 * \brief EDC housekeeping raw data type.
 */
typedef struct
{
    uint8_t buffer[30];
    int32_t length;
} pl_edc_hk_raw_t;

/**
 * \brief EDC housekeepign data buffer.
 */
extern pl_edc_hk_raw_t edc_hk_buf;

/**
 * \brief Read EDC handle.
 */
extern xTaskHandle xTaskReadEDCHandle;

/**
 * \brief Read EDC task.
 *
 * \return None.
 */
void vTaskReadEDC(void);

#endif /* READ_EDC_H_ */

/** \} End of read_edc group */
