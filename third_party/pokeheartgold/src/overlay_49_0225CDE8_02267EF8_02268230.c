#include "global.h"

s32 ov49_02265434(void *, s32);

s32 ov49_022655E0(void *, s32 *, s32 *, s32 *);

s32 ov49_022655F4(void *, u16, u16, s32);

s32 ov49_02265628(void *);

s32 ov49_02265660(void *, s32 *);

s32 sub_020182A8(void *, s32, s32, s32);

s32 ov49_02268230(void *arg0);

s32 ov49_02268230(void *arg0) {
    s32 v3;
    s32 v2;
    s32 v1;
    s32 v6;
    s32 v5;
    void *v4;
    void *v0;
    int v8;
    s32 v9;
    s32 v10;
    void *v7;
    void *v11;
    void *v12;
    void *v13;

    v5 = 0;
    if ((s32) * (u16 *)((u8 *)arg0 + 4) > 0) {
        v11 = arg0 + 0xA8;
        v4 = arg0;
        v7 = arg0 + 8;
        v4 = v7;
        v0 = v11;
        v12 = v7;
        v13 = arg0;
        do {
            if (*(s16 *)arg0 == 0xE) {
                ov49_022655F4(v11, 0, 0xE38, 0xC000);
            } else {
                ov49_02265628(v11);
            }
            ov49_02265434(v4, *(s16 *)arg0);
            ov49_02265660(v0, &v6);
            ov49_022655E0(v12, &v1, &v2, &v3);
            v9 = v2 + v6;
            v2 = v9;
            sub_020182A8(*(void **)((u8 *)v13 + 0xD8), v1, v9, v3);
            v4 += 0x28;
            v11 += 0xC;
            v0 += 0xC;
            v12 += 0x28;
            v8 = v5 + 1;
            v13 += 4;
            v5 = v8;
        } while (v8 < (s32) * (u16 *)((u8 *)arg0 + 4));
    }
    v10 = 0;
    if ((s32) * (s16 *)arg0 >= 0x16) {
        v10 = 1;
    }
    return v10;
}
