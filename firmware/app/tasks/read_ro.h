/*
 * read_ro.h
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
 * \author Renato Augusto Schenkel Meneghin Marchiori <renato.meneghin@posgrad.ufsc.br>
 * 
 * \version 0.0.1
 * 
 * \date 2026/09/01
 * 
 * \defgroup read_ro Read RO
 * \ingroup tasks
 * \{
 */

#ifndef READ_RO_H_
#define READ_RO_H_

#include <FreeRTOS.h>
#include <task.h>
#include <drivers/ro/ro.h>

#define TASK_READ_RO_NAME                  "RO Task"           /**< Task name. */
#define TASK_READ_RO_STACK_SIZE            300                 /**< Stack size in bytes. */
#define TASK_READ_RO_PRIORITY              3                   /**< Task priority. */
#define TASK_READ_RO_PERIOD_MS             (60000)             /**< Task period in milliseconds. */
#define TASK_READ_RO_INIT_TIMEOUT_MS       5000                /**< Wait time to initialize the task in milliseconds. */
#define TASK_READ_RO_INITIAL_DELAY_MS      15000               /**< Delay, in milliseconds, before the first execution. */
/**
 * \brief RO housekeeping raw data type.
 */
typedef struct
{
    uint8_t buffer[sizeof(ro_hk_t)];
    int32_t length;
} pl_ro_hk_raw_t;

/*
 * \brief RO telemetry data type.
 */
typedef struct
{
    sys_time_t timestamp;           /**< Timestamp of the Payload data. */
    uint8_t id;                     /**< Payload id [Ex: CONFIG_PL_ID_EDC_1] */
    ro_hk_t hk;                    /**< RO Housekeeping */
    //ro_state_t state;              /**< RO State */
} ro_telemetry_t;

/**
 * \brief RO payload state type.
 */
typedef struct
{
    ro_telemetry_t *c_ro;     /**< Pointer to the active RO telemetry data. */
} ro_payload_state_t;

/**
 * \brief RO housekeeping data buffer.
 */
extern pl_ro_hk_raw_t ro_hk_buf;

/**
 * \brief Read RO Handle
 */
extern XTaskHandle xTaskReadROHandle;

/**
 * \brief Read RO task function.
 * 
 * \return None
 */
void xTaskReadRO(void *p);

#endif /* READ_RO_H_ */

/** \} End of read_ro Group */