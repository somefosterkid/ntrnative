#include "global.h"

s32 ov96_021F4724(void *);

void ov96_021F43EC(void *arg0);

void ov96_021F43EC(void *arg0) {
    u8 v0;

    v0 = 0;
    do {
        *(s8 *)((u8 *)(arg0 + v0) + 0x70) = 0;
        v0 += 1;
    } while ((u32)v0 < 0xCU);
    *(s32 *)((u8 *)arg0 + 0x6C) = 0;
    ov96_021F4724(arg0 + 0x68);
}
