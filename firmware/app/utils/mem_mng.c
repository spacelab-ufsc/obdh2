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
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 1.0.0
 * 
 * \date 2024/02/24
 * 
 * \addtogroup mem_mng
 * \{
 */

#include <stdint.h>
#include <string.h>

#include <config/config.h>
#include <system/sys_log/sys_log.h>
#include <drivers/flash/flash.h>
#include <devices/media/media.h>
#include <structs/obdh_data.h>
#include <portmacro.h>

#include "mem_mng.h"

#define CRC8_POLYNOMIAL  0x07U     
#define CRC8_INITIAL_VAL 0x00U     
#define BAK_INIT_VAL     0xAAU   

static uint8_t crc8(uint8_t *data, uint8_t len);

static inline bool crc_is_valid(uint8_t *data, uint8_t len, uint8_t crc_to_check);

int mem_mng_check_fram(void)
{
    int err = -1;

    uint8_t mem_word_ref[8] = CONFIG_MEM_INIT_WORD;
    uint8_t mem_word[8];

    if (media_read(MEDIA_FRAM, CONFIG_MEM_ADR_INIT_WORD, mem_word, 8U) == 0)
    {
        if (memcmp(&mem_word_ref[0], &mem_word[0], 8U) == 0)
        {
            sys_log_print_event_from_module(SYS_LOG_INFO, MEM_MNG_NAME, "FRAM was already initalized");
            sys_log_new_line();

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
    tel->data.hib_duration                  = OBDH_PARAM_HIB_DURATION_DEFAULT_VAL;
    tel->data.initial_hib_executed          = OBDH_PARAM_INITIAL_HIB_EXECUTED_DEFAULT_VAL;
    tel->data.initial_hib_time_count        = OBDH_PARAM_INITIAL_HIB_TIME_COUNT_DEFAULT_VAL;
    tel->data.ant_deployment_executed       = OBDH_PARAM_ANT_DEPLOYMENT_EXECUTED_DEFAULT_VAL;
    tel->data.ant_deployment_counter        = OBDH_PARAM_ANT_DEPLOYMENT_COUNTER_DEFAULT_VAL;
    tel->data.hibernation_on                = OBDH_PARAM_HIBERNATION_ON_DEFAULT_VAL;
    tel->data.main_edc                      = OBDH_PARAM_MAIN_EDC_DEFAULT_VAL;
    tel->data.general_telemetry_on          = OBDH_PARAM_GENERAL_TELEMETRY_ON_DEFAUL_VAL;
    tel->data.ts_read_sensors               = OBDH_PARAM_TS_READ_SENSORS_DEFAULT_VAL;
    tel->data.main_payload_state            = OBDH_PARAM_MAIN_PAYLOAD_STATE_DEFAULT_VAL;
    tel->data.sec_payload_state             = OBDH_PARAM_SEC_PAYLOAD_STATE_DEFAULT_VAL;
    tel->data.ts_last_contact               = OBDH_PARAM_TS_LAST_CONTACT_TIME_DEFAULT_VAL;
    tel->data.last_tran_ev_id               = OBDH_PARAM_LAST_TRAN_EV_ID_DEFAULT_VAL;
    tel->data.ts_commission_timeout         = OBDH_PARAM_TS_COMMISSION_TIMEOUT_DEFAULT_VAL;
    tel->data.eps_beacon_on                 = OBDH_PARAM_EPS_BEACON_ON_DEFAULT_VAL;
    tel->data.batt_crit_level_mv            = OBDH_PARAM_BATT_CRITICAL_LEVEL_MV_DEFAULT_VAL;
    tel->data.manual_experiments            = OBDH_PARAM_MANUAL_EXPERIMENT_ON_DEFAUL_VAL;
    tel->data.tc_queue_size                 = OBDH_PARAM_TC_QUEUE_SIZE_DEFAULT_VAL;
    tel->data.ts_next_sched_tc              = OBDH_PARAM_TS_NEXT_SCHED_TC_DEFAULT_VAL;

    uint8_t buf[50] = OBDH_PARAM_POSITION_BIN_TLE_DEFAULT_VAL;

    (void)memcpy(tel->data.position.bin_tle, buf, 50U);

    tel->data.position.timestamp            = OBDH_PARAM_POSITION_TIMESTAMP_DEFAULT_VAL;
    tel->data.position.latitude             = OBDH_PARAM_POSITION_LATITUDE_DEFAULT_VAL;
    tel->data.position.longitude            = OBDH_PARAM_POSITION_LONGITUDE_DEFAULT_VAL;
    tel->data.position.altitude             = OBDH_PARAM_POSITION_ALTITUDE_DEFAULT_VAL;
    tel->data.position.ts_last_tle_update   = OBDH_PARAM_POSITION_TS_LAST_TLE_UPT_DEFAULT_VAL;
    tel->data.media.last_page_obdh_data     = OBDH_PARAM_MEDIA_LAST_OBDH_DATA_DEFAULT_VAL;
    tel->data.media.last_page_eps_data      = OBDH_PARAM_MEDIA_LAST_EPS_DATA_DEFAULT_VAL;
    tel->data.media.last_page_ttc_0_data    = OBDH_PARAM_MEDIA_LAST_TTC_0_DATA_DEFAULT_VAL;
    tel->data.media.last_page_ttc_1_data    = OBDH_PARAM_MEDIA_LAST_TTC_1_DATA_DEFAULT_VAL;
    tel->data.media.last_page_ant_data      = OBDH_PARAM_MEDIA_LAST_ANT_DATA_DEFAULT_VAL;
    tel->data.media.last_page_edc_data      = OBDH_PARAM_MEDIA_LAST_EDC_DATA_DEFAULT_VAL;
    tel->data.media.last_page_px_data       = OBDH_PARAM_MEDIA_LAST_PX_DATA_DEFAULT_VAL;
    tel->data.media.last_page_sbcd_pkts     = OBDH_PARAM_MEDIA_LAST_SBCD_PKTS_DEFAULT_VAL;
}

int mem_mng_save_obdh_data_to_fram(obdh_telemetry_t *tel)
{
    int err = -1;

    uint8_t buf[sizeof(obdh_telemetry_t) + 1U];

    (void)memcpy(buf, (void*)tel, sizeof(obdh_telemetry_t));

    buf[sizeof(obdh_telemetry_t)] = crc8(buf, sizeof(obdh_telemetry_t));

    if (media_write(MEDIA_FRAM, CONFIG_MEM_ADR_SYS_PARAM, buf, (sizeof(obdh_telemetry_t) + 1U)) == 0)
    {
        err = 0;
    }

    return err;
}

int mem_mng_load_obdh_data_from_fram(obdh_telemetry_t *tel)
{
    int err = -1;

    uint8_t sys_par[sizeof(obdh_telemetry_t) + 1U];

    if (media_read(MEDIA_FRAM, CONFIG_MEM_ADR_SYS_PARAM, sys_par, (sizeof(obdh_telemetry_t) + 1U)) == 0)
    {
        if (crc_is_valid(sys_par, sizeof(obdh_telemetry_t), sys_par[sizeof(obdh_telemetry_t)]))
        {
            (void)memcpy((void*)tel, sys_par, sizeof(obdh_telemetry_t));
            err = 0;
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, MEM_MNG_NAME, "OBDH data CRC wasn't valid!");
            sys_log_new_line();
        }
    }

    return err;
}

int mem_mng_write_data_to_flash_page(uint8_t *data, uint32_t *page, uint32_t page_size, uint32_t start_page, uint32_t end_page)
{
    int err = -1;
    (void)start_page;

    if ((*page) > end_page)
    {
        sys_log_print_event_from_module(SYS_LOG_INFO, MEM_MNG_NAME, "Module sector will overflow! Cleaning memory to avoid corruption...");
        sys_log_new_line();

        uint8_t retry_count = 5U;

        do
        {
            err = mem_mng_erase_flash(&sat_data_buf.obdh);
        } while ((err < 0) && (retry_count > 0U));
    }

    if (media_write(MEDIA_NOR, (*page) * page_size, data, page_size) == 0)
    {
        portENTER_CRITICAL();

        (*page)++;    // cppcheck-suppress misra-c2012-17.8

        portEXIT_CRITICAL();

        err = 0;
    }

    return err;
}

void mem_mng_save_obdh_data_bak(obdh_telemetry_t *tel)
{
    uintptr_t base_addr = CONFIG_MEM_ADR_SYS_PARAM_BAK;
    uint8_t buf[BAK_DATA_SIZE + 2U];

    flash_erase(base_addr);

    (void)memcpy(buf, (void*)tel, BAK_DATA_SIZE);

    buf[BAK_DATA_SIZE] = crc8(buf, BAK_DATA_SIZE);
    buf[BAK_DATA_SIZE + 1U] = BAK_INIT_VAL;

    for (uint8_t i = 0U; i < BAK_DATA_SIZE + 2U; ++i)
    {
        uintptr_t addr = base_addr + i;
        flash_write_single(buf[i], addr);
    }
}

int mem_mng_load_obdh_data_bak(obdh_telemetry_t *tel)
{
    int err = -1;

    const uintptr_t base_addr = CONFIG_MEM_ADR_SYS_PARAM_BAK;
    uint8_t buf[BAK_DATA_SIZE + 2U];

    for (uint8_t i = 0U; i < BAK_DATA_SIZE + 2U; ++i)
    {
        uintptr_t addr = base_addr + i;
        buf[i] = flash_read_single(addr);
    }

    if ((buf[BAK_DATA_SIZE] == crc8(buf, BAK_DATA_SIZE)) && (buf[BAK_DATA_SIZE + 1U] == BAK_INIT_VAL))
    {
        (void)memcpy((void*)tel, buf, BAK_DATA_SIZE);
        err = 0;
    }

    return err;
}

void mem_mng_reset_page_count(media_data_t *media)
{
    portENTER_CRITICAL();
    media->last_page_obdh_data    = CONFIG_MEM_OBDH_DATA_START_PAGE;
    media->last_page_eps_data     = CONFIG_MEM_EPS_DATA_START_PAGE;
    media->last_page_ttc_0_data   = CONFIG_MEM_TTC_0_DATA_START_PAGE;
    media->last_page_ant_data     = CONFIG_MEM_ANT_DATA_START_PAGE;
    media->last_page_edc_data     = CONFIG_MEM_EDC_DATA_START_PAGE;
    media->last_page_px_data      = CONFIG_MEM_PX_DATA_START_PAGE;
    media->last_page_sbcd_pkts    = CONFIG_MEM_SBCD_PKTS_START_PAGE;
    portEXIT_CRITICAL();
}

int mem_mng_erase_flash(obdh_telemetry_t *tel)
{
    int err = 0;

    if (media_erase(MEDIA_NOR, MEDIA_ERASE_DIE, 0U) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, MEM_MNG_NAME, "Failed to erase flash die 0");
        sys_log_new_line();
        err--;
    }

    if (media_erase(MEDIA_NOR, MEDIA_ERASE_DIE, 1U) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, MEM_MNG_NAME, "Failed to erase flash die 1");
        sys_log_new_line();
        err--;
    }

    if (err == 0)
    {
        mem_mng_reset_page_count(&tel->data.media);
    }

    return err;
}

