/**
 * read_ro.c
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
 * along with OBDH 2.0. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Read EDC data task definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Renato Augusto Schenkel Meneghin Marchiori <renato.meneghin@posgrad.ufsc.br>
 * 
 * \version 0.0.1
 * 
 * \date 2026/09/01
 * 
 * \addtogroup read_ro
 * \{
 */

#include <FreeRTOS.h>
#include <task.h>
#include <system/sys_log/sys_log.h>
#include <devices/ro/ro.h>
#include <devices/media/media.h>
#include <drivers/ro/ro.h>
#include <system/system.h>
#include <utils/mem_mng.h>
#include <math.h>

#include <structs/satellite.h>

#include "read_ro.h"
#include "startup.h"

xTaskHandle xTaskReadROHandle;

pl_ro_hk_raw_t ro_hk_buf = {0};

static void print_ro_hk(ro_hk_t *hk);

void xTaskReadRO(void *p)
{
    (void)p;

    /* Wait startup task to finish */
    (void)xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_READ_RO_INIT_TIMEOUT_MS));

    vTaskDelay(pdMS_TO_TICKS(TASK_READ_RO_INITIAL_DELAY_MS));

    media_info_t nor_info = media_get_info(MEDIA_NOR);

    TickType_t last_cycle = xTaskGetTickCount();

    while(1)
    {
        // Get the active RO payload ID from the satellite data buffer
        // Enum para identificar o Payload RO ativo, que é armazenado no campo main_payload_state da estrutura obdh_data_t dentro do buffer de dados do satélite (sat_data_buf).
        ro_type_t pl_ro_active = (ro_type_t) sat_data_buf.obdh.data.main_payload_state;

        // Get the pointer to the RO telemetry data structure
        /*
        // [Em Processo] sat_data_buf.obdh.data.main_payload_state é um ponteiro para 
        // [Em Processo] a estrutura de dados de telemetria do RO, que é armazenada no buffer de dados do satélite (sat_data_buf).
        // [Em Processo] A estrutura atual é uma forma de manter a coerência com os códigos do EDC e do PayloadX
        // [Planejamento] Futuramente, este código será alterado de forma a compensar e a ter somente os dados
        // [Planejamento] do RO. O módulo RO-instrument ainda se encontra em desenvolvimento, e a estrutura de dados do RO ainda não foi definida.
        */         
        ro_telemetry_t * const ro = sat_data_buf.state.c_ro;

        if ((pl_ro_active != RO_NONE) && (ro != NULL))
        {
            /* Update RO clock */
            if (payload_set_clock(pl_ro_active, system_get_time()) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, 
                                                TASK_READ_RO_NAME, 
                                                "Error while setting RO clock!");
                sys_log_new_line();
            }

            vTaskDelay(pdMS_TO_TICKS(50));     /* Wait a while for the next command */

            /* Force RO capture task on */
            if (payload_write_cmd(pl_ro_active, RO_CMD_START_CAPTURE) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, 
                                                TASK_READ_RO_NAME, 
                                                "Error while starting RO capture task!");
                
                sys_log_new_line();
            }

            vTaskDelay(pdMS_TO_TICKS(50));     /* Wait a while for the next command */

            sys_log_print_event_from_module(SYS_LOG_INFO, 
                                            TASK_READ_RO_NAME, 
                                            "Active RO ID: ");

            sys_log_print_hex((uint32_t)pl_ro_active);
            sys_log_new_line();

            /* Read housekeeping data */
            if (payload_get_data(pl_ro_active, 
                                PAYLOAD_RO_HK, 
                                ro_hk_buf.buffer, 
                                &ro_hk_buf.length) == 0)
            {
                (void)memcpy(&ro->hk, 
                            ro_hk_buf.buffer, 
                            sizeof(ro_hk_t)); // cppcheck-suppress misra-c2012-21.15

                vTaskDelay(pdMS_TO_TICKS(50U));

                print_ro_hk(&ro->hk);
            }
            else 
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, 
                                                TASK_READ_RO_NAME, 
                                                "Error reading the housekeeping data!");
                sys_log_new_line();
            }

            vTaskDelay(pdMS_TO_TICKS(500));     /* Wait a while for the next command */

            /* Read packets */
            uint8_t state_arr[10] = {0};
            int32_t state_len = 0;

            if (payload_get_data(pl_ro_active, PAYLOAD_RO_STATE, state_arr, &state_len) == 0)
            {
                if (state_len >= (int32_t)sizeof(ro_state_t))
                {
                    (void)memcpy(&ro->state, state_arr, RO_FRAME_STATE_LEN); // cppcheck-suppress misra-c2012-21.15

                    ro->timestamp = system_get_time();

                    if (ro->state.ptt_available > 0U)
                    {
                        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_READ_RO_NAME, "");
                        sys_log_print_uint(ro->state.ptt_available);
                        sys_log_print_msg(" PTT packet(s) available to read!");
                        sys_log_new_line();

                        vTaskDelay(pdMS_TO_TICKS(50U));

                        uint8_t i = 0;
                        for(i = 0; i < ro->state.ptt_available; i++)
                        {
                            uint8_t ptt_arr[sizeof(ro_ptt_t)] = {0};
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
