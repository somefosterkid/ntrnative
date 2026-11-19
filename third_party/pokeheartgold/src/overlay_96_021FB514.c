#include "global.h"

s32 ov96_021FB514(void *arg0, void *arg1);

s32 ov96_021FB56C(void *arg0, void *arg1);

s32 ov96_021FB514(void *arg0, void *arg1) {
    u16 v0;
    u16 v1;

    if (*(u16 *)((u8 *)arg1 + 0xE) != 0) {
        v1 = *(u16 *)((u8 *)arg0 + 0xC);
        v0 = *(u16 *)((u8 *)arg1 + 0xC);
        if ((u32)v0 > (u32)v1) {
            *(void **)((u8 *)(*(void **)arg1) + 4) = arg0;
            *(void **)arg0 = (void *)*(void **)arg1;
            *(void **)((u8 *)arg0 + 4) = arg1;
            *(void **)arg1 = arg0;
            return 1;
        }
        if (v0 == v1) {
            if ((u32) * (u16 *)((u8 *)arg1 + 0xA) > (u32) * (u16 *)((u8 *)arg0 + 0xA)) {
                *(void **)((u8 *)(*(void **)arg1) + 4) = arg0;
                *(void **)arg0 = (void *)*(void **)arg1;
                *(void **)((u8 *)arg0 + 4) = arg1;
                *(void **)arg1 = arg0;
                *(s32 *)((u8 *)arg1 + 0x10) = 1;
                return 1;
            }
            *(s32 *)((u8 *)arg0 + 0x10) = 1;
            goto error;
        }
        goto error;
    }
    *(void **)((u8 *)(*(void **)arg1) + 4) = arg0;
    *(void **)arg0 = (void *)*(void **)arg1;
    *(void **)((u8 *)arg0 + 4) = arg1;
    *(void **)arg1 = arg0;
    return 1;
error:
    return 0;
}

s32 ov96_021FB56C(void *arg0, void *arg1) {
    u16 v0;
    u16 v1;
    u16 v2;
    u16 v3;

    v2 = *(u16 *)((u8 *)arg0 + 8);
    v0 = *(u16 *)((u8 *)arg1 + 8);
    if ((u32)v0 < (u32)v2) {
        *(void **)((u8 *)(*(void **)arg1) + 4) = arg0;
        *(void **)arg0 = (void *)*(void **)arg1;
        *(void **)((u8 *)arg0 + 4) = arg1;
        *(void **)arg1 = arg0;
        return 1;
    }
    if (v0 == v2) {
        v3 = *(u16 *)((u8 *)arg0 + 0xC);
        v1 = *(u16 *)((u8 *)arg1 + 0xC);
        if ((u32)v1 > (u32)v3) {
            *(void **)((u8 *)(*(void **)arg1) + 4) = arg0;
            *(void **)arg0 = (void *)*(void **)arg1;
            *(void **)((u8 *)arg0 + 4) = arg1;
            *(void **)arg1 = arg0;
            return 1;
        }
        if (v1 == v3) {
            if ((u32) * (u16 *)((u8 *)arg1 + 0xA) > (u32) * (u16 *)((u8 *)arg0 + 0xA)) {
                *(void **)((u8 *)(*(void **)arg1) + 4) = arg0;
                *(void **)arg0 = (void *)*(void **)arg1;
                *(void **)((u8 *)arg0 + 4) = arg1;
                *(void **)arg1 = arg0;
                *(s32 *)((u8 *)arg1 + 0x10) = 1;
                return 1;
            }
            *(s32 *)((u8 *)arg0 + 0x10) = 1;
            goto error;
        }
        goto error;
    }
error:
    return 0;
}
