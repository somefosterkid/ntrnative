#include "global.h"

#include "overlay_49.h"

s32 ov49_02267AF0(void *, void *, s32);

void ov49_02266748(void *arg0, void *arg1) {
    ov49_02267AF0(arg0, arg1, 0);
}

void ov49_02266754(void *arg0, void *arg1) {
    ov49_02267AF0(arg0, arg1, 1);
}

void ov49_02266760(void *arg0, void *arg1) {
    ov49_02267AF0(arg0, arg1, 2);
}

s32 ov49_02265B14(void *, void *, s32, s32);

s32 ov49_02265B28(void *, void *, s32, s32);

s32 sub_020182A0(void *, s32);

s32 sub_020182A4(void *);

s32 ov49_0226676C(void *arg0, void *arg1) {
    s32 v0;
    s16 v1;
    s32 v2;
    s32 v3;
    s32 v4;
    void *v5;

    v2 = *(s16 *)((u8 *)arg1 + 2) * 3;
    v3 = (s32)(v2 + ((u32)(v2 >> 4) >> 0x1B)) >> 5;
    if (v3 != *(s16 *)((u8 *)arg1 + 0x954)) {
        *(s16 *)((u8 *)arg1 + 0x954) = (s16)v3;
        if ((u32) * (s16 *)((u8 *)arg1 + 0x956) > (u32)v3) {
            if (v3 != 0) {
                sub_020182A0(arg1 + 0xC + (0x78 * (v3 - 1)), 0);
            }
            sub_020182A0(arg1 + 0xC + (0x78 * v3), 1);
        }
    }
    v1 = *(s16 *)((u8 *)arg1 + 2);
    if ((s32)v1 < 0x20) {
        *(s16 *)((u8 *)arg1 + 2) = (s16)(v1 + 1);
    }
    v4 = 0;
    v0 = 0;
    if ((s32) * (s16 *)((u8 *)arg1 + 0x956) > 0) {
        v5 = arg1 + 0xC;
        do {
            if (sub_020182A4(v5) != 0) {
                if (v4 == (*(s16 *)((u8 *)arg1 + 0x956) - 1)) {
                    v0 = ov49_02265B28(arg0, arg1, v4, 0);
                } else {
                    ov49_02265B14(arg0, arg1, v4, 0);
                }
            }
            v4 += 1;
            v5 += 0x78;
        } while (v4 < (s32) * (s16 *)((u8 *)arg1 + 0x956));
    }
    return v0;
}
