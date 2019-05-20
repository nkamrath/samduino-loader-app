//fletcher 16 checksum header

#ifndef _FLETCHER_16_H_
#define _FLETCHER_16_H_

#include <stdint.h>

uint16_t fletcher16(uint8_t* data, uint16_t dataLen);

#endif