/*
 * fsat_pkt.h
 * 
 * Copyright (C) 2020, SpaceLab.
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
 * \brief FloripaSat packet implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.3.3
 * 
 * \date 14/03/2020
 * 
 * \addtogroup fsat_pkt
 * \{
 */

#include <string.h>

#include "fsat_pkt.h"

void fsat_pkt_add_id(fsat_pkt_pl_t *pkt, uint8_t id)
{
    pkt->id = id;
}

void fsat_pkt_add_callsign(fsat_pkt_pl_t *pkt, const char *callsign)
{
    unsigned int cs_len = strlen(callsign);

    if (cs_len > 7)
    {
        return;     /* Invalid callsign size */
    }

    strncpy(pkt->callsign, callsign, cs_len);
}

void fsat_pkt_add_payload(fsat_pkt_pl_t *pkt, uint8_t *pl, uint16_t len)
{
    memcpy(pkt->payload, pl, len);

    pkt->length = len;
}

void fsat_pkt_encode(fsat_pkt_pl_t pkt, uint8_t *pl, uint16_t *len)
{
    /* Packet ID */
    pl[0] = pkt.id;

    /* Callsign */
    uint8_t cs_len = 0;
    for(cs_len=0; cs_len<7; cs_len++)
    {
        if (pkt.callsign[cs_len] == '\0')
        {
            break;
        }
    }

    uint8_t i = 0;
    for(i=0; i<7-cs_len; i++)
    {
        pl[1+i] = FSAT_PKT_CALLSIGN_PADDING_CHAR;
    }

    memcpy(pl+1+i, pkt.callsign, 7-i);

    /* Packet data */
    memcpy(pl+1+7, pkt.payload, pkt.length);

    *len = 1 + 7 + pkt.length;
}

void fsat_pkt_decode(uint8_t *raw_pkt, uint16_t len, fsat_pkt_pl_t *pkt)
{
    /* Copy packet ID */
    pkt->id = raw_pkt[0];

    /* Copy callsign */
    uint8_t i = 0;
    for(i=0; i<7; i++)
    {
        if ((raw_pkt[i+1] >= 0x41) && (raw_pkt[i+1] <= 0x5A))   /* 0x41 = 'A', 0x5A = 'Z' */
        {
            break;
        }
    }

    memcpy(pkt->callsign, raw_pkt+1+i, 7-i);

    pkt->callsign[7-i] = '\0';

    /* Copy payload data */
    memcpy(pkt->payload, raw_pkt+1+7, len-7-1);

    /* Payload data length */
    pkt->length = len-7-1;
}

/** \} End of fsat_pkt group */
