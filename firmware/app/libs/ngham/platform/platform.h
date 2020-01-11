//**************************************************************//
// NGHam protocol - Jon Petter Skagmo, LA3JPA, 2014.            //
// Licensed under LGPL.                                         //
//**************************************************************//

#ifndef PLATFORM_NGHAM_H
#define PLATFORM_NGHAM_H

#include "ngham_packets.h"	// Packet structs, NA-values etc.
#include <stdint.h>

// NGHAM_BYTES_TILL_ACTION_HALFWAY is number of payload bytes until ngham_action_reception_halfway is executed
// Don't care about this if you don't use the mentioned function.
#define NGHAM_BYTES_TILL_ACTION_HALFWAY 10	

extern uint8_t rx_buf[];	// Should hold the largest packet - ie. 255 B
extern uint16_t rx_buf_len;
extern rx_pkt_t rx_pkt;
extern uint8_t decoder_state;

void ngham_action_send_data(uint8_t *d, uint16_t d_len, uint8_t priority);
void ngham_action_set_packet_size(uint8_t size);
uint8_t ngham_action_get_rssi(void);
uint8_t ngham_action_get_noise_floor(void);
void ngham_action_handle_packet(uint8_t condition, rx_pkt_t* p);
void ngham_action_reception_started(void);
void ngham_action_reception_halfway(void);

#endif