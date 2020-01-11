#include "ngham_packets.h"

void rx_pkt_init(rx_pkt_t *p){
	p->pl_len = 0;
	p->ngham_flags = 0;
	p->rssi = RSSI_NA;
	p->noise = RSSI_NA;
	p->errors = 0;
	p->timestamp_toh_us = TIMESTAMP_NA;
}

void tx_pkt_init(tx_pkt_t *p){
	p->pl_len = 0;
	p->ngham_flags = 0;
	p->priority = PKT_PRIORITY_NORMAL;
}
