#include "global.h"

#include "frontier/overlay_80_022340E8.h"

extern s32 ov80_0223C01C;

extern s32 ov80_0223C028;

u32 ov80_02235324(ArcadeContext *ctx) {
    u16 temp_r0;
    u16 temp_r2;
    u8 temp_r3;
    u8 var_r1;

    temp_r3 = ctx->type;
    temp_r2 = ctx->unk1A;
    if ((u32)temp_r3 <= 1U) {
        if ((u32)temp_r2 >= 8U) {
            var_r1 = 6;
        } else {
            var_r1 = *(u8 *)(((u8 *)&ov80_0223C01C) + temp_r2);
        }
    } else if ((u32)temp_r2 >= 8U) {
        var_r1 = 0x11;
    } else {
        var_r1 = *(u8 *)(((u8 *)&ov80_0223C028) + temp_r2);
    }
    if ((temp_r3 == 0) && ((temp_r0 = ctx->winStreak, (temp_r0 == 0x15)) || (temp_r0 == 0x31))) {
        var_r1 = 0x14;
    }
    return (u32)var_r1;
}
