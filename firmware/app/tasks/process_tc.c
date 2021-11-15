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
 * 
 * \version 0.8.35
 * 
 * \date 2021/07/06
 * 
 * \addtogroup process_tc
 * \{
 */

#include <string.h>

#include <config/config.h>
#include <config/keys.h>

#include <system/sys_log/sys_log.h>
#include <system/system.h>
#include <devices/ttc/ttc.h>
#include <devices/media/media.h>
#include <devices/payload/payload.h>
#include <hmac/sha.h>

#include <structs/satellite.h>

#include <fsat_pkt/fsat_pkt.h>

#include "process_tc.h"
#include "startup.h"

xTaskHandle xTaskProcessTCHandle;

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
//static void process_tc_get_payload_data(uint8_t *pkt, uint16_t pkt_len);

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
//static void process_tc_get_parameter(uint8_t *pkt, uint16_t pkt_len);

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

            if (ttc_recv(TTC_1, pkt, &pkt_len) != 0)
            {
                switch(pkt[0])
                {
                    case CONFIG_PKT_ID_UPLINK_PING_REQ:
                        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Ping TC received!");
                        sys_log_new_line();

                        process_tc_ping_request(pkt, pkt_len);

                        break;
                    case CONFIG_PKT_ID_UPLINK_DATA_REQ:
                        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Data request TC received!");
                        sys_log_new_line();

                        process_tc_data_request(pkt, pkt_len);

                        break;
                    case CONFIG_PKT_ID_UPLINK_BROADCAST_MSG:
                        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Broadcast message TC received!");
                        sys_log_new_line();

                        process_tc_broadcast_message(pkt, pkt_len);

                        break;
                    case CONFIG_PKT_ID_UPLINK_ENTER_HIBERNATION:
                        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Executing the TC \"Enter Hibernation\"...");
                        sys_log_new_line();

                        process_tc_enter_hibernation(pkt, pkt_len);

                        break;
                    case CONFIG_PKT_ID_UPLINK_LEAVE_HIBERNATION:
                        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Executing the TC \"Leave Hibernation\"...");
                        sys_log_new_line();

                        process_tc_leave_hibernation(pkt, pkt_len);

                        break;
                    case CONFIG_PKT_ID_UPLINK_ACTIVATE_MODULE:
                        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Executing the TC \"Activate Module\"...");
                        sys_log_new_line();

                        process_tc_activate_module(pkt, pkt_len);

                        break;
                    case CONFIG_PKT_ID_UPLINK_DEACTIVATE_MODULE:
                        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Executing the TC \"Deactivate Module\"...");
                        sys_log_new_line();

                        process_tc_deactivate_module(pkt, pkt_len);

                        break;
                    case CONFIG_PKT_ID_UPLINK_ACTIVATE_PAYLOAD:
                        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Executing the TC \"Activate Payload\"...");
                        sys_log_new_line();

                        process_tc_activate_payload(pkt, pkt_len);

                        break;
                    case CONFIG_PKT_ID_UPLINK_DEACTIVATE_PAYLOAD:
                        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Executing the TC \"Deactivate Payload\"...");
                        sys_log_new_line();

                        process_tc_deactivate_payload(pkt, pkt_len);

                        break;
                    case CONFIG_PKT_ID_UPLINK_ERASE_MEMORY:
                        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Executing the TC \"Erase Memory\"...");
                        sys_log_new_line();

                        process_tc_erase_memory(pkt, pkt_len);

                        break;
                    case CONFIG_PKT_ID_UPLINK_FORCE_RESET:
                        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Executing the TC \"Force Reset\"...");
                        sys_log_new_line();

                        process_tc_force_reset(pkt, pkt_len);

                        break;
                    case CONFIG_PKT_ID_UPLINK_GET_PAYLOAD_DATA:
                        break;
                    case CONFIG_PKT_ID_UPLINK_SET_PARAM:
                        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Executing the TC \"Set Parameter\"...");
                        sys_log_new_line();

                        process_tc_set_parameter(pkt, pkt_len);

                        break;
                    case CONFIG_PKT_ID_UPLINK_GET_PARAM:
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

