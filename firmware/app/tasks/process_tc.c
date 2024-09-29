/*
 * process_tc.c
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
 * \brief Process TC task implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 0.10.19
 * 
 * \date 2021/07/06
 * 
 * \addtogroup process_tc
 * \{
 */

#include <stdint.h>
#include <string.h>

#include <config/config.h>
#include <config/keys.h>

#include <system/sys_log/sys_log.h>
#include <system/system.h>
#include <devices/ttc/ttc.h>
#include <devices/eps/eps.h>
#include <devices/media/media.h>
#include <devices/payload/payload.h>
#include <drivers/edc/edc.h>
#include <utils/mem_mng.h>
#include <hmac/sha.h>

#include <structs/satellite.h>

#include <fsat_pkt/fsat_pkt.h>

#include "process_tc.h"
#include "mission_manager.h"
#include "pos_det.h"
#include "startup.h"

xTaskHandle xTaskProcessTCHandle;

/**
 * \brief Formats payload for data request telecommand.
 *
 * \param[in] pkt_pl is the buffer used to store the formatted data as a TC payload.
 *
 * \param[in] pkt_pl_len is the number of bytes of the given packet payload.
 *
 * \param[in] data_id is the type of data requested.
 *
 * \param[in] data is the data from memory to be formatted.
 *
 * \return The status/error code.
 */
static int8_t format_data_request(uint8_t *pkt_pl, uint16_t *pkt_pl_len, uint8_t data_id, void *data);

/**
 * \brief Sends telecommand feedback to ground station.
 *
 * \param[in] pkt is the received packet.
 *
 * \return The status/error code.
 */
static int8_t send_tc_feedback(uint8_t *pkt);

/**
 * \brief Ping request telecommand.
 *
 * \param[in] pkt is the packet to process.
 *
 * \param[in] pkt_len is the number of bytes of the given packet.
 *
 * \return None.
 */
static void process_tc_ping_request(uint8_t *pkt, uint16_t pkt_len);

/**
 * \brief Data request telecommand.
 *
 * \param[in] pkt is the packet to process.
 *
 * \param[in] pkt_len is the number of bytes of the given packet.
 *
 * \return None.
 */
static void process_tc_data_request(uint8_t *pkt, uint16_t pkt_len);

/**
 * \brief Broadcast message telecommand.
 *
 * \param[in] pkt is the packet to process.
 *
 * \param[in] pkt_len is the number of bytes of the given packet.
 *
 * \return None.
 */
static void process_tc_broadcast_message(uint8_t *pkt, uint16_t pkt_len);

/**
 * \brief Enter hibernation telecommand.
 *
 * \param[in] pkt is the packet to process.
 *
 * \param[in] pkt_len is the number of bytes of the given packet.
 *
 * \return None.
 */
static void process_tc_enter_hibernation(uint8_t *pkt, uint16_t pkt_len);

/**
 * \brief Leave hibernation telecommand.
 *
 * \param[in] pkt is the packet to process.
 *
 * \param[in] pkt_len is the number of bytes of the given packet.
 *
 * \return None.
 */
static void process_tc_leave_hibernation(uint8_t *pkt, uint16_t pkt_len);

/**
 * \brief Activate module telecommand.
 *
 * \param[in] pkt is the packet to process.
 *
 * \param[in] pkt_len is the number of bytes of the given packet.
 *
 * \return None.
 */
static void process_tc_activate_module(uint8_t *pkt, uint16_t pkt_len);

/**
 * \brief Deactivate module telecommand.
 *
 * \param[in] pkt is the packet to process.
 *
 * \param[in] pkt_len is the number of bytes of the given packet.
 *
 * \return None.
 */
static void process_tc_deactivate_module(uint8_t *pkt, uint16_t pkt_len);

/**
 * \brief Activate payload telecommand.
 *
 * \param[in] pkt is the packet to process.
 *
 * \param[in] pkt_len is the number of bytes of the given packet.
 *
 * \return None.
 */
static void process_tc_activate_payload(uint8_t *pkt, uint16_t pkt_len);

/**
 * \brief Deactivate payload telecommand.
 *
 * \param[in] pkt is the packet to process.
 *
 * \param[in] pkt_len is the number of bytes of the given packet.
 *
 * \return None.
 */
static void process_tc_deactivate_payload(uint8_t *pkt, uint16_t pkt_len);

/**
 * \brief Erase memory telecommand.
 *
 * \param[in] pkt is the packet to process.
 *
 * \param[in] pkt_len is the number of bytes of the given packet.
 *
 * \return None.
 */
static void process_tc_erase_memory(uint8_t *pkt, uint16_t pkt_len);

/**
 * \brief Force reset telecommand.
 *
 * \param[in] pkt is the packet to process.
 *
 * \param[in] pkt_len is the number of bytes of the given packet.
 *
 * \return None.
 */
static void process_tc_force_reset(uint8_t *pkt, uint16_t pkt_len);

/**
 * \brief Get payload data telecommand.
 *
 * \param[in] pkt is the packet to process.
 *
 * \param[in] pkt_len is the number of bytes of the given packet.
 *
 * \return None.
 */
static void process_tc_get_payload_data(uint8_t *pkt, uint16_t pkt_len);

/**
 * \brief Set parameter telecommand.
 *
 * \param[in] pkt is the packet to process.
 *
 * \param[in] pkt_len is the number of bytes of the given packet.
 *
 * \return None.
 */
static void process_tc_set_parameter(uint8_t *pkt, uint16_t pkt_len);

/**
 * \brief Get parameter telecommand.
 *
 * \param[in] pkt is the packet to process.
 *
 * \param[in] pkt_len is the number of bytes of the given packet.
 *
 * \return None.
 */
static void process_tc_get_parameter(uint8_t *pkt, uint16_t pkt_len);

/**
 * \brief Update TLE telecommand.
 *
 * \param[in] pkt is the packet to process.
 *
 * \param[in] pkt_len is the number of bytes of the given packet.
 *
 * \return None.
 */
static void process_tc_update_tle(uint8_t *pkt, uint16_t pkt_len);

/**
 * \brief Transmit packet telecommand.
 *
 * \param[in] pkt is the packet to process.
 *
 * \param[in] pkt_len is the number of bytes of the given packet.
 *
 * \return None.
 */
static void process_tc_transmit_packet(uint8_t *pkt, uint16_t pkt_len);

/**
 * \brief Checks if a given HMAC is valid or not.
 *
 * \param[in] msg is the message to compute the hash.
 *
 * \param[in] msg_len is the number of bytes of the given message.
 *
 * \param[in] msg_hash is the received hash of the given message.
 *
 * \param[in] msg_hash_len is the number of bytes of the received hash.
 *
 * \param[in] key is the TC key.
 *
 * \param[in] key_len is the number of bytes of the TC key.
 *
 * \return TRUE/FALSE if the key is valid or not.
 */
static bool process_tc_validate_hmac(uint8_t *msg, uint16_t msg_len, uint8_t *msg_hash, uint16_t msg_hash_len, uint8_t *key, uint16_t key_len);

void vTaskProcessTC(void)
{
    /* Wait startup task to finish */
    xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_PROCESS_TC_INIT_TIMEOUT_MS));

    /* Delay before the first cycle */
    vTaskDelay(pdMS_TO_TICKS(TASK_PROCESS_TC_INITIAL_DELAY_MS));

    while(1)
    {
        TickType_t last_cycle = xTaskGetTickCount();

        int pkts = ttc_avail(TTC_1);

        if (pkts > 0)
        {
            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "New ");
            sys_log_print_uint((unsigned int)pkts);
            sys_log_print_msg(" packet(s) available!");
            sys_log_new_line();

            uint8_t pkt[300] = {0};
            uint16_t pkt_len = 0;

            if (ttc_recv(TTC_1, pkt, &pkt_len) == 0)
            {
                switch(pkt[0])
                {
                    case PKT_ID_UPLINK_PING_REQ:
                        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Ping TC received!");
                        sys_log_new_line();

                        process_tc_ping_request(pkt, pkt_len);

                        break;
                    case PKT_ID_UPLINK_DATA_REQ:
                        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Data request TC received!");
                        sys_log_new_line();

                        process_tc_data_request(pkt, pkt_len);

                        break;
                    case PKT_ID_UPLINK_BROADCAST_MSG:
                        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Broadcast message TC received!");
                        sys_log_new_line();

                        process_tc_broadcast_message(pkt, pkt_len);

                        break;
                    case PKT_ID_UPLINK_ENTER_HIBERNATION:
                        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Executing the TC \"Enter Hibernation\"...");
                        sys_log_new_line();

                        process_tc_enter_hibernation(pkt, pkt_len);

                        break;
                    case PKT_ID_UPLINK_LEAVE_HIBERNATION:
                        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Executing the TC \"Leave Hibernation\"...");
                        sys_log_new_line();

                        process_tc_leave_hibernation(pkt, pkt_len);

                        break;
                    case PKT_ID_UPLINK_ACTIVATE_MODULE:
                        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Executing the TC \"Activate Module\"...");
                        sys_log_new_line();

                        process_tc_activate_module(pkt, pkt_len);

                        break;
                    case PKT_ID_UPLINK_DEACTIVATE_MODULE:
                        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Executing the TC \"Deactivate Module\"...");
                        sys_log_new_line();

                        process_tc_deactivate_module(pkt, pkt_len);

                        break;
                    case PKT_ID_UPLINK_ACTIVATE_PAYLOAD:
                        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Executing the TC \"Activate Payload\"...");
                        sys_log_new_line();

                        process_tc_activate_payload(pkt, pkt_len);

                        break;
                    case PKT_ID_UPLINK_DEACTIVATE_PAYLOAD:
                        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Executing the TC \"Deactivate Payload\"...");
                        sys_log_new_line();

                        process_tc_deactivate_payload(pkt, pkt_len);

                        break;
                    case PKT_ID_UPLINK_ERASE_MEMORY:
                        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Executing the TC \"Erase Memory\"...");
                        sys_log_new_line();

                        process_tc_erase_memory(pkt, pkt_len);

                        break;
                    case PKT_ID_UPLINK_FORCE_RESET:
                        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Executing the TC \"Force Reset\"...");
                        sys_log_new_line();

                        process_tc_force_reset(pkt, pkt_len);

                        break;
                    case PKT_ID_UPLINK_GET_PAYLOAD_DATA:
                        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Executing the TC \"Get Payload Data\"...");
                        sys_log_new_line();

                        process_tc_get_payload_data(pkt, pkt_len);
                        break;
                    case PKT_ID_UPLINK_SET_PARAM:
                        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Executing the TC \"Set Parameter\"...");
                        sys_log_new_line();

                        process_tc_set_parameter(pkt, pkt_len);

                        break;
                    case PKT_ID_UPLINK_GET_PARAM:
                        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Executing the TC \"Get Parameter\"...");
                        sys_log_new_line();

                        process_tc_get_parameter(pkt, pkt_len);

                        break;
                    case PKT_ID_UPLINK_TRANSMIT_PACKET:
                        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Executing the TC \"Transmit Packet\"...");
                        sys_log_new_line();

                        process_tc_transmit_packet(pkt, pkt_len);

                        break;
                    case PKT_ID_UPLINK_UPDATE_TLE:
                        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Executing the TC \"Update TLE\"...");
                        sys_log_new_line();

                        process_tc_update_tle(pkt, pkt_len);

                        break;
                    default:
                        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Unknown packet received!");
                        sys_log_new_line();

                        break;
                }
            }
        }

        vTaskDelayUntil(&last_cycle, pdMS_TO_TICKS(TASK_PROCESS_TC_PERIOD_MS));
    }
}

