#include "global.h"

s32 ov45_0222DE8C(s32, s32, s32);

u16 ov45_0222DF38(s32, s32);

void ov45_0222D8F0(s32 arg0, s32 arg1);

void *ov45_0222E04C(void *, u16);
s32 ov45_0222E0A4(u8 *, u8 *);
s32 ov45_0222E0E0(u8 *, s32, s32, s32, void *, void *, void *, void *, u16, u16, u16, u16, s32, s32, s32);
void ov45_0222D940(u8 *arg0, void *arg1);
void ov45_0222D990(u8 *arg0, void *arg1);

void ov45_0222D8F0(s32 arg0, s32 arg1) {
    s32 v1;
    s32 v0;
    s32 v2;
    s32 v3;
    s32 v4;
    s32 v5;
    long long new_var;
    s32 v6;
    v0 = arg0;
    v1 = arg1;
    v6 = 0;
    v5 = 8 + arg0;
    do {
        v2 = ov45_0222DF38(v5, v6);
        if (v2 & 2) {
            ov45_0222DE8C(v5, v6, v2 & (~2));
        }
        v6 += 1;
    } while (v6 < 0x14);
    v3 = v0 + 8;
    v4 = 2 | ov45_0222DF38(new_var = v0 + 8, v1);
    v0 = v3;
    ov45_0222DE8C(v3, v1, v4);
}

void ov45_0222D940(u8 *arg0, void *arg1) {
    u8 *v0;

    v0 = ov45_0222E04C(arg0 + 0x5C, 8);
    if (v0 != 0) {
        ov45_0222E0E0(v0, 0, 0, 0, *(void **)arg1, *(void **)((u8 *)arg1 + 4), 0, 0, (s32) * (u16 *)((u8 *)arg1 + 8), (s32) * (u16 *)((u8 *)arg1 + 0xA), 0, 0, 0x258, 8, 0);
        ov45_0222E0A4(arg0 + 0x5C, v0);
    }
}

void ov45_0222D990(u8 *arg0, void *arg1) {
    u8 *v0;

    if ((u32) * (u32 *)((u8 *)arg1 + 0xC) < 0x1BU) {
        v0 = ov45_0222E04C(arg0 + 0x5C, 7);
        if (v0 != 0) {
            ov45_0222E0E0(v0, *(u32 *)((u8 *)arg1 + 0xC), 0, 0, *(void **)arg1, *(void **)((u8 *)arg1 + 4), 0, 0, (s32) * (u16 *)((u8 *)arg1 + 8), (s32) * (u16 *)((u8 *)arg1 + 0xA), 0, 0, 0x258, 7, 1);
            ov45_0222E0A4(arg0 + 0x5C, v0);
        }
    }
}