void process_tc_ping_request(uint8_t *pkt, uint16_t pkt_len)
{
    if (pkt_len >= 8U)
    {
        fsat_pkt_pl_t pong_pl = {0};

        /* Packet ID */
        fsat_pkt_add_id(&pong_pl, CONFIG_PKT_ID_DOWNLINK_PING_ANS);

        /* Source callsign */
        fsat_pkt_add_callsign(&pong_pl, CONFIG_SATELLITE_CALLSIGN);

        if (memcpy(&pong_pl.payload[0], &pkt[1], 7) == &pong_pl.payload[0])
        {
            pong_pl.length = 7U;

            uint8_t pong_pl_raw[16] = {0};
            uint16_t pong_pl_raw_len = 0;

            fsat_pkt_encode(pong_pl, pong_pl_raw, &pong_pl_raw_len);

            if (ttc_send(TTC_1, pong_pl_raw, pong_pl_raw_len) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error transmitting a ping answer!");
                sys_log_new_line();
            }
        }
    }
}

void process_tc_data_request(uint8_t *pkt, uint16_t pkt_len)
{
    if (pkt_len >= (1U + 7U + 1U + 4U + 4U))
    {
        fsat_pkt_pl_t data_pl = {0};

        /* Packet ID */
        fsat_pkt_add_id(&data_pl, CONFIG_PKT_ID_DOWNLINK_DATA_REQUEST_ANS);

        /* Source callsign */
        fsat_pkt_add_callsign(&data_pl, CONFIG_SATELLITE_CALLSIGN);

        uint8_t tc_key[16] = CONFIG_TC_KEY_DATA_REQUEST;

        if (process_tc_validate_hmac(pkt, 1U + 7U + 1U + 4U + 4U, &pkt[17], 20U, tc_key, sizeof(CONFIG_TC_KEY_DATA_REQUEST)-1U))
        {
            switch(pkt[8])
            {
                case CONFIG_DATA_ID_OBDH:
                    sys_log_print_event_from_module(SYS_LOG_WARNING, TASK_PROCESS_TC_NAME, "OBDH data request not implemented!");
                    sys_log_new_line();

                    break;
                case CONFIG_DATA_ID_EPS:
                    sys_log_print_event_from_module(SYS_LOG_WARNING, TASK_PROCESS_TC_NAME, "EPS data request not implemented!");
                    sys_log_new_line();

                    break;
                case CONFIG_DATA_ID_TTC_0:
                    sys_log_print_event_from_module(SYS_LOG_WARNING, TASK_PROCESS_TC_NAME, "TTC 0 data request not implemented!");
                    sys_log_new_line();

                    break;
                case CONFIG_DATA_ID_TTC_1:
                    sys_log_print_event_from_module(SYS_LOG_WARNING, TASK_PROCESS_TC_NAME, "TTC 1 data request not implemented!");
                    sys_log_new_line();

                    break;
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

void process_tc_broadcast_message(uint8_t *pkt, uint16_t pkt_len)
{
    if (pkt_len >= 15U)
    {
        fsat_pkt_pl_t broadcast_pl = {0};

        /* Packet ID */
        fsat_pkt_add_id(&broadcast_pl, CONFIG_PKT_ID_DOWNLINK_MESSAGE_BROADCAST);

        /* Source callsign */
        fsat_pkt_add_callsign(&broadcast_pl, CONFIG_SATELLITE_CALLSIGN);

        uint16_t msg_len = pkt_len - 7U - 7U - 1U;

        if (memcpy(&broadcast_pl.payload[0], &pkt[1], 14U + msg_len) == &broadcast_pl.payload[0])
        {
            broadcast_pl.length = 14U + msg_len;

            uint8_t broadcast_pl_raw[55] = {0};
            uint16_t broadcast_pl_raw_len = 0;

            fsat_pkt_encode(broadcast_pl, broadcast_pl_raw, &broadcast_pl_raw_len);

            if (ttc_send(TTC_1, broadcast_pl_raw, broadcast_pl_raw_len) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error transmitting a message broadcast!");
                sys_log_new_line();
            }
        }
    }
}

void process_tc_enter_hibernation(uint8_t *pkt, uint16_t pkt_len)
{
    if (pkt_len >= 30U)
    {
        uint8_t tc_key[16] = CONFIG_TC_KEY_ENTER_HIBERNATION;

        if (process_tc_validate_hmac(pkt, 1U + 7U + 2U, &pkt[10], 20U, tc_key, sizeof(CONFIG_TC_KEY_ENTER_HIBERNATION)-1U))
        {
            sat_data_buf.obdh.data.mode = OBDH_MODE_HIBERNATION;
            sat_data_buf.obdh.data.ts_last_mode_change = system_get_time();
            sat_data_buf.obdh.data.mode_duration = (((sys_time_t)pkt[8] << 8) | (sys_time_t)pkt[9]) * 60UL * 60UL;
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error executing the \"Enter hibernation\" TC! Invalid key!");
            sys_log_new_line();
        }
    }
}

void process_tc_leave_hibernation(uint8_t *pkt, uint16_t pkt_len)
{
    if (pkt_len >= 30U)
    {
        uint8_t tc_key[16] = CONFIG_TC_KEY_LEAVE_HIBERNATION;

        if (process_tc_validate_hmac(pkt, 1U + 7U, &pkt[8], 20U, tc_key, sizeof(CONFIG_TC_KEY_LEAVE_HIBERNATION)-1U))
        {
            sat_data_buf.obdh.data.mode = OBDH_MODE_NORMAL;
            sat_data_buf.obdh.data.ts_last_mode_change = system_get_time();
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error executing the \"Leave hibernation\" TC! Invalid key!");
            sys_log_new_line();
        }
    }
}

void process_tc_activate_module(uint8_t *pkt, uint16_t pkt_len)
{
    if (pkt_len >= 29U)
    {
        switch(pkt[8])
        {
            case CONFIG_MODULE_ID_BATTERY_HEATER:
            {
                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Activating the battery heater...");
                sys_log_new_line();

                uint8_t tc_key[16] = CONFIG_TC_KEY_ACTIVATE_MODULE;

                if (process_tc_validate_hmac(pkt, 1U + 7U + 1U, &pkt[9], 20U, tc_key, sizeof(CONFIG_TC_KEY_ACTIVATE_MODULE)-1U))
                {
                    /* Enable the EPS heater */
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "TC not implemented yet");
                    sys_log_new_line();
                }
                else
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error activating the battery heater module! Invalid key!");
                    sys_log_new_line();
                }

                break;
            }
            case CONFIG_MODULE_ID_BEACON:
            {
                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Activating the beacon...");
                sys_log_new_line();

                uint8_t tc_key[16] = CONFIG_TC_KEY_ACTIVATE_MODULE;

                if (process_tc_validate_hmac(pkt, 1U + 7U + 1U, &pkt[9], 20U, tc_key, sizeof(CONFIG_TC_KEY_ACTIVATE_MODULE)-1U))
                {
                    /* Enable the beacon */
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "TC not implemented yet");
                    sys_log_new_line();
                }
                else
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error activating the beacon module! Invalid key!");
                    sys_log_new_line();
                }

                break;
            }
            case CONFIG_MODULE_ID_PERIODIC_TELEMETRY:
            {
                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Activating the periodic telemetry...");
                sys_log_new_line();

                uint8_t tc_key[16] = CONFIG_TC_KEY_ACTIVATE_MODULE;

                if (process_tc_validate_hmac(pkt, 1U + 7U + 1U, &pkt[9], 20U, tc_key, sizeof(CONFIG_TC_KEY_ACTIVATE_MODULE)-1U))
                {
                    /* Enable the periodic telemetry */
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "TC not implemented yet");
                    sys_log_new_line();
                }
                else
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error activating the periodic telemetry module! Invalid key!");
                    sys_log_new_line();
                }

                break;
            }
            default:
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Invalid module to activate!");
                sys_log_new_line();

                break;
        }
    }
}

