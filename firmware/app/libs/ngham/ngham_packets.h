//**************************************************************//
// NGHam protocol - Jon Petter Skagmo, LA3JPA, 2014.            //
// Licensed under LGPL.                                         //
//**************************************************************//

#ifndef NGHAM_PACKETS_H
#define NGHAM_PACKETS_H

#include "stdint.h"

// Possible packet conditions
#define PKT_CONDITION_OK		0	// Successfully received packet
#define PKT_CONDITION_FAIL		1	// Receiption failed after receiving the packet
#define PKT_CONDITION_PREFAIL	2	// Reception failed before receiving the whole packet

// Packet priority in transmission
#define PKT_PRIORITY_NORMAL			0
#define PKT_PRIORITY_FIRST_IN_SLOT	10	// Should be put first in the next time slot for timing purposes

// Size of payload in packet struct
#define PKT_PL_SIZE 512

// Basic "not available"-values
#define RSSI_NA				0xff
#define TIMESTAMP_NA		0xffffffff

// If the following flag is set in a packet, NGHam extensions are used and first byte is type
#define NGHAM_FLAG_TYPE_EXTENSION	0x01

#define SPP_PL_MAX 255	// Not a power of two, so no ring buffers!

#define NGHAM_PL_MAX 220

// Ensure compatibility when used with MinGW etc.
#ifdef _WIN32
#define ATTRIBUTE_PACKED __attribute__ ((packed,gcc_struct))
#else
#define ATTRIBUTE_PACKED __attribute__ ((packed))
#endif

typedef struct ATTRIBUTE_PACKED{
	uint32_t timestamp_toh_us;		// Time stamp of sync word detection
	uint8_t noise;			// Same as above
	uint8_t rssi;			// In dBm + 200
	uint8_t errors;			// Recovered symbols
	uint8_t ngham_flags;
	uint8_t pl[PKT_PL_SIZE];
	uint16_t pl_len;
}rx_pkt_t;

typedef struct ATTRIBUTE_PACKED{
	uint8_t priority;
	uint8_t ngham_flags;
	uint8_t pl[PKT_PL_SIZE];
	uint16_t pl_len;
}tx_pkt_t;

// NGHam SPP header
typedef struct ATTRIBUTE_PACKED{
	uint8_t start;
	uint16_t crc;
	uint8_t pl_type;
	uint8_t pl_len;
}ngh_spphdr_t;

void rx_pkt_init(rx_pkt_t *p);
void tx_pkt_init(tx_pkt_t *p);

#endif
