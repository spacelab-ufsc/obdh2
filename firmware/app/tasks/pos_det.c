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
 * 
 * \version 0.10.7
 * 
 * \date 2023/07/19
 * 
 * \addtogroup pos_det
 * \{
 */

#include <system/system.h>
#include <system/sys_log/sys_log.h>
#include <config/config.h>
#include <predict/predict.h>
#include <predict/unsorted.h>
#include <structs/satellite.h>

#include "pos_det.h"
#include "startup.h"

xTaskHandle xTaskPosDetHandle;

static predict_orbital_elements_t satellite;
static struct predict_sgp4 sgp4_model;
static struct predict_sdp4 sdp4_model;

void vTaskPosDet(void)
{
    /* Wait startup task to finish */
    xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_POS_DET_INIT_TIMEOUT_MS));

    while(1)
    {
        TickType_t last_cycle = xTaskGetTickCount();

        /* Load TLE lines */
        const char *tle_line_1 = "1 25544U 98067A   24223.83784911  .00020194  00000+0  36238-3 0  9994";
        const char *tle_line_2 = "2 25544  51.6408  44.5872 0005770 185.1957 306.5656 15.49872002467029";
        
        /* Populate orbit elements */
        if (predict_parse_tle(&satellite, &sgp4_model, &sdp4_model, tle_line_1, tle_line_2) != NULL)
        {
            /* Predict satellite position */
            struct predict_position my_orbit;

            sys_time_t now = system_get_time();

            predict_julian_date_t curr_time = julian_from_timestamp(now + 1723341922ULL);   /* 1723341922ULL Corresponds to ISO Time Stamp: 2024-08-11T02:05:22Z */

            predict_orbit(&satellite, &my_orbit, curr_time);

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
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_POS_DET_NAME, "Failed to parse TLEs");
            sys_log_new_line();
        }

        vTaskDelayUntil(&last_cycle, pdMS_TO_TICKS(TASK_POS_DET_PERIOD_MS));
    }
}

/** \} End of pos_det group */