static void process_tc_ping_request(uint8_t *pkt, uint16_t pkt_len)
{
    if (pkt_len >= 8U)
    {
        /* Update last valid tc parameter */
        sat_data_buf.obdh.data.last_valid_tc = pkt[0];

        fsat_pkt_pl_t pong_pl = {0};

        /* Packet ID */
        fsat_pkt_add_id(&pong_pl, PKT_ID_DOWNLINK_PING_ANS);

        /* Source callsign */
        fsat_pkt_add_callsign(&pong_pl, CONFIG_SATELLITE_CALLSIGN);

        (void)memcpy(&pong_pl.payload[0], &pkt[1], 7);

        pong_pl.length = 7U;

        uint8_t pong_pl_raw[16] = {0};
        uint16_t pong_pl_raw_len = 0;

        fsat_pkt_encode(&pong_pl, pong_pl_raw, &pong_pl_raw_len);

        if (sat_data_buf.obdh.data.mode != OBDH_MODE_HIBERNATION)
        {
            if (ttc_send(TTC_1, pong_pl_raw, pong_pl_raw_len) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error transmitting a ping answer!");
                sys_log_new_line();
            }
        }
    }
}

static void process_tc_data_request(uint8_t *pkt, uint16_t pkt_len)
{
    /* If the satellite is in hibernation mode there is no point in processing this telecommand */
    if ((pkt_len >= (1U + 7U + 1U + 4U + 4U)) && (sat_data_buf.obdh.data.mode != OBDH_MODE_HIBERNATION))
    {
        fsat_pkt_pl_t data_req_ans_pkt = {0};
        uint8_t data_req_ans_raw[300] = {0};
        uint16_t data_req_ans_raw_len = UINT16_MAX;

        /* Packet ID */
        fsat_pkt_add_id(&data_req_ans_pkt, PKT_ID_DOWNLINK_DATA_REQUEST_ANS);

        /* Source callsign */
        fsat_pkt_add_callsign(&data_req_ans_pkt, CONFIG_SATELLITE_CALLSIGN);

        uint8_t tc_key[16] = CONFIG_TC_KEY_DATA_REQUEST;

        if (process_tc_validate_hmac(pkt, 1U + 7U + 1U + 4U + 4U, &pkt[17], 20U, tc_key, sizeof(CONFIG_TC_KEY_DATA_REQUEST)-1U))
        {
            /* Update last valid tc parameter */
            sat_data_buf.obdh.data.last_valid_tc = pkt[0];

            uint32_t start_idx = ((uint32_t)pkt[9] << 24) | ((uint32_t)pkt[10] << 16) | ((uint32_t)pkt[11] << 8) | (uint32_t)pkt[12];
            uint32_t end_idx = ((uint32_t)pkt[13] << 24) | ((uint32_t)pkt[14] << 16) | ((uint32_t)pkt[15] << 8) | (uint32_t)pkt[16];

            media_info_t nor_info = media_get_info(MEDIA_NOR);

            switch(pkt[8])
            {
                case DATA_ID_OBDH:
                {
                    uint32_t start_page = sat_data_buf.obdh.data.media.last_page_obdh_data - (uint32_t)end_idx;
                    uint32_t end_page   = sat_data_buf.obdh.data.media.last_page_obdh_data - (uint32_t)start_idx;

                    uint8_t page_buf[256] = {0};

                    if (end_page <= CONFIG_MEM_OBDH_DATA_END_PAGE)
                    {
                        uint32_t i = 0;
                        for(i = start_page; i < end_page; i++)
                        {
                            if (media_read(MEDIA_NOR, i * nor_info.page_size, page_buf, sizeof(obdh_telemetry_t)) == 0)
                            {
                                /* Requester callsign */
                                (void)memcpy(&data_req_ans_pkt.payload[0], &pkt[1], 7);

                                /* Data ID */
                                data_req_ans_pkt.payload[7] = DATA_ID_OBDH;

                                /* Format payload */
                                (void)format_data_request(data_req_ans_pkt.payload, &data_req_ans_pkt.length, DATA_ID_OBDH, page_buf);

                                vTaskDelay(pdMS_TO_TICKS(10U));

                                fsat_pkt_encode(&data_req_ans_pkt, data_req_ans_raw, &data_req_ans_raw_len);

                                if (sat_data_buf.obdh.data.mode != OBDH_MODE_HIBERNATION)
                                {
                                    if (ttc_send(TTC_0, data_req_ans_raw, data_req_ans_raw_len) != 0)
                                    {
                                        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error transmitting the OBDH data log of memory page ");
                                        sys_log_print_uint(i);
                                        sys_log_print_msg("!");
                                        sys_log_new_line();
                                    }
                                }
                            }
                            vTaskDelay(pdMS_TO_TICKS(25U));
                        }
                    }

                    break;
                }
                case DATA_ID_EPS:
                {
                    uint32_t start_page = sat_data_buf.obdh.data.media.last_page_eps_data - (uint32_t)end_idx;
                    uint32_t end_page   = sat_data_buf.obdh.data.media.last_page_eps_data - (uint32_t)start_idx;

                    uint8_t page_buf[256] = {0};

                    if ((start_page >= CONFIG_MEM_EPS_DATA_START_PAGE) && (end_page <= CONFIG_MEM_EPS_DATA_END_PAGE))
                    {
                        uint32_t i = 0;
                        for(i = start_page; i < end_page; i++)
                        {
                            if (media_read(MEDIA_NOR, i * nor_info.page_size, page_buf, sizeof(eps_telemetry_t)) == 0)
                            {
                                /* Requester callsign */
                                (void)memcpy(&data_req_ans_pkt.payload[0], &pkt[1], 7);

                                /* Data ID */
                                data_req_ans_pkt.payload[7] = DATA_ID_EPS;

                                /* Format payload */
                                (void)format_data_request(data_req_ans_pkt.payload, &data_req_ans_pkt.length, DATA_ID_EPS, page_buf);

                                vTaskDelay(pdMS_TO_TICKS(10U));

                                fsat_pkt_encode(&data_req_ans_pkt, data_req_ans_raw, &data_req_ans_raw_len);

                                if (sat_data_buf.obdh.data.mode != OBDH_MODE_HIBERNATION)
                                {
                                    if (ttc_send(TTC_0, data_req_ans_raw, data_req_ans_raw_len) != 0)
                                    {
                                        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error transmitting the EPS data log of memory page ");
                                        sys_log_print_uint(i);
                                        sys_log_print_msg("!");
                                        sys_log_new_line();
                                    }
                                }
                            }
                            vTaskDelay(pdMS_TO_TICKS(25U));
                        }
                    }

                    break;
                }
                case DATA_ID_TTC_0:
                {
                    uint32_t start_page = sat_data_buf.obdh.data.media.last_page_ttc_0_data - (uint32_t)end_idx;
                    uint32_t end_page   = sat_data_buf.obdh.data.media.last_page_ttc_0_data - (uint32_t)start_idx;

                    uint8_t page_buf[256] = {0};

                    if ((start_page >= CONFIG_MEM_TTC_0_DATA_START_PAGE) && (end_page <= CONFIG_MEM_TTC_0_DATA_END_PAGE))
                    {
                        uint32_t i = 0;
                        for(i = start_page; i < end_page; i++)
                        {
                            if (media_read(MEDIA_NOR, i * nor_info.page_size, page_buf, sizeof(ttc_telemetry_t)) == 0)
                            {
                                /* Requester callsign */
                                (void)memcpy(&data_req_ans_pkt.payload[0], &pkt[1], 7);

                                /* Data ID */
                                data_req_ans_pkt.payload[7] = DATA_ID_TTC_0;

                                /* Format payload */
                                (void)format_data_request(data_req_ans_pkt.payload, &data_req_ans_pkt.length, DATA_ID_TTC_0, page_buf);

                                vTaskDelay(pdMS_TO_TICKS(10U));

                                fsat_pkt_encode(&data_req_ans_pkt, data_req_ans_raw, &data_req_ans_raw_len);

                                if (sat_data_buf.obdh.data.mode != OBDH_MODE_HIBERNATION)
                                {
                                    if (ttc_send(TTC_0, data_req_ans_raw, data_req_ans_raw_len) != 0)
                                    {
                                        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error transmitting the TTC 0 data log of memory page ");
                                        sys_log_print_uint(i);
                                        sys_log_print_msg("!");
                                        sys_log_new_line();
                                    }
                                }
                            }
                            vTaskDelay(pdMS_TO_TICKS(25U));
                        }
                    }

                    break;
                }
                case DATA_ID_TTC_1:
                {
                    uint32_t start_page = sat_data_buf.obdh.data.media.last_page_ttc_1_data - (uint32_t)end_idx;
                    uint32_t end_page   = sat_data_buf.obdh.data.media.last_page_ttc_1_data - (uint32_t)start_idx;

                    uint8_t page_buf[256] = {0};

                    if ((start_page >= CONFIG_MEM_TTC_1_DATA_START_PAGE) && (end_page <= CONFIG_MEM_TTC_1_DATA_END_PAGE))
                    {
                        uint32_t i = 0;
                        for(i = start_page; i < end_page; i++)
                        {
                            if (media_read(MEDIA_NOR, i * nor_info.page_size, page_buf, sizeof(ttc_telemetry_t)) == 0)
                            {
                                /* Requester callsign */
                                (void)memcpy(&data_req_ans_pkt.payload[0], &pkt[1], 7);

                                /* Data ID */
                                data_req_ans_pkt.payload[7] = DATA_ID_TTC_1;

                                /* Format payload */
                                (void)format_data_request(data_req_ans_pkt.payload, &data_req_ans_pkt.length, DATA_ID_TTC_1, page_buf);

                                vTaskDelay(pdMS_TO_TICKS(10U));

                                fsat_pkt_encode(&data_req_ans_pkt, data_req_ans_raw, &data_req_ans_raw_len);

                                if (sat_data_buf.obdh.data.mode != OBDH_MODE_HIBERNATION)
                                {
                                    if (ttc_send(TTC_0, data_req_ans_raw, data_req_ans_raw_len) != 0)
                                    {
                                        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error transmitting the TTC 1 data log of memory page ");
                                        sys_log_print_uint(i);
                                        sys_log_print_msg("!");
                                        sys_log_new_line();
                                    }
                                }
                            }
                            vTaskDelay(pdMS_TO_TICKS(25U));
                        }
                    }

                    break;
                }
                case DATA_ID_ANT:
                {
                    uint32_t start_page = sat_data_buf.obdh.data.media.last_page_ant_data - (uint32_t)end_idx;
                    uint32_t end_page   = sat_data_buf.obdh.data.media.last_page_ant_data - (uint32_t)start_idx;

                    uint8_t page_buf[256] = {0};

                    if ((start_page >= CONFIG_MEM_ANT_DATA_START_PAGE) && (end_page <= CONFIG_MEM_ANT_DATA_END_PAGE))
                    {
                        uint32_t i = 0;
                        for(i = start_page; i < end_page; i++)
                        {
                            if (media_read(MEDIA_NOR, i * nor_info.page_size, page_buf, sizeof(antenna_telemetry_t)) == 0)
                            {
                                /* Requester callsign */
                                (void)memcpy(&data_req_ans_pkt.payload[0], &pkt[1], 7);

                                /* Data ID */
                                data_req_ans_pkt.payload[7] = DATA_ID_ANT;

                                /* Format payload */
                                (void)format_data_request(data_req_ans_pkt.payload, &data_req_ans_pkt.length, DATA_ID_ANT, page_buf);

                                vTaskDelay(pdMS_TO_TICKS(10U));

                                fsat_pkt_encode(&data_req_ans_pkt, data_req_ans_raw, &data_req_ans_raw_len);

                                if (sat_data_buf.obdh.data.mode != OBDH_MODE_HIBERNATION)
                                {
                                    if (ttc_send(TTC_0, data_req_ans_raw, data_req_ans_raw_len) != 0)
                                    {
                                        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error transmitting the Antenna data log of memory page ");
                                        sys_log_print_uint(i);
                                        sys_log_print_msg("!");
                                        sys_log_new_line();
                                    }
                                }
                            }
                            vTaskDelay(pdMS_TO_TICKS(25U));
                        }
                    }

                    break;
                }
                case DATA_ID_SBCD_PKTS:
                {
                    uint32_t start_page = sat_data_buf.obdh.data.media.last_page_sbcd_pkts - (uint32_t)end_idx;
                    uint32_t end_page   = sat_data_buf.obdh.data.media.last_page_sbcd_pkts - (uint32_t)start_idx;

                    uint8_t page_buf[256] = {0};

                    if ((start_page >= CONFIG_MEM_SBCD_PKTS_START_PAGE) && (end_page <= CONFIG_MEM_SBCD_PKTS_END_PAGE))
                    {
                        uint32_t i = 0;
                        for(i = start_page; i < end_page; i++)
                        {
                            if (media_read(MEDIA_NOR, i * nor_info.page_size, page_buf, sizeof(edc_ptt_t)) == 0)
                            {
                                /* Requester callsign */
                                (void)memcpy(&data_req_ans_pkt.payload[0], &pkt[1], 7);

                                /* Data ID */
                                data_req_ans_pkt.payload[7] = DATA_ID_SBCD_PKTS;

                                /* Format payload */
                                (void)format_data_request(data_req_ans_pkt.payload, &data_req_ans_pkt.length, DATA_ID_SBCD_PKTS, page_buf);

                                vTaskDelay(pdMS_TO_TICKS(10U));

                                fsat_pkt_encode(&data_req_ans_pkt, data_req_ans_raw, &data_req_ans_raw_len);

                                if (sat_data_buf.obdh.data.mode != OBDH_MODE_HIBERNATION)
                                {
                                    if (ttc_send(TTC_0, data_req_ans_raw, data_req_ans_raw_len) != 0)
                                    {
                                        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error transmitting the SBCD PKTS data log of memory page ");
                                        sys_log_print_uint(i);
                                        sys_log_print_msg("!");
                                        sys_log_new_line();
                                    }
                                }
                            }
                            vTaskDelay(pdMS_TO_TICKS(25U));
                        }
                    }

                    break;
                }
                case DATA_ID_PAYLOAD_INFO:
                {
                    uint32_t start_page = sat_data_buf.obdh.data.media.last_page_edc_data - (uint32_t)end_idx;
                    uint32_t end_page   = sat_data_buf.obdh.data.media.last_page_edc_data - (uint32_t)start_idx;

                    uint8_t page_buf[256] = {0};

                    if ((start_page >= CONFIG_MEM_EDC_DATA_START_PAGE) && (end_page <= CONFIG_MEM_EDC_DATA_END_PAGE))
                    {
                        uint32_t i = 0;
                        for(i = start_page; i < end_page; i++)
                        {
                            if (media_read(MEDIA_NOR, i * nor_info.page_size, page_buf, sizeof(payload_telemetry_t)) == 0)
                            {
                                /* Requester callsign */
                                (void)memcpy(&data_req_ans_pkt.payload[0], &pkt[1], 7);

                                /* Data ID */
                                data_req_ans_pkt.payload[7] = DATA_ID_PAYLOAD_INFO;

                                /* Format payload */
                                (void)format_data_request(data_req_ans_pkt.payload, &data_req_ans_pkt.length, DATA_ID_PAYLOAD_INFO, page_buf);

                                vTaskDelay(pdMS_TO_TICKS(10U));

                                fsat_pkt_encode(&data_req_ans_pkt, data_req_ans_raw, &data_req_ans_raw_len);

                                if (sat_data_buf.obdh.data.mode != OBDH_MODE_HIBERNATION)
                                {
                                    if (ttc_send(TTC_0, data_req_ans_raw, data_req_ans_raw_len) != 0)
                                    {
                                        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error transmitting the EDC data log of memory page ");
                                        sys_log_print_uint(i);
                                        sys_log_print_msg("!");
                                        sys_log_new_line();
                                    }
                                }
                            }
                            vTaskDelay(pdMS_TO_TICKS(25U));
                        }
                    }

                    break;
                }
                default:
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error executing the \"Data Request\" TC! Unknown data ID!");
                    sys_log_new_line();

                    break;
            }
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error executing the \"Data Request\" TC! Invalid key!");
            sys_log_new_line();
        }
    }
}

