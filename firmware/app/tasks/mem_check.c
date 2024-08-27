/*
 * mem_check.c
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
 * \brief Memory Health Check Tasks definitions
 * 
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 0.10.19
 * 
 * \date 2024/07/24
 * 
 * \addtogroup mem_check 
 * \{
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <config/config.h>
#include <system/sys_log/sys_log.h>
#include <devices/media/media.h>
#include <drivers/flash/flash.h>

#include <utils/mem_mng.h>
#include <structs/satellite.h>

#include "mem_check.h"
#include "startup.h"
#include "mode_check.h"

#define PAGE_SIZE           ((uint32_t)256UL)
#define SEG_SIZE            ((uint32_t)128UL)
#define PAGE_TO_ADDR(page)  ((page) * PAGE_SIZE)
#define PAGE_CHECK_DEPTH    ((uint32_t)CONFIG_HEALTH_CHECK_PAGES_TO_VALIDATE)
#define ARR_SIZE(arr)       (sizeof((arr))/sizeof((arr)[0]))
#define ADDR_TO_PAGE(addr)  ((uint32_t)(addr) / PAGE_SIZE)

TaskHandle_t xTaskHealthCheckMemHandle;

static void prepare_obdh_s(obdh_telemetry_t *tel);
static void nor_sector_check(const char *msg, const uint8_t *data, uint32_t first_page);
static void fram_sector_check(const char *msg, const uint8_t *data, uint32_t addr);
static void int_flash_sector_check(const char *msg, const uint8_t *data, uint32_t addr);
static bool mem_check_pages(media_t media, const uint8_t *page_data, uint32_t first_page, uint32_t last_page, uint32_t *pages_left);
static int32_t mem_full_clean(void);

void vTaskHealthCheckMem(void) 
{
    /* Wait startup task to finish */
    xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_HEALTH_CHECK_MEM_INIT_TIMEOUT_MS));

    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_HEALTH_CHECK_MEM_NAME,  "Preparing Health Check on memories...");
    sys_log_new_line();

    vTaskDelay(pdMS_TO_TICKS(10U));

    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_HEALTH_CHECK_MEM_NAME,  "Cleaning memories...");
    sys_log_new_line();

    if (mem_full_clean() < 0) 
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MEM_NAME, "Failed to clean memories");
        sys_log_new_line();
    }

    uint8_t test_data[PAGE_SIZE];

    for (uint16_t i = 0U; i < PAGE_SIZE; ++i)
    {
        test_data[i] = i;
    }

    vTaskDelay(pdMS_TO_TICKS(10U));

    while (true) 
    {
        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_HEALTH_CHECK_MEM_NAME, "Starting Health Check...");
        sys_log_new_line();

        const uint32_t nor_pages[] = {CONFIG_MEM_OBDH_DATA_START_PAGE, CONFIG_MEM_EPS_DATA_START_PAGE,
                                      CONFIG_MEM_TTC_0_DATA_START_PAGE, CONFIG_MEM_TTC_1_DATA_START_PAGE,
                                      CONFIG_MEM_ANT_DATA_START_PAGE, CONFIG_MEM_EDC_DATA_START_PAGE,
                                      CONFIG_MEM_PX_DATA_START_PAGE, CONFIG_MEM_SBCD_PKTS_START_PAGE};

        const char *nor_msgs[] = {"OBDH", "EPS", "TTC 0", "TTC 1", "ANT", "EDC", "PX", "SBDC"};

        for (uint8_t i = 0U; i < ARR_SIZE(nor_pages); ++i) 
        {
            nor_sector_check(nor_msgs[i], test_data, nor_pages[i]);
        }

        vTaskDelay(pdMS_TO_TICKS(10U));

        const uint32_t fram_addrs[] = {CONFIG_MEM_ADR_INIT_WORD, CONFIG_MEM_ADR_SYS_PARAM, CONFIG_MEM_ADR_SYS_TIME};
        const char *fram_msgs[] = {"INIT_WORD", "SYS_PARAM", "SYS_TIME"};

        for (uint8_t i = 0U; i < ARR_SIZE(fram_addrs); ++i)
        {
            fram_sector_check(fram_msgs[i], test_data, fram_addrs[i]);
        }

        vTaskDelay(pdMS_TO_TICKS(10U));

        int_flash_sector_check("Internal Flash Seg A Test", test_data, 0ULL);

        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_HEALTH_CHECK_MEM_NAME, "Health Check finished. Cleaning up...");
        sys_log_new_line();

        vTaskDelay(pdMS_TO_TICKS(10U));
        
        if (mem_full_clean() < 0)
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MEM_NAME, "Failed to clean memories after test");
            sys_log_new_line();
        }

        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_HEALTH_CHECK_MEM_NAME, "Preparing load config test...");
        sys_log_new_line();

        obdh_telemetry_t test_params;
        obdh_telemetry_t loaded_params;

        prepare_obdh_s(&test_params);

        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_HEALTH_CHECK_MEM_NAME, "Starting load config test...");
        sys_log_new_line();

        if (mem_mng_save_obdh_data_to_fram(&test_params) != 0)
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MEM_NAME, "Could not save params to FRAM");
            sys_log_new_line();
        }
        
        if (mem_mng_load_obdh_data_from_fram(&loaded_params) != 0)
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MEM_NAME, "Could not read params from FRAM");
            sys_log_new_line();
        }

        bool param_test_result = (memcmp(&test_params, &loaded_params, sizeof(obdh_telemetry_t)) == 0);

        sys_log_print_test_result(param_test_result, "System param loading from FRAM Test");
        sys_log_new_line();

        /* TODO */
        /* Test Redundancy from internal FLASH **COMPLETELY** */

        (void)memset(&loaded_params, 0, sizeof(obdh_telemetry_t));

        mem_mng_save_obdh_data_bak(&test_params);

        vTaskDelay(pdMS_TO_TICKS(10U));

        if (mem_mng_load_obdh_data_bak(&loaded_params) != 0) 
        {
            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_HEALTH_CHECK_MEM_NAME, "Could not load media info from INT FLASH Correctly");
            sys_log_new_line();
        }

        vTaskDelay(pdMS_TO_TICKS(10U));

        bool int_flash_res = (memcmp(&test_params, &loaded_params, BAK_DATA_SIZE) == 0);

        sys_log_print_test_result(int_flash_res, "Media info backup Test");
        sys_log_new_line();

        vTaskDelay(pdMS_TO_TICKS(10U));

        uint8_t *test_ptr = (uint8_t*)&test_params;
        uint8_t *pos_ptr = (uint8_t*)&test_params.data.position;
        bool offset_test = (pos_ptr == &test_ptr[BAK_DATA_SIZE]);

        sys_log_print_test_result(offset_test, "Struct Offset Test");
        sys_log_new_line();

        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_HEALTH_CHECK_MEM_NAME, "Cleaning up...");
        sys_log_new_line();

        vTaskDelay(pdMS_TO_TICKS(10U));
        
        if (mem_full_clean() < 0)
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MEM_NAME, "Failed to clean memories after test");
            sys_log_new_line();
        }

        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_HEALTH_CHECK_MEM_NAME, "Health Check finished!!");
        sys_log_new_line();

        /* Notify Next Health Check */
        xTaskNotify(xTaskHealthCheckModeHandle, 0U, eNoAction);

        vTaskSuspend(NULL);
    }
}

