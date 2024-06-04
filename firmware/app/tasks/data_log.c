/*
 * data_log.c
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
 * \brief Data log task implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.10.9
 * 
 * \date 2021/05/24
 * 
 * \addtogroup data_log
 * \{
 */

#include <stdint.h>

#include <system/sys_log/sys_log.h>
#include <devices/media/media.h>
#include <structs/satellite.h>

#include "data_log.h"
#include "startup.h"

xTaskHandle xTaskDataLogHandle;

/**
 * \brief Writes data to a given flash memory page.
 *
 * \param[in] is the array of bytes to write.
 *
 * \param[in,out] page is a pointer to the current page to write.
 *
 * \param[in] page_size is the page size, in bytes, of the flash memory.
 *
 * \param[in] start_page is the possible start page to write.
 *
 * \param[in] end_page is the possible end page to write.
 *
 * \return The status/error code.
 */
int write_data_to_flash_page(uint8_t *data, uint32_t *page, uint32_t page_size, uint32_t start_page, uint32_t end_page);

void vTaskDataLog(void)
{
    /* Wait startup task to finish */
    xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_DATA_LOG_INIT_TIMEOUT_MS));

    media_info_t nor_info = media_get_info(MEDIA_NOR);

    uint32_t mem_page = 0U;
    uint8_t page_buf[256] = {0};
    void *null_ptr;

    while(1)
    {
        TickType_t last_cycle = xTaskGetTickCount();

        /* OBDH data */
        if (memcpy(&page_buf[0], &sat_data_buf.obdh, sizeof(obdh_telemetry_t)) == &page_buf[0])
        {
            if (write_data_to_flash_page(page_buf, &sat_data_buf.obdh.data.media.last_page_obdh_data, nor_info.page_size, CONFIG_MEM_OBDH_DATA_START_PAGE, CONFIG_MEM_OBDH_DATA_END_PAGE) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_DATA_LOG_NAME, "Error writing the OBDH data to the flash memory!");
                sys_log_new_line();
            }
        }

        null_ptr = memset(&page_buf[0], 0, 256);

        /* EPS data */
        if (memcpy(&page_buf[0], &sat_data_buf.eps, sizeof(eps_telemetry_t)) == &page_buf[0])
        {
            if (write_data_to_flash_page(page_buf, &sat_data_buf.obdh.data.media.last_page_eps_data, nor_info.page_size, CONFIG_MEM_EPS_DATA_START_PAGE, CONFIG_MEM_EPS_DATA_END_PAGE) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_DATA_LOG_NAME, "Error writing the EPS data to the flash memory!");
                sys_log_new_line();
            }
        }

        null_ptr = memset(&page_buf[0], 0, 256);

        /* TTC 0 data */
        if (memcpy(&page_buf[0], &sat_data_buf.ttc_0, sizeof(ttc_telemetry_t)) == &page_buf[0])
        {
            if (write_data_to_flash_page(page_buf, &sat_data_buf.obdh.data.media.last_page_ttc_0_data, nor_info.page_size, CONFIG_MEM_TTC_0_DATA_START_PAGE, CONFIG_MEM_TTC_0_DATA_END_PAGE) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_DATA_LOG_NAME, "Error writing the TTC 0 data to the flash memory!");
                sys_log_new_line();
            }
        }

        null_ptr = memset(&page_buf[0], 0, 256);

        /* TTC 1 data */
        if (memcpy(&page_buf[0], &sat_data_buf.ttc_1, sizeof(ttc_telemetry_t)) == &page_buf[0])
        {
            if (write_data_to_flash_page(page_buf, &sat_data_buf.obdh.data.media.last_page_ttc_1_data, nor_info.page_size, CONFIG_MEM_TTC_1_DATA_START_PAGE, CONFIG_MEM_TTC_1_DATA_END_PAGE) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_DATA_LOG_NAME, "Error writing the TTC 1 data to the flash emory!");
                sys_log_new_line();
            }
        }

        null_ptr = memset(&page_buf[0], 0, 256);

        /* Antenna data */
        if (memcpy(&page_buf[0], &sat_data_buf.antenna, sizeof(antenna_telemetry_t)) == &page_buf[0])
        {
            if (write_data_to_flash_page(page_buf, &sat_data_buf.obdh.data.media.last_page_ant_data, nor_info.page_size, CONFIG_MEM_ANT_DATA_START_PAGE, CONFIG_MEM_ANT_DATA_END_PAGE) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_DATA_LOG_NAME, "Error writing the antenna data to the flash memory!");
                sys_log_new_line();
            }
        }

        null_ptr = memset(&page_buf[0], 0, 256);

        /* EDC data */
        if (memcpy(&page_buf[0], &sat_data_buf.edc_0, sizeof(payload_telemetry_t)) == &page_buf[0])
        {
            if (write_data_to_flash_page(page_buf, &sat_data_buf.obdh.data.media.last_page_edc_data, nor_info.page_size, CONFIG_MEM_EDC_DATA_START_PAGE, CONFIG_MEM_EDC_DATA_END_PAGE) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_DATA_LOG_NAME, "Error writing the EDC data to the flash memory!");
                sys_log_new_line();
            }
        }

        null_ptr = memset(&page_buf[0], 0, 256);

        /* Payload-X data */
        if (memcpy(&page_buf[0], &sat_data_buf.payload_x, sizeof(payload_telemetry_t)) == &page_buf[0])
        {
            if (write_data_to_flash_page(page_buf, &sat_data_buf.obdh.data.media.last_page_px_data, nor_info.page_size, CONFIG_MEM_PX_DATA_START_PAGE, CONFIG_MEM_PX_DATA_END_PAGE) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_DATA_LOG_NAME, "Error writing the Payload-X data to the flash memory!");
                sys_log_new_line();
            }
        }

        null_ptr = memset(&page_buf[0], 0, 256);

        vTaskDelayUntil(&last_cycle, pdMS_TO_TICKS(TASK_DATA_LOG_PERIOD_MS));
    }
}

int write_data_to_flash_page(uint8_t *data, uint32_t *page, uint32_t page_size, uint32_t start_page, uint32_t end_page)
{
    int err = -1;

    if (media_write(MEDIA_NOR, (*page) * page_size, data, page_size) == 0)
    {
        (*page)++;    // cppcheck-suppress misra-c2012-17.8

        if (*page > end_page)
        {
            *page = start_page;
        }

        err = 0;
    }

    return err;
}

/** \} End of file_system group */