static void process_tc_broadcast_message(uint8_t *pkt, uint16_t pkt_len)
{
    if (pkt_len >= 15U)
    {
        /* Update last valid tc parameter */
        sat_data_buf.obdh.data.last_valid_tc = pkt[0];

        fsat_pkt_pl_t broadcast_pl = {0};

        /* Packet ID */
        fsat_pkt_add_id(&broadcast_pl, PKT_ID_DOWNLINK_MESSAGE_BROADCAST);

        /* Source callsign */
        fsat_pkt_add_callsign(&broadcast_pl, CONFIG_SATELLITE_CALLSIGN);

        uint16_t msg_len = pkt_len - 7U - 7U - 1U;

        (void)memcpy(&broadcast_pl.payload[0], &pkt[1], 14U + msg_len);
    
        broadcast_pl.length = 14U + msg_len;

        uint8_t broadcast_pl_raw[55] = {0};
        uint16_t broadcast_pl_raw_len = 0;

        fsat_pkt_encode(&broadcast_pl, broadcast_pl_raw, &broadcast_pl_raw_len);

        if (sat_data_buf.obdh.data.mode != OBDH_MODE_HIBERNATION)
        {
            if (ttc_send(TTC_1, broadcast_pl_raw, broadcast_pl_raw_len) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error transmitting a message broadcast!");
                sys_log_new_line();
            }
        }
    }
}

static void process_tc_enter_hibernation(uint8_t *pkt, uint16_t pkt_len)
{
    if (pkt_len >= 30U)
    {
        uint8_t tc_key[16] = CONFIG_TC_KEY_ENTER_HIBERNATION;

        if (process_tc_validate_hmac(pkt, 1U + 7U + 2U, &pkt[10], 20U, tc_key, sizeof(CONFIG_TC_KEY_ENTER_HIBERNATION)-1U))
        {
            /* Update last valid tc parameter */
            sat_data_buf.obdh.data.last_valid_tc = pkt[0];

            const event_t enter_hib = { .event = EV_NOTIFY_MODE_CHANGE_RQ, .args[0] = OBDH_MODE_HIBERNATION,  .args[1] = pkt[8], .args[2] = pkt[9] };
            (void)notify_event_to_mission_manager(&enter_hib);

            if (xTaskNotifyWait(0U, UINT32_MAX, NULL, pdMS_TO_TICKS(TASK_PROCESS_TC_MAX_WAIT_TIME_MS)) == pdTRUE)
            {
                (void)send_tc_feedback(pkt);
            }
            else 
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Mission manager notify timed out for \"Enter hibernation\"");
                sys_log_new_line();
            }
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error executing the \"Enter hibernation\" TC! Invalid key!");
            sys_log_new_line();
        }
    }
}

static void process_tc_leave_hibernation(uint8_t *pkt, uint16_t pkt_len)
{
    if (pkt_len >= 28U)
    {
        uint8_t tc_key[16] = CONFIG_TC_KEY_LEAVE_HIBERNATION;

        if (process_tc_validate_hmac(pkt, 1U + 7U, &pkt[8], 20U, tc_key, sizeof(CONFIG_TC_KEY_LEAVE_HIBERNATION)-1U))
        {
            /* Update last valid tc parameter */
            sat_data_buf.obdh.data.last_valid_tc = pkt[0];

            const event_t leave_hib = { .event = EV_NOTIFY_MODE_CHANGE_RQ, .args[0] = OBDH_WAKE_UP,  .args[1] = 0U, .args[2] = 0U };
            (void)notify_event_to_mission_manager(&leave_hib);

            if (xTaskNotifyWait(0U, UINT32_MAX, NULL, pdMS_TO_TICKS(TASK_PROCESS_TC_MAX_WAIT_TIME_MS)) == pdTRUE)
            {
                (void)send_tc_feedback(pkt);
            }
            else 
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Mission manager notify timed out for \"Leave hibernation\"");
                sys_log_new_line();
            }
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error executing the \"Leave hibernation\" TC! Invalid key!");
            sys_log_new_line();
        }
    }
}

