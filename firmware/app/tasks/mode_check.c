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
 * \version 1.0.0
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
#include <utils/mem_mng.h>
#include <conops/conops.h>

#include "mode_check.h"
#include "mission_manager.h"
#include "process_tc.h"
#include "pos_det.h"

TaskHandle_t xTaskHealthCheckModeHandle;

static void vTimeControlMockup(TimerHandle_t timer)
{
    (void)timer;
    system_increment_time();
}

void vTaskHealthCheckMode(void *p)
{
    (void)p;

    TimerHandle_t sys_timer = xTimerCreate("System Timer", pdMS_TO_TICKS(1000U), pdTRUE, NULL, vTimeControlMockup);
    (void)xTimerStart(sys_timer, pdMS_TO_TICKS(10U));

    while(1)
    {
        if (xTaskNotifyWait(0UL, UINT32_MAX, NULL, portMAX_DELAY) == pdTRUE)
        {
            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_HEALTH_CHECK_MODE_NAME, "Starting Operation Mode Health Check...");
            sys_log_new_line();

            (void)mem_mng_load_obdh_data_from_default_values(&sat_data_buf.obdh);

            const struct conops_event in_brazil_ev = {
                .src = 0U,
                .ev_name = "InBrazil",
                .callback = NULL,
                .ev_id = EV_IN_BRAZIL,
            };

            if (notify_event_to_mission_manager(&in_brazil_ev) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MODE_NAME, "Failed to enqueue `in brazil` event");
                sys_log_new_line();
            }

            vTaskDelay(pdMS_TO_TICKS(TASK_PROCESS_TC_MAX_WAIT_TIME_MS));

            bool payload_test = (sat_data_buf.obdh.data.mode == OBDH_MODE_DEPLOYMENT) && ((sat_data_buf.obdh.data.main_payload_state == (uint8_t)PAYLOAD_NONE) && (sat_data_buf.obdh.data.sec_payload_state == (uint8_t)PAYLOAD_NONE));
            bool hibernation_test = (sat_data_buf.obdh.data.hibernation_on == true);
            bool beacon_off = (sat_data_buf.obdh.data.general_telemetry_on == false);

            sys_log_print_test_result(payload_test, "DM(InBrazil) - Payload State test");
            sys_log_new_line();
            sys_log_print_test_result(hibernation_test, "DM(InBrazil) - Hibernation test");
            sys_log_new_line();
            sys_log_print_test_result(beacon_off, "DM(InBrazil) - Beacon test");
            sys_log_new_line();

            const struct conops_event out_brazil_ev = {
                .src = 0U,
                .ev_name = "OutBrazil",
                .callback = NULL,
                .ev_id = EV_OUT_OF_BRAZIL,
            };

            if (notify_event_to_mission_manager(&out_brazil_ev) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MODE_NAME, "Failed to enqueue `out of brazil` event");
                sys_log_new_line();
            }

            vTaskDelay(pdMS_TO_TICKS(TASK_PROCESS_TC_MAX_WAIT_TIME_MS));

            payload_test = (sat_data_buf.obdh.data.mode == OBDH_MODE_DEPLOYMENT) && ((sat_data_buf.obdh.data.main_payload_state == (uint8_t)PAYLOAD_NONE) && (sat_data_buf.obdh.data.sec_payload_state == (uint8_t)PAYLOAD_NONE));
            hibernation_test = (sat_data_buf.obdh.data.hibernation_on == true);
            beacon_off = (sat_data_buf.obdh.data.general_telemetry_on == false);

            sys_log_print_test_result(payload_test, "DM(OutBrazil) - Payload State test");
            sys_log_new_line();
            sys_log_print_test_result(hibernation_test, "DM(OutBrazil) - Hibernation test");
            sys_log_new_line();
            sys_log_print_test_result(beacon_off, "DM(OutBrazil) - Beacon test");
            sys_log_new_line();

            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_HEALTH_CHECK_MODE_NAME, "Testing Commission Mode transition...");
            sys_log_new_line();

            const struct conops_event commission = {
                .src = 0U,
                .ev_name = "COMM",
                .callback = NULL,
                .ev_id = EV_DEPLOYMENT_COMPLETE,
            };

            if (notify_event_to_mission_manager(&commission) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MODE_NAME, "Failed to enqueue `deployment complete` event");
                sys_log_new_line();
            }

            vTaskDelay(pdMS_TO_TICKS(TASK_PROCESS_TC_MAX_WAIT_TIME_MS));

            payload_test = (sat_data_buf.obdh.data.mode == OBDH_MODE_COMMISSION) && ((sat_data_buf.obdh.data.main_payload_state == (uint8_t)PAYLOAD_NONE) && (sat_data_buf.obdh.data.sec_payload_state == (uint8_t)PAYLOAD_NONE));
            hibernation_test = (sat_data_buf.obdh.data.hibernation_on == false) && (sat_data_buf.obdh.data.hib_duration == 0U);
            bool beacon_on = (sat_data_buf.obdh.data.general_telemetry_on == true);

            sys_log_print_test_result(payload_test, "CM - Payload State test");
            sys_log_new_line();
            sys_log_print_test_result(hibernation_test, "CM - Hibernation test");
            sys_log_new_line();
            sys_log_print_test_result(beacon_on, "CM - Beacon test");
            sys_log_new_line();

            if (notify_event_to_mission_manager(&in_brazil_ev) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MODE_NAME, "Failed to enqueue `in brazil` event");
                sys_log_new_line();
            }

            vTaskDelay(pdMS_TO_TICKS(TASK_PROCESS_TC_MAX_WAIT_TIME_MS));

            payload_test = (sat_data_buf.obdh.data.mode == OBDH_MODE_COMMISSION) && ((sat_data_buf.obdh.data.main_payload_state == (uint8_t)PAYLOAD_NONE) && (sat_data_buf.obdh.data.sec_payload_state == (uint8_t)PAYLOAD_NONE));

            sys_log_print_test_result(payload_test, "CM(InBrazil) - Payload State test");
            sys_log_new_line();

            if (notify_event_to_mission_manager(&out_brazil_ev) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MODE_NAME, "Failed to enqueue `out of brazil` event");
                sys_log_new_line();
            }

            vTaskDelay(pdMS_TO_TICKS(TASK_PROCESS_TC_MAX_WAIT_TIME_MS));

            payload_test = (sat_data_buf.obdh.data.mode == OBDH_MODE_COMMISSION) && ((sat_data_buf.obdh.data.main_payload_state == (uint8_t)PAYLOAD_NONE) && (sat_data_buf.obdh.data.sec_payload_state == (uint8_t)PAYLOAD_NONE));

            sys_log_print_test_result(payload_test, "CM(OutBrazil) - Payload State test");
            sys_log_new_line();

            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_HEALTH_CHECK_MODE_NAME, "Testing Normal Mode transition...");
            sys_log_new_line();

            const struct conops_event normal = {
                .src = 0U,
                .ev_name = "NM",
                .callback = NULL,
                .ev_id = EV_TC_GOTO_NORMAL_MODE,
            };

            if (notify_event_to_mission_manager(&normal) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MODE_NAME, "Failed to enqueue `goto normal mode` event");
                sys_log_new_line();
            }

            vTaskDelay(pdMS_TO_TICKS(TASK_PROCESS_TC_MAX_WAIT_TIME_MS));

            payload_test = (sat_data_buf.obdh.data.mode == OBDH_MODE_NORMAL) && ((sat_data_buf.obdh.data.main_payload_state == (uint8_t)PL_ID_EDC_1) && (sat_data_buf.obdh.data.sec_payload_state == (uint8_t)PAYLOAD_NONE));
            bool edc_pointer_test = (sat_data_buf.state.c_edc == &sat_data_buf.edc_0);

            sys_log_print_test_result(payload_test, "NM(GOTO_NORMAL_MODE) - Payload State test");
            sys_log_new_line();
            sys_log_print_test_result(edc_pointer_test, "NM(GOTO_NORMAL_MODE) - EDC pointer test");
            sys_log_new_line();

            if (notify_event_to_mission_manager(&in_brazil_ev) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MODE_NAME, "Failed to enqueue `in_brazil` event");
                sys_log_new_line();
            }

            vTaskDelay(pdMS_TO_TICKS(TASK_PROCESS_TC_MAX_WAIT_TIME_MS));

            payload_test = (sat_data_buf.obdh.data.mode == OBDH_MODE_NORMAL) && ((sat_data_buf.obdh.data.main_payload_state == (uint8_t)PL_ID_EDC_1) && (sat_data_buf.obdh.data.sec_payload_state == (uint8_t)PAYLOAD_NONE));
            edc_pointer_test = (sat_data_buf.state.c_edc == &sat_data_buf.edc_0);

            sys_log_print_test_result(payload_test, "NM(InBRAZIL) - Payload State test");
            sys_log_new_line();
            sys_log_print_test_result(edc_pointer_test, "NM(InBRAZIL) - EDC pointer test");
            sys_log_new_line();

            sat_data_buf.obdh.data.manual_experiments = true;

            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_HEALTH_CHECK_MODE_NAME, "Testing Stand-by Mode transition...");
            sys_log_new_line();

            if (notify_event_to_mission_manager(&out_brazil_ev) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MODE_NAME, "Failed to enqueue `out of brazil` event");
                sys_log_new_line();
            }

            vTaskDelay(pdMS_TO_TICKS(TASK_PROCESS_TC_MAX_WAIT_TIME_MS));

            payload_test = (sat_data_buf.obdh.data.mode == OBDH_MODE_STAND_BY) && ((sat_data_buf.obdh.data.main_payload_state == (uint8_t)PAYLOAD_NONE) && (sat_data_buf.obdh.data.sec_payload_state == (uint8_t)PAYLOAD_NONE));

            sys_log_print_test_result(payload_test, "NM(OutBRAZIL) - Payload State test");
            sys_log_new_line();
            sys_log_print_test_result(edc_pointer_test, "NM(OutBRAZIL) - EDC pointer test");
            sys_log_new_line();

            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_HEALTH_CHECK_MODE_NAME, "Testing Normal Mode transition and EDC switch...");
            sys_log_new_line();

            sat_data_buf.obdh.data.main_edc = PL_ID_EDC_2;

            if (notify_event_to_mission_manager(&in_brazil_ev) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MODE_NAME, "Failed to enqueue `in brazil` event");
                sys_log_new_line();
            }

            vTaskDelay(pdMS_TO_TICKS(TASK_PROCESS_TC_MAX_WAIT_TIME_MS));

            payload_test = (sat_data_buf.obdh.data.mode == OBDH_MODE_NORMAL) && ((sat_data_buf.obdh.data.main_payload_state == (uint8_t)PL_ID_EDC_2) && (sat_data_buf.obdh.data.sec_payload_state == (uint8_t)PAYLOAD_NONE));
            edc_pointer_test = (sat_data_buf.state.c_edc == &sat_data_buf.edc_1);

            sys_log_print_test_result(payload_test, "SBM(InBRAZIL) - Payload State test");
            sys_log_new_line();

            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_HEALTH_CHECK_MODE_NAME, "Testing Experiment Mode transition...");
            sys_log_new_line();

            sat_data_buf.obdh.data.manual_experiments = false;

            if (notify_event_to_mission_manager(&out_brazil_ev) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MODE_NAME, "Failed to enqueue `out of brazil` event");
                sys_log_new_line();
            }

            vTaskDelay(pdMS_TO_TICKS(TASK_PROCESS_TC_MAX_WAIT_TIME_MS));

            payload_test = (sat_data_buf.obdh.data.mode == OBDH_MODE_EXPERIMENT) && ((sat_data_buf.obdh.data.main_payload_state == (uint8_t)PAYLOAD_NONE) && (sat_data_buf.obdh.data.sec_payload_state == (uint8_t)PAYLOAD_X));

            sys_log_print_test_result(payload_test, "NM(OutBRAZIL) - Payload State test");
            sys_log_new_line();

            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_HEALTH_CHECK_MODE_NAME, "Testing Stand-by Mode transition from Experiment...");
            sys_log_new_line();

            const struct conops_event exp_timeout = {
                .src = 10U,
                .ev_id = EV_PX_FINISHED,
                .ev_name = "EXP_TIME",
                .callback = NULL,
            };

            if (notify_event_to_mission_manager(&exp_timeout) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MODE_NAME, "Failed to enqueue `Px finished` event");
                sys_log_new_line();
            }

            vTaskDelay(pdMS_TO_TICKS(TASK_PROCESS_TC_MAX_WAIT_TIME_MS));

            payload_test = (sat_data_buf.obdh.data.mode == OBDH_MODE_STAND_BY) && ((sat_data_buf.obdh.data.main_payload_state == (uint8_t)PAYLOAD_NONE) && (sat_data_buf.obdh.data.sec_payload_state == (uint8_t)PAYLOAD_NONE));

            sys_log_print_test_result(payload_test, "EXP(PX-Finished) - Payload State test");
            sys_log_new_line();

            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_HEALTH_CHECK_MODE_NAME, "Testing FDIR Mode transition...");
            sys_log_new_line();

            const struct conops_event fdir = {
                .src = 10U,
                .ev_id = EV_BATTERY_LEVEL_CRITICAL,
                .ev_name = "BATT_LVL",
                .callback = NULL,
            };

            if (notify_event_to_mission_manager(&fdir) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MODE_NAME, "Failed to enqueue `battery level` event");
                sys_log_new_line();
            }

            vTaskDelay(pdMS_TO_TICKS(TASK_PROCESS_TC_MAX_WAIT_TIME_MS));

            payload_test = (sat_data_buf.obdh.data.mode == OBDH_MODE_FDIR) && ((sat_data_buf.obdh.data.main_payload_state == (uint8_t)PAYLOAD_NONE) && (sat_data_buf.obdh.data.sec_payload_state == (uint8_t)PAYLOAD_NONE));

            sys_log_print_test_result(payload_test, "FDIR(BATT_LVL) - Payload State test");
            sys_log_new_line();

            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_HEALTH_CHECK_MODE_NAME, "Testing FDIR exit transition...");
            sys_log_new_line();

            const struct conops_event fdir_exit = {
                .src = 10U,
                .ev_id = EV_FDIR_RESOLVED,
                .ev_name = "FDIR_RES",
                .callback = NULL,
            };

            if (notify_event_to_mission_manager(&fdir_exit) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MODE_NAME, "Failed to enqueue `fdir res` event");
                sys_log_new_line();
            }

            vTaskDelay(pdMS_TO_TICKS(TASK_PROCESS_TC_MAX_WAIT_TIME_MS));

            if (is_satellite_in_brazil((float) sat_data_buf.obdh.data.position.latitude, (float)sat_data_buf.obdh.data.position.longitude))
            {
                payload_test = (sat_data_buf.obdh.data.mode == OBDH_MODE_NORMAL) && ((sat_data_buf.obdh.data.main_payload_state == (uint8_t)PL_ID_EDC_2) && (sat_data_buf.obdh.data.sec_payload_state == (uint8_t)PAYLOAD_NONE));
            }
            else
            {
                payload_test = (sat_data_buf.obdh.data.mode == OBDH_MODE_STAND_BY) && ((sat_data_buf.obdh.data.main_payload_state == (uint8_t)PAYLOAD_NONE) && (sat_data_buf.obdh.data.sec_payload_state == (uint8_t)PAYLOAD_NONE));
            }

            sys_log_print_test_result(payload_test, "FDIR(BATT_LVL_RECOVERY) - Payload State test");
            sys_log_new_line();

            const struct conops_event enter_hib = {
                .src = 10U,
                .ev_id = EV_TC_ENTER_HIBERNATION,
                .ev_name = "HIB",
                .callback = NULL,
            };

            if (notify_event_to_mission_manager(&enter_hib) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MODE_NAME, "Failed to enqueue `enter hibernation` event");
                sys_log_new_line();
            }

            vTaskDelay(pdMS_TO_TICKS(200UL));

            const uint32_t hib_time = ((uint32_t)10UL * (uint32_t)60UL * (uint32_t)60UL);
            bool test_result = (sat_data_buf.obdh.data.hibernation_on == true) && (sat_data_buf.obdh.data.hib_duration == hib_time);

            sys_log_print_test_result(test_result, "Enter hibernation Test");
            sys_log_new_line();

            const struct conops_event leave_hib = {
                .src = 0U,
                .ev_id = EV_TC_LEAVE_HIBERNATION,
                .ev_name = "LHIB",
                .callback = NULL,
            };

            if (notify_event_to_mission_manager(&leave_hib) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MODE_NAME, "Failed to enqueue `leave hibernation` event");
                sys_log_new_line();
            }

            vTaskDelay(pdMS_TO_TICKS(200U));

            test_result = (sat_data_buf.obdh.data.hibernation_on == false) && (sat_data_buf.obdh.data.hib_duration == 0U);

            sys_log_print_test_result(test_result, "Leave hibernation Test");
            sys_log_new_line();
        }

        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_HEALTH_CHECK_MODE_NAME, "Operation Mode Health Check Finished!!!");
        sys_log_new_line();

        vTaskSuspend(NULL);
    }
}

/** \} End of mode_check group */

