/*
 * mem_mng.c
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
 * \brief Memory management routines implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.10.9
 * 
 * \date 2024/02/24
 * 
 * \addtogroup mem_mng
 * \{
 */

#include <stdint.h>
#include <string.h>

#include <config/config.h>

#include "mem_mng.h"

int mem_mng_check_fram(void)
{
    int err = -1;

    uint8_t mem_word_ref[8] = CONFIG_MEM_INIT_WORD;
    uint8_t mem_word[8];

    if (media_read(MEDIA_FRAM, CONFIG_MEM_ADR_INIT_WORD, mem_word, 8U) == 0)
    {
        if (memcmp(&mem_word_ref[0], &mem_word[0], 8U) == 0)
        {
            err = 0;
        }
    }

    return err;
}

int mem_mng_init_fram(void)
{
    int err = -1;

    uint8_t mem_word_ref[8] = CONFIG_MEM_INIT_WORD;

    if (media_write(MEDIA_FRAM, CONFIG_MEM_ADR_INIT_WORD, mem_word_ref, 8U) == 0)
    {
        err = 0;
    }

    return err;
}

int mem_mng_reset_fram(void)
{
    int err = -1;

    uint8_t mem_word[8] = {0};

    if (media_write(MEDIA_FRAM, CONFIG_MEM_ADR_INIT_WORD, mem_word, 8U) == 0)
    {
        err = 0;
    }

    return err;
}

void mem_mng_load_obdh_data_from_default_values(obdh_telemetry_t *tel)
{
    tel->timestamp                          = OBDH_TIMESTAMP_DEFAULT_VAL;
    tel->data.temperature                   = OBDH_PARAM_TEMPERATURE_UC_DEFAULT_VAL;
    tel->data.current                       = OBDH_PARAM_CURRENT_DEFAULT_VAL;
    tel->data.voltage                       = OBDH_PARAM_VOLTAGE_DEFAULT_VAL;
    tel->data.last_reset_cause              = OBDH_PARAM_LAST_RESET_CAUSE_DEFAULT_VAL;
    tel->data.reset_counter                 = OBDH_PARAM_RESET_COUNTER_DEFAULT_VAL;
    tel->data.last_valid_tc                 = OBDH_PARAM_LAST_VALID_TC_DEFAULT_VAL;
    tel->data.hw_version                    = OBDH_PARAM_HW_VERSION_DEFAULT_VAL;
    tel->data.fw_version                    = OBDH_PARAM_FW_VERSION_DEFAULT_VAL;
    tel->data.mode                          = OBDH_PARAM_MODE_DEFAULT_VAL;
    tel->data.ts_last_mode_change           = OBDH_PARAM_TS_LAST_MODE_CHANGE_DEFAULT_VAL;
    tel->data.mode_duration                 = OBDH_PARAM_MODE_DURATION_DEFAULT_VAL;
    tel->data.initial_hib_executed          = OBDH_PARAM_INITIAL_HIB_EXECUTED_DEFAULT_VAL;
    tel->data.initial_hib_time_count        = OBDH_PARAM_INITIAL_HIB_TIME_COUNT_DEFAULT_VAL;
    tel->data.ant_deployment_executed       = OBDH_PARAM_ANT_DEPLOYMENT_EXECUTED_DEFAULT_VAL;
    tel->data.ant_deployment_counter        = OBDH_PARAM_ANT_DEPLOYMENT_COUNTER_DEFAULT_VAL;
    tel->data.position.timestamp            = OBDH_PARAM_POSITION_TIMESTAMP_DEFAULT_VAL;

    uint8_t tle_line1[70] = OBDH_PARAM_POSITION_TLE_LINE1_DEFAULT_VAL;
    uint8_t tle_line2[70] = OBDH_PARAM_POSITION_TLE_LINE2_DEFAULT_VAL;

    (void)memcpy(&tel->data.position.tle_line1, &tle_line1[0], 70U);
    (void)memcpy(&tel->data.position.tle_line2, &tle_line2[0], 70U);

    tel->data.position.latitude             = OBDH_PARAM_POSITION_LATITUDE_DEFAULT_VAL;
    tel->data.position.longitude            = OBDH_PARAM_POSITION_LONGITUDE_DEFAULT_VAL;
    tel->data.position.altitude             = OBDH_PARAM_POSITION_ALTITUDE_DEFAULT_VAL;
    tel->data.media.last_page_obdh_data     = OBDH_PARAM_MEDIA_LAST_OBDH_DATA_DEFAULT_VAL;
    tel->data.media.last_page_eps_data      = OBDH_PARAM_MEDIA_LAST_EPS_DATA_DEFAULT_VAL;
    tel->data.media.last_page_ttc_0_data    = OBDH_PARAM_MEDIA_LAST_TTC_0_DATA_DEFAULT_VAL;
    tel->data.media.last_page_ttc_1_data    = OBDH_PARAM_MEDIA_LAST_TTC_1_DATA_DEFAULT_VAL;
    tel->data.media.last_page_ant_data      = OBDH_PARAM_MEDIA_LAST_ANT_DATA_DEFAULT_VAL;
    tel->data.media.last_page_edc_data      = OBDH_PARAM_MEDIA_LAST_EDC_DATA_DEFAULT_VAL;
    tel->data.media.last_page_px_data       = OBDH_PARAM_MEDIA_LAST_PX_DATA_DEFAULT_VAL;
    tel->data.media.last_page_sbcd_pkts     = OBDH_PARAM_MEDIA_LAST_SBCD_PKTS_DEFAULT_VAL;
}

int mem_mng_save_obdh_data_to_fram(obdh_telemetry_t* tel)
{
    int err = -1;

    uint8_t buf[256];

    (void)memcpy(buf, tel, sizeof(obdh_telemetry_t));

    if (media_write(MEDIA_FRAM, CONFIG_MEM_ADR_SYS_PARAM, buf, 256U) == 0)
    {
        err = 0;
    }

    return err;
}

int mem_mng_load_obdh_data_from_fram(obdh_telemetry_t *tel)
{
    int err = -1;

    uint8_t sys_par[256];

    if (media_read(MEDIA_FRAM, CONFIG_MEM_ADR_SYS_PARAM, sys_par, sizeof(obdh_telemetry_t)) == 0)
    {
        (void)memcpy(tel, sys_par, sizeof(obdh_telemetry_t));
        err = 0;
    }

    return err;
}

/** \} End of mem_mng group */