static void int_flash_sector_check(const char *msg, const uint8_t *data, uint32_t addr)
{
    uint8_t buf[SEG_SIZE];
    bool test_result;

    (void)memcpy(buf, data, SEG_SIZE);

    if (media_write(MEDIA_INT_FLASH, addr, buf, SEG_SIZE) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MEM_NAME, "Failed to write to Int Flash segment: ");
        sys_log_print_hex(addr);
        sys_log_new_line();

        sys_log_print_test_result(false, msg);
        sys_log_new_line();

        return; // cppcheck-suppress misra-c2012-15.5
    }

    vTaskDelay(pdMS_TO_TICKS(10U));

    (void)memset(buf, 0U, SEG_SIZE);

    if (media_read(MEDIA_INT_FLASH, addr, buf, SEG_SIZE) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MEM_NAME, "Failed to read from Int Flash segment: ");
        sys_log_print_hex(addr);
        sys_log_new_line();

        sys_log_print_test_result(false, msg);
        sys_log_new_line();

        return; // cppcheck-suppress misra-c2012-15.5
    }
    
    vTaskDelay(pdMS_TO_TICKS(10U));

    test_result = (memcmp(data, buf, SEG_SIZE) == 0);

    sys_log_print_test_result(test_result, msg);
    sys_log_new_line();
}

static void fram_sector_check(const char *msg, const uint8_t *data, uint32_t addr)
{
    uint32_t page = ADDR_TO_PAGE(addr);
    uint32_t last_page = page + (uint32_t)1ULL;
    uint32_t pages_left;
    bool test_result;

    do {
        test_result = mem_check_pages(MEDIA_FRAM, data, page, last_page, &pages_left);

        sys_log_print_test_result(test_result, msg);
        sys_log_print_msg(" Sector Test");
        sys_log_new_line();

        page++;
    } while (pages_left > 0ULL);
}

