#ifndef CRC_CCITT_H
#define	CRC_CCITT_H

#include <stdint.h>

uint16_t crc_ccitt_byte(uint8_t buf, uint16_t crc);
uint16_t crc_ccitt(uint8_t *buf, uint16_t buf_len);

#endif
