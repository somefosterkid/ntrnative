#include "global.h"

#include "overlay_112_ir_state.h"

s32 ov112_021E5A68(s32 *, s32, s32, s32);


void ov112_021E6134(void);

void ov112_021E6134(void) {
    s32 v0;
    u16 v1;
    u16 v2;

    v2 = ov112_021FFB24.unk30;
    if ((u32)v2 > 0x80U) {
        v2 = 0x80;
    }
    v1 = (u16) ((s32)ov112_021FFB24.unk20);
    *(s8 *)(&v0) = (s8)((s32)v1 >> 8);
    *(s8 *)((u8 *)(&v0) + 1) = (s8)v1;
    *(s8 *)((u8 *)(&v0) + 2) = (s8)v2;
    ov112_021E5A68(&v0, 3, 0xC, 2);
}
