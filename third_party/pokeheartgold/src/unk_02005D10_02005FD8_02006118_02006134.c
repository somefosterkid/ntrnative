#include "global.h"

#include "unk_02005D10.h"

void sub_0200592C(u16 seqID, u16 trackBitMask, s32 pitch);

BOOL PlaySE_SetPitch(u16 seqNo, int pitch) {
    BOOL result = PlaySE(seqNo);
    if (result) {
        sub_0200592C(seqNo, 0xFFFF, pitch);
    }
    return result;
}
