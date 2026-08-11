/*
 * obdh_data.c
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
 * \brief OBDH data structure implementation.
 * 
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 1.0.0
 * 
 * \date 2024/08/25
 * 
 * \addtogroup obdh_data 
 * \{
 */

#include <FreeRTOS.h>
#include <task.h>
#include <system/sys_log/sys_log.h>
#include <system/system.h>
#include <conops/conops.h>
#include <tasks/mission_manager.h>
#include <tasks/sched_tc.h>
#include <devices/payload/payload.h>
#include <devices/eps/eps.h>

#include "obdh_data.h"
#include "satellite.h"

#define OBDH_DATA_LOG_NAME  "OBDH Data"

int8_t obdh_set_param(uint8_t param_id, uint32_t *buf)
{
    int8_t err = 0;

    switch(param_id)
    {
        case OBDH_PARAM_ID_SYSTEM_TIME:
        {
            system_set_time(*buf);
            break;
        }
        case OBDH_PARAM_ID_MODE:
        {
            uint16_t mode = UINT16_MAX; 

            switch (*buf)
            {
                case OBDH_MODE_DEPLOYMENT:
                    mode = EV_TC_GOTO_DEPLOYMENT_MODE;
                    break;
                case OBDH_MODE_NORMAL:
                    mode = EV_TC_GOTO_NORMAL_MODE;
                    break;
                case OBDH_MODE_STAND_BY:
                    mode = EV_TC_GOTO_STAND_BY_MODE;
                    break;
                case OBDH_MODE_EXPERIMENT:
                    mode = EV_TC_GOTO_EXPERIMENT_MODE;
                    break;
                case OBDH_MODE_FDIR:
                    mode = EV_TC_GOTO_FDIR_MODE;
                    break;
                case OBDH_MODE_MANUAL:
                    mode = EV_TC_GOTO_MANUAL_MODE;
                    break;
                case OBDH_MODE_COMMISSION:
                    mode = EV_TC_GOTO_COMMISSION_MODE;
                    break;
                default:
                    break;
            }

            const struct conops_event mode_change = {
                .ev_id = mode,
                .ev_name = "ModeCH",
                .callback = NULL,
                .src = 0U,
            };

            if (notify_event_to_mission_manager(&mode_change) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, OBDH_DATA_LOG_NAME, "Failed to notify mode change to Mission Manager!");
                sys_log_new_line();
                err = -1;
            }

            break;
        }
        case OBDH_PARAM_ID_INITIAL_HIB_EXECUTED:    
        {
            if ((*buf == 0x00U) || (*buf == 0x01U))
            {
                sat_data_buf.obdh.data.initial_hib_executed = (bool)(*buf);
            }
            else 
            {
                err = -1;
            }
            break;
        }
        case OBDH_PARAM_ID_ANT_DEPLOYMENT_COUNTER:    
        {
            sat_data_buf.obdh.data.ant_deployment_counter = (uint8_t)(*buf);
            break;
        }
        case OBDH_PARAM_ID_MAIN_EDC:
        {
            if ((*buf == PL_ID_EDC_1) || (*buf == PL_ID_EDC_2))
            {
                sat_data_buf.obdh.data.main_edc = (uint8_t)(*buf);
            }
            else 
            {
                err = -1;
            }

            break;
        }
        case OBDH_PARAM_ID_GENERAL_TELEMETRY_ON:
        {
            if ((*buf == 0x00U) || (*buf == 0x01U))
            {
                sat_data_buf.obdh.data.general_telemetry_on = (bool)(*buf);
            }
            else 
            {
                err = -1;
            }

            break;
        }
        case OBDH_PARAM_ID_RESET_DEVICE:
        {
            if (*buf == 0x01U)
            {
                system_reset();
            }

            break;
        }
        case OBDH_PARAM_ID_RESET_TC_QUEUE:
        {
            if (*buf == 0x01U)
            {
                reset_sched_tc_queue();
            }

            break;
        }
        case OBDH_PARAM_ID_MAIN_PAYLOAD_STATE:
        {
            if ((*buf == PL_ID_EDC_1) || (*buf == PL_ID_EDC_2))
            {
                const struct conops_event payload_activate = {
                    .ev_id = EV_TC_ENABLE_PAYLOAD,
                    .src = (uint16_t)*buf,
                    .callback = NULL,
                    .ev_name = "MAIN_PL",
                };

                if (notify_event_to_mission_manager(&payload_activate) != 0)
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, OBDH_DATA_LOG_NAME, "Failed to notify payload activation to Mission Manager!");
                    sys_log_new_line();
                    err = -1;
                }
            }
            else if (*buf == 0U)
            {
                const struct conops_event payload_deactivate = {
                    .ev_id = EV_TC_DISABLE_PAYLOAD,
                    .src = sat_data_buf.obdh.data.main_payload_state,
                    .callback = NULL,
                    .ev_name = "MAIN_PL",
                };

                if (notify_event_to_mission_manager(&payload_deactivate) != 0)
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, OBDH_DATA_LOG_NAME, "Failed to notify payload deactivation to Mission Manager!");
                    sys_log_new_line();
                    err = -1;
                }
            }
            else 
            {
                err = -1;
            }
            break;
        }
        case OBDH_PARAM_ID_SEC_PAYLOAD_STATE:
        {
            if (*buf == PL_ID_PAYLOAD_X)
            {
                const struct conops_event payload_activate = {
                    .ev_id = EV_TC_ENABLE_PAYLOAD,
                    .src = (uint16_t)*buf,
                    .callback = NULL,
                    .ev_name = "SEC_PL",
                };

                if (notify_event_to_mission_manager(&payload_activate) != 0)
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, OBDH_DATA_LOG_NAME, "Failed to notify payload activation to Mission Manager!");
                    sys_log_new_line();
                    err = -1;
                }
            }
            else if (*buf == 0U)
            {
                const struct conops_event payload_deactivate = {
                    .ev_id = EV_TC_DISABLE_PAYLOAD,
                    .src = (uint16_t)PAYLOAD_X,
                    .callback = NULL,
                    .ev_name = "SEC_PL",
                };

                if (notify_event_to_mission_manager(&payload_deactivate) != 0)
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, OBDH_DATA_LOG_NAME, "Failed to notify payload deactivation to Mission Manager!");
                    sys_log_new_line();
                    err = -1;
                }
            }
            else 
            {
                err = -1;
            }
            break;
        }
        case OBDH_PARAM_ID_HIB_DURATION:
        {
            taskENTER_CRITICAL();
            sat_data_buf.obdh.data.hib_duration = *buf;
            taskEXIT_CRITICAL();
            break;
        }
        case OBDH_PARAM_ID_TS_COMMISSION_TIMEOUT:
        {
            taskENTER_CRITICAL();
            sat_data_buf.obdh.data.ts_commission_timeout = *buf;
            taskEXIT_CRITICAL();
            break;
        }
        case OBDH_PARAM_ID_EPS_BEACON_ON:
        {
            uint8_t retry_count = 5U;

            if ((*buf == 0x00U) || (*buf == 0x01U))
            {
                sat_data_buf.obdh.data.eps_beacon_on = (bool)(*buf);
                do 
                {
                    err = eps_set_param(SL_EPS2_REG_BEACON_ENABLE, *buf);
                    vTaskDelay(100U);
                    --retry_count;
                } while ((err < 0) && (retry_count > 0U));
            }
            else 
            {
                err = -1;
            }

            break;
        }
        case OBDH_PARAM_ID_BATT_CRITICAL_LEVEL_MV:
        {
            sat_data_buf.obdh.data.batt_crit_level_mv = *buf;
            break;
        }
        case OBDH_PARAM_ID_MANUAL_EXPERIMENT_ON:
        {
            if ((*buf == 0x00U) || (*buf == 0x01U))
            {
                sat_data_buf.obdh.data.manual_experiments = (bool)(*buf);
            }
            else 
            {
                err = -1;
            }

            break;
        }
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, OBDH_DATA_LOG_NAME, "Received invalid parameter: ");
            sys_log_print_hex((uint32_t)param_id);
            sys_log_new_line();
            err = -1;
            break;
    }

    return err;
}

