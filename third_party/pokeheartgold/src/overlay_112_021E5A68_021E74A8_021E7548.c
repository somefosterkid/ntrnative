#include "global.h"

s32 ov112_021E5EC4(u8 *, u8 *, u16);

extern s32 ov112_021FFAA4;

extern s32 ov112_021FFB24;

void ov112_021E7548(void *arg0, void *arg1, void *arg2, void *arg3);

void ov112_021E7548(void *arg0, void *arg1, void *arg2, void *arg3) {
    s32 temp_r2;

    *(void **)((u8 *)(&ov112_021FFAA4) + 8) = arg0;
    *(void **)((u8 *)(&ov112_021FFAA4) + 0x38) = arg1;
    *(void **)((u8 *)(&ov112_021FFAA4) + 0x34) = arg2;
    *(void **)((u8 *)(&ov112_021FFAA4) + 0x30) = arg3;
    *(s8 *)((u8 *)(&ov112_021FFB24) + 0x1C) = 0x40;
    *(s8 *)((u8 *)(&ov112_021FFB24) + 0x1D) = 0x20;
    ov112_021E5EC4(0, 0, 0xD4C);
    *(u16 *)((u8 *)(&ov112_021FFB24) + 0x3C) = 0x2BCAU;
    *(s16 *)((u8 *)(&ov112_021FFB24) + 0x3A) = 0;
    temp_r2 = *(u16 *)((u8 *)(&ov112_021FFB24) + 0x3C) + 0x7F;
    *(s16 *)((u8 *)(&ov112_021FFB24) + 0x38) = (s16)(temp_r2 / 0x80);
}
