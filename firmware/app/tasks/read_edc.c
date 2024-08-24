/*
 * read_edc.h
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
 * \brief Read EDC task implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 0.10.19
 * 
 * \date 2020/08/16
 * 
 * \addtogroup read_edc
 * \{
 */

#include <FreeRTOS.h>
#include <task.h>
#include <system/sys_log/sys_log.h>
#include <devices/payload/payload.h>
#include <devices/media/media.h>
#include <drivers/edc/edc.h>
#include <system/system.h>
#include <utils/mem_mng.h>

#include <structs/satellite.h>

#include "read_edc.h"
#include "startup.h"

xTaskHandle xTaskReadEDCHandle;

pl_edc_hk_raw_t edc_hk_buf = {0};

static void print_edc_hk(uint8_t *hk);

void vTaskReadEDC(void)
{
    payload_telemetry_t ** const edc = &sat_data_buf.state.c_edc;

    /* Wait startup task to finish */
    xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_READ_EDC_INIT_TIMEOUT_MS));

    media_info_t nor_info = media_get_info(MEDIA_NOR);

    TickType_t last_cycle = xTaskGetTickCount();

    while(1)
    {
        payload_t pl_edc_active = sat_data_buf.state.active_payload[0];

        if ((pl_edc_active != PAYLOAD_NONE) && (sat_data_buf.state.edc_active))
        {
            /* Read housekeeping data */
            if (payload_get_data(pl_edc_active, PAYLOAD_EDC_RAW_HK, edc_hk_buf.buffer, &edc_hk_buf.length) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_READ_EDC_NAME, "Error reading the housekeeping data!");
                sys_log_new_line();
            }
            else 
            {
                (void)memcpy((*edc)->data, edc_hk_buf.buffer, EDC_FRAME_HK_LEN);

                vTaskDelay(pdMS_TO_TICKS(50U));

                print_edc_hk(edc_hk_buf.buffer);
            }

            vTaskDelay(pdMS_TO_TICKS(500));     /* Wait a while for the next command */

            /* Read packets */
            uint8_t state_arr[10] = {0};
            int32_t state_len = 0;

            if (payload_get_data(pl_edc_active, PAYLOAD_EDC_STATE, state_arr, &state_len) == 0)
            {
                if (state_len >= (int32_t)sizeof(edc_state_t))
                {
                    (void)memcpy(&(*edc)->data[EDC_FRAME_HK_LEN], state_arr, EDC_FRAME_STATE_LEN);

                    (*edc)->timestamp = system_get_time();

                    edc_state_t state = *(edc_state_t*)&state_arr[0];

                    if (state.ptt_available > 0)
                    {
                        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_READ_EDC_NAME, "");
                        sys_log_print_uint(state.ptt_available);
                        sys_log_print_msg(" PTT packet(s) available to read!");
                        sys_log_new_line();

                        vTaskDelay(pdMS_TO_TICKS(50U));

                        uint8_t i = 0;
                        for(i = 0; i < state.ptt_available; i++)
                        {
                            uint8_t ptt_arr[50] = {0};
                            int32_t ptt_len = 0;

                            if (payload_get_data(pl_edc_active, PAYLOAD_EDC_PTT, ptt_arr, &ptt_len) == 0)
                            {
                                if (mem_mng_write_data_to_flash_page(ptt_arr, &sat_data_buf.obdh.data.media.last_page_sbcd_pkts, nor_info.page_size, CONFIG_MEM_SBCD_PKTS_START_PAGE, CONFIG_MEM_SBCD_PKTS_END_PAGE) != 0)
                                {
                                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_READ_EDC_NAME, "Error writing the PTT packet to the flash memory!");
                                    sys_log_new_line();
                                }

                                edc_ptt_t *ptt = (edc_ptt_t*)&ptt_arr[0];

                                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_READ_EDC_NAME, "Received PTT packet:");
                                sys_log_new_line();
                                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_READ_EDC_NAME, "\tTime: ");
                                sys_log_print_uint(ptt->time_tag);
                                sys_log_print_msg(" sec");
                                sys_log_new_line();
                                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_READ_EDC_NAME, "\tError code: ");
                                sys_log_print_uint(ptt->error_code);
                                sys_log_new_line();
                                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_READ_EDC_NAME, "\tCarrier frequency: ");
                                sys_log_print_uint(ptt->carrier_freq);
                                sys_log_print_msg(" Hz");
                                sys_log_new_line();
                                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_READ_EDC_NAME, "\tCarrier amplitude: ");
                                sys_log_print_uint(ptt->carrier_abs);
                                sys_log_new_line();
                                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_READ_EDC_NAME, "\tUser message: ");
                                sys_log_dump_hex(ptt->user_msg, ptt->msg_byte_length);
                                sys_log_new_line();
                            }
                            else
                            {
                                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_READ_EDC_NAME, "Error reading PTT package!");
                                sys_log_new_line();
                            }

                            vTaskDelay(pdMS_TO_TICKS(500));     /* Wait a while for the next command */
                        }
                    }
                }
            }
            else
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_READ_EDC_NAME, "Error reading the state data!");
                sys_log_new_line();
            }
        }

        vTaskDelayUntil(&last_cycle, pdMS_TO_TICKS(TASK_READ_EDC_PERIOD_MS));
    }
}

static void print_edc_hk(uint8_t *hk)
{
    uint16_t curr_dig = (hk[10] << 8U) | hk[9];
    uint16_t curr_rf = (hk[12] << 8U) | hk[11];
    uint16_t volt = (hk[14] << 8U) | hk[13];
    int16_t temp = (int16_t)hk[15] - 40;

    sys_log_print_event_from_module(SYS_LOG_INFO, "EDC Status", "Current(Dig): ");
    sys_log_print_uint((uint32_t) curr_dig);
    sys_log_print_msg(" mA");
    sys_log_new_line();

    sys_log_print_event_from_module(SYS_LOG_INFO, "EDC Status", "Current(RF): ");
    sys_log_print_uint((uint32_t) curr_rf);
    sys_log_print_msg(" mA");
    sys_log_new_line();

    sys_log_print_event_from_module(SYS_LOG_INFO, "EDC Status", "Sys Voltage: ");
    sys_log_print_uint((uint32_t) volt);
    sys_log_print_msg(" mV");
    sys_log_new_line();

    sys_log_print_event_from_module(SYS_LOG_INFO, "EDC Status", "Board Temp: ");
    sys_log_print_int((int32_t) temp);
    sys_log_print_msg(" oC");
    sys_log_new_line();
}

/** \} End of read_edc group */
