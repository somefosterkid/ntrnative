#include "global.h"

#include "bg_window.h"
#include "overlay_96_app_0221040c.h"

void ov96_0221058C(Ov96App_0221040C *arg0, s32 arg1) {
    u8 sp1C[5];
    u8 temp_r4;
    u8 temp_r4_2;
    u8 temp_r4_3;
    u8 var_r4;

    temp_r4 = (u8)(arg1 / 100);
    *(s8 *)((u8 *)(sp1C) + 3) = (s8)(((s32)temp_r4 % 5) * 4);
    *(s8 *)(sp1C) = (s8)(((s32)temp_r4 / 5) * 8);
    temp_r4_2 = (u8)((s32)(arg1 - (0x64 * temp_r4)) / 10);
    *(s8 *)((u8 *)(sp1C) + 4) = (s8)(((s32)temp_r4_2 % 5) * 4);
    *(s8 *)((u8 *)(sp1C) + 1) = (s8)(((s32)temp_r4_2 / 5) * 8);
    temp_r4_3 = (u8)(arg1 % 10);
    *(s8 *)((u8 *)(sp1C) + 5) = (s8)(((s32)temp_r4_3 % 5) * 4);
    *(s8 *)((u8 *)(sp1C) + 2) = (s8)(((s32)temp_r4_3 / 5) * 8);
    var_r4 = 0;
    do {
        CopyRectToBgTilemapRect(arg0->bgConfig, 6U, (u8)(var_r4 * 4), 0U, 4U, 8U, arg0->scrnData->rawData, (u8)(s32) * (u8 *)(sp1C + 3 + var_r4), (u8)(s32) * (u8 *)(sp1C + var_r4), 0x14U, 0x10U);
        var_r4 += 1;
    } while ((u32)var_r4 < 3U);
    ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 6U);
}