static void process_tc_activate_module(uint8_t *pkt, uint16_t pkt_len)
{
    int8_t err = -2;

    if (pkt_len >= 29U)
    {
        uint8_t tc_key[16] = CONFIG_TC_KEY_ACTIVATE_MODULE;

        if (process_tc_validate_hmac(pkt, 1U + 7U + 1U, &pkt[9], 20U, tc_key, sizeof(CONFIG_TC_KEY_ACTIVATE_MODULE)-1U))
        {
            /* Update last valid tc parameter */
            sat_data_buf.obdh.data.last_valid_tc = pkt[0];

            switch(pkt[8])
            {
                case MODULE_ID_BATTERY_HEATER:
                {
                    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Activating the battery heater...");
                    sys_log_new_line();

                    /* Enable the EPS heater */
                    if (eps_set_param(SL_EPS2_REG_BAT_HEATER_1_MODE, SL_EPS2_HEATER_MODE_MANUAL) == 0)
                    {
                        if (eps_set_param(SL_EPS2_REG_BAT_HEATER_1_DUTY_CYCLE, 20U) == 0)
                        {
                            ++err;
                        }
                    }

                    if (eps_set_param(SL_EPS2_REG_BAT_HEATER_2_MODE, SL_EPS2_HEATER_MODE_MANUAL) == 0)
                    {
                        if (eps_set_param(SL_EPS2_REG_BAT_HEATER_2_DUTY_CYCLE, 20U) == 0)
                        {
                            ++err;
                        }
                    }

                    if (err == 0)
                    {
                        (void)send_tc_feedback(pkt);
                    }

                    break;
                }
                case MODULE_ID_BEACON:
                {
                    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Activating the beacon...");
                    sys_log_new_line();

                    if (eps_set_param(SL_EPS2_REG_BEACON_ENABLE, 0x01U) == 0)
                    {
                        (void)send_tc_feedback(pkt);
                    }

                    break;
                }
                case MODULE_ID_PERIODIC_TELEMETRY:
                {
                    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Activating the periodic telemetry...");
                    sys_log_new_line();

                    /* Enable periodic general telemetry */
                    sat_data_buf.obdh.data.general_telemetry_on = true;
                    (void)send_tc_feedback(pkt);

                    break;
                }
                default:
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Invalid module to activate!");
                    sys_log_new_line();

                    break;
            }
        }
        else 
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error executing the \"Activate module\" TC! Invalid key!");
            sys_log_new_line();
        }
    }
}

static void process_tc_deactivate_module(uint8_t *pkt, uint16_t pkt_len)
{
    int8_t err = -2;

    if (pkt_len >= 29U)
    {
        uint8_t tc_key[16] = CONFIG_TC_KEY_DEACTIVATE_MODULE;

        if (process_tc_validate_hmac(pkt, 1U + 7U + 1U, &pkt[9], 20U, tc_key, sizeof(CONFIG_TC_KEY_DEACTIVATE_MODULE)-1U))
        {
            /* Update last valid tc parameter */
            sat_data_buf.obdh.data.last_valid_tc = pkt[0];

            switch(pkt[8])
            {
                case MODULE_ID_BATTERY_HEATER:
                {
                    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Deactivating the battery heater...");
                    sys_log_new_line();

                    /* Disable the EPS heater */
                    if (eps_set_param(SL_EPS2_REG_BAT_HEATER_1_MODE, SL_EPS2_HEATER_MODE_MANUAL) == 0)
                    {
                        if (eps_set_param(SL_EPS2_REG_BAT_HEATER_1_DUTY_CYCLE, 0U) == 0)
                        {
                            ++err;
                        }
                    }

                    if (eps_set_param(SL_EPS2_REG_BAT_HEATER_2_MODE, SL_EPS2_HEATER_MODE_MANUAL) == 0)
                    {
                        if (eps_set_param(SL_EPS2_REG_BAT_HEATER_2_DUTY_CYCLE, 0U) == 0)
                        {
                            ++err;
                        }
                    }

                    if (err == 0)
                    {
                        (void)send_tc_feedback(pkt);
                    }

                    break;
                }
                case MODULE_ID_BEACON:
                {
                    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Deactivating the beacon...");
                    sys_log_new_line();

                    if (eps_set_param(SL_EPS2_REG_BEACON_ENABLE, 0x00U) == 0)
                    {
                        (void)send_tc_feedback(pkt);
                    }

                    break;
                }
                case MODULE_ID_PERIODIC_TELEMETRY:
                {
                    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Deactivating the periodic telemetry...");
                    sys_log_new_line();

                    /* Disable periodic general telemetry */
                    sat_data_buf.obdh.data.general_telemetry_on = false;
                    (void)send_tc_feedback(pkt);

                    break;
                }
                default:
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Invalid module to deactivate!");
                    sys_log_new_line();
                    break;
            }
        }
        else 
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error executing the \"Deactivate module\" TC! Invalid key!");
            sys_log_new_line();
        }
    }
}

static void process_tc_activate_payload(uint8_t *pkt, uint16_t pkt_len)
{
    int8_t err = 0;
    event_t pl_event = { 0 };

    if (pkt_len >= 29U)
    {
        switch(pkt[8])
        {
            case PL_ID_EDC_1:
            {
                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Activating the EDC 1 payload...");
                sys_log_new_line();

                uint8_t tc_key[16] = CONFIG_TC_KEY_ACTIVATE_PAYLOAD_EDC;

                if (process_tc_validate_hmac(pkt, 1U + 7U + 1U, &pkt[9], 20U, tc_key, sizeof(CONFIG_TC_KEY_ACTIVATE_PAYLOAD_EDC)-1U))
                {
                    /* Update last valid tc parameter */
                    sat_data_buf.obdh.data.last_valid_tc = pkt[0];
                    pl_event.event = EV_NOTIFY_ACTIVATE_PAYLOAD_RQ;
                    pl_event.args[0] = PL_ID_EDC_1;
                    (void)notify_event_to_mission_manager(&pl_event);
                }
                else
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error activating the EDC 1 payload! Invalid key!");
                    sys_log_new_line();
                    err = -1;
                }

                break;
            }
            case PL_ID_EDC_2:
            {
                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Activating the EDC 2 payload...");
                sys_log_new_line();

                uint8_t tc_key[16] = CONFIG_TC_KEY_ACTIVATE_PAYLOAD_EDC;

                if (process_tc_validate_hmac(pkt, 1U + 7U + 1U, &pkt[9], 20U, tc_key, sizeof(CONFIG_TC_KEY_ACTIVATE_PAYLOAD_EDC)-1U))
                {
                    /* Update last valid tc parameter */
                    sat_data_buf.obdh.data.last_valid_tc = pkt[0];
                    pl_event.event = EV_NOTIFY_ACTIVATE_PAYLOAD_RQ;
                    pl_event.args[0] = PL_ID_EDC_2;
                    (void)notify_event_to_mission_manager(&pl_event);
                }
                else
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error activating the EDC 2 payload! Invalid key!");
                    sys_log_new_line();
                    err = -1;
                }

                break;
            }
            case PL_ID_PAYLOAD_X:
            {
                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Activating the Payload-X payload...");
                sys_log_new_line();

                uint8_t tc_key[16] = CONFIG_TC_KEY_ACTIVATE_PAYLOAD_PAYLOAD_X;

                if (process_tc_validate_hmac(pkt, 1U + 7U + 1U, &pkt[9], 20U, tc_key, sizeof(CONFIG_TC_KEY_ACTIVATE_PAYLOAD_PAYLOAD_X)-1U))
                {
                    /* Update last valid tc parameter */
                    sat_data_buf.obdh.data.last_valid_tc = pkt[0];
                    pl_event.event = EV_NOTIFY_ACTIVATE_PAYLOAD_RQ;
                    pl_event.args[0] = PL_ID_PAYLOAD_X;
                    (void)notify_event_to_mission_manager(&pl_event);
                }
                else
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error activating the Payload-X payload! Invalid key!");
                    sys_log_new_line();
                    err = -1;
                }

                break;
            }
            default:
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Invalid payload to activate!");
                sys_log_new_line();
                err = -1;
                break;
        }

        if (err == 0)
        {
            if (xTaskNotifyWait(0U, UINT32_MAX, NULL, pdMS_TO_TICKS(TASK_PROCESS_TC_MAX_WAIT_TIME_MS)) == pdTRUE)
            {
                (void)send_tc_feedback(pkt);
            }
            else 
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Mission manager notify timed out for \"Activate payload\"");
                sys_log_new_line();
            }
        }
    }
}

static void process_tc_deactivate_payload(uint8_t *pkt, uint16_t pkt_len)
{
    event_t pl_event = { 0 };
    int8_t err = 0;

    if (pkt_len >= 29U)
    {
        switch(pkt[8])
        {
            case PL_ID_EDC_1:
            {
                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Deactivating the EDC 1 payload...");
                sys_log_new_line();

                uint8_t tc_key[16] = CONFIG_TC_KEY_DEACTIVATE_PAYLOAD_EDC;

                if (process_tc_validate_hmac(pkt, 1U + 7U + 1U, &pkt[9], 20U, tc_key, sizeof(CONFIG_TC_KEY_DEACTIVATE_PAYLOAD_EDC)-1U))
                {
                    /* Update last valid tc parameter */
                    sat_data_buf.obdh.data.last_valid_tc = pkt[0];
                    pl_event.event = EV_NOTIFY_DEACTIVATE_PAYLOAD_RQ;
                    pl_event.args[0] = PL_ID_EDC_1;
                    (void)notify_event_to_mission_manager(&pl_event);
                }
                else
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error deactivating the EDC 1 payload! Invalid key!");
                    sys_log_new_line();
                    err = -1;
                }

                break;
            }
            case PL_ID_EDC_2:
            {
                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Deactivating the EDC 2 payload...");
                sys_log_new_line();

                uint8_t tc_key[16] = CONFIG_TC_KEY_DEACTIVATE_PAYLOAD_EDC;

                if (process_tc_validate_hmac(pkt, 1U + 7U + 1U, &pkt[9], 20U, tc_key, sizeof(CONFIG_TC_KEY_DEACTIVATE_PAYLOAD_EDC)-1U))
                {
                    /* Update last valid tc parameter */
                    sat_data_buf.obdh.data.last_valid_tc = pkt[0];
                    pl_event.event = EV_NOTIFY_DEACTIVATE_PAYLOAD_RQ;
                    pl_event.args[0] = PL_ID_EDC_2;
                    (void)notify_event_to_mission_manager(&pl_event);
                }
                else
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error deactivating the EDC 2 payload! Invalid key!");
                    sys_log_new_line();
                    err = -1;
                }

                break;
            }
            case PL_ID_PAYLOAD_X:
            {
                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Deactivating the Payload-X payload...");
                sys_log_new_line();

                uint8_t tc_key[16] = CONFIG_TC_KEY_DEACTIVATE_PAYLOAD_PAYLOAD_X;

                if (process_tc_validate_hmac(pkt, 1U + 7U + 1U, &pkt[9], 20U, tc_key, sizeof(CONFIG_TC_KEY_DEACTIVATE_PAYLOAD_PAYLOAD_X)-1U))
                {
                    /* Update last valid tc parameter */
                    sat_data_buf.obdh.data.last_valid_tc = pkt[0];
                    pl_event.event = EV_NOTIFY_DEACTIVATE_PAYLOAD_RQ;
                    pl_event.args[0] = PL_ID_PAYLOAD_X;
                    (void)notify_event_to_mission_manager(&pl_event);
                }
                else
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error deactivating the Payload-X payload! Invalid key!");
                    sys_log_new_line();
                    err = -1;
                }

                break;
            }
            default:
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Invalid payload to deactivate!");
                sys_log_new_line();
                err = -1;
                break;
        }

        if (err == 0)
        {
            if (xTaskNotifyWait(0U, UINT32_MAX, NULL, pdMS_TO_TICKS(TASK_PROCESS_TC_MAX_WAIT_TIME_MS)) == pdTRUE)
            {
                (void)send_tc_feedback(pkt);
            }
            else 
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Mission manager notify timed out for \"Deactivate payload\"");
                sys_log_new_line();
            }
        }
    }
}

static void process_tc_erase_memory(uint8_t *pkt, uint16_t pkt_len)
{
    if (pkt_len >= 28U)
    {
        uint8_t tc_key[16] = CONFIG_TC_KEY_ERASE_MEMORY;

        if (process_tc_validate_hmac(pkt, 1U + 7U, &pkt[8], 20U, tc_key, sizeof(CONFIG_TC_KEY_ERASE_MEMORY)-1U))
        {
            /* Update last valid tc parameter */
            sat_data_buf.obdh.data.last_valid_tc = pkt[0];

            if (mem_mng_erase_flash(&sat_data_buf.obdh) < 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error erasing flash memory!");
                sys_log_new_line();
            }
            else 
            {
                (void)send_tc_feedback(pkt);
            }
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error executing the \"Erase Memory\" TC! Invalid key!");
            sys_log_new_line();
        }
    }
}

static void process_tc_force_reset(uint8_t *pkt, uint16_t pkt_len)
{
    if (pkt_len >= 28U)
    {
        uint8_t tc_key[16] = CONFIG_TC_KEY_FORCE_RESET;

        if (process_tc_validate_hmac(pkt, 1U + 7U, &pkt[8], 20U, tc_key, sizeof(CONFIG_TC_KEY_FORCE_RESET)-1U))
        {
            (void)eps_set_param(SL_EPS2_REG_RESET_EPS, 0x01U);
            (void)ttc_set_param(TTC_0, SL_TTC2_REG_RESET_DEVICE, 0x01U);
            (void)ttc_set_param(TTC_1, SL_TTC2_REG_RESET_DEVICE, 0x01U);
            system_reset();
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error executing the \"Force Reset\" TC! Invalid key!");
            sys_log_new_line();
        }
    }
}

static void process_tc_get_payload_data(uint8_t *pkt, uint16_t pkt_len)
{
    if (pkt_len >= (1U + 7U + 1U + 12U + 20U))
    {
        uint8_t tc_key[16] = CONFIG_TC_KEY_GET_PAYLOAD_DATA;

        if (process_tc_validate_hmac(pkt, 1U + 7U + 1U + 12U, &pkt[21], 20U, tc_key, sizeof(CONFIG_TC_KEY_GET_PAYLOAD_DATA) - 1U))
        {
            /* Update last valid tc parameter */
            sat_data_buf.obdh.data.last_valid_tc = pkt[0];

            switch (pkt[8]) 
            {
                case PL_ID_EDC_1:
                {
                    uint8_t data_id = pkt[9];

                    fsat_pkt_pl_t pl_data;

                    if (format_data_request(pl_data.payload, &pl_data.length, data_id, &sat_data_buf.edc_0) == 0)
                    {
                        uint8_t pkt_raw[60];
                        uint16_t pkt_len;

                        /* Prepare Packet */
                        (void)memcpy(&pl_data.payload[0], &pkt[1], 7); /* Requester callsign */
                        pl_data.payload[7] = pkt[9]; /* Payload Arg */
                        fsat_pkt_add_id(&pl_data, PKT_ID_DOWNLINK_PAYLOAD_DATA);
                        fsat_pkt_add_callsign(&pl_data, CONFIG_SATELLITE_CALLSIGN);
                        fsat_pkt_encode(&pl_data, pkt_raw, &pkt_len);
                        
                        if (sat_data_buf.obdh.data.mode != OBDH_MODE_HIBERNATION)
                        {
                            if (ttc_send(TTC_0, pkt_raw, pkt_len) != 0)
                            {
                                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error transmitting a \"Get Payload Data\" answer!");
                                sys_log_new_line();
                            }
                        }
                        
                    }

                    break;
                }
                case PL_ID_EDC_2:
                {
                    uint8_t data_id = pkt[9];

                    fsat_pkt_pl_t pl_data;

                    if (format_data_request(pl_data.payload, &pl_data.length, data_id, &sat_data_buf.edc_1) == 0)
                    {
                        uint8_t pkt_raw[60];
                        uint16_t pkt_len;

                        /* Prepare Packet */
                        (void)memcpy(&pl_data.payload[0], &pkt[1], 7); /* Requester callsign */
                        pl_data.payload[7] = pkt[9]; /* Payload Arg */
                        fsat_pkt_add_id(&pl_data, PKT_ID_DOWNLINK_PAYLOAD_DATA);
                        fsat_pkt_add_callsign(&pl_data, CONFIG_SATELLITE_CALLSIGN);
                        fsat_pkt_encode(&pl_data, pkt_raw, &pkt_len);
                        
                        if (sat_data_buf.obdh.data.mode != OBDH_MODE_HIBERNATION)
                        {
                            if (ttc_send(TTC_0, pkt_raw, pkt_len) != 0)
                            {
                                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error transmitting a \"Get Payload Data\" answer!");
                                sys_log_new_line();
                            }
                        }
                        
                    }
                    break;
                }
                default:
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Invalid payload ID!");
                    sys_log_new_line();
                    break;
            }
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error executing the \"Get Payload Data\" TC! Invalid key!");
            sys_log_new_line();
        }
    }
}

static void process_tc_set_parameter(uint8_t *pkt, uint16_t pkt_len)
{
    int8_t err = 0;

    if (pkt_len >= (1U + 7U + 1U + 1U + 4U + 20U))
    {
        uint8_t tc_key[16] = CONFIG_TC_KEY_SET_PARAMETER;

        if (process_tc_validate_hmac(pkt, 1U + 7U + 1U + 1U + 4U, &pkt[14], 20U, tc_key, sizeof(CONFIG_TC_KEY_SET_PARAMETER)-1U))
        {
            /* Update last valid tc parameter */
            sat_data_buf.obdh.data.last_valid_tc = pkt[0];

            uint32_t buf = ((uint32_t)pkt[10] << 24) |
                           ((uint32_t)pkt[11] << 16) |
                           ((uint32_t)pkt[12] << 8) |
                           (uint32_t)pkt[13];

            switch(pkt[8])
            {
                case SUBSYSTEM_ID_OBDH:
                    if (obdh_set_param(pkt[9], &buf) == 0)
                    {
                        if (pkt[9] == OBDH_PARAM_ID_MODE)
                        {
                            /* Check for notification from mission_manager */
                            if (xTaskNotifyWait(0U, UINT32_MAX, NULL, pdMS_TO_TICKS(TASK_PROCESS_TC_MAX_WAIT_TIME_MS)) != pdTRUE)
                            {
                                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Mission manager notify timed out for \"Set param MODE\"");
                                sys_log_new_line();
                                err = -1;
                            }
                        }

                        if (pkt[9] == OBDH_PARAM_ID_SYSTEM_TIME)
                        {
                            if (eps_set_param(SL_EPS2_REG_TIME_COUNTER, buf) != 0)
                            {
                                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Failed to synchronize system time with EPS!");
                                sys_log_new_line();
                                err = -1;
                            }

                            if (ttc_set_param(TTC_0, SL_TTC2_REG_TIME_COUNTER, buf) != 0)
                            {
                                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Failed to synchronize system time with TTC 0!");
                                sys_log_new_line();
                                err = -1;
                            }

                            if (ttc_set_param(TTC_1, SL_TTC2_REG_TIME_COUNTER, buf) != 0)
                            {
                                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Failed to synchronize system time with TTC 1!");
                                sys_log_new_line();
                                err = -1;
                            }
                        }
                    }
                    else 
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error writing a OBDH parameter!");
                        sys_log_new_line();
                        err = -1;
                    }

                    break;
                case SUBSYSTEM_ID_TTC_1:
                    if (ttc_set_param(TTC_0, pkt[9], buf) != 0)
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error writing a TTC 0 parameter!");
                        sys_log_new_line();
                        err = -1;
                    }

                    break;
                case SUBSYSTEM_ID_TTC_2:
                    if (ttc_set_param(TTC_1, pkt[9], buf) != 0)
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error writing a TTC 1 parameter!");
                        sys_log_new_line();
                        err = -1;
                    }

                    break;
                case SUBSYSTEM_ID_EPS:
                    if (eps_set_param(pkt[9], buf) != 0)
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error writing a EPS parameter!");
                        sys_log_new_line();
                        err = -1;
                    }

                    break;
                default:
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Invalid subsystem to set a parameter!");
                    sys_log_new_line();

                    break;
            }

            if (err == 0)
            {
                (void)send_tc_feedback(pkt);
            }
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error executing the \"Set Parameter\" TC! Invalid key!");
            sys_log_new_line();
        }
    }
}

static void process_tc_get_parameter(uint8_t *pkt, uint16_t pkt_len)
{
    if (pkt_len >= (1U + 7U + 1U + 1U + 20U))
    {
        uint8_t tc_key[16] = CONFIG_TC_KEY_GET_PARAMETER;

        if (process_tc_validate_hmac(pkt, 1U + 7U + 1U + 1U, &pkt[10], 20U, tc_key, sizeof(CONFIG_TC_KEY_GET_PARAMETER)-1U))
        {
            int err = 0;

            uint32_t buf = UINT32_MAX;

            switch(pkt[8])
            {
                case SUBSYSTEM_ID_OBDH:
                    if (obdh_get_param(pkt[9], &buf) != 0)
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error reading a parameter from OBDH!");
                        sys_log_new_line();
                        err = -1;
                    }

                    break;
                case SUBSYSTEM_ID_TTC_1:
                    if (ttc_get_param(TTC_0, pkt[9], &buf) != 0)
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error reading a parameter from TTC 0!");
                        sys_log_new_line();
                        err = -1;
                    }

                    break;
                case SUBSYSTEM_ID_TTC_2:
                    if (ttc_get_param(TTC_1, pkt[9], &buf) != 0)
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error reading a parameter from TTC 1!");
                        sys_log_new_line();
                        err = -1;
                    }

                    break;
                case SUBSYSTEM_ID_EPS:
                    if (eps_get_param(pkt[9], &buf) != 0)
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error reading a EPS parameter!");
                        sys_log_new_line();
                        err = -1;
                    }

                    break;
                default:
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Invalid subsystem to get a parameter!");
                    sys_log_new_line();
                    err = -1;
                    break;
            }

            if (err == 0)
            {
                fsat_pkt_pl_t param_pl = {0};

                /* Packet ID */
                fsat_pkt_add_id(&param_pl, PKT_ID_DOWNLINK_PARAM_VALUE);

                /* Source callsign */
                fsat_pkt_add_callsign(&param_pl, CONFIG_SATELLITE_CALLSIGN);

                (void)memcpy(&param_pl.payload[0], &pkt[1], 7);

                param_pl.payload[8] = pkt[8];
                param_pl.payload[9] = pkt[9];
                param_pl.payload[10] = (uint8_t)((buf >> 24) & 0xFFU);
                param_pl.payload[11] = (uint8_t)((buf >> 16) & 0xFFU);
                param_pl.payload[12] = (uint8_t)((buf >> 8) & 0xFFU);
                param_pl.payload[13] = (uint8_t)(buf & 0xFFU);

                param_pl.length = 6U;

                uint8_t param_pl_raw[16] = {0};
                uint16_t param_pl_raw_len = 0;

                fsat_pkt_encode(&param_pl, param_pl_raw, &param_pl_raw_len);

                if (sat_data_buf.obdh.data.mode != OBDH_MODE_HIBERNATION)
                {
                    if (ttc_send(TTC_1, param_pl_raw, param_pl_raw_len) != 0)
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error transmitting a \"get parameter\" answer!");
                        sys_log_new_line();
                    }
                }
            }

            /* Update last valid tc parameter, this is made after transmission 
             * because the requested parameter could be last_valid_tc aswell */
            sat_data_buf.obdh.data.last_valid_tc = pkt[0];

        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error executing the \"Get Parameter\" TC! Invalid key!");
            sys_log_new_line();
        }
    }
}

