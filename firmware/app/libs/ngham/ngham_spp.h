//**************************************************************//
// NGHam protocol - Jon Petter Skagmo, LA3JPA, 2014.            //
// Licensed under LGPL.                                         //
//**************************************************************//

#include <stdint.h>

#include "ngham_packets.h"
#include "ngham_paths.h"
#include PATH_NGHAM_PLATFORM_SPP

#define NGHAM_SPP_TYPE_RX		0x00	// Packet types
#define NGHAM_SPP_TYPE_TX		0x01	// Packet types
#define NGHAM_SPP_TYPE_LOCAL	0x02
#define NGHAM_SPP_TYPE_CMD		0x03

void ngham_parse_byte(port_ctx_t* ctx, uint8_t c);
void ngham_spp_fill_header(ngh_spphdr_t* hdr, uint8_t type, uint8_t* d, uint16_t d_len);
void ngham_print_cmd(port_ctx_t* ctx, uint8_t* d, uint16_t d_len);
void ngham_print_rx_pkt(rx_pkt_t* p);
void ngham_pack_tx_pkt_local(tx_pkt_t* p);