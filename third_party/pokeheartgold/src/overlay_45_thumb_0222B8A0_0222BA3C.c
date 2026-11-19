#include "global.h"

s32 ov45_0222BAC4(void *, void *);

s32 ov45_0222BADC(void *, void *);

u8 ov45_0222CC50(void *, s32);

s32 ov45_0222CC7C(void *, s32);

s32 ov45_0222E9F8(void *);

void ov45_0222BA3C(void *arg0);

void ov45_0222BA3C(void *arg0) {
    s32 v0;
    void *v1;

    if (ov45_0222BADC(arg0 + 0x108, *(void **)arg0) == 0) {
        *(s32 *)((u8 *)arg0 + 0x52C) = 1;
        return;
    }
    v0 = 0;
    v1 = arg0;
    do {
        *(s8 *)((u8 *)(arg0 + v0) + 0x174) = ov45_0222CC50(arg0 + 0x4BC, v0);
        *(s32 *)((u8 *)v1 + 0x180) = ov45_0222CC7C(arg0 + 0x4BC, v0);
        v0 += 1;
        v1 += 4;
    } while (v0 < 0xC);
    MI_CpuCopy8(arg0 + 0x118, arg0 + 0x130, 0x10U);
    ov45_0222E9F8(arg0 + 0x128);
    MI_CpuCopy8(arg0 + 0x108, arg0 + 0x130, 0x10U);
    ov45_0222BAC4(arg0 + 0x108, *(void **)arg0);
}