void process_tc_deactivate_module(uint8_t *pkt, uint16_t pkt_len)
{
    if (pkt_len >= 29U)
    {
        switch(pkt[8])
        {
            case CONFIG_MODULE_ID_BATTERY_HEATER:
            {
                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Deactivating the battery heater...");
                sys_log_new_line();

                uint8_t tc_key[16] = CONFIG_TC_KEY_DEACTIVATE_MODULE;

                if (process_tc_validate_hmac(pkt, 1U + 7U + 1U, &pkt[9], 20U, tc_key, sizeof(CONFIG_TC_KEY_DEACTIVATE_MODULE)-1U))
                {
                    /* Enable the EPS heater */
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "TC not implemented yet");
                    sys_log_new_line();
                }
                else
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error deactivating the battery heater module! Invalid key!");
                    sys_log_new_line();
                }

                break;
            }
            case CONFIG_MODULE_ID_BEACON:
            {
                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Deactivating the beacon...");
                sys_log_new_line();

                uint8_t tc_key[16] = CONFIG_TC_KEY_DEACTIVATE_MODULE;

                if (process_tc_validate_hmac(pkt, 1U + 7U + 1U, &pkt[9], 20U, tc_key, sizeof(CONFIG_TC_KEY_DEACTIVATE_MODULE)-1U))
                {
                    /* Enable the beacon */
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "TC not implemented yet");
                    sys_log_new_line();
                }
                else
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error deactivating the beacon module! Invalid key!");
                    sys_log_new_line();
                }

                break;
            }
            case CONFIG_MODULE_ID_PERIODIC_TELEMETRY:
            {
                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Deactivating the periodic telemetry...");
                sys_log_new_line();

                uint8_t tc_key[16] = CONFIG_TC_KEY_DEACTIVATE_MODULE;

                if (process_tc_validate_hmac(pkt, 1U + 7U + 1U, &pkt[9], 20U, tc_key, sizeof(CONFIG_TC_KEY_DEACTIVATE_MODULE)-1U))
                {
                    /* Enable the periodic telemetry */
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "TC not implemented yet");
                    sys_log_new_line();
                }
                else
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error deactivating the periodic telemetry module! Invalid key!");
                    sys_log_new_line();
                }

                break;
            }
            default:
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Invalid module to deactivate!");
                sys_log_new_line();

                break;
        }
    }
}

void process_tc_activate_payload(uint8_t *pkt, uint16_t pkt_len)
{
    if (pkt_len >= 29U)
    {
        switch(pkt[8])
        {
            case CONFIG_PL_ID_EDC_1:
            {
                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Activating the EDC 1 payload...");
                sys_log_new_line();

                uint8_t tc_key[16] = CONFIG_TC_KEY_ACTIVATE_PAYLOAD_EDC;

                if (process_tc_validate_hmac(pkt, 1U + 7U + 1U, &pkt[9], 20U, tc_key, sizeof(CONFIG_TC_KEY_ACTIVATE_PAYLOAD_EDC)-1U))
                {
                    if (payload_enable(PAYLOAD_EDC_0) != 0)
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error activating the EDC 1 payload!");
                        sys_log_new_line();
                    }
                }
                else
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error activating the EDC 1 payload! Invalid key!");
                    sys_log_new_line();
                }

                break;
            }
            case CONFIG_PL_ID_EDC_2:
            {
                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Activating the EDC 2 payload...");
                sys_log_new_line();

                uint8_t tc_key[16] = CONFIG_TC_KEY_ACTIVATE_PAYLOAD_EDC;

                if (process_tc_validate_hmac(pkt, 1U + 7U + 1U, &pkt[9], 20U, tc_key, sizeof(CONFIG_TC_KEY_ACTIVATE_PAYLOAD_EDC)-1U))
                {
                    if (payload_enable(PAYLOAD_EDC_1) != 0)
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error activating the EDC 2 payload!");
                        sys_log_new_line();
                    }
                }
                else
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error activating the EDC 2 payload! Invalid key!");
                    sys_log_new_line();
                }

                break;
            }
            case CONFIG_PL_ID_PAYLOAD_X:
            {
                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Activating the Payload-X payload...");
                sys_log_new_line();

                uint8_t tc_key[16] = CONFIG_TC_KEY_ACTIVATE_PAYLOAD_PAYLOAD_X;

                if (process_tc_validate_hmac(pkt, 1U + 7U + 1U, &pkt[9], 20U, tc_key, sizeof(CONFIG_TC_KEY_ACTIVATE_PAYLOAD_PAYLOAD_X)-1U))
                {
                    if (payload_enable(PAYLOAD_X) != 0)
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error activating the Payload-X payload!");
                        sys_log_new_line();
                    }
                }
                else
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error activating the Payload-X payload! Invalid key!");
                    sys_log_new_line();
                }

                break;
            }
            case CONFIG_PL_ID_RADIATION_MONITOR:
            {
                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Activating the Harsh payload...");
                sys_log_new_line();

                uint8_t tc_key[16] = CONFIG_TC_KEY_ACTIVATE_PAYLOAD_HARSH;

                if (process_tc_validate_hmac(pkt, 1U + 7U + 1U, &pkt[9], 20U, tc_key, sizeof(CONFIG_TC_KEY_ACTIVATE_PAYLOAD_HARSH)-1U))
                {
                    if (payload_enable(PAYLOAD_HARSH) != 0)
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error activating the Harsh payload!");
                        sys_log_new_line();
                    }
                }
                else
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error activating the Harsh payload! Invalid key!");
                    sys_log_new_line();
                }

                break;
            }
            default:
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Invalid payload to activate!");
                sys_log_new_line();

                break;
        }
    }
}

void process_tc_deactivate_payload(uint8_t *pkt, uint16_t pkt_len)
{
    if (pkt_len >= 29U)
    {
        switch(pkt[8])
        {
            case CONFIG_PL_ID_EDC_1:
            {
                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Deactivating the EDC 1 payload...");
                sys_log_new_line();

                uint8_t tc_key[16] = CONFIG_TC_KEY_DEACTIVATE_PAYLOAD_EDC;

                if (process_tc_validate_hmac(pkt, 1U + 7U + 1U, &pkt[9], 20U, tc_key, sizeof(CONFIG_TC_KEY_DEACTIVATE_PAYLOAD_EDC)-1U))
                {
                    if (payload_disable(PAYLOAD_EDC_0) != 0)
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error deactivating the EDC 1 payload!");
                        sys_log_new_line();
                    }
                }
                else
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error deactivating the EDC 1 payload! Invalid key!");
                    sys_log_new_line();
                }

                break;
            }
            case CONFIG_PL_ID_EDC_2:
            {
                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Deactivating the EDC 2 payload...");
                sys_log_new_line();

                uint8_t tc_key[16] = CONFIG_TC_KEY_DEACTIVATE_PAYLOAD_EDC;

                if (process_tc_validate_hmac(pkt, 1U + 7U + 1U, &pkt[9], 20U, tc_key, sizeof(CONFIG_TC_KEY_DEACTIVATE_PAYLOAD_EDC)-1U))
                {
                    if (payload_disable(PAYLOAD_EDC_1) != 0)
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error deactivating the EDC 2 payload!");
                        sys_log_new_line();
                    }
                }
                else
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error deactivating the EDC 2 payload! Invalid key!");
                    sys_log_new_line();
                }

                break;
            }
            case CONFIG_PL_ID_PAYLOAD_X:
            {
                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Deactivating the Payload-X payload...");
                sys_log_new_line();

                uint8_t tc_key[16] = CONFIG_TC_KEY_DEACTIVATE_PAYLOAD_PAYLOAD_X;

                if (process_tc_validate_hmac(pkt, 1U + 7U + 1U, &pkt[9], 20U, tc_key, sizeof(CONFIG_TC_KEY_DEACTIVATE_PAYLOAD_PAYLOAD_X)-1U))
                {
                    if (payload_disable(PAYLOAD_X) != 0)
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error deactivating the Payload-X payload!");
                        sys_log_new_line();
                    }
                }
                else
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error deactivating the Payload-X payload! Invalid key!");
                    sys_log_new_line();
                }

                break;
            }
            case CONFIG_PL_ID_RADIATION_MONITOR:
            {
                sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Deactivating the Harsh payload...");
                sys_log_new_line();

                uint8_t tc_key[16] = CONFIG_TC_KEY_DEACTIVATE_PAYLOAD_HARSH;

                if (process_tc_validate_hmac(pkt, 1U + 7U + 1U, &pkt[9], 20U, tc_key, sizeof(CONFIG_TC_KEY_DEACTIVATE_PAYLOAD_HARSH)-1U))
                {
                    if (payload_disable(PAYLOAD_HARSH) != 0)
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error deactivating the Harsh payload!");
                        sys_log_new_line();
                    }
                }
                else
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error deactivating the Harsh payload! Invalid key!");
                    sys_log_new_line();
                }

                break;
            }
            default:
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Invalid payload to deactivate!");
                sys_log_new_line();

                break;
        }
    }
}

