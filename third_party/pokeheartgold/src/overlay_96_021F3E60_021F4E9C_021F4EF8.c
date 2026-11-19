#include "global.h"

#include "bg_window.h"

void ov96_021F4EF8(u8 *arg0, BgConfig *arg1);

void ov96_021F4EF8(u8 *arg0, BgConfig *arg1) {
    u8 sp1C[5];
    BgConfig *var_r6;
    u8 *var_r5;
    u8 temp_r0;
    u8 var_r4;
    u8 var_r7;

    var_r5 = arg0;
    var_r7 = *(u8 *)((u8 *)arg0 + 0xE8);
    var_r6 = arg1;
    var_r4 = (u8)((u8)var_r7 / 100);
    *(s8 *)((u8 *)(sp1C) + 3) = (s8)(((u8)var_r4 % 5) * 4);
    *(s8 *)(sp1C) = (s8)(((u8)var_r4 / 5) * 8);
    var_r4 = ((u8)var_r7 / 10) - (0xA * var_r4);
    *(s8 *)((u8 *)(sp1C) + 4) = (s8)(((u8)var_r4 % 5) * 4);
    *(s8 *)((u8 *)(sp1C) + 1) = (s8)(((u8)var_r4 / 5) * 8);
    var_r4 = (u8)((u8)var_r7 % 10);
    *(s8 *)((u8 *)(sp1C) + 5) = (s8)(((u8)var_r4 % 5) * 4);
    *(s8 *)((u8 *)(sp1C) + 2) = (s8)(((u8)var_r4 / 5) * 8);
    var_r4 = 0;
    var_r7 = 4;
    do {
        CopyRectToBgTilemapRect(var_r6, 6U, (u8)((var_r4 * 4) + 3), 7U, 4U, 8U, (const void *)(*(u8 **)((u8 *)var_r5 + 0xB0) + 0xC), (u8)(u8) * (u8 *)(sp1C + 3 + var_r4), (u8)(u8) * (u8 *)(sp1C + var_r4), 0x14U, 0x10U);
        var_r4 += 1;
    } while ((u32)var_r4 < 3U);
    temp_r0 = *(u8 *)((u8 *)var_r5 + 0xE8);
    var_r5 += 0xE9;
    *var_r5 = temp_r0;
}
