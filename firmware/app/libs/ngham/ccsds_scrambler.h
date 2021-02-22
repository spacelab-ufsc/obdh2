//**************************************************************//
// NGHam protocol - Jon Petter Skagmo, LA3JPA, 2014.            //
// Licensed under LGPL.                                         //
//**************************************************************//

#ifndef CCSDS_SCRAMBLER_H
#define CCSDS_SCRAMBLER_H

#include <stdint.h>

extern const uint8_t ccsds_poly[255];

void ccsds_scrambler_init();
uint8_t ccsds_scrambler_xor(uint8_t data);

#endif