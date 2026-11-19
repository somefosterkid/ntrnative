#include "global.h"

#include "bg_window.h"

extern s32 ov43_0222EFA0;

void ov43_0222C890(void *arg0, BgConfig **arg1, s32 arg2, s32 arg3);

void ov43_0222C890(void *arg0, BgConfig **arg1, s32 arg2, s32 arg3) {
    s32 sp14;
    s32 sp10;
    BgConfig **spC;
    s32 *var_r5;
    u32 var_r2;
    u32 var_r3;
    s32 var_r4;
    u8 var_r0;

    var_r5 = &ov43_0222EFA0;
    spC = arg1;
    sp10 = arg2;
    sp14 = arg3;
    var_r4 = 0;
    do {
        if ((sp14 == 1) && (sp10 == var_r4)) {
            var_r0 = *(u8 *)((u8 *)var_r5 + 8);
        } else {
            var_r0 = *(u8 *)((u8 *)var_r5 + 7);
        }
        var_r3 = *(s16 *)((u8 *)arg0 + 2);
        if ((s32)var_r3 < 0) {
            var_r3 = 0 - var_r3;
        }
        var_r2 = *(s16 *)arg0;
        if ((s32)var_r2 < 0) {
            var_r2 = 0 - var_r2;
        }
        BgTilemapRectChangePalette(*spC, *(u8 *)((u8 *)var_r5 + 6), (u8)(*(s16 *)var_r5 + var_r2), (u8)(*(s16 *)((u8 *)var_r5 + 2) + var_r3), (u8)(s32) * (u8 *)((u8 *)var_r5 + 4), (u8)(s32) * (u8 *)((u8 *)var_r5 + 5), (u8)(s32)var_r0);
        var_r4 += 1;
        var_r5 = (void *)((u8 *)var_r5 + (0xA));
    } while (var_r4 < 9);
    ScheduleBgTilemapBufferTransfer(*spC, 2U);
    ScheduleBgTilemapBufferTransfer(*spC, 0U);
}
