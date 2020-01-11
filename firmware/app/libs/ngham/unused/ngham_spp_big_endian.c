//**************************************************************//
// NGHam protocol - Jon Petter Skagmo, LA3JPA, 2014.            //
// Licensed under LGPL.                                         //
//**************************************************************//

#include "ngham_packets.h"
#include "crc_ccitt.h"
#include "../terminal.h"	// TODO: Put definition of port context port_ctx_t elsewhere!
#include "../config.h"		// TODO: Packer call from elsewhere


#define SPP_START			0x24	// Packet start byte definition
#define SPP_STATE_START 	0x00	// States
#define SPP_STATE_HEADER	0x01
#define SPP_STATE_PAYLOAD	0x02
#define SPP_TYPE_RF			0x00	// Packet types
#define SPP_TYPE_LOCAL		0x01
#define SPP_TYPE_CMD		0x02

void ngham_pack_byte(port_ctx_t* ctx, uint8_t c){
	switch(ctx->state){
		case SPP_STATE_START:
			if (c == SPP_START){
				ctx->state++;	// Start found; go to next state
				ctx->d_ip = sizeof(ngh_spphdr_rev_t)-2;
				// Header is filled in reverse order to correct endianness
				// Starts at next last byte (next first) as SPP_START is already received
			}
			break;
		case SPP_STATE_HEADER:
			((uint8_t*)&ctx->ngh_spp)[ctx->d_ip] = c;	// Fill header backwards
			if (!ctx->d_ip) ctx->state++;	// When zero is reached, d_ip is ready to count up
			else ctx->d_ip--;
			break;
			
		case SPP_STATE_PAYLOAD:
			// Fill ctx->d with payload in correct order
			if (ctx->d_ip < PORT_BUF_SIZE) ctx->d[ctx->d_ip++] = c;
			if (ctx->d_ip >= ctx->ngh_spp.pl_len){
				ctx->state = SPP_STATE_START;

				// Calc CRC
				int j;
				uint16_t crc = 0xffff;
				for (j=0; j<3; j++) crc = crc_ccitt_byte(((uint8_t*)&ctx->ngh_spp)[2-j], crc);
				for (j=0; j<ctx->d_ip; j++) crc = crc_ccitt_byte(ctx->d[j], crc);
				crc ^= 0xffff;

				
				if (crc == ctx->ngh_spp.crc){
					switch(ctx->ngh_spp.pl_type){
						// Data to be sent
						case SPP_TYPE_RF:
							{
								tx_pkt_t p;
								tx_pkt_init(&p);
								p.ngham_flags = ctx->d[0];	// First byte in NGH SPP RF type is flags
								p.pl_len = ctx->d_ip-1;
								for (j=0; j<p.pl_len; j++) p.pl[j] = ctx->d[j+1];
								packer_call(&p);
							}
							break;
						// Command
						case SPP_TYPE_CMD:
							{
								// Prepare response
								ngh_spphdr_rev_t r_hdr;
								uint8_t r[sizeof(r_hdr)+SPP_PL_MAX];
								r_hdr.start = SPP_START;
								r_hdr.pl_type = SPP_TYPE_CMD;
								r_hdr.pl_len = 0;
								cmd(ctx->d, ctx->ngh_spp.pl_len, &r[sizeof(r_hdr)], &r_hdr.pl_len);
								for (j=0; j<3; j++) r[5-j] = ((uint8_t*)&r_hdr)[j];		// Copy length and type into header in reverse order
								r_hdr.crc = crc_ccitt(&r[3], r_hdr.pl_len+3);
								for (j=0; j<3; j++) r[2-j] = ((uint8_t*)&r_hdr)[j+3];	// Copy CRC and stat into header in reverse order

								// Send to port
								port_output(ctx, r, sizeof(r_hdr)+r_hdr.pl_len);
							}
							break;
					}
				}
				else port_output(ctx, "CRC", 3);
			}
			break;
	}
}

void ngham_unpack(rx_pkt_t* p){
	int j;
	ngh_spphdr_rev_t hdr_rev;
	uint8_t hdr[sizeof(hdr_rev)];
	hdr_rev.start = SPP_START;
	hdr_rev.pl_type = SPP_TYPE_RF;
	hdr_rev.pl_len = p->pl_len+4;
	uint8_t* p_u8 = (uint8_t*)&p->noise;	// SPP-protocol must match with rx_pkt_t struct

	// Calc CRC
	hdr_rev.crc = 0xffff;
	for (j=0; j<3; j++) hdr_rev.crc = crc_ccitt_byte(((uint8_t*)&hdr_rev.pl_type)[3-j], hdr_rev.crc);
	for (j=0; j<(p->pl_len+4); j++) hdr_rev.crc = crc_ccitt_byte(p_u8[j], hdr_rev.crc);
	hdr_rev.crc ^= 0xffff;

	// Copy remaining and send to port
	for (j=0; j<sizeof(ngh_spphdr_rev_t); j++) hdr[j] = ((uint8_t*)&hdr_rev)[sizeof(ngh_spphdr_rev_t)-j-1];
	port_unpacker_output(PROTOCOL_NGHAM, (uint8_t*)&hdr, sizeof(ngh_spphdr_rev_t));
	port_unpacker_output(PROTOCOL_NGHAM, p_u8, p->pl_len+4);
}