static void process_tc_update_tle(uint8_t *pkt, uint16_t pkt_len)
{
    if (pkt_len >= (1U + 7U + 1U + 69U + 20U))
    {
        uint8_t tc_key[16] = CONFIG_TC_KEY_UPDATE_TLE;

        if (process_tc_validate_hmac(pkt, 1U + 7U + 1U + 69U, &pkt[78], 20U, tc_key, sizeof(CONFIG_TC_KEY_UPDATE_TLE)-1U))
        {
            /* Update last valid tc parameter */
            sat_data_buf.obdh.data.last_valid_tc = pkt[0];

            if ((pkt[8] == 0x01U) || (pkt[8] == 0x02U))
            {
                if(update_tle_line(pkt[8], &pkt[9]))
                {
                    /* Notify Position Determination Task of TLE update */
                    xTaskNotify(xTaskPosDetHandle, 0U, eNoAction);
                }

                (void)send_tc_feedback(pkt);
            }
            else 
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Invalid TLE line number received!");
                sys_log_new_line();
            }
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error executing the \"Update TLE\" TC! Invalid key!");
            sys_log_new_line();
        }
    }
}

static void process_tc_transmit_packet(uint8_t *pkt, uint16_t pkt_len)
{
    if (pkt_len >= (1U + 7U + 20U))
    {
        uint8_t tc_key[16] = CONFIG_TC_KEY_TRANSMIT_PACKET;

        uint8_t msg_len = pkt_len - 1U - 7U - 20U;

        if (process_tc_validate_hmac(pkt, pkt_len - 20U, &pkt[pkt_len - 21U], 20U, tc_key, 16U))
        {
            if (msg_len > 0)
            {
                fsat_pkt_pl_t pkt_broacast;
                uint8_t raw_pkt[60];
                uint16_t raw_pkt_len;

                fsat_pkt_add_id(&pkt_broacast, PKT_ID_DOWNLINK_PACKET_BROADCAST);
                fsat_pkt_add_callsign(&pkt_broacast, CONFIG_SATELLITE_CALLSIGN);

                (void)memcpy(pkt_broacast.payload, &pkt[1U + 7U], msg_len);
                pkt_broacast.length = msg_len;

                fsat_pkt_encode(&pkt_broacast, raw_pkt, &raw_pkt_len);

                if (sat_data_buf.obdh.data.mode != OBDH_MODE_HIBERNATION)
                {
                    if (ttc_send(TTC_0, raw_pkt, raw_pkt_len) != 0)
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error transmitting a \"Packet Broadcast\"!");
                        sys_log_new_line();
                    }
                }
            }
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error executing the \"Transmit Packet\" TC! Invalid key!");
            sys_log_new_line();
        }

    }
}

static bool process_tc_validate_hmac(uint8_t *msg, uint16_t msg_len, uint8_t *msg_hash, uint16_t msg_hash_len, uint8_t *key, uint16_t key_len)
{
    bool res = false;

    uint8_t hash[20] = {0};

    if (hmac(SHA1, msg, msg_len, key, key_len, hash) == 0)
    {
        if (memcmp(hash, msg_hash, (size_t)msg_hash_len) == 0)
        {
            res = true;
        }
    }

    return res;
}

