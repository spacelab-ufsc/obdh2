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
 * \version 0.10.20
 * 
 * \date 2024/08/25
 * 
 * \addtogroup obdh_data 
 * \{
 */

#include <system/sys_log/sys_log.h>
#include <system/system.h>
#include <tasks/mission_manager.h>

#include "obdh_data.h"
#include "satellite.h"

#define OBDH_DATA_LOG_NAME  "OBDH DATA"

int8_t obdh_set_param(uint8_t param_id, uint32_t *buf)
{
    int8_t err = 0;

    switch(param_id)
    {
        case OBDH_PARAM_ID_TIME_COUNTER:
        {
            system_set_time(*buf);
            break;
        }
        case OBDH_PARAM_ID_MODE:
        {
            const event_t mode_change = { .event = EV_NOTIFY_MODE_CHANGE_RQ, .args[0] = (uint8_t)(*buf), .args[1] = 0U, .args[2] = 0U };

            if (notify_event_to_mission_manager(&mode_change) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, OBDH_DATA_LOG_NAME, "Invalid Mode requested!");
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
        case OBDH_PARAM_ID_MODE_DURATION:
        {
            sat_data_buf.obdh.data.mode_duration = *buf;
            break;
        }
        case OBDH_PARAM_ID_MANUAL_MODE_ON:
        {
            if (*buf == 0x00U)
            {
                taskENTER_CRITICAL();
                sat_data_buf.obdh.data.manual_mode_on = false;
                taskEXIT_CRITICAL();
            }
            else if (*buf == 0x01U)
            {
                taskENTER_CRITICAL();
                sat_data_buf.obdh.data.manual_mode_on = true;
                taskEXIT_CRITICAL();
            }
            else
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, OBDH_DATA_LOG_NAME, "Invalid value for Manual Mode ID");
                sys_log_new_line();
                err = -1;
            }

            break;
        }
        case OBDH_PARAM_ID_MAIN_EDC:
        {
            if ((*buf == CONFIG_PL_ID_EDC_1) || (*buf == CONFIG_PL_ID_EDC_2))
            {
                taskENTER_CRITICAL();
                sat_data_buf.obdh.data.main_edc = (uint8_t)(*buf);
                taskEXIT_CRITICAL();
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
        }
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, OBDH_DATA_LOG_NAME, "Invalid parameter to set in OBDH!");
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
        case OBDH_PARAM_ID_TIME_COUNTER:             *buf = system_get_time();                                                break;
        case OBDH_PARAM_ID_TEMPERATURE_UC:           *buf = sat_data_buf.obdh.data.temperature;                               break;
        case OBDH_PARAM_ID_INPUT_CURRENT:            *buf = sat_data_buf.obdh.data.current;                                   break;
        case OBDH_PARAM_ID_INPUT_VOLTAGE:            *buf = sat_data_buf.obdh.data.voltage;                                   break;
        case OBDH_PARAM_ID_LAST_RESET_CAUSE:         *buf = sat_data_buf.obdh.data.last_reset_cause;                          break;
        case OBDH_PARAM_ID_RESET_COUNTER:            *buf = sat_data_buf.obdh.data.reset_counter;                             break;
        case OBDH_PARAM_ID_LAST_VALID_TC:            *buf = sat_data_buf.obdh.data.last_valid_tc;                             break;
        case OBDH_PARAM_ID_TEMPERATURE_ANTENNA:      *buf = sat_data_buf.antenna.data.temperature;                            break;
        case OBDH_PARAM_ID_ANTENNA_STATUS:           *buf = sat_data_buf.antenna.data.status.code;                            break;
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
        case OBDH_PARAM_ID_MANUAL_MODE_ON:           *buf = sat_data_buf.obdh.data.manual_mode_on;                            break;
        case OBDH_PARAM_ID_MAIN_EDC:                 *buf = sat_data_buf.obdh.data.main_edc;                                  break;
        case OBDH_PARAM_ID_GENERAL_TELEMETRY_ON:     *buf = sat_data_buf.obdh.data.general_telemetry_on;                      break;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, OBDH_DATA_LOG_NAME, "Invalid parameter to get from OBDH!");
            sys_log_new_line();
            err = -1;
            break;
    }

    return err;
}

/** \} End of obdh_data group */
