#include "global.h"

#include "overlay_80_0222AEF8.h"
#include "unk_02035900.h"

void ov80_0222B24C(s32 arg0, s32 arg1, u16 *arg2, void *arg3) {
    s32 v4;
    s32 v5;
    u16 *v6;
    u16 *v7;
    u16 v0;
    u16 v2;
    u16 v3;
    void *v1;
    void *v8;
    void *v9;
    void *v10;
    v8 = arg3;
    *((u8 *)(((u8 *)v8) + 0x702)) = (u8)((*((u8 *)(((u8 *)v8) + 0x702))) + 1);
    if ((arg0 != sub_0203769C()) && (sub_0203769C() != 0)) {
        s32 v11;
        v5 = 0;
        v6 = arg2;
        v10 = v8;
        do {
            v0 = *v6;
            v5 += 1;
            v6 += 1;
            *((u16 *)(((u8 *)v10) + 0x254)) = v0;
            v10 += 2;
        } while (v5 < 6);
        v11 = 0;
        v7 = arg2;
        do {
            v2 = *((u16 *)(((u8 *)v7) + 0xC));
            v1 = v8 + v11;
            v11 += 1;
            v7 += 1;
            *((s8 *)(((u8 *)v1) + 0x260)) = (s8)v2;
        } while (v11 < 6);
        v4 = 0;
        v9 = arg2;
        v9 = v9 + 0x18;
        do {
            v4 += 1;
            *((s32 *)(((u8 *)v8) + 0x268)) = (s32)(*((u16 *)v9));
            v3 = *((u16 *)(((u8 *)v9) + 0xC));
            *((s32 *)(((u8 *)v8) + 0x268)) = (s32)((*((s32 *)(((u8 *)v8) + 0x268))) | (v3 << 0x10));
            v9 += 2;
            v8 += 4;
        } while (v4 < 6);
    }
}
