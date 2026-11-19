#include "global.h"

#include "bg_window.h"
#include "overlay_82_battle_hall_app.h"

s32 ov82_0223E140(BgConfig *, s32, s32, u8, s32, s32, s32, s32);

s32 ov82_0223EECC(void *, s32);

s32 ov82_0223F1AC(void *, void *, s32, s32, u8, u8, u8, u8);

void ov82_0223E0B0(void *arg0);

void ov82_0223E0B0(void *arg0) {
    s32 sp10;
    u8 sp14;
    u8 temp_r7;
    u8 temp_r6;
    u8 temp_r4;

    sp10 = (s32)(u8)(GetWindowX(arg0 + 0x7C) - 1);
    temp_r7 = GetWindowY(arg0 + 0x7C) - 1;
    temp_r6 = GetWindowWidth(arg0 + 0x7C) + 2;
    temp_r4 = GetWindowHeight(arg0 + 0x7C) + 2;
    sp14 = GetWindowBgId(arg0 + 0x7C);
    ov82_0223F1AC(arg0, (u8 *)(arg0 + 0x7C), (s32)(0), (s32)(0), (s32)(1), (s32)(2), (s32)(0xF), (s32)(0));
    ov82_0223EECC(arg0, 4);
    ov82_0223E140(((BattleHallApp *)arg0)->bgConfig, sp14, sp10, temp_r7, (s32)temp_r6, (s32)temp_r4, 1, 1);
    ScheduleBgTilemapBufferTransfer(((BattleHallApp *)arg0)->bgConfig, (u8)sp14);
}
