/*
 * cmd_engine.c
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
 * \brief Command engine routing execution and matching look up loops.
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

#include <stddef.h>
#include "cmd_engine.h"

static const cmd_entry_t *g_cmd_table = NULL;
static uint8_t g_cmd_count = 0;

void cmd_engine_init(const cmd_entry_t *table, uint8_t count)
{
    g_cmd_table = table;
    g_cmd_count = count;
}

void cmd_engine_execute(uint8_t cmd_id, uint8_t *pkt, uint16_t pkt_len, bool is_scheduled)
{
    /* Safety-critical boundary guards preventing null exceptions */
    if ((g_cmd_table == NULL) || (pkt == NULL) || (g_cmd_count == 0))
    {
        return;
    }

    /* Safe deterministic integer verification avoiding unsafe string operations */
    for (uint8_t i = 0; i < g_cmd_count; i++)
    {
        if (g_cmd_table[i].id == cmd_id)
        {
            g_cmd_table[i].handler(pkt, pkt_len, is_scheduled);
            return;
        }
    }
}

/** \} End of addtogroup cmd_engine */
