//**************************************************************//
// NGHam protocol - Jon Petter Skagmo, LA3JPA, 2014.            //
// Licensed under LGPL.                                         //
//**************************************************************//

#ifndef NGHAM_H
#define NGHAM_H

#include <stdint.h>

#include "ngham_packets.h" // tx_pkt_t

extern const uint8_t NGH_SYNC[];

#define NGH_PREAMBLE_SIZE				4
#define NGH_SYNC_SIZE					4
#define NGH_PREAMBLE_SIZE_FOUR_LEVEL	8
#define NGH_SYNC_SIZE_FOUR_LEVEL		8
#define NGH_SIZE_TAG_SIZE				3
#define NGH_MAX_CODEWORD_SIZE			255
#define NGH_MAX_TOT_SIZE				(NGH_PREAMBLE_SIZE_FOUR_LEVEL+NGH_SYNC_SIZE_FOUR_LEVEL+NGH_SIZE_TAG_SIZE+NGH_MAX_CODEWORD_SIZE)

#define NGH_PADDING_bm                  0x1f
#define NGH_FLAGS_bm                    0xe0
#define NGH_FLAGS_bp                    5

void ngham_init_arrays(void);
void ngham_deinit_arrays(void);
void ngham_init(void);
void ngham_encode(tx_pkt_t* p);
void ngham_decode(uint8_t d);

#endif
