/*
 * commands.c
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
 * along with OBDH 2.0. If not, see <http://gnu.org>.
 * 
 */

/**
 * \brief Maps custom layout routing hooks directly to internal backend flight functions.
 * 
 * \author Your Name <your.email@example.com>
 * 
 * \version 1.0.0
 * 
 * \date 2026/06/21
 * 
 * \addtogroup process_tc
 * \{
 */

#include <stddef.h>
#include <system/sys_log/sys_log.h>
#include "commands.h"

/* Standard tag for identifying telemetry operations in logs */
#define TASK_PROCESS_TC_NAME          "PROCESS_TC"

/* External link references pointing back to the repository's real functions */
extern void process_tc_ping_request(uint8_t *pkt, uint16_t pkt_len, bool is_scheduled);
extern void process_tc_data_request(uint8_t *pkt, uint16_t pkt_len, bool is_scheduled);
extern void process_tc_broadcast_message(uint8_t *pkt, uint16_t pkt_len, bool is_scheduled);
extern void process_tc_enter_hibernation(uint8_t *pkt, uint16_t pkt_len, bool is_scheduled);

void cmd_handler_ping(uint8_t *pkt, uint16_t pkt_len, bool is_scheduled)
{
    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Ping TC received via Engine!");
    sys_log_new_line();

    process_tc_ping_request(pkt, pkt_len, is_scheduled);
}

void cmd_handler_data_request(uint8_t *pkt, uint16_t pkt_len, bool is_scheduled)
{
    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Data request TC received via Engine!");
    sys_log_new_line();

    process_tc_data_request(pkt, pkt_len, is_scheduled);
}

void cmd_handler_broadcast_message(uint8_t *pkt, uint16_t pkt_len, bool is_scheduled)
{
    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Broadcast message TC received via Engine!");
    sys_log_new_line();

    /* Strictly enforce the repository's rules against scheduling broadcast commands */
    if (!is_scheduled)
    {
        process_tc_broadcast_message(pkt, pkt_len, is_scheduled);
    }
}

void cmd_handler_enter_hibernation(uint8_t *pkt, uint16_t pkt_len, bool is_scheduled)
{
    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_PROCESS_TC_NAME, "Executing the TC \"Enter Hibernation\" via Engine...");
    sys_log_new_line();

    process_tc_enter_hibernation(pkt, pkt_len, is_scheduled);
}

/* Immutable layout securely mapped to onboard memory structures */
const cmd_entry_t g_commands[] = {
    {PKT_ID_UPLINK_PING_REQ,        cmd_handler_ping},
    {PKT_ID_UPLINK_DATA_REQ,        cmd_handler_data_request},
    {PKT_ID_UPLINK_BROADCAST_MSG,   cmd_handler_broadcast_message},
    {PKT_ID_UPLINK_ENTER_HIBERNATE, cmd_handler_enter_hibernation}
};

const uint8_t g_num_commands = sizeof(g_commands) / sizeof(g_commands[0]);

/** \} End of addtogroup process_tc */
