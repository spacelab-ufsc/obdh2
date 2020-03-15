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
 * \version 0.3.1
 * 
 * \date 14/03/2020
 * 
 * \addtogroup fsat_pkt
 * \{
 */

#include <string.h>

#include "fsat_pkt.h"

void fsat_pkt_encode(fsat_pkt_pl_t *pkt, uint8_t *pl, uint16_t *len)
{
}

void fsat_pkt_decode(uint8_t *raw_pkt, uint16_t len, fsat_pkt_pl_t *pkt)
{
    /* Copy packet ID */
    pkt->id = raw_pkt[0];

    uint8_t i = 0;
    for(i=1; i<8; i++)
    {
        if ((raw_pkt[i] >= 0x41) && (raw_pkt[i] <= 0x5A))   /* 0x41 = 'A', 0x5A = 'Z' */
        {
            break;
        }
    }

    /* Copy callsign */
    memcpy(pkt->callsign, raw_pkt+i+1, 7+1-i);

    pkt->callsign[i+1] = '\0';

    /* Copy payload data */
    memcpy(pkt->payload, raw_pkt+7+1, len-7-1);
}

/** \} End of fsat_pkt group */
