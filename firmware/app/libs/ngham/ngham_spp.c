//**************************************************************//
// NGHam protocol - Jon Petter Skagmo, LA3JPA, 2014.            //
// Licensed under LGPL.                                         //
// This is made somewhat platform specific for the Owl VHF      //
// with serial port contexts, and is meant as an guidance.      //
//**************************************************************//

#include "ngham_spp.h"
#include "crc_ccitt.h"
#include <string.h> // For memcpy

// Definition of port context port_ctx_t and port_* functions,
// as well as packer_call which sends data to transmit chain
#include "ngham_paths.h"
#include PATH_NGHAM_PLATFORM_SPP

// Packet start byte definition
#define SPP_START			0x24	

// States
#define SPP_STATE_START 	0x00	
#define SPP_STATE_HEADER	0x01
#define SPP_STATE_PAYLOAD	0x02

void ngham_parse_byte(port_ctx_t* ctx, uint8_t c){
	switch(ctx->state){
		case SPP_STATE_START:
			if (c == SPP_START){
				ctx->state = SPP_STATE_HEADER;	// Start found; go to next state
				ctx->d_ip = 1;	// Starts at next first as SPP_START is already received
			}
			break;
			
		case SPP_STATE_HEADER:
			// Fill ctx->d with header - no check for size, as buffer is much larger than header (5B)
			ctx->d[ctx->d_ip++] = c;
			
			if (ctx->d_ip >= sizeof(ngh_spphdr_t)){		
				// Target length in d_op
				ctx->d_op = sizeof(ngh_spphdr_t) + ((ngh_spphdr_t*)ctx->d)->pl_len;
				ctx->state = SPP_STATE_PAYLOAD;
			}
			break;
			
		case SPP_STATE_PAYLOAD:
			// Fill ctx->d with payload
			if (ctx->d_ip < PORT_BUF_SIZE) ctx->d[ctx->d_ip++] = c;
			
			// If received length has met target length (set in STATE_HEADER)
			if (ctx->d_ip >= ctx->d_op){
				ngh_spphdr_t* hdr = (ngh_spphdr_t*)ctx->d;

				if (crc_ccitt(ctx->d+3, ctx->d_ip-3) == hdr->crc){
					switch(hdr->pl_type){
						
						// Data to be sent
						case NGHAM_SPP_TYPE_TX:
							{
								tx_pkt_t p;
								tx_pkt_init(&p);
								
								// Set flags, length (excluding flag-byte) and copy data
								p.ngham_flags = ctx->d[sizeof(ngh_spphdr_t)];
								p.pl_len = hdr->pl_len-1;
								memcpy(p.pl, ctx->d+sizeof(ngh_spphdr_t)+1, p.pl_len);
								
								// Packer call define - this is where the TX-packets are sent
								packer_call(&p);
								
								// TODO: Generate response!
							}
							break;
							
						// Command
						case NGHAM_SPP_TYPE_CMD:
							{
								uint8_t rep[REPLY_SIZE];	// CMD can be longer than SPP_PL_MAX
								uint16_t rep_len = 0;
								cmd(ctx->d+sizeof(ngh_spphdr_t), hdr->pl_len, rep, &rep_len, REPLY_SIZE);	// Run command
								ngham_print_cmd(ctx, rep, rep_len);	// Send reply
							}
							break;
					}
				}
				
				ctx->state = SPP_STATE_START;
			}
			break;
	}
}

void ngham_spp_fill_header(ngh_spphdr_t* hdr, uint8_t type, uint8_t* d, uint16_t d_len){
	uint16_t j, crc;
	
	hdr->start = SPP_START;
	hdr->pl_type = type;
	hdr->pl_len = d_len;
	
	crc = crc_ccitt_byte(hdr->pl_type, 0xffff);
	crc = crc_ccitt_byte(hdr->pl_len, crc);
	for (j=0; j<d_len; j++) crc = crc_ccitt_byte(d[j], crc);
	crc ^= 0xffff;
	
	hdr->crc = crc;
}

void ngham_print_cmd(port_ctx_t* ctx, uint8_t* d, uint16_t d_len){
	ngh_spphdr_t hdr;
	uint16_t shortened_len, offset = 0;
	
	// Split into multiple packets if necessary
	while (d_len){
		if (d_len > SPP_PL_MAX) shortened_len = SPP_PL_MAX;
		else shortened_len = d_len;
		
		ngham_spp_fill_header(&hdr, NGHAM_SPP_TYPE_CMD, d+offset, shortened_len);
		
		// Copy remaining and send to port
		port_output(ctx, (uint8_t*)&hdr, sizeof(ngh_spphdr_t));
		port_output(ctx, d+offset, hdr.pl_len);
		
		d_len -= shortened_len;
		offset += shortened_len;
	}
}

// Output buffer should be prefilled with 
void ngham_print_rx_pkt(rx_pkt_t* p){
	ngh_spphdr_t hdr;
	
	ngham_spp_fill_header(&hdr, NGHAM_SPP_TYPE_RX, (uint8_t*)p, p->pl_len+8);
	port_unpacker_output(PACKER_NGHAM, (uint8_t*)&hdr, sizeof(ngh_spphdr_t));
	port_unpacker_output(PACKER_NGHAM, (uint8_t*)p, hdr.pl_len);
}

void ngham_pack_tx_pkt_local(tx_pkt_t* p){
	ngh_spphdr_t hdr;
	
	ngham_spp_fill_header(&hdr, NGHAM_SPP_TYPE_LOCAL, (uint8_t*)&(p->ngham_flags), p->pl_len+1);
	port_unpacker_output(PACKER_NGHAM, (uint8_t*)&hdr, sizeof(ngh_spphdr_t));
	port_unpacker_output(PACKER_NGHAM, (uint8_t*)&(p->ngham_flags), hdr.pl_len);
}
