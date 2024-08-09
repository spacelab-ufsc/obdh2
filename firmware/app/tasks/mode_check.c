/*
 * mode_check.c
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
 * \brief Operation Mode Health Check Tasks implementation
 *
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 *
 * \version 0.10.19
 *
 * \date 2024/08/09
 *
 * \addtogroup mode_check
 * \{
 */

#include <stdint.h>
#include <stdbool.h>

#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>

#include <structs/satellite.h>
#include <structs/obdh_data.h>
#include <system/system.h>
#include <system/sys_log/sys_log.h>
#include <devices/payload/payload.h>

#include "mode_check.h"
#include "op_ctrl.h"

TaskHandle_t xTaskHealthCheckModeHandle;

static void vTimeControlMockup(TimerHandle_t timer)
{
    system_increment_time();
}

void vTaskHealthCheckMode(void)
{
    TimerHandle_t sys_timer = xTimerCreate("System Timer", pdMS_TO_TICKS(1000U), pdTRUE, NULL, vTimeControlMockup);

    xTimerStart(sys_timer, pdMS_TO_TICKS(10U));

    while(1)
    {
        bool test_result;

        if (xTaskNotifyWait(0UL, UINT32_MAX, NULL, portMAX_DELAY) == pdTRUE)
        {
            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_HEALTH_CHECK_MODE_NAME, "Starting Operation Mode Health Check...");
            sys_log_new_line();
            
            notify_op_ctrl(SAT_NOTIFY_IN_BRAZIL);

            taskYIELD();

            test_result = (sat_data_buf.obdh.data.mode == OBDH_MODE_NORMAL) && (sat_data_buf.state.active_payload == PAYLOAD_EDC_0) && (sat_data_buf.state.active_payload == PAYLOAD_EDC_1);

            sys_log_print_test_result(test_result, "In Brazil Notify Test");
            sys_log_new_line();

            notify_op_ctrl(SAT_NOTIFY_OUT_OF_BRAZIL);

            vTaskDelay(pdMS_TO_TICKS(2000U));

            test_result = (sat_data_buf.obdh.data.mode == OBDH_MODE_NORMAL) && (sat_data_buf.state.active_payload == PAYLOAD_X);

            sys_log_print_test_result(test_result, "Out of Brazil Notify Test");
            sys_log_new_line();

            notify_op_ctrl(SAT_NOTIFY_PX_FINISHED);

            vTaskDelay(pdMS_TO_TICKS(2000U));

            test_result = (sat_data_buf.obdh.data.mode == OBDH_MODE_STAND_BY) && (sat_data_buf.state.active_payload == PAYLOAD_NONE);

            sys_log_print_test_result(test_result, "Payload X finished Notify Test");
            sys_log_new_line();

            notify_op_ctrl(SAT_NOTIFY_IN_BRAZIL);
            taskYIELD();
            notify_op_ctrl(SAT_NOTIFY_OUT_OF_BRAZIL);
            taskYIELD();
            notify_op_ctrl(SAT_NOTIFY_PX_FINISHED);

            vTaskDelay(pdMS_TO_TICKS(2000U));

            test_result = (sat_data_buf.obdh.data.mode == OBDH_MODE_STAND_BY) && (sat_data_buf.state.active_payload == PAYLOAD_NONE);

            sys_log_print_test_result(test_result, "Position relative Notifications Test");
            sys_log_new_line();

            sat_data_buf.obdh.data.mode_duration = 10;
            notify_op_ctrl(SAT_NOTIFY_ENTER_HIBERNATION);

            vTaskDelay(pdMS_TO_TICKS(2000U));

            test_result = (sat_data_buf.obdh.data.mode == OBDH_MODE_HIBERNATION);

            sys_log_print_test_result(test_result, "Enter hibernation Test");
            sys_log_new_line();

            notify_op_ctrl(SAT_NOTIFY_LEAVE_HIBERNATION);

            vTaskDelay(pdMS_TO_TICKS(2000U));

            test_result = (sat_data_buf.obdh.data.mode == OBDH_MODE_STAND_BY);

            sys_log_print_test_result(test_result, "Leave hibernation Test");
            sys_log_new_line();

            sat_data_buf.obdh.data.mode_duration = 10;
            notify_op_ctrl(SAT_NOTIFY_ENTER_HIBERNATION);

            vTaskDelay(pdMS_TO_TICKS(2000U));

            TickType_t hib_time_init = xTaskGetTickCount();

            vTaskDelayUntil(&hib_time_init, pdMS_TO_TICKS(11000U));

            test_result = (sat_data_buf.obdh.data.mode != OBDH_MODE_HIBERNATION);

            sys_log_print_test_result(test_result, "Hibernation time Test");
            sys_log_new_line();

            sat_data_buf.obdh.data.mode_duration = 10;
            notify_op_ctrl(SAT_NOTIFY_ENTER_HIBERNATION);

            taskYIELD();

            notify_op_ctrl(SAT_NOTIFY_IN_BRAZIL);

            vTaskDelay(pdMS_TO_TICKS(2000U));

            test_result = (sat_data_buf.obdh.data.mode == OBDH_MODE_HIBERNATION);

            sys_log_print_test_result(test_result, "Hibernation mode persistency Test [EDC]");
            sys_log_new_line();

            taskYIELD();

            notify_op_ctrl(SAT_NOTIFY_OUT_OF_BRAZIL);

            vTaskDelay(pdMS_TO_TICKS(2000U));

            test_result = (sat_data_buf.obdh.data.mode == OBDH_MODE_HIBERNATION);

            sys_log_print_test_result(test_result, "Hibernation mode persistency Test [PX]");
            sys_log_new_line();
        }

        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_HEALTH_CHECK_MODE_NAME, "Operation Mode Health Check Finished!!!");
        sys_log_new_line();

        vTaskSuspend(NULL);
    }
}

/** \} End of mode_check group */

