//**************************************************************//
// NGHam protocol - Jon Petter Skagmo, LA3JPA, 2014.            //
// Licensed under LGPL.                                         //
//**************************************************************//

#ifndef NGHAM_EXTENSIONS_H
#define NGHAM_EXTENSIONS_H

#include "stdint.h"
#include "ngham_packets.h"

// Possible values for the type field. After type byte, length follows.
#define PKT_TYPE_DATA		0
#define PKT_TYPE_ID			1
#define PKT_TYPE_STAT		2
#define PKT_TYPE_SIMPLEDIGI	3
#define PKT_TYPE_POS		4
#define PKT_TYPE_TOH		5
#define PKT_TYPE_DEST		6 // Destination/receiver callsign
#define PKT_TYPE_CMD_REQ	7 // Command packet
#define PKT_TYPE_CMD_REPLY	8 // Command packet
#define PKT_TYPE_REQUEST	9

#define PKT_TYPES 10
#define PKT_SIZE_VARIABLE 0xffff

extern const char* PKT_TYPE_STRINGS[];
extern const uint16_t PKT_TYPE_SIZES[];

// Additional NA-values
#define TEMP_NA		0xff
#define VOLT_NA		0xff
#define UINT8_NA	0xff
#define INT32_NA 	0x7fffffff
#define COG_NA 		0x7ff

// Ensure compatibility when used with MinGW etc.
#ifdef _WIN32
#define ATTRIBUTE_PACKED __attribute__ ((packed,gcc_struct))
#else
#define ATTRIBUTE_PACKED __attribute__ ((packed))
#endif

// TOH packet
typedef struct ATTRIBUTE_PACKED{
	uint32_t toh_us;			// Time of hour in microseconds
	uint8_t toh_val;			// Validity
}ngham_toh_t;

// Statistics packet
typedef struct ATTRIBUTE_PACKED{
	uint16_t hw_ver;			// 10b company, 6b product
	uint16_t serial;			// Serial nr.
	uint16_t sw_ver;			// 4b major, 4b minor, 8b build
	uint32_t uptime_s;			// Time in whole seconds since startup
	uint8_t voltage;			// Input voltage in desivolts (0-25.5)
	int8_t temp;				// System temp in deg. celsius (-128 to 127)
	uint8_t signal;				// Received signal strength in dBm - 200, -200 to 54 (0xff=N/A)
	uint8_t noise;				// Noise floor, same as above
	uint16_t cntr_rx_ok;		// Packets successfully received
	uint16_t cntr_rx_fix;		// Packets with corrected errors
	uint16_t cntr_rx_err;		// Packets with uncorrectable errors
	uint16_t cntr_tx;			// Packets sent
}ngham_stat_t;

// Position packet
typedef struct ATTRIBUTE_PACKED{
	int32_t latitude;          // In degrees * 10^7
	int32_t longitude;         // In degrees * 10^7
	int32_t altitude;          // In centimeters
	unsigned int sog:20;     // Hundreds of meters per second
	unsigned int cog:12;   // Tenths of degrees
	uint8_t hdop;              // In tenths
}ngham_pos_t;

// Always first in a packet, except when resent by another station.
typedef struct ATTRIBUTE_PACKED{
	uint8_t call_ssid[6]; // 7 x 6 bit (SIXBIT DEC, which is ASCII-32 and limited to 0-64) empty characters padded with 0, 6 bit SSID
	uint8_t sequence;    // Wraps around from 255 to 0
}ngham_id_t;

typedef struct ATTRIBUTE_PACKED{
	uint8_t call_ssid[6]; // 7 x 6 bit (SIXBIT DEC, which is ASCII-32 and limited to 0-64) empty characters padded with 0, 6 bit SSID
}ngham_dest_t;

uint8_t* ngh_ext_allocate_pkt(tx_pkt_t* p, uint8_t pkt_type, uint16_t data_len);
void ngh_ext_append_pkt(tx_pkt_t* p, uint8_t type, uint8_t* data, uint16_t size);
uint16_t ngh_ext_numpkts(uint8_t* d, uint16_t d_len);
uint8_t ngh_ext_encode_callsign(uint8_t* enc_callsign, char* callsign);
void ngh_ext_decode_callsign(char* callsign, uint8_t* enc_callsign);

#endif