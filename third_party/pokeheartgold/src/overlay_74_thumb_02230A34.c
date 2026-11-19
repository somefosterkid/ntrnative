#include "global.h"

u8 ov74_02231064(void);

s32 ov74_022314A0(void);

void ov74_02230A34(void);

void *ov74_02231054(void);
s32 ov74_02231460(void *, u32, s32);
void ov74_02230A4C(void *arg0, u32 arg1, s32 arg2);
s32 ov74_02230A74(void);
s32 ov74_02230A7C(void);

void ov74_02230A34(void) {
    u8 v0;
    int new_var;
    v0 = ov74_02231064();
    if (((v0 == 0) || (v0 == (new_var = 1))) || (v0 == 2)) {
        ov74_022314A0();
    }
}

void ov74_02230A4C(void *startAddr, u32 nBytes, s32 arg2) {
    if (nBytes <= (u32) * (u16 *)((u8 *)(ov74_02231054()) + 0x34)) {
        DC_FlushRange(startAddr, nBytes);
        ov74_02231460(startAddr, nBytes, arg2);
    }
}

s32 ov74_02230A74(void) {
    return 0x5C0;
}

s32 ov74_02230A7C(void) {
    return 0x5C0;
}
