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
 * \version 1.0.0
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
#include <conops/conops.h>
#include <predict/predict.h>
#include <predict/unsorted.h>
#include <structs/satellite.h>

#include "pos_det.h"
#include "startup.h"
#include "mission_manager.h"

xTaskHandle xTaskPosDetHandle;

void vTaskPosDet(void *p)
{
    (void)p;

    static predict_orbital_elements_t satellite;
    static struct predict_sgp4 sgp4_model;
    static struct predict_sdp4 sdp4_model;

    /* Pointer used to see if TLE parsing was sucessfull */
    predict_orbital_elements_t *sat = NULL;

    /* Wait startup task to finish */
    (void)xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_POS_DET_INIT_TIMEOUT_MS));

    /* Parses binary TLE from FRAM (or default) */
    sat = predict_parse_compact_tle(&satellite, &sgp4_model, &sdp4_model, sat_data_buf.obdh.data.position.bin_tle);

    TickType_t last_cycle = xTaskGetTickCount();

    while(1)
    {
        /* Reload TLE lines if an update occured */
        if (xTaskNotifyWait(0UL, UINT32_MAX, NULL, 0UL) == pdTRUE)
        {
            sat = predict_parse_compact_tle(&satellite, &sgp4_model, &sdp4_model, sat_data_buf.obdh.data.position.bin_tle);
        }

        if (sat != NULL)
        {
            /* Predict satellite position */
            struct predict_position my_orbit;

            sys_time_t now = system_get_time();

            predict_julian_date_t curr_time = julian_from_timestamp(now);

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

            bool in_brazil = is_satellite_in_brazil(lat, lon);

            if (in_brazil)
            {
                const struct conops_event in_brazil_ev = {
                    .ev_id = EV_IN_BRAZIL,
                    .src = 0U,
                    .ev_name = "InBrazil",
                    .callback = NULL,
                };

                if (notify_event_to_mission_manager(&in_brazil_ev) != 0)
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_POS_DET_NAME, "Failed to notify \"in brazil\" event");
                    sys_log_new_line();
                }
            }
            else 
            {
                const struct conops_event out_of_brazil_ev = {
                    .ev_id = EV_OUT_OF_BRAZIL,
                    .src = 0U,
                    .ev_name = "OutBrazil",
                    .callback = NULL,
                };

                if (notify_event_to_mission_manager(&out_of_brazil_ev) != 0)
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_POS_DET_NAME, "Failed to notify \"out of brazil\" event");
                    sys_log_new_line();
                }
            }
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_POS_DET_NAME, "Failed to parse last available TLEs!");
            sys_log_new_line();
        }

        vTaskDelayUntil(&last_cycle, pdMS_TO_TICKS(TASK_POS_DET_PERIOD_MS));
    }
}

int update_tle_line(obdh_telemetry_t *obdh, const uint8_t *bin_tle)
{
    int err = 0;

    (void)memcpy(obdh->data.position.bin_tle, bin_tle, 50U);

    /* Store timestamp of the update */
    obdh->data.position.ts_last_tle_update = system_get_time();

    /* Save new OBDH data to fram */
    if (mem_mng_save_obdh_data_to_fram(obdh) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_POS_DET_NAME, "Failed to save OBDH data after TLE Update!");
        sys_log_new_line();
        err = -1;
    }

    /* Notify Position Determination Task of TLE update */
    (void)xTaskNotify(xTaskPosDetHandle, 0U, eNoAction);

    return err;
}

/** \} End of pos_det group */
