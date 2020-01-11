//**************************************************************//
// NGHam protocol - Jon Petter Skagmo, LA3JPA, 2014.            //
// Licensed under LGPL.                                         //
//**************************************************************//

#include "../ngham_packets.h"	// Packet structs, NA-values etc.
#include "../ngham_extension.h"

#include "platform.h"

#include <stdint.h>

// Temporary buffer for the decoder, rx packet variable and state variable.
uint8_t rx_buf[];	// Should hold the largest packet - ie. 255 B
uint16_t rx_buf_len;
rx_pkt_t rx_pkt;
uint8_t decoder_state;

// Should return RSSI in dBm + 200
uint8_t ngham_action_get_rssi(void){
	return RSSI_NA;
}

// Should return noise floor in dBm + 200
uint8_t ngham_action_get_noise_floor(void){
	return RSSI_NA;
}

// Set packet size demodulator, if applicable, to make the demodulator stop outputting data when the packet is finished.
void ngham_action_set_packet_size(uint8_t size){
}

// Data to be transmitted (to modulator). Priority can be ignored if NGHam is not used for timing purposes.
void ngham_action_send_data(uint8_t *d, uint16_t d_len, uint8_t priority){
	switch (priority){
		case PKT_PRIORITY_NORMAL:
			// Send d[]
			break;
		case PKT_PRIORITY_FIRST_IN_SLOT:
			// Send d[]
			break;
	}
}

// Will always be called after packet reception is finished - whether it was successful or not.
// This function should also handle reinitialization of your decoder/sync word detector.
void ngham_action_handle_packet(uint8_t condition, rx_pkt_t* p){			
	switch (condition){
		case PKT_CONDITION_OK:
			// Add statistics?
			// Handle the received packet in p->pl[]
			break;
		case PKT_CONDITION_PREFAIL:
			// Count as fail
			break;
		case PKT_CONDITION_FAIL:
			// Count as fail and prepare for new sync word immediately
			break;
	}

	// Reset RX packet content
	rx_pkt_init(&rx_pkt);
}

// Not required: Code to be executed when reception has just started.
// This could be handling the time stamp of the sync word or starting RSSI readout.
void ngham_action_reception_started(void){
	// rx_pkt.timestamp = ;
}

// Not required: If there is more to do a little after reception start, do it here.
void ngham_action_reception_halfway(void){
}