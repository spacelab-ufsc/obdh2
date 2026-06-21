/*
 * cmd_engine.h
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
 * \brief Command routing engine lookup definitions.
 * 
 * \author Your Name <your.email@example.com>
 * 
 * \version 1.0.0
 * 
 * \date 2026/06/21
 * 
 * \addtogroup cmd_engine
 * \{
 */

#ifndef CMD_ENGINE_H_
#define CMD_ENGINE_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * \brief Layout mapping configuration for binary telecommand engine tracking.
 */
typedef struct {
    uint8_t id;                                                         /**< Telecommand operational hex code ID */
    void (*handler)(uint8_t *pkt, uint16_t pkt_len, bool is_scheduled); /**< Dedicated subsystem execution loop pointer */
} cmd_entry_t;

/**
 * \brief Binds the static immutable commands look up routing table to the core engine.
 *
 * \param[in] table is the pointer referencing the static registration structure arrays.
 *
 * \param[in] count is the total number of functions securely logged inside the database.
 *
 * \return None.
 */
void cmd_engine_init(const cmd_entry_t *table, uint8_t count);

/**
 * \brief Looks up operational packets using fast, safe integer comparisons.
 *
 * \param[in] cmd_id is the unique binary code extracted from the stream header interface.
 *
 * \param[in] pkt is the raw data string packet array pointer.
 *
 * \param[in] pkt_len is the length restriction parameter bounding the tracking sequence.
 *
 * \param[in] is_scheduled tags if command processing dropped down from the system timeline queue.
 *
 * \return None.
 */
void cmd_engine_execute(uint8_t cmd_id, uint8_t *pkt, uint16_t pkt_len, bool is_scheduled);

#endif /* CMD_ENGINE_H_ */

/** \} End of addtogroup cmd_engine */