static void nor_sector_check(const char *msg, const uint8_t *data, uint32_t first_page)
{
    uint32_t page;
    uint32_t last_page;
    uint32_t pages_left;
    bool test_result;

    page = first_page;
    last_page = page + PAGE_CHECK_DEPTH;

    do {
        test_result = mem_check_pages(MEDIA_NOR, data, page, last_page, &pages_left);

        sys_log_print_test_result(test_result, msg);
        sys_log_print_msg(" Sector, Page: ");
        sys_log_print_uint(page);
        sys_log_print_msg(" Test");
        sys_log_new_line();

        page++;
    } while (pages_left > 0ULL);
}

static bool mem_check_pages(media_t media, const uint8_t *page_data, uint32_t first_page, uint32_t last_page, uint32_t *pages_left)
{
    uint8_t buf[PAGE_SIZE];
    bool test_result = false;
    uint32_t addr = PAGE_TO_ADDR(first_page);

    *pages_left = last_page - first_page;

    (void)memcpy(buf, page_data, PAGE_SIZE);

    if (media_write(media, addr, buf, PAGE_SIZE) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MEM_NAME, "Failed to write to NOR page: ");
        sys_log_print_uint(first_page);
        sys_log_new_line();
        return false; // cppcheck-suppress misra-c2012-15.5
    }

    vTaskDelay(pdMS_TO_TICKS(10U));

    (void)memset(buf, 0U, PAGE_SIZE);

    if (media_read(media, addr, buf, PAGE_SIZE) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MEM_NAME, "Failed to read NOR page: ");
        sys_log_print_uint(first_page);
        sys_log_new_line();
        return false; // cppcheck-suppress misra-c2012-15.5
    }
    
    vTaskDelay(pdMS_TO_TICKS(10U));

    test_result = (memcmp(page_data, buf, PAGE_SIZE) == 0);

    (*pages_left)--;

    return test_result;
}

static int32_t mem_full_clean(void) 
{
    int32_t err = 0;

    if (media_erase(MEDIA_NOR, MEDIA_ERASE_DIE, 0U) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MEM_NAME, "Failed to erase NOR die ");
        sys_log_print_uint((uint32_t) 0ULL);
        sys_log_new_line();
        err--;
    }

    if (media_erase(MEDIA_NOR, MEDIA_ERASE_DIE, 1U) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MEM_NAME, "Failed to erase NOR die ");
        sys_log_print_uint((uint32_t) 1ULL);
        sys_log_new_line();
        err--;
    }

    if (media_erase(MEDIA_INT_FLASH, MEDIA_ERASE_SECTOR, FLASH_SEG_A_ADR) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MEM_NAME, "Failed to erase Int Flash Segment A");
        sys_log_new_line();
        err--;
    }

    if (media_erase(MEDIA_INT_FLASH, MEDIA_ERASE_SECTOR, FLASH_SEG_B_ADR) != 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MEM_NAME, "Failed to erase Int Flash Segment B");
        sys_log_new_line();
        err--;
    }

    const uint32_t addrs[] = {CONFIG_MEM_ADR_INIT_WORD, CONFIG_MEM_ADR_SYS_PARAM, CONFIG_MEM_ADR_SYS_TIME};
    uint8_t buf[PAGE_SIZE];

    (void)memset(buf, 0xFFU, PAGE_SIZE);

    for (uint8_t i = 0U; i < ARR_SIZE(addrs); ++i)
    {
        if (media_write(MEDIA_FRAM, addrs[i], buf, PAGE_SIZE) != 0)
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_HEALTH_CHECK_MEM_NAME, "Failed to erase FRAM on addr: ");
            sys_log_print_hex(addrs[i]);
            sys_log_new_line();
            err--;
        }
    }

    return err;
}

static void prepare_obdh_s(obdh_telemetry_t *tel) 
{
    tel->timestamp                          = 0xaaaaaaaa;
    tel->data.temperature                   = 300U;
    tel->data.current                       = 40U;
    tel->data.voltage                       = 3300U;
    tel->data.last_reset_cause              = 0x01;
    tel->data.reset_counter                 = 0xa0;
    tel->data.last_valid_tc                 = 0x10;
    tel->data.hw_version                    = 0x02;
    tel->data.fw_version                    = 0x1018;
    tel->data.mode                          = 0x01;
    tel->data.ts_last_mode_change           = 0xbbbbbbbb;
    tel->data.mode_duration                 = 0xcccccccc;
    tel->data.initial_hib_executed          = true;
    tel->data.initial_hib_time_count        = 40U;
    tel->data.ant_deployment_executed       = true;
    tel->data.ant_deployment_counter        = 5U;
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

/** \} End of mem_check group */