void process_tc_erase_memory(uint8_t *pkt, uint16_t pkt_len)
{
    if (pkt_len >= 28U)
    {
        uint8_t tc_key[16] = CONFIG_TC_KEY_ERASE_MEMORY;

        if (process_tc_validate_hmac(pkt, 1U + 7U, &pkt[8], 20U, tc_key, sizeof(CONFIG_TC_KEY_ERASE_MEMORY)-1U))
        {
            if (media_erase(MEDIA_NOR, MEDIA_ERASE_DIE, 0U) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error erasing the NOR memory!");
                sys_log_new_line();
            }
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error executing the \"Erase Memory\" TC! Invalid key!");
            sys_log_new_line();
        }
    }
}

void process_tc_force_reset(uint8_t *pkt, uint16_t pkt_len)
{
    if (pkt_len >= 28U)
    {
        uint8_t tc_key[16] = CONFIG_TC_KEY_FORCE_RESET;

        if (process_tc_validate_hmac(pkt, 1U + 7U, &pkt[8], 20U, tc_key, sizeof(CONFIG_TC_KEY_FORCE_RESET)-1U))
        {
            system_reset();
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error executing the \"Force Reset\" TC! Invalid key!");
            sys_log_new_line();
        }
    }
}

//void process_tc_get_payload_data(uint8_t *pkt, uint16_t pkt_len)
//{
//}

void process_tc_set_parameter(uint8_t *pkt, uint16_t pkt_len)
{
    if (pkt_len >= (1U + 7U + 1U + 1U + 4U + 20U))
    {
        uint8_t tc_key[16] = CONFIG_TC_KEY_SET_PARAMETER;

        if (process_tc_validate_hmac(pkt, 1U + 7U + 1U + 1U + 4U, &pkt[14], 20U, tc_key, sizeof(CONFIG_TC_KEY_SET_PARAMETER)-1U))
        {
            switch(pkt[8])
            {
                case CONFIG_SUBSYSTEM_ID_OBDH:
                    break;
                case CONFIG_SUBSYSTEM_ID_TTC_1:
                    break;
                case CONFIG_SUBSYSTEM_ID_TTC_2:
                    break;
                case CONFIG_SUBSYSTEM_ID_EPS:
                    break;
                default:
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Invalid subsystem to set a parameter!");
                    sys_log_new_line();

                    break;
            }
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_PROCESS_TC_NAME, "Error executing the \"Set Parameter\" TC! Invalid key!");
            sys_log_new_line();
        }
    }
}

//void process_tc_get_parameter(uint8_t *pkt, uint16_t pkt_len)
//{
//}

bool process_tc_validate_hmac(uint8_t *msg, uint16_t msg_len, uint8_t *msg_hash, uint16_t msg_hash_len, uint8_t *key, uint16_t key_len)
{
    bool res = false;

    uint8_t hash[20] = {0};

    if (hmac(SHA1, msg, msg_len, key, key_len, hash) == 0)
    {
        if (memcmp(msg, msg_hash, (size_t)msg_hash_len) == 0)
        {
            res = true;
        }
    }

    return res;
}

/** \} End of process_tc group */
