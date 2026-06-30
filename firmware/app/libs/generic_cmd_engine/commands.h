/*
 * commands.h
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
 * \brief Spacecraft subsystem callback registration and tracking.
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

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include "cmd_engine.h"

/* Operational ID definitions matching the real repository configurations */
#define PKT_ID_UPLINK_PING_REQ            0x40
#define PKT_ID_UPLINK_DATA_REQ            0x41
#define PKT_ID_UPLINK_BROADCAST_MSG       0x42
#define PKT_ID_UPLINK_ENTER_HIBERNATE     0x43

/**
 * \brief Forwards binary telemetry packet requests to the native system ping handler.
 *
 * \param[in] pkt is the raw array holding the data stream.
 *
 * \param[in] pkt_len is the length boundary constraint tracked across execution.
 *
 * \param[in] is_scheduled marks if the processing block dropped down from the system calendar.
 *
 * \return None.
 */
void cmd_handler_ping(uint8_t *pkt, uint16_t pkt_len, bool is_scheduled);

/**
 * \brief Forwards data request packets down to core retrieval pipelines.
 *
 * \param[in] pkt is the raw array holding the data stream.
 *
 * \param[in] pkt_len is the length boundary constraint tracked across execution.
 *
 * \param[in] is_scheduled marks if the processing block dropped down from the system calendar.
 *
 * \return None.
 */
void cmd_handler_data_request(uint8_t *pkt, uint16_t pkt_len, bool is_scheduled);

/**
 * \brief Decodes input frames and broadcasts messaging events safely.
 *
 * \param[in] pkt is the raw array holding the data stream.
 *
 * \param[in] pkt_len is the length boundary constraint tracked across execution.
 *
 * \param[in] is_scheduled marks if the processing block dropped down from the system calendar.
 *
 * \return None.
 */
void cmd_handler_broadcast_message(uint8_t *pkt, uint16_t pkt_len, bool is_scheduled);

/**
 * \brief Processes and handles critical system power level adjustments.
 *
 * \param[in] pkt is the raw array holding the data stream.
 *
 * \param[in] pkt_len is the length boundary constraint tracked across execution.
 *
 * \param[in] is_scheduled marks if the processing block dropped down from the system calendar.
 *
 * \return None.
 */
void cmd_handler_enter_hibernation(uint8_t *pkt, uint16_t pkt_len, bool is_scheduled);

extern const cmd_entry_t g_commands[];
extern const uint8_t g_num_commands;

#endif /* COMMANDS_H_ */

/** \} End of addtogroup process_tc */
