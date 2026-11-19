#include "global.h"

void *ov45_0222E04C(void *, u16);

s32 ov45_0222E0A4(u8 *, u8 *);

s32 ov45_0222E0E0(u8 *, s32, s32, s32, void *, void *, void *, void *, u16, u16, u16, u16, s32, s32, s32);

void ov45_0222DAE0(u8 *arg0, void *arg1);

void ov45_0222DAE0(u8 *arg0, void *arg1) {
    u8 *v0;

    v0 = ov45_0222E04C(arg0 + 0x5C, 0xC);
    if (v0 != 0) {
        ov45_0222E0E0(v0, *(s32 *)arg1, 0, 0, *(void **)((u8 *)arg1 + 4), *(void **)((u8 *)arg1 + 8), *(void **)((u8 *)arg1 + 0xC), *(void **)((u8 *)arg1 + 0x10), (s32) * (u16 *)((u8 *)arg1 + 0x14), (s32) * (u16 *)((u8 *)arg1 + 0x16), (s32) * (u16 *)((u8 *)arg1 + 0x18), (s32) * (u16 *)((u8 *)arg1 + 0x1A), 0x1C2, 0xC, 4);
        ov45_0222E0A4(arg0 + 0x5C, v0);
    }
}

void ov45_0222DB3C(u8 *arg0, void *arg1);

void ov45_0222DB3C(u8 *arg0, void *arg1) {
    u8 *v0;

    v0 = ov45_0222E04C(arg0 + 0x5C, 0xD);
    if (v0 != 0) {
        ov45_0222E0E0(v0, *(s32 *)arg1, 0, 0, *(void **)((u8 *)arg1 + 4), *(void **)((u8 *)arg1 + 8), *(void **)((u8 *)arg1 + 0xC), *(void **)((u8 *)arg1 + 0x10), (s32) * (u16 *)((u8 *)arg1 + 0x14), (s32) * (u16 *)((u8 *)arg1 + 0x16), (s32) * (u16 *)((u8 *)arg1 + 0x18), (s32) * (u16 *)((u8 *)arg1 + 0x1A), 0x1C2, 0xD, 5);
        ov45_0222E0A4(arg0 + 0x5C, v0);
    }
}
