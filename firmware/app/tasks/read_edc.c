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
 * \version 1.0.0
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
#include <math.h>

#include <structs/satellite.h>

#include "read_edc.h"
#include "startup.h"

xTaskHandle xTaskReadEDCHandle;

pl_edc_hk_raw_t edc_hk_buf = {0};

static void print_edc_hk(edc_hk_t *hk);

void vTaskReadEDC(void *p)
{
    (void)p;

    /* Wait startup task to finish */
    (void)xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_READ_EDC_INIT_TIMEOUT_MS));

    vTaskDelay(pdMS_TO_TICKS(TASK_READ_EDC_INITIAL_DELAY_MS));

    media_info_t nor_info = media_get_info(MEDIA_NOR);

    TickType_t last_cycle = xTaskGetTickCount();

    while(1)
    {
        payload_t pl_edc_active = (payload_t)sat_data_buf.obdh.data.main_payload_state;

        edc_telemetry_t * const edc = sat_data_buf.state.c_edc;

        if ((pl_edc_active != PAYLOAD_NONE) && (edc != NULL))
        {
            /* Update EDC clock */
            if (payload_set_clock(pl_edc_active, system_get_time()) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_READ_EDC_NAME, "Error while setting EDC clock!");
                sys_log_new_line();
            }

            vTaskDelay(pdMS_TO_TICKS(50));     /* Wait a while for the next command */

            /* Force EDC PTT task on */
            if (payload_write_cmd(pl_edc_active, EDC_CMD_PTT_RESUME) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_READ_EDC_NAME, "Error while resuming EDC PTT task!");
                sys_log_new_line();
            }

            vTaskDelay(pdMS_TO_TICKS(50));     /* Wait a while for the next command */

            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_READ_EDC_NAME, "Active EDC ID: ");
            sys_log_print_hex((uint32_t)pl_edc_active);
            sys_log_new_line();

            /* Read housekeeping data */
            if (payload_get_data(pl_edc_active, PAYLOAD_EDC_HK, edc_hk_buf.buffer, &edc_hk_buf.length) == 0)
            {
                (void)memcpy(&edc->hk, edc_hk_buf.buffer, sizeof(edc_hk_t)); // cppcheck-suppress misra-c2012-21.15

                vTaskDelay(pdMS_TO_TICKS(50U));

                print_edc_hk(&edc->hk);
            }
            else 
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_READ_EDC_NAME, "Error reading the housekeeping data!");
                sys_log_new_line();
            }

            vTaskDelay(pdMS_TO_TICKS(500));     /* Wait a while for the next command */

            /* Read packets */
            uint8_t state_arr[10] = {0};
            int32_t state_len = 0;

            if (payload_get_data(pl_edc_active, PAYLOAD_EDC_STATE, state_arr, &state_len) == 0)
            {
                if (state_len >= (int32_t)sizeof(edc_state_t))
                {
                    (void)memcpy(&edc->state, state_arr, EDC_FRAME_STATE_LEN); // cppcheck-suppress misra-c2012-21.15

                    edc->timestamp = system_get_time();

                    if (edc->state.ptt_available > 0U)
                    {
                        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_READ_EDC_NAME, "");
                        sys_log_print_uint(edc->state.ptt_available);
                        sys_log_print_msg(" PTT packet(s) available to read!");
                        sys_log_new_line();

                        vTaskDelay(pdMS_TO_TICKS(50U));

                        uint8_t i = 0;
                        for(i = 0; i < edc->state.ptt_available; i++)
                        {
                            uint8_t ptt_arr[sizeof(edc_ptt_t)] = {0};
                            int32_t ptt_len = 0;

                            if (payload_get_data(pl_edc_active, PAYLOAD_EDC_PTT, ptt_arr, &ptt_len) == 0)
                            {
                                if (mem_mng_write_data_to_flash_page(ptt_arr, &sat_data_buf.obdh.data.media.last_page_sbcd_pkts, nor_info.page_size, CONFIG_MEM_SBCD_PKTS_START_PAGE, CONFIG_MEM_SBCD_PKTS_END_PAGE) != 0)
                                {
                                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_READ_EDC_NAME, "Error writing the PTT packet to the flash memory!");
                                    sys_log_new_line();
                                }

                                (void)memcpy(&edc->ptt, ptt_arr, sizeof(edc_ptt_t)); // cppcheck-suppress misra-c2012-21.15

                                int32_t ptt_power = -67 + (20 * log10(edc->ptt.carrier_abs/32768.0));

                                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_READ_EDC_NAME, "Received PTT packet:");
                                sys_log_new_line();
                                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_READ_EDC_NAME, "\tTime: ");
                                sys_log_print_uint(edc->ptt.time_tag);
                                sys_log_print_msg(" sec");
                                sys_log_new_line();
                                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_READ_EDC_NAME, "\tError code: ");
                                sys_log_print_uint(edc->ptt.error_code);
                                sys_log_new_line();
                                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_READ_EDC_NAME, "\tCarrier frequency: ");
                                sys_log_print_uint(edc->ptt.carrier_freq);
                                sys_log_print_msg(" Hz");
                                sys_log_new_line();
                                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_READ_EDC_NAME, "\tReceived signal power: ");
                                sys_log_print_int(ptt_power);
                                sys_log_print_msg(" dBm");
                                sys_log_new_line();
                                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_READ_EDC_NAME, "\tUser message: ");
                                sys_log_dump_hex(edc->ptt.user_msg, edc->ptt.msg_byte_length);
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

static void print_edc_hk(edc_hk_t *hk)
{
    sys_log_print_event_from_module(SYS_LOG_INFO, "EDC Status", "Current(Dig): ");
    sys_log_print_uint((uint32_t) hk->current_supply_d);
    sys_log_print_msg(" mA");
    sys_log_new_line();

    sys_log_print_event_from_module(SYS_LOG_INFO, "EDC Status", "Current(RF): ");
    sys_log_print_uint((uint32_t) hk->current_supply_a);
    sys_log_print_msg(" mA");
    sys_log_new_line();

    sys_log_print_event_from_module(SYS_LOG_INFO, "EDC Status", "Sys Voltage: ");
    sys_log_print_uint((uint32_t) hk->voltage_supply);
    sys_log_print_msg(" mV");
    sys_log_new_line();

    sys_log_print_event_from_module(SYS_LOG_INFO, "EDC Status", "Board Temp: ");
    sys_log_print_int((int32_t) hk->temp);
    sys_log_print_msg(" oC");
    sys_log_new_line();
}

/** \} End of read_edc group */
