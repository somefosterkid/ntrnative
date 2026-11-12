// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nnsys/src/g2d/g2di_BitReader.h.
#ifndef POKEBW2_NNSYS_G2DI_BITREADER_H
#define POKEBW2_NNSYS_G2DI_BITREADER_H

#include "types.h"

// Reads a glyph's pixels a few bits at a time, from the high bits of each byte. The names are NitroSystem's
typedef struct {
    const u8 *src;
    s8 availableBits;
    u8 bits;
    u8 padding_[2];
} NNSiG2dBitReader;

static inline void NNSi_G2dBitReaderInit(NNSiG2dBitReader *reader, const void *src) {
    reader->availableBits = 0;
    reader->src = (const u8 *)src;
    reader->bits = 0;
}

u32 NNSi_G2dBitReaderRead(NNSiG2dBitReader *reader, int nBits);

#endif // POKEBW2_NNSYS_G2DI_BITREADER_H
