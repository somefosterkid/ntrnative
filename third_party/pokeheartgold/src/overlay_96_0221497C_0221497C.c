#include "global.h"

#include "bg_window.h"

#include "overlay_96_ov96_course_hud.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov96_0221497C(Ov96CourseHud *hud, s32 arg1) {
    u8 sp1C[4];
    u8 temp_r4;
    u8 temp_r4_2;
    u8 var_r4;

    temp_r4 = (u8)(arg1 / 10);
    sp1C[2] = (s8)(((s32)temp_r4 % 5) * 4);
    sp1C[0] = (s8)(((s32)temp_r4 / 5) * 8);
    temp_r4_2 = (u8)(arg1 % 10);
    sp1C[3] = (s8)(((s32)temp_r4_2 % 5) * 4);
    sp1C[1] = (s8)(((s32)temp_r4_2 / 5) * 8);
    var_r4 = 0;
    do {
        CopyRectToBgTilemapRect(hud->bgConfig, 6U, (u8)((var_r4 * 4) + 6), 0xAU, 4U, 8U, (const void *)hud->scrnData->rawData, (u8)(s32)sp1C[2 + var_r4], (u8)(s32)sp1C[var_r4], 0x14U, 0x10U);
        var_r4 += 1;
    } while ((u32)var_r4 < 2U);
    ScheduleBgTilemapBufferTransfer(hud->bgConfig, 6U);
}
#else
void ov96_0221497C(Ov96CourseHud *hud, s32 arg1) {
    u8 sp1C;
    u8 temp_r4;
    u8 temp_r4_2;
    u8 var_r4;

    temp_r4 = (u8)(arg1 / 10);
    *(s8 *)((u8 *)(&sp1C) + 2) = (s8)(((s32)temp_r4 % 5) * 4);
    *(s8 *)(&sp1C) = (s8)(((s32)temp_r4 / 5) * 8);
    temp_r4_2 = (u8)(arg1 % 10);
    *(s8 *)((u8 *)(&sp1C) + 3) = (s8)(((s32)temp_r4_2 % 5) * 4);
    *(s8 *)((u8 *)(&sp1C) + 1) = (s8)(((s32)temp_r4_2 / 5) * 8);
    var_r4 = 0;
    do {
        CopyRectToBgTilemapRect(hud->bgConfig, 6U, (u8)((var_r4 * 4) + 6), 0xAU, 4U, 8U, (const void *)hud->scrnData->rawData, (u8)(s32) * (u8 *)(&sp1C + 2 + var_r4), (u8)(s32) * (u8 *)(&sp1C + var_r4), 0x14U, 0x10U);
        var_r4 += 1;
    } while ((u32)var_r4 < 2U);
    ScheduleBgTilemapBufferTransfer(hud->bgConfig, 6U);
}
#endif