static int8_t format_data_request(uint8_t *pkt_pl, uint16_t *pkt_pl_len, uint8_t data_id, void *data)
{
	int8_t err = 0;

	/* Response payload includes the TC ID and the Requester callsign, which have lenght of 1 and 7 bytes, respectively. 
     * That is the reason for the `7U + 1U` offset */
	uint8_t *pl = &pkt_pl[7U + 1U]; 

uint32_t timestamp = system_get_time();

	switch (data_id)
	{
		case DATA_ID_OBDH:
		{
			obdh_telemetry_t *tel = (obdh_telemetry_t *)data;
			sys_time_t mode_duration = system_get_time() - sat_data_buf.obdh.data.ts_last_mode_change;

			pl[0] = (timestamp >> 24U) & 0xFFU;
			pl[1] = (timestamp >> 16U) & 0xFFU;
			pl[2] = (timestamp >> 8U) & 0xFFU;
			pl[3] = timestamp & 0xFFU;
			pl[4] = (tel->data.temperature >> 8U) & 0xFFU;
			pl[5] = tel->data.temperature & 0xFFU;
			pl[6] = (tel->data.current >> 8U) & 0xFFU;
			pl[7] = tel->data.current & 0xFFU;
			pl[8] = (tel->data.voltage >> 8U) & 0xFFU;
			pl[9] = tel->data.voltage & 0xFFU;
			pl[10] = (tel->data.reset_counter >> 8U) & 0xFFU;
			pl[11] = tel->data.reset_counter & 0xFFU;
			pl[12] = tel->data.last_reset_cause;
			pl[13] = tel->data.last_valid_tc;
			pl[14] = tel->data.ant_deployment_counter;
			pl[15] = tel->data.initial_hib_time_count;
			pl[16] = tel->data.hw_version;
			pl[17] = (tel->data.fw_version >> 24U) & 0xFFU;
			pl[18] = (tel->data.fw_version >> 16U) & 0xFFU;
			pl[19] = (tel->data.fw_version >> 8U) & 0xFFU;
			pl[20] = tel->data.fw_version & 0xFFU;
			pl[21] = tel->data.mode;
			pl[22] = (tel->data.ts_last_mode_change >> 24U) & 0xFFU;
			pl[23] = (tel->data.ts_last_mode_change >> 16U) & 0xFFU;
			pl[24] = (tel->data.ts_last_mode_change >> 8U) & 0xFFU;
			pl[25] = tel->data.ts_last_mode_change & 0xFFU;
			pl[26] = (mode_duration >> 24U) & 0xFFU;
			pl[27] = (mode_duration >> 16U) & 0xFFU;
			pl[28] = (mode_duration >> 8U) & 0xFFU;
			pl[29] = mode_duration & 0xFFU;
			pl[30] = tel->data.initial_hib_executed;
			pl[31] = tel->data.ant_deployment_executed;
			pl[32] = tel->data.manual_mode_on;
			pl[33] = tel->data.main_edc;
			pl[34] = tel->data.general_telemetry_on;
			pl[35] = (tel->data.media.last_page_obdh_data >> 24U) & 0xFFU;
			pl[36] = (tel->data.media.last_page_obdh_data >> 16U) & 0xFFU;
			pl[37] = (tel->data.media.last_page_obdh_data >> 8U) & 0xFFU;
			pl[38] = tel->data.media.last_page_obdh_data & 0xFFU;
			pl[39] = (tel->data.media.last_page_eps_data >> 24U) & 0xFFU;
			pl[40] = (tel->data.media.last_page_eps_data >> 16U) & 0xFFU;
			pl[41] = (tel->data.media.last_page_eps_data >> 8U) & 0xFFU;
			pl[42] = tel->data.media.last_page_eps_data & 0xFFU;
			pl[43] = (tel->data.media.last_page_ttc_0_data >> 24U) & 0xFFU;
			pl[44] = (tel->data.media.last_page_ttc_0_data >> 16U) & 0xFFU;
			pl[45] = (tel->data.media.last_page_ttc_0_data >> 8U) & 0xFFU;
			pl[46] = tel->data.media.last_page_ttc_0_data & 0xFFU;
			pl[47] = (tel->data.media.last_page_ttc_1_data >> 24U) & 0xFFU;
			pl[48] = (tel->data.media.last_page_ttc_1_data >> 16U) & 0xFFU;
			pl[49] = (tel->data.media.last_page_ttc_1_data >> 8U) & 0xFFU;
			pl[50] = tel->data.media.last_page_ttc_1_data & 0xFFU;
			pl[51] = (tel->data.media.last_page_ant_data >> 24U) & 0xFFU;
			pl[52] = (tel->data.media.last_page_ant_data >> 16U) & 0xFFU;
			pl[53] = (tel->data.media.last_page_ant_data >> 8U) & 0xFFU;
			pl[54] = tel->data.media.last_page_ant_data & 0xFFU;
			pl[55] = (tel->data.media.last_page_edc_data >> 24U) & 0xFFU;
			pl[56] = (tel->data.media.last_page_edc_data >> 16U) & 0xFFU;
			pl[57] = (tel->data.media.last_page_edc_data >> 8U) & 0xFFU;
			pl[58] = tel->data.media.last_page_edc_data & 0xFFU;
			pl[59] = (tel->data.media.last_page_px_data >> 24U) & 0xFFU;
			pl[60] = (tel->data.media.last_page_px_data >> 16U) & 0xFFU;
			pl[61] = (tel->data.media.last_page_px_data >> 8U) & 0xFFU;
			pl[62] = tel->data.media.last_page_px_data & 0xFFU;
			pl[63] = (tel->data.media.last_page_sbcd_pkts >> 24U) & 0xFFU;
			pl[64] = (tel->data.media.last_page_sbcd_pkts >> 16U) & 0xFFU;
			pl[65] = (tel->data.media.last_page_sbcd_pkts >> 8U) & 0xFFU;
			pl[66] = tel->data.media.last_page_sbcd_pkts & 0xFFU;
			pl[67] = (tel->data.position.timestamp >> 24U) & 0xFFU;
			pl[68] = (tel->data.position.timestamp >> 16U) & 0xFFU;
			pl[69] = (tel->data.position.timestamp >> 8U) & 0xFFU;
			pl[70] = tel->data.position.timestamp & 0xFFU;
			pl[71] = (((uint16_t)tel->data.position.latitude) >> 8U) & 0xFFU;
			pl[72] = ((uint16_t)tel->data.position.latitude) & 0xFFU;
			pl[73] = (((uint16_t)tel->data.position.longitude) >> 8U) & 0xFFU;
			pl[74] = ((uint16_t)tel->data.position.longitude) & 0xFFU;
			pl[75] = (((uint16_t)tel->data.position.altitude) >> 8U) & 0xFFU;
			pl[76] = ((uint16_t)tel->data.position.altitude) & 0xFFU;

			*pkt_pl_len = (uint16_t) 86U; /* 7b RQ CALLSIGN + 1b TC ID + 77b OBDH DATA */

			break;
		}

		case DATA_ID_EPS:
		{
			eps_telemetry_t *tel = (eps_telemetry_t *)data;

			pl[0] = (tel->timestamp >> 24U) & 0xFFU;
			pl[1] = (tel->timestamp >> 16U) & 0xFFU;
			pl[2] = (tel->timestamp >> 8U) & 0xFFU;
			pl[3] = tel->timestamp & 0xFFU;
			pl[4] = (tel->data.time_counter >> 24U) & 0xFFU;
			pl[5] = (tel->data.time_counter >> 16U) & 0xFFU;
			pl[6] = (tel->data.time_counter >> 8U) & 0xFFU;
			pl[7] = tel->data.time_counter & 0xFFU;
			pl[8] = (tel->data.temperature_uc >> 8U) & 0xFFU;
			pl[9] = tel->data.temperature_uc & 0xFFU;
			pl[10] = (tel->data.current >> 8U) & 0xFFU;
			pl[11] = tel->data.current & 0xFFU;
			pl[12] = tel->data.last_reset_cause & 0xFFU;
			pl[13] = (tel->data.reset_counter >> 8U) & 0xFFU;
			pl[14] = tel->data.reset_counter & 0xFFU;
			pl[15] = (tel->data.solar_panel_voltage_my_px >> 8U) & 0xFFU;
			pl[16] = tel->data.solar_panel_voltage_my_px & 0xFFU;
			pl[17] = (tel->data.solar_panel_voltage_mx_pz >> 8U) & 0xFFU;
			pl[18] = tel->data.solar_panel_voltage_mx_pz & 0xFFU;
			pl[19] = (tel->data.solar_panel_voltage_mz_py >> 8U) & 0xFFU;
			pl[20] = tel->data.solar_panel_voltage_mz_py & 0xFFU;
			pl[21] = (tel->data.solar_panel_current_my >> 8U) & 0xFFU;
			pl[22] = tel->data.solar_panel_current_my & 0xFFU;
			pl[23] = (tel->data.solar_panel_current_py >> 8U) & 0xFFU;
			pl[24] = tel->data.solar_panel_current_py & 0xFFU;
			pl[25] = (tel->data.solar_panel_current_mx >> 8U) & 0xFFU;
			pl[26] = tel->data.solar_panel_current_mx & 0xFFU;
			pl[27] = (tel->data.solar_panel_current_px >> 8U) & 0xFFU;
			pl[28] = tel->data.solar_panel_current_px & 0xFFU;
			pl[29] = (tel->data.solar_panel_current_mz >> 8U) & 0xFFU;
			pl[30] = tel->data.solar_panel_current_mz & 0xFFU;
			pl[31] = (tel->data.solar_panel_current_pz >> 8U) & 0xFFU;
			pl[32] = tel->data.solar_panel_current_pz & 0xFFU;
			pl[33] = tel->data.mppt_1_duty_cycle & 0xFFU;
			pl[34] = tel->data.mppt_2_duty_cycle & 0xFFU;
			pl[35] = tel->data.mppt_3_duty_cycle & 0xFFU;
			pl[36] = (tel->data.solar_panel_output_voltage >> 8U) & 0xFFU;
			pl[37] = tel->data.solar_panel_output_voltage & 0xFFU;
			pl[38] = (tel->data.main_power_bus_voltage >> 8U) & 0xFFU;
			pl[39] = tel->data.main_power_bus_voltage & 0xFFU;
			pl[40] = (tel->data.rtd_0_temperature >> 8U) & 0xFFU;
			pl[41] = tel->data.rtd_0_temperature & 0xFFU;
			pl[42] = (tel->data.rtd_1_temperature >> 8U) & 0xFFU;
			pl[43] = tel->data.rtd_1_temperature & 0xFFU;
			pl[44] = (tel->data.rtd_2_temperature >> 8U) & 0xFFU;
			pl[45] = tel->data.rtd_2_temperature & 0xFFU;
			pl[46] = (tel->data.rtd_3_temperature >> 8U) & 0xFFU;
			pl[47] = tel->data.rtd_3_temperature & 0xFFU;
			pl[48] = (tel->data.rtd_4_temperature >> 8U) & 0xFFU;
			pl[49] = tel->data.rtd_4_temperature & 0xFFU;
			pl[50] = (tel->data.rtd_5_temperature >> 8U) & 0xFFU;
			pl[51] = tel->data.rtd_5_temperature & 0xFFU;
			pl[52] = (tel->data.rtd_6_temperature >> 8U) & 0xFFU;
			pl[53] = tel->data.rtd_6_temperature & 0xFFU;
			pl[54] = (tel->data.battery_voltage >> 8U) & 0xFFU;
			pl[55] = tel->data.battery_voltage & 0xFFU;
			pl[56] = (tel->data.battery_current >> 8U) & 0xFFU;
			pl[57] = tel->data.battery_current & 0xFFU;
			pl[58] = (tel->data.battery_average_current >> 8U) & 0xFFU;
			pl[59] = tel->data.battery_average_current & 0xFFU;
			pl[60] = (tel->data.battery_acc_current >> 8U) & 0xFFU;
			pl[61] = tel->data.battery_acc_current & 0xFFU;
			pl[62] = (tel->data.battery_charge >> 8U) & 0xFFU;
			pl[63] = tel->data.battery_charge & 0xFFU;
			pl[64] = (tel->data.battery_monitor_temperature >> 8U) & 0xFFU;
			pl[65] = tel->data.battery_monitor_temperature & 0xFFU;
			pl[66] = tel->data.battery_monitor_status;
			pl[67] = tel->data.battery_monitor_protection;
			pl[68] = tel->data.battery_monitor_cycle_counter;
			pl[69] = (tel->data.raac >> 8U) & 0xFFU;
			pl[70] = tel->data.raac & 0xFFU;
			pl[71] = (tel->data.rsac >> 8U) & 0xFFU;
			pl[72] = tel->data.rsac & 0xFFU;
			pl[73] = tel->data.rarc;
			pl[74] = tel->data.rsrc;
			pl[75] = (tel->data.battery_heater_1_duty_cycle >> 8U) & 0xFFU;
			pl[76] = tel->data.battery_heater_1_duty_cycle & 0xFFU;
			pl[77] = (tel->data.battery_heater_2_duty_cycle >> 8U) & 0xFFU;
			pl[78] = tel->data.battery_heater_2_duty_cycle & 0xFFU;
			pl[79] = tel->data.mppt_1_mode;
			pl[80] = tel->data.mppt_2_mode;
			pl[81] = tel->data.mppt_3_mode;
			pl[82] = tel->data.battery_heater_1_mode;
			pl[83] = tel->data.battery_heater_2_mode;

			*pkt_pl_len = (uint16_t) 92U; /* 7b RQ CALLSIGN + 1b TC ID + 84b EPS DATA */

			break;
		}

		case DATA_ID_TTC_0:
		{
			ttc_telemetry_t *tel = (ttc_telemetry_t *)data;

			pl[0] = (tel->timestamp >> 24U) & 0xFFU;
			pl[1] = (tel->timestamp >> 16U) & 0xFFU;
			pl[2] = (tel->timestamp >> 8U) & 0xFFU;
			pl[3] = tel->timestamp & 0xFFU;
			pl[4] = (tel->data.time_counter >> 24U) & 0xFFU;
			pl[5] = (tel->data.time_counter >> 16U) & 0xFFU;
			pl[6] = (tel->data.time_counter >> 8U) & 0xFFU;
			pl[7] = tel->data.time_counter & 0xFFU;
			pl[8] = (tel->data.reset_counter >> 8U) & 0xFFU;
			pl[9] = tel->data.reset_counter & 0xFFU;
			pl[10] = tel->data.last_reset_cause;
			pl[11] = (tel->data.voltage_mcu >> 8U) & 0xFFU;
			pl[12] = tel->data.voltage_mcu & 0xFFU;
			pl[13] = (tel->data.current_mcu >> 8U) & 0xFFU;
			pl[14] = tel->data.current_mcu & 0xFFU;
			pl[15] = (tel->data.temperature_mcu >> 8U) & 0xFFU;
			pl[16] = tel->data.temperature_mcu & 0xFFU;
			pl[17] = (tel->data.voltage_radio >> 8U) & 0xFFU;
			pl[18] = tel->data.voltage_radio & 0xFFU;
			pl[19] = (tel->data.current_radio >> 8U) & 0xFFU;
			pl[20] = tel->data.current_radio & 0xFFU;
			pl[21] = (tel->data.temperature_radio >> 8U) & 0xFFU;
			pl[22] = tel->data.temperature_radio & 0xFFU;
			pl[23] = tel->data.last_valid_tc;
			pl[24] = (tel->data.rssi_last_valid_tc >> 8U) & 0xFFU;
			pl[25] = tel->data.rssi_last_valid_tc & 0xFFU;
			pl[26] = (tel->data.temperature_antenna >> 8U) & 0xFFU;
			pl[27] = tel->data.temperature_antenna & 0xFFU;
			pl[28] = (tel->data.antenna_status >> 8U) & 0xFFU;
			pl[29] = tel->data.antenna_status & 0xFFU;
			pl[30] = tel->data.deployment_status;
			pl[31] = tel->data.hibernation_status;
			pl[32] = (tel->data.tx_packet_counter >> 24U) & 0xFFU;
			pl[33] = (tel->data.tx_packet_counter >> 16U) & 0xFFU;
			pl[34] = (tel->data.tx_packet_counter >> 8U) & 0xFFU;
			pl[35] = tel->data.tx_packet_counter & 0xFFU;
			pl[36] = (tel->data.rx_packet_counter >> 24U) & 0xFFU;
			pl[37] = (tel->data.rx_packet_counter >> 16U) & 0xFFU;
			pl[38] = (tel->data.rx_packet_counter >> 8U) & 0xFFU;
			pl[39] = tel->data.rx_packet_counter & 0xFFU;

			*pkt_pl_len = (uint16_t) 48U; /* 7b RQ CALLSIGN + 1b TC ID + 40b TTC DATA */

			break;
		}

		case DATA_ID_TTC_1:
		{
			ttc_telemetry_t *tel = (ttc_telemetry_t *)data;

			pl[0] = (tel->timestamp >> 24U) & 0xFFU;
			pl[1] = (tel->timestamp >> 16U) & 0xFFU;
			pl[2] = (tel->timestamp >> 8U) & 0xFFU;
			pl[3] = tel->timestamp & 0xFFU;
			pl[4] = (tel->data.time_counter >> 24U) & 0xFFU;
			pl[5] = (tel->data.time_counter >> 16U) & 0xFFU;
			pl[6] = (tel->data.time_counter >> 8U) & 0xFFU;
			pl[7] = tel->data.time_counter & 0xFFU;
			pl[8] = (tel->data.reset_counter >> 8U) & 0xFFU;
			pl[9] = tel->data.reset_counter & 0xFFU;
			pl[10] = tel->data.last_reset_cause;
			pl[11] = (tel->data.voltage_mcu >> 8U) & 0xFFU;
			pl[12] = tel->data.voltage_mcu & 0xFFU;
			pl[13] = (tel->data.current_mcu >> 8U) & 0xFFU;
			pl[14] = tel->data.current_mcu & 0xFFU;
			pl[15] = (tel->data.temperature_mcu >> 8U) & 0xFFU;
			pl[16] = tel->data.temperature_mcu & 0xFFU;
			pl[17] = (tel->data.voltage_radio >> 8U) & 0xFFU;
			pl[18] = tel->data.voltage_radio & 0xFFU;
			pl[19] = (tel->data.current_radio >> 8U) & 0xFFU;
			pl[20] = tel->data.current_radio & 0xFFU;
			pl[21] = (tel->data.temperature_radio >> 8U) & 0xFFU;
			pl[22] = tel->data.temperature_radio & 0xFFU;
			pl[23] = tel->data.last_valid_tc;
			pl[24] = (tel->data.rssi_last_valid_tc >> 8U) & 0xFFU;
			pl[25] = tel->data.rssi_last_valid_tc & 0xFFU;
			pl[26] = (tel->data.temperature_antenna >> 8U) & 0xFFU;
			pl[27] = tel->data.temperature_antenna & 0xFFU;
			pl[28] = (tel->data.antenna_status >> 8U) & 0xFFU;
			pl[29] = tel->data.antenna_status & 0xFFU;
			pl[30] = tel->data.deployment_status;
			pl[31] = tel->data.hibernation_status;
			pl[32] = (tel->data.tx_packet_counter >> 24U) & 0xFFU;
			pl[33] = (tel->data.tx_packet_counter >> 16U) & 0xFFU;
			pl[34] = (tel->data.tx_packet_counter >> 8U) & 0xFFU;
			pl[35] = tel->data.tx_packet_counter & 0xFFU;
			pl[36] = (tel->data.rx_packet_counter >> 24U) & 0xFFU;
			pl[37] = (tel->data.rx_packet_counter >> 16U) & 0xFFU;
			pl[38] = (tel->data.rx_packet_counter >> 8U) & 0xFFU;
			pl[39] = tel->data.rx_packet_counter & 0xFFU;

			*pkt_pl_len = (uint16_t) 48U; /* 7b RQ CALLSIGN + 1b TC ID + 40b TTC DATA */

			break;
		}

		case DATA_ID_ANT:
		{
			antenna_telemetry_t *tel = (antenna_telemetry_t *)data;

			pl[0] = (tel->timestamp >> 24U) & 0xFFU;
			pl[1] = (tel->timestamp >> 16U) & 0xFFU;
			pl[2] = (tel->timestamp >> 8U) & 0xFFU;
			pl[3] = tel->timestamp & 0xFFU;
			pl[4] = (tel->data.status.code >> 8U) & 0xFFU;
			pl[5] = tel->data.status.code & 0xFFU;
			pl[6] = tel->data.status.antenna_1.status;
			pl[7] = tel->data.status.antenna_1.timeout;
			pl[8] = tel->data.status.antenna_1.burning;
			pl[9] = tel->data.status.antenna_2.status;
			pl[10] = tel->data.status.antenna_2.timeout;
			pl[11] = tel->data.status.antenna_2.burning;
			pl[12] = tel->data.status.antenna_3.status;
			pl[13] = tel->data.status.antenna_3.timeout;
			pl[14] = tel->data.status.antenna_3.burning;
			pl[15] = tel->data.status.antenna_4.status;
			pl[16] = tel->data.status.antenna_4.timeout;
			pl[17] = tel->data.status.antenna_4.burning;
            pl[18] = tel->data.status.ignoring_switches;
            pl[19] = tel->data.status.independent_burn;
            pl[20] = tel->data.status.armed;
			pl[21] = (tel->data.temperature >> 8U) & 0xFFU;
			pl[22] = tel->data.temperature & 0xFFU;

			*pkt_pl_len = (uint16_t) 31U; /* 7b RQ CALLSIGN + 1b TC ID + 23b ANT DATA */

			break;
		}

		case DATA_ID_SBCD_PKTS:
		{
			edc_ptt_t *tel = (edc_ptt_t *)data;

			pl[0] = (tel->time_tag >> 24U) & 0xFFU;
			pl[1] = (tel->time_tag >> 16U) & 0xFFU;
			pl[2] = (tel->time_tag >> 8U) & 0xFFU;
			pl[3] = tel->time_tag & 0xFFU;
			pl[4] = tel->error_code;
			pl[5] = (((uint32_t)tel->carrier_freq) >> 24U) & 0xFFU;
			pl[6] = (((uint32_t)tel->carrier_freq) >> 16U) & 0xFFU;
			pl[7] = (((uint32_t)tel->carrier_freq) >> 8U) & 0xFFU;
			pl[8] = ((uint32_t)tel->carrier_freq) & 0xFFU;
			pl[9] = (tel->carrier_abs >> 8U) & 0xFFU;
			pl[10] = tel->carrier_abs & 0xFFU;
			pl[11] = tel->msg_byte_length;

            /* Copying PTT user message */
            (void)memcpy(&pl[12], tel->user_msg, 36U);

			*pkt_pl_len = (uint16_t) 56U; /* 7b RQ CALLSIGN + 1b TC ID + 48b SBCD PKT DATA */

			break;
		}

		case DATA_ID_PAYLOAD_INFO:
		{
            payload_telemetry_t *tel = (payload_telemetry_t *)data;
            edc_hk_t *hk = (edc_hk_t*)&tel->data[0];

            /* The state data is stored right after the housekeeping data 
             * on payload_telemetry_t's data field. The offset of 26 is 
             * precisely the housekeeping data length*/
            edc_state_t *st = (edc_state_t*)&tel->data[26];

			pl[0] = (tel->timestamp >> 24U) & 0xFFU;
			pl[1] = (tel->timestamp >> 16U) & 0xFFU;
			pl[2] = (tel->timestamp >> 8U) & 0xFFU;
			pl[3] = tel->timestamp & 0xFFU;
			pl[4] = (hk->current_time >> 24U) & 0xFFU;
			pl[5] = (hk->current_time >> 16U) & 0xFFU;
			pl[6] = (hk->current_time >> 8U) & 0xFFU;
			pl[7] = hk->current_time & 0xFFU;
			pl[8] = (hk->elapsed_time >> 24U) & 0xFFU;
			pl[9] = (hk->elapsed_time >> 16U) & 0xFFU;
			pl[10] = (hk->elapsed_time >> 8U) & 0xFFU;
			pl[11] = hk->elapsed_time & 0xFFU;
			pl[12] = (hk->current_supply_d >> 8U) & 0xFFU;
			pl[13] = hk->current_supply_d & 0xFFU;
			pl[14] = (hk->current_supply_a >> 8U) & 0xFFU;
			pl[15] = hk->current_supply_a & 0xFFU;
			pl[16] = (hk->voltage_supply >> 8U) & 0xFFU;
			pl[17] = hk->voltage_supply & 0xFFU;
			pl[18] = (uint8_t) hk->temp;
			pl[19] = hk->pll_sync_bit;
			pl[20] = (hk->adc_rms >> 8U) & 0xFFU;
			pl[21] = hk->adc_rms & 0xFFU;
			pl[22] = (hk->num_rx_ptt >> 24U) & 0xFFU;
			pl[23] = (hk->num_rx_ptt >> 16U) & 0xFFU;
			pl[24] = (hk->num_rx_ptt >> 8U) & 0xFFU;
			pl[25] = hk->num_rx_ptt & 0xFFU;
			pl[26] = hk->max_parl_decod;
			pl[27] = hk->mem_err_count;
			pl[28] = (st->current_time >> 24U) & 0xFFU;
			pl[29] = (st->current_time >> 16U) & 0xFFU;
			pl[30] = (st->current_time >> 8U) & 0xFFU;
			pl[31] = st->current_time & 0xFFU;
			pl[32] = st->ptt_available;
			pl[33] = st->ptt_is_paused;
			pl[34] = st->sampler_state;
            pl[35] = tel->id;

			*pkt_pl_len = (uint16_t) 44U; /* 7b RQ CALLSIGN + 1b TC ID + 36b EDC STATE + HK DATA + EDC ID */

			break;
		}

		default: 
			err = -1;
			break;
	}

    return err;
}

