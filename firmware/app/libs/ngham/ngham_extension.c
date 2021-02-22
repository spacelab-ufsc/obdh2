//**************************************************************//
// NGHam protocol - Jon Petter Skagmo, LA3JPA, 2014.            //
// Licensed under LGPL.                                         //
//**************************************************************//

#include "ngham_extension.h"

#include "ngham_packets.h"
#include "stdint.h"
#include <string.h>
#include <stdio.h>

const char* PKT_TYPE_STRINGS[] = {
	"data", 
	"id", 
	"stat", 
	"sdigi", 
	"pos", 
	"toh", 
	"dest", 
	"cmd_req", 
	"cmd_reply",
	"request"
};

const uint16_t PKT_TYPE_SIZES[] = {
	PKT_SIZE_VARIABLE, 
	sizeof(ngham_id_t), 
	sizeof(ngham_stat_t), 
	1, 
	sizeof(ngham_pos_t), 
	sizeof(ngham_toh_t), 
	sizeof(ngham_dest_t), 
	PKT_SIZE_VARIABLE,
	PKT_SIZE_VARIABLE,
	1
};

//void ngh_ext_readout_example(uint8_t* d, uint16_t d_len){
//	uint16_t start = 0;
//	uint16_t j;
//	// ngham_extract_numpkts does necessary length check of each field
//	for (j=0; j<ngham_extract_numpkts(d, d_len); j++){
//		switch(d[start]){
//			case PKT_TYPE_ID:
//				{
//					//char callsign[11];
//					//ngham_decode_callsign(callsign, d+start+2);
//					// Callsign string in variable "callsign"
//				}
//				break;
//			case PKT_TYPE_STAT:
//				{
//					//ngham_stat_t* stat = (ngham_stat_t*)(d+start+2);
//					// Access eg. RSSI as stat->signal-200
//				}
//				break;
//		}
//		start += d[start+1] + 2; // next start
//	}
//}			

// Will set data type and length and increase tx_pkt length to fit data, 
// then return pointer to beginning of data without actually copying any data
uint8_t* ngh_ext_allocate_pkt(tx_pkt_t* p, uint8_t pkt_type, uint16_t data_len){
	if ( (p->pl_len + 2 + data_len) > NGHAM_PL_MAX ) return NULL;
	p->pl[p->pl_len] = pkt_type;
	p->pl[p->pl_len+1] = data_len;
	p->pl_len += 2 + data_len;
	return p->pl+p->pl_len-data_len;
}

// Append extension packet with given data, type and size to tx_pkt
void ngh_ext_append_pkt(tx_pkt_t* p, uint8_t type, uint8_t* data, uint16_t size){
	if ( (p->pl_len + 2 + size) > NGHAM_PL_MAX ) return;
	p->pl[p->pl_len] = type;
	p->pl[p->pl_len+1] = size;
	memcpy(&p->pl[p->pl_len+2], data, size);
	p->pl_len += 2 + size;
}

// Returns number of sub packets and verifies them
uint16_t ngh_ext_numpkts(uint8_t* d, uint16_t d_len){
	// Go through all sub packets
	uint16_t start, packets;
	start = 0;
	packets = 0;
	
	while( (d_len >= (start+2)) && (d_len >= (start+2+d[start+1])) ){
		// If PKT_TYPE is invalid valid or packet type does not have correct length
		if ( (d[start]>PKT_TYPES) || 
			 ((PKT_TYPE_SIZES[d[start]] != PKT_SIZE_VARIABLE) &&
			  (PKT_TYPE_SIZES[d[start]] != d[start+1])) ){
			return 0;
		}
		packets++;
		start += d[start+1] + 2; // next start
	}
	return packets;
}

uint8_t ngh_ext_encode_callsign(uint8_t* enc_callsign, char* callsign){
	uint32_t temp;
	uint8_t j, copy[7], ssid = 0;

	// Convert to DEC SIXBIT until length is 7, zero terminated, or dash (SSID start)
	for (j=0; (j<7) && callsign[j] && (callsign[j]!='-'); j++){
		// Lowercase converted to uppercase
		if((callsign[j] >= 0x61) && (callsign[j] <= 0x7a)) copy[j] = callsign[j]-64;
		else copy[j] = callsign[j]-32;
	}
	if (j<7) copy[j] = 0;	// Zero terminate if preliminary end
	
	// Get SSID, if any
	if (callsign[j] == '-'){
		j++;
		// First number
		if ( (callsign[j] >= 0x30) && (callsign[j] <= 0x39) ) ssid += (callsign[j] - '0');
		else return 0;
		j++;
		// Second number
		if ( (callsign[j] >= 0x30) && (callsign[j] <= 0x39) ){
			ssid *= 10;
			ssid += (callsign[j] - '0');
			j++;
		}
		else if (callsign[j] != 0) return 0;
	}
	
	temp = ((copy[0]<<18)&0xfc0000) | ((copy[1]<<12)&0x3f000) | ((copy[2]<<6)&0xfc0) | (copy[3]&0x3f);
	enc_callsign[0] = (temp>>16) & 0xff;
	enc_callsign[1] = (temp>>8) & 0xff;
	enc_callsign[2] = temp & 0xff;

	temp = ((copy[4]<<18)&0xfc0000) | ((copy[5]<<12)&0x3f000) | ((copy[6]<<6)&0xfc0) | (ssid&0x3f);
	enc_callsign[3] = (temp>>16) & 0xff;
	enc_callsign[4] = (temp>>8) & 0xff;
	enc_callsign[5] = temp & 0xff;

	return 1;
}

// 11 long array (7 characters, dash, two decimals and termination)
// Destination "callsign", source "enc_callsign"
void ngh_ext_decode_callsign(char* callsign, uint8_t* enc_callsign){
	uint32_t temp;
	uint8_t j, ssid;
	
	temp = ((enc_callsign[0]<<16)&0xff0000) | ((enc_callsign[1]<<8)&0xff00) | (enc_callsign[2]&0xff);
	callsign[0] = (temp>>18) & 0x3f;
	callsign[1] = (temp>>12) & 0x3f;
	callsign[2] = (temp>>6) & 0x3f;
	callsign[3] = temp & 0x3f;
	
	temp = ((enc_callsign[3]<<16)&0xff0000) | ((enc_callsign[4]<<8)&0xff00) | (enc_callsign[5]&0xff);
	callsign[4] = (temp>>18) & 0x3f;
	callsign[5] = (temp>>12) & 0x3f;
	callsign[6] = (temp>>6) & 0x3f;
	callsign[7] = 0;	// Zero terminate (needed if max length)
	
	// Find end of callsign and convert from DEC SIXBIT
	for (j=0; (j<7) && callsign[j]; j++) callsign[j] += 32;
	
	// If non-zero SSID
	ssid = temp & 0x3f;
	if (ssid){
		callsign[j++] = '-';
		if (ssid > 9){
			callsign[j++] = (ssid/10) + '0';
			ssid %= 10;
		}
		callsign[j++] = ssid + '0';
		callsign[j] = 0;
	}
}