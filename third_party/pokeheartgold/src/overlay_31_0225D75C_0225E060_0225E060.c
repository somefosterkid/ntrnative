#include "global.h"

#include "bg_window.h"

extern s32 ov31_0225EF48;

void ov31_0225E060(BgConfig *arg0, s32 arg1, s32 arg2);

void ov31_0225E060(BgConfig *arg0, s32 arg1, s32 arg2) {
    void *sp14;
    s32 sp10;
    s32 var_r7;
    u8 temp_r0;
    u8 temp_r4;
    void *var_r5;

    sp10 = arg1;
    sp14 = GetBgTilemapBuffer(arg0, (u8)arg1);
    if (arg2 != 6) {
        var_r5 = ((u8 *)&ov31_0225EF48) + (arg2 << 5);
        var_r7 = 0;
        temp_r4 = (u8)sp10;
        do {
            temp_r0 = *(u8 *)var_r5;
            if (temp_r0 == 1) {
                LoadRectToBgTilemapRect(arg0, temp_r4, sp14 + ((*(u8 *)((u8 *)var_r5 + 1) + (*(u8 *)((u8 *)var_r5 + 2) << 5)) * 2), *(u8 *)((u8 *)var_r5 + 3), (u8)(s32) * (u8 *)((u8 *)var_r5 + 4), (u8)(s32) * (u8 *)((u8 *)var_r5 + 5), (u8)(s32) * (u8 *)((u8 *)var_r5 + 6));
            } else if (temp_r0 == 2) {
                FillBgTilemapRect(arg0, temp_r4, 0U, *(u8 *)((u8 *)var_r5 + 3), (u8)(s32) * (u8 *)((u8 *)var_r5 + 4), (u8)(s32) * (u8 *)((u8 *)var_r5 + 5), (u8)(s32) * (u8 *)((u8 *)var_r5 + 6), 0U);
            }
            var_r7 += 1;
            var_r5 += 8;
        } while (var_r7 < 4);
    }
}