int8_t obdh_get_param(uint8_t param_id, uint32_t *buf)
{
    int8_t err = 0;

    switch(param_id)
    {
        case OBDH_PARAM_ID_SYSTEM_TIME:              *buf = system_get_time();                                                break;
        case OBDH_PARAM_ID_TEMPERATURE_UC:           *buf = sat_data_buf.obdh.data.temperature;                               break;
        case OBDH_PARAM_ID_INPUT_CURRENT:            *buf = sat_data_buf.obdh.data.current;                                   break;
        case OBDH_PARAM_ID_INPUT_VOLTAGE:            *buf = sat_data_buf.obdh.data.voltage;                                   break;
        case OBDH_PARAM_ID_LAST_RESET_CAUSE:         *buf = sat_data_buf.obdh.data.last_reset_cause;                          break;
        case OBDH_PARAM_ID_RESET_COUNTER:            *buf = sat_data_buf.obdh.data.reset_counter;                             break;
        case OBDH_PARAM_ID_LAST_VALID_TC:            *buf = sat_data_buf.obdh.data.last_valid_tc;                             break;
        case OBDH_PARAM_ID_HARDWARE_VERSION:         *buf = sat_data_buf.obdh.data.hw_version;                                break;
        case OBDH_PARAM_ID_FIRMWARE_VERSION:         *buf = sat_data_buf.obdh.data.fw_version;                                break;
        case OBDH_PARAM_ID_MODE:                     *buf = sat_data_buf.obdh.data.mode;                                      break;
        case OBDH_PARAM_ID_TIMESTAMP_LAST_MODE:      *buf = sat_data_buf.obdh.data.ts_last_mode_change;                       break;
        case OBDH_PARAM_ID_MODE_DURATION:            *buf = system_get_time() - sat_data_buf.obdh.data.ts_last_mode_change;   break;
        case OBDH_PARAM_ID_INITIAL_HIB_EXECUTED:     *buf = sat_data_buf.obdh.data.initial_hib_executed;                      break;
        case OBDH_PARAM_ID_INITIAL_HIB_TIME_COUNTER: *buf = sat_data_buf.obdh.data.initial_hib_time_count;                    break;
        case OBDH_PARAM_ID_ANT_DEPLOYMENT_EXECUTED:  *buf = sat_data_buf.obdh.data.ant_deployment_executed;                   break;
        case OBDH_PARAM_ID_ANT_DEPLOYMENT_COUNTER:   *buf = sat_data_buf.obdh.data.ant_deployment_counter;                    break;
        case OBDH_PARAM_ID_LATITUDE:                 *buf = sat_data_buf.obdh.data.position.latitude;                         break;
        case OBDH_PARAM_ID_LONGITUDE:                *buf = sat_data_buf.obdh.data.position.longitude;                        break;
        case OBDH_PARAM_ID_ALTITUDE:                 *buf = sat_data_buf.obdh.data.position.altitude;                         break;
        case OBDH_PARAM_ID_LAST_PAGE_OBDH_DATA:      *buf = sat_data_buf.obdh.data.media.last_page_obdh_data;                 break;
        case OBDH_PARAM_ID_LAST_PAGE_EPS_DATA:       *buf = sat_data_buf.obdh.data.media.last_page_eps_data;                  break;
        case OBDH_PARAM_ID_LAST_PAGE_TTC_0_DATA:     *buf = sat_data_buf.obdh.data.media.last_page_ttc_0_data;                break;
        case OBDH_PARAM_ID_LAST_PAGE_TTC_1_DATA:     *buf = sat_data_buf.obdh.data.media.last_page_ttc_1_data;                break;
        case OBDH_PARAM_ID_LAST_PAGE_ANT_DATA:       *buf = sat_data_buf.obdh.data.media.last_page_ant_data;                  break;
        case OBDH_PARAM_ID_LAST_PAGE_EDC_DATA:       *buf = sat_data_buf.obdh.data.media.last_page_edc_data;                  break;
        case OBDH_PARAM_ID_LAST_PAGE_PX_DATA:        *buf = sat_data_buf.obdh.data.media.last_page_px_data;                   break;
        case OBDH_PARAM_ID_LAST_PAGE_SBCD_PKTS:      *buf = sat_data_buf.obdh.data.media.last_page_sbcd_pkts;                 break;
        case OBDH_PARAM_ID_MAIN_EDC:                 *buf = sat_data_buf.obdh.data.main_edc;                                  break;
        case OBDH_PARAM_ID_GENERAL_TELEMETRY_ON:     *buf = sat_data_buf.obdh.data.general_telemetry_on;                      break;
        case OBDH_PARAM_ID_TS_LAST_TLE_UPDATE:       *buf = sat_data_buf.obdh.data.position.ts_last_tle_update;               break;
        case OBDH_PARAM_ID_TS_READ_SENSORS:          *buf = sat_data_buf.obdh.data.ts_read_sensors;                           break;
        case OBDH_PARAM_ID_MAIN_PAYLOAD_STATE:       *buf = sat_data_buf.obdh.data.main_payload_state;                        break;
        case OBDH_PARAM_ID_SEC_PAYLOAD_STATE:        *buf = sat_data_buf.obdh.data.sec_payload_state;                         break;
        case OBDH_PARAM_ID_HIB_DURATION:             *buf = sat_data_buf.obdh.data.hib_duration;                              break;
        case OBDH_PARAM_ID_TS_POSITION:              *buf = sat_data_buf.obdh.data.position.timestamp;                        break;
        case OBDH_PARAM_ID_TS_LAST_CONTACT:          *buf = sat_data_buf.obdh.data.ts_last_contact;                           break;
        case OBDH_PARAM_ID_TS_COMMISSION_TIMEOUT:    *buf = sat_data_buf.obdh.data.ts_commission_timeout;                     break;
        case OBDH_PARAM_ID_EPS_BEACON_ON:            *buf = sat_data_buf.obdh.data.eps_beacon_on;                             break;
        case OBDH_PARAM_ID_BATT_CRITICAL_LEVEL_MV:   *buf = sat_data_buf.obdh.data.batt_crit_level_mv;                        break;
        case OBDH_PARAM_ID_HIBERNATION_ON:           *buf = sat_data_buf.obdh.data.hibernation_on;                            break;
        case OBDH_PARAM_ID_MANUAL_EXPERIMENT_ON:     *buf = sat_data_buf.obdh.data.manual_experiments;                        break;
        case OBDH_PARAM_ID_TC_QUEUE_SIZE:            *buf = sat_data_buf.obdh.data.tc_queue_size;                             break;
        case OBDH_PARAM_ID_TS_NEXT_SCHED_TC:         *buf = sat_data_buf.obdh.data.ts_next_sched_tc;                          break;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, OBDH_DATA_LOG_NAME, "Received invalid parameter: ");
            sys_log_print_hex((uint32_t)param_id);
            sys_log_new_line();
            err = -1;
            break;
    }

    return err;
}

/** \} End of obdh_data group */