static int8_t send_tc_feedback(uint8_t *pkt)
{
    int8_t err = 0;
    fsat_pkt_pl_t feedback = {0};

    uint8_t feedback_pkt[21] = {0};
    uint16_t feedback_pkt_len = 0;

    /* Packet ID */
    fsat_pkt_add_id(&feedback, PKT_ID_DOWNLINK_TC_FEEDBACK);

    /* Source callsign */
    fsat_pkt_add_callsign(&feedback, CONFIG_SATELLITE_CALLSIGN);

    /* Requester callsign */
    (void)memcpy(feedback.payload, &pkt[1], 7U);

    /* TC packet ID */
    feedback.payload[8] = pkt[0];
    
    /* Current timestamp */
    sys_time_t time = system_get_time();
    feedback.payload[9] = (time >> 24U) & 0xFFU;
    feedback.payload[10] = (time >> 16U) & 0xFFU;
    feedback.payload[11] = (time >> 8U) & 0xFFU;
    feedback.payload[12] = time & 0xFFU;

    /* Payload lenght */
    feedback.length = 13U;

    fsat_pkt_encode(&feedback, feedback_pkt, &feedback_pkt_len);

    if (sat_data_buf.obdh.data.mode != OBDH_MODE_HIBERNATION)
    {
        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Transmitting \"TC Feedback\"...");
        sys_log_new_line();

        if (ttc_send(TTC_0, feedback_pkt, feedback_pkt_len) != 0)
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error transmitting a \"TC Feedback\"!");
            sys_log_new_line();

            err = -1;
        }
    }

    return err;
}

/** \} End of process_tc group */
