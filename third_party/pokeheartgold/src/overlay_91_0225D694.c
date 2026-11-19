#include "global.h"

s32 ov91_0225D768(void *, s32, s32, s32);

void ov91_0225D694(u8 *arg0, s32 arg1, s32 arg2, s32 arg3);

s32 ov91_0225D84C(u8 *);
void ov91_0225D6A0(u8 *arg0);

void ov91_0225D694(u8 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    ov91_0225D768(arg0 + (arg1 << 6), arg1, arg2, arg3);
}

void ov91_0225D6A0(u8 *arg0) {
    s32 v0;
    u8 *v1;

    v1 = arg0;
    v0 = 0;
    do {
        ov91_0225D84C(v1);
        v0 += 1;
        v1 += 0x40;
    } while (v0 < 4);
}
