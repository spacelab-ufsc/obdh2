/*
 * fsat_pkt.h
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
 * \brief FloripaSat packet definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.8.21
 * 
 * \date 2020/03/14
 * 
 * \defgroup fsat_pkt FloripaSat Packet Handler
 * \{
 */

#ifndef FSAT_PKT_H_
#define FSAT_PKT_H_

#include <stdint.h>

#define FSAT_PKT_CALLSIGN_PADDING_CHAR      (' ')

/**
 * \brief FloripaSat packet.
 */
typedef struct
{
    uint8_t id;                             /**< Packet ID. */
    char callsign[7+1];                     /**< Callsign (7 bytes). */
    uint8_t payload[220-7-1];               /**< Packet payload. */
    uint16_t length;                        /**< Packet payload length. */
} fsat_pkt_pl_t;

/**
 * \brief Add the ID code to a FSat packet.
 *
 * \param[in,out] pkt is the packet to add the ID.
 *
 * \param[in] id is the ID code.
 *
 * \return None.
 */
void fsat_pkt_add_id(fsat_pkt_pl_t *pkt, uint8_t id);

/**
 * \brief Add the source callsign to a FSat packet.
 *
 * \param[in,out] pkt is the packet to add the source callsign.
 *
 * \param[in] callsign is the callsign of the packet source.
 *
 * \return The status/error code.
 */
int fsat_pkt_add_callsign(fsat_pkt_pl_t *pkt, const char *callsign);

/**
 * \brief Add the payload to a FSat packet.
 *
 * \param[in,out] pkt is the packet to add the payload data.
 *
 * \param[in] pl is the payload.
 *
 * \param[in] len is the payload length in bytes.
 *
 * \return None.
 */
void fsat_pkt_add_payload(fsat_pkt_pl_t *pkt, uint8_t *pl, uint16_t len);

/**
 * \brief Encodes a raw packet.
 *
 * \param[in] pkt is a pointer to the packet structure to encode.
 *
 * \param[in,out] pl is a pointer to store the raw packet.
 *
 * \param[in,out] len is the length of the result raw packet in bytes.
 *
 * \return None.
 */
void fsat_pkt_encode(fsat_pkt_pl_t *pkt, uint8_t *pl, uint16_t *len);

/**
 * \brief Decodes a raw packet.
 *
 * \param[in] raw_pkt is an array with a raw packet.
 *
 * \param[in] len is length of the raw packet in bytes.
 *
 * \param[in] pkt is the packet structure to store the result.
 *
 * \return None.
 */
void fsat_pkt_decode(uint8_t *raw_pkt, uint16_t len, fsat_pkt_pl_t *pkt);

#endif /* FSAT_PKT_H_ */

/** \} End of fsat_pkt group */