int mem_mng_load_tc_queue_from_fram(struct conops_cmd_queue *queue)
{
    int err = -1;

    uint8_t sys_par[sizeof(*queue) + 1U];

    if (media_read(MEDIA_FRAM, CONFIG_MEM_ADR_TC_QUEUE, sys_par, (sizeof(*queue) + 1U)) == 0)
    {
        if (crc_is_valid(sys_par, sizeof(*queue), sys_par[sizeof(*queue)]))
        {
            (void)memcpy((void*)queue, sys_par, sizeof(*queue));
            err = 0;
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, MEM_MNG_NAME, "TC queue CRC wasn't valid!");
            sys_log_new_line();
        }
    }

    return err;
}

int mem_mng_save_tc_queue_to_fram(struct conops_cmd_queue *queue)
{
    int err = -1;

    uint8_t buf[sizeof(*queue) + 1U];

    queue->queue_lock(queue->lock);

    (void)memcpy(buf, (void*)queue, sizeof(*queue));
    
    queue->queue_unlock(queue->lock);

    buf[sizeof(*queue)] = crc8(buf, sizeof(*queue));

    if (media_write(MEDIA_FRAM, CONFIG_MEM_ADR_TC_QUEUE, buf, (sizeof(*queue) + 1U)) == 0)
    {
        err = 0;
    }

    return err;
}

static uint8_t crc8(uint8_t *data, uint8_t len)
{
    uint8_t crc = CRC8_INITIAL_VAL;

    uint8_t i = 0U;
    for(i = 0; i < len; i++)
    {
        crc ^= data[i];

        uint8_t j = 0U;
        for (j = 0U; j < 8U; j++)
        {
            crc = (crc << 1) ^ ((crc & 0x80U) ? CRC8_POLYNOMIAL : 0U);
        }

        crc &= 0xFFU;
    }

    return crc;
}

static inline bool crc_is_valid(uint8_t *data, uint8_t len, uint8_t crc_to_check)
{
    return (crc8(data, len) == crc_to_check) ? true : false;
}

/** \} End of mem_mng group */
