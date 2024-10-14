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
 * along with OBDH 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
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
#include "mission_manager.h"
#include "process_tc.h"

TaskHandle_t xTaskHealthCheckModeHandle;

static void vTimeControlMockup(TimerHandle_t timer)
{
    (void)timer;
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
            
            const event_t in_brazil_ev = { .event = EV_NOTIFY_IN_BRAZIL, .args[0] = 0U, .args[1] = 0U, .args[2] = 0U };

            if (notify_event_to_mission_manager(&in_brazil_ev) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MODE_NAME, "Failed to enqueue `in brazil` event");
                sys_log_new_line();
            }

            vTaskDelay(pdMS_TO_TICKS(TASK_PROCESS_TC_MAX_WAIT_TIME_MS));

            test_result = (sat_data_buf.obdh.data.mode == OBDH_MODE_NORMAL) && ((sat_data_buf.state.active_payload[0] == PAYLOAD_EDC_0) || (sat_data_buf.state.active_payload[0] == PAYLOAD_EDC_1));

            sys_log_print_test_result(test_result, "In Brazil Notify Test");
            sys_log_new_line();

            const event_t out_of_brazil = { .event = EV_NOTIFY_OUT_OF_BRAZIL, .args[0] = 0U, .args[1] = 0U, .args[2] = 0U };

            if (notify_event_to_mission_manager(&out_of_brazil) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MODE_NAME, "Failed to enqueue `out of brazil` event");
                sys_log_new_line();
            }

            vTaskDelay(pdMS_TO_TICKS(TASK_PROCESS_TC_MAX_WAIT_TIME_MS));

            test_result = (sat_data_buf.obdh.data.mode == OBDH_MODE_NORMAL) && (sat_data_buf.state.active_payload[1] == PAYLOAD_X) && (sat_data_buf.state.active_payload[0] == PAYLOAD_NONE); 

            sys_log_print_test_result(test_result, "Out of Brazil Notify Test");
            sys_log_new_line();

            const event_t px_finished = { .event = EV_NOTIFY_PX_FINISHED, .args[0] = 0U, .args[1] = 0U, .args[2] = 0U };

            if (notify_event_to_mission_manager(&px_finished) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MODE_NAME, "Failed to enqueue `px finished` event");
                sys_log_new_line();
            }

            vTaskDelay(pdMS_TO_TICKS(150));

            test_result = (sat_data_buf.obdh.data.mode == OBDH_MODE_STAND_BY) && (sat_data_buf.state.active_payload[0] == PAYLOAD_NONE) && (sat_data_buf.state.active_payload[1] == PAYLOAD_NONE);

            sys_log_print_test_result(test_result, "Payload X finished Notify Test");
            sys_log_new_line();

            (void)notify_event_to_mission_manager(&in_brazil_ev);
            taskYIELD();
            (void)notify_event_to_mission_manager(&out_of_brazil);
            taskYIELD();
            (void)notify_event_to_mission_manager(&px_finished);

            vTaskDelay(pdMS_TO_TICKS(TASK_PROCESS_TC_MAX_WAIT_TIME_MS));

            test_result = (sat_data_buf.obdh.data.mode == OBDH_MODE_STAND_BY) && (sat_data_buf.state.active_payload[0] == PAYLOAD_NONE) && (sat_data_buf.state.active_payload[1] == PAYLOAD_NONE);

            sys_log_print_test_result(test_result, "Position relative Notifications Test");
            sys_log_new_line();

            const event_t enter_hib = { .event = EV_NOTIFY_MODE_CHANGE_RQ, .args[0] = OBDH_MODE_HIBERNATION,  .args[1] = 0x11U, .args[2] = 0x11U };
            if (notify_event_to_mission_manager(&enter_hib) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MODE_NAME, "Failed to enqueue `enter hibernation` event");
                sys_log_new_line();
            }

            vTaskDelay(pdMS_TO_TICKS(TASK_PROCESS_TC_MAX_WAIT_TIME_MS));

            uint32_t hib_time = ((uint32_t)0x1111UL * (uint32_t)60UL * (uint32_t)60UL);
            test_result = (sat_data_buf.obdh.data.mode == OBDH_MODE_HIBERNATION) && (sat_data_buf.obdh.data.mode_duration  == hib_time);

            sys_log_print_test_result(test_result, "Enter hibernation Test");
            sys_log_new_line();

            const event_t leave_hib = { .event = EV_NOTIFY_MODE_CHANGE_RQ, .args[0] = OBDH_WAKE_UP,  .args[1] = 0U, .args[2] = 0U };
            if (notify_event_to_mission_manager(&leave_hib) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MODE_NAME, "Failed to enqueue `leave hibernation` event");
                sys_log_new_line();
            }

            vTaskDelay(pdMS_TO_TICKS(2000U));

            test_result = (sat_data_buf.obdh.data.mode == OBDH_MODE_STAND_BY);

            sys_log_print_test_result(test_result, "Leave hibernation Test");
            sys_log_new_line();

            if (notify_event_to_mission_manager(&enter_hib) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MODE_NAME, "Failed to enqueue `enter hibernation` event");
                sys_log_new_line();
            }

            vTaskDelay(pdMS_TO_TICKS(TASK_PROCESS_TC_MAX_WAIT_TIME_MS));

            taskYIELD();

            if (notify_event_to_mission_manager(&in_brazil_ev) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MODE_NAME, "Failed to enqueue `in brazil` event");
                sys_log_new_line();
            }

            vTaskDelay(pdMS_TO_TICKS(TASK_PROCESS_TC_MAX_WAIT_TIME_MS));

            test_result = (sat_data_buf.obdh.data.mode == OBDH_MODE_HIBERNATION);

            sys_log_print_test_result(test_result, "Hibernation mode persistency Test [EDC]");
            sys_log_new_line();

            taskYIELD();

            if (notify_event_to_mission_manager(&out_of_brazil) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MODE_NAME, "Failed to enqueue `out of brazil` event");
                sys_log_new_line();
            }

            vTaskDelay(pdMS_TO_TICKS(TASK_PROCESS_TC_MAX_WAIT_TIME_MS));

            test_result = (sat_data_buf.obdh.data.mode == OBDH_MODE_HIBERNATION);

            sys_log_print_test_result(test_result, "Hibernation mode persistency Test [PX]");
            sys_log_new_line();

            const event_t mode_normal = { .event = EV_NOTIFY_MODE_CHANGE_RQ, .args[0] = OBDH_MODE_NORMAL,  .args[1] = 0U, .args[2] = 0U };

            if (notify_event_to_mission_manager(&mode_normal) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MODE_NAME, "Failed to enqueue `mode change to normal` event");
                sys_log_new_line();
            }

            vTaskDelay(pdMS_TO_TICKS(TASK_PROCESS_TC_MAX_WAIT_TIME_MS));

            test_result = (sat_data_buf.obdh.data.mode == OBDH_MODE_NORMAL) && ((sat_data_buf.state.active_payload[0] == PAYLOAD_EDC_0) || (sat_data_buf.state.active_payload[0] == PAYLOAD_EDC_1));

            sys_log_print_test_result(test_result, "TC change mode to NORMAL Test");
            sys_log_new_line();

            const event_t mode_stand_by = { .event = EV_NOTIFY_MODE_CHANGE_RQ, .args[0] = OBDH_MODE_STAND_BY,  .args[1] = 0U, .args[2] = 0U };

            if (notify_event_to_mission_manager(&mode_stand_by) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MODE_NAME, "Failed to enqueue `mode change to stand by` event");
                sys_log_new_line();
            }

            vTaskDelay(pdMS_TO_TICKS(TASK_PROCESS_TC_MAX_WAIT_TIME_MS));

            test_result = (sat_data_buf.obdh.data.mode == OBDH_MODE_STAND_BY) && (sat_data_buf.state.active_payload[0] == PAYLOAD_NONE) && (sat_data_buf.state.active_payload[1] == PAYLOAD_NONE);

            sys_log_print_test_result(test_result, "TC change mode to STAND_BY Test");
            sys_log_new_line();

            vTaskDelay(pdMS_TO_TICKS(TASK_PROCESS_TC_MAX_WAIT_TIME_MS));

            sat_data_buf.obdh.data.manual_mode_on = true;

            if (notify_event_to_mission_manager(&in_brazil_ev) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MODE_NAME, "Failed to enqueue `in brazil (manual)` event");
                sys_log_new_line();
            }

            test_result = (sat_data_buf.obdh.data.mode == OBDH_MODE_STAND_BY) && (sat_data_buf.state.active_payload[0] == PAYLOAD_NONE) && (sat_data_buf.state.active_payload[1] == PAYLOAD_NONE);

            sys_log_print_test_result(test_result, "Manual mode check");
            sys_log_new_line();

        }

        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_HEALTH_CHECK_MODE_NAME, "Operation Mode Health Check Finished!!!");
        sys_log_new_line();

        vTaskSuspend(NULL);
    }
}

/** \} End of mode_check group */

