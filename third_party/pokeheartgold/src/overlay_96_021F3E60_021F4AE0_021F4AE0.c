#include "global.h"

#include "gf_gfx_planes.h"

s32 ov96_021F4724(void *);

s32 ov96_021F4E9C(u8, s32 *, s32);

extern s32 ov96_0221C01C;

void ov96_021F4AE0(u8 *arg0);

void ov96_021F4AE0(u8 *arg0) {
    u16 temp_r0;
    void *temp_r4;

    temp_r4 = (void *)(arg0 + 0x68);
    *(u16 *)((u8 *)temp_r4 + 0x1A) = (u16)(*(u16 *)((u8 *)temp_r4 + 0x1A) + 1);
    temp_r0 = *(u16 *)((u8 *)temp_r4 + 0x1A);
    if ((u32)temp_r0 > 0x46U) {
        ov96_021F4724(temp_r4);
        return;
    }
    ov96_021F4E9C((u8)temp_r0, &ov96_0221C01C, *(s32 *)((u8 *)temp_r4 + 0x90));
    *(u8 *)((u8 *)temp_r4 + 0xEA) = (u8)(*(u8 *)((u8 *)temp_r4 + 0xEA) + 0x20);
    reg_G2S_DB_BG2OFS = (*(u8 *)((u8 *)temp_r4 + 0xEA) << 0x10) & 0x01FF0000;
}

s32 ov96_021F4DAC(u32, s32 *, s32);

extern s32 ov96_0221C050;

extern s32 ov96_0221C080;

void ov96_021F4B34(void *arg0);

void ov96_021F4B34(void *arg0) {
    s32 temp_r0_2;
    s32 temp_r1;
    u16 temp_r0;
    u32 temp_r5;
    void *temp_r4;

    temp_r4 = (void *)(arg0 + 0x68);
    *(u16 *)((u8 *)temp_r4 + 0x1A) = (u16)(*(u16 *)((u8 *)temp_r4 + 0x1A) + 1);
    temp_r0 = *(u16 *)((u8 *)temp_r4 + 0x1A);
    if ((u32)temp_r0 > 0x32U) {
        ov96_021F4724(temp_r4);
        return;
    }
    if (temp_r0 == 5) {
        GfGfx_EngineBTogglePlanes(8U, 1U);
    } else if ((u32)temp_r0 > 0xFU) {
        temp_r0_2 = temp_r0 - 0xF;
        if (temp_r0_2 <= 4) {
            temp_r1 = temp_r0_2 * 0x10;
            temp_r5 = (u32)((temp_r1 + ((u32)(temp_r1 >> 1) >> 0x1E)) << 0xE) >> 0x10;
            G2S_SetBlendAlpha(8, 0x34, 0x10 - temp_r5, (s32)temp_r5);
        }
    }
    ov96_021F4DAC((u8) * (u16 *)((u8 *)temp_r4 + 0x1A), &ov96_0221C050, *(s32 *)((u8 *)temp_r4 + 0x90));
    ov96_021F4DAC((u8) * (u16 *)((u8 *)temp_r4 + 0x1A), &ov96_0221C080, *(s32 *)((u8 *)temp_r4 + 0x94));
}
