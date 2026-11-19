#include "global.h"

s32 ov45_0222F7B0(s32);

s32 ov45_0222FA40(void *, s32);

s32 ov45_0222FA74(void *, s32, s32);

s32 ov45_0222FB5C(void *, s32, s32);

void ov45_0222FB84(void *arg0, s32 arg1);

void ov45_0222FB84(void *arg0, s32 arg1) {
    s32 v0;
    s32 v1;
    s32 v2;
    void *v3;

    v1 = ov45_0222F7B0(arg1);
    ov45_0222FA40(arg0, 1);
    v2 = 0;
    if ((s32) * (u16 *)((u8 *)arg0 + 0x11E) > 0) {
        v3 = arg0;
        do {
            v0 = ov45_0222FB5C(arg0, v2, 0);
            if ((v0 != -1) && (v1 == *(s32 *)((u8 *)v3 + 0x34))) {
                ov45_0222FA74(arg0, v0, 1);
            }
            v2 += 1;
            v3 += 0xC;
        } while (v2 < (s32) * (u16 *)((u8 *)arg0 + 0x11E));
    }
}
