// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/random.h, the lines in use so far.
#ifndef BWPORT_GFL_RANDOM_H
#define BWPORT_GFL_RANDOM_H

#include "types.h"
#include "nitro/math.h"

u32 GFL_RandomLC(u32 max);
u32 GFL_RandomLCAlt(u32 max);
u32 GFL_RandomMT(void);

// A random number below range, or any 32-bit number if range is 0
static inline u32 GFL_RandomMTRange(u32 range) {
    u64 value = GFL_RandomMT();
    if (range != 0) {
        value *= range;
        value >>= 32;
    }
    return value;
}

// Seeds a random context from the time and the hardware
void buildSeed(MATHRandContext32 *context);

#endif // BWPORT_GFL_RANDOM_H
