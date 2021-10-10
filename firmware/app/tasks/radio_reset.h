/*
 * radio_reset.h
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
 * \brief Periodic radio reset task definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.47
 * 
 * \date 2020/01/29
 * 
 * \defgroup radio_reset Radio Reset
 * \ingroup tasks
 * \{
 */

#ifndef RADIO_RESET_H_
#define RADIO_RESET_H_

#include <FreeRTOS.h>
#include <task.h>

#define TASK_RADIO_RESET_NAME               "RadioReset"    /**< Task name. */
#define TASK_RADIO_RESET_STACK_SIZE         128             /**< Stack size in bytes. */
#define TASK_RADIO_RESET_PRIORITY           5               /**< Task priority. */
#define TASK_RADIO_RESET_PERIOD_MS          60000           /**< Task period in milliseconds. */
#define TASK_RADIO_RESET_INITIAL_DELAY_MS   60000           /**< Delay, in milliseconds, before the first execution. */

/**
 * \brief Radio reset handle.
 */
extern xTaskHandle xTaskRadioResetHandle;

/**
 * \brief Periodic radio reset task.
 *
 * \return None.
 */
void vTaskRadioReset(void);

#endif /* RADIO_RESET_H_ */

/** \} End of radio_reset group */
