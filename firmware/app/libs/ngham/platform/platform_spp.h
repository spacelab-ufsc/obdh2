#include <stdint.h>
#include "../ngham_packets.h"

#define PORT_BUF_SIZE 1024

typedef struct{
	uint8_t d[PORT_BUF_SIZE];
	uint16_t d_ip;			// Input index - counts position in buffer d
	uint16_t d_op;			// Output index - if d is used as ring buffer
	ngh_spphdr_t ngh_spp;	// For NGH SPP
}port_ctx_t;
