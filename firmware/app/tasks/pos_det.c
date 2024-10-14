/*
 * pos_det.h
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
 * \brief Position determination task implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 0.10.20
 * 
 * \date 2023/07/19
 * 
 * \addtogroup pos_det
 * \{
 */

#include <stdint.h>
#include <string.h>

#include <system/system.h>
#include <system/sys_log/sys_log.h>
#include <config/config.h>
#include <predict/predict.h>
#include <predict/unsorted.h>
#include <structs/satellite.h>

#include "pos_det.h"
#include "startup.h"
#include "mission_manager.h"

static char tle_line_1[70];
static char tle_line_2[70];

xTaskHandle xTaskPosDetHandle;

void vTaskPosDet(void)
{
    static predict_orbital_elements_t satellite;
    static struct predict_sgp4 sgp4_model;
    static struct predict_sdp4 sdp4_model;

    /* Flag used to control notification sending */
    bool sat_is_inside_brazil = false;

    /* Wait startup task to finish */
    xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_POS_DET_INIT_TIMEOUT_MS));

    TickType_t last_cycle = xTaskGetTickCount();

    while(1)
    {
        /* Reload TLE lines if an update occured */
        if (xTaskNotifyWait(0UL, UINT32_MAX, NULL, 0UL) == pdTRUE)
        {
            (void)strncpy(tle_line_1, (const char*)sat_data_buf.obdh.data.position.tle_line1, 70U);
            (void)strncpy(tle_line_2, (const char*)sat_data_buf.obdh.data.position.tle_line2, 70U);

            /* Save new OBDH data to fram */
            if (mem_mng_save_obdh_data_to_fram(&sat_data_buf.obdh) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_POS_DET_NAME, "Failed to save OBDH data after TLE Update!");
                sys_log_new_line();
            }

            /* Store timestamp of the update */
            sat_data_buf.obdh.data.position.ts_last_tle_update = system_get_time();
        }

        /* Populate orbit elements */
        if (predict_parse_tle(&satellite, &sgp4_model, &sdp4_model, (const char*)sat_data_buf.obdh.data.position.tle_line1, (const char*)sat_data_buf.obdh.data.position.tle_line2) != NULL)
        {
            /* Predict satellite position */
            struct predict_position my_orbit;

            sys_time_t now = system_get_time();

            predict_julian_date_t curr_time = julian_from_timestamp(now + 1723341922ULL);   /* 1723341922ULL Corresponds to ISO Time Stamp: 2024-08-11T02:05:22Z */

            (void)predict_orbit(&satellite, &my_orbit, curr_time);

            float lat = predictRAD2DEG(my_orbit.latitude);
            float lon = predictRAD2DEG(my_orbit.longitude);
            float alt = my_orbit.altitude;

            sat_data_buf.obdh.data.position.latitude    = (int16_t)lat;
            sat_data_buf.obdh.data.position.longitude   = (int16_t)lon;
            sat_data_buf.obdh.data.position.altitude    = (int16_t)alt;
            sat_data_buf.obdh.data.position.timestamp   = now;

            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_POS_DET_NAME, "Current position (lat/lon/alt): ");
            sys_log_print_float(lat, 2);
            sys_log_print_msg(" deg/");
            sys_log_print_float(lon, 2);
            sys_log_print_msg(" deg/");
            sys_log_print_float(alt, 2);
            sys_log_print_msg(" km");
            sys_log_new_line();

            bool current_position = is_satellite_in_brazil(sat_data_buf.obdh.data.position.latitude, sat_data_buf.obdh.data.position.longitude);

            if (current_position && !sat_is_inside_brazil)
            {
                sat_is_inside_brazil = true;

                const event_t in_brazil_ev = { .event = EV_NOTIFY_IN_BRAZIL, .args[0] = 0U, .args[1] = 0U, .args[2] = 0U };

                if (notify_event_to_mission_manager(&in_brazil_ev) != 0)
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_POS_DET_NAME, "Failed to notify \"in brazil\" event");
                    sys_log_new_line();
                }
            }

            if (!current_position && sat_is_inside_brazil)
            {
                sat_is_inside_brazil = false;

                const event_t out_of_brazil_ev = { .event = EV_NOTIFY_OUT_OF_BRAZIL, .args[0] = 0U, .args[1] = 0U, .args[2] = 0U };

                if (notify_event_to_mission_manager(&out_of_brazil_ev) != 0)
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_POS_DET_NAME, "Failed to notify \"out of brazil\" event");
                    sys_log_new_line();
                }
            }
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_POS_DET_NAME, "Failed to parse TLEs");
            sys_log_new_line();
        }

        vTaskDelayUntil(&last_cycle, pdMS_TO_TICKS(TASK_POS_DET_PERIOD_MS));
    }
}

bool update_tle_line(uint8_t line_number, const uint8_t *tle_line)
{
    static uint8_t update_status;
    bool both_lines_updated = false;

    switch (line_number) 
    {
        case 0x01:
            (void)memcpy(tle_line_1, tle_line, 69U);
            tle_line_1[69] = '\0';
            update_status |= 0x01U;
            break;
        case 0x02:
            (void)memcpy(tle_line_2, tle_line, 69U);
            tle_line_2[69] = '\0';
            update_status |= 0x02U;
            break;
        default:
            break;
    }

    if (((update_status & 0x01U) != 0U) && ((update_status & 0x02U) != 0U))
    {
        both_lines_updated = true;
        update_status = 0U;
    }

    return both_lines_updated;
}

/** \} End of pos_det group */
