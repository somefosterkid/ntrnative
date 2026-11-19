#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "string_util.h"
#include "overlay_83_battle_castle_self_app.h"

u8 ov83_0223FC48(BattleCastleSelfApp *app, Window *window, int messageID, u32 x, u32 y, u32 renderDelay, u8 textColor, u8 shadowColor, u8 bgColor, u8 fontID);

u8 ov83_0223FD14(BattleCastleSelfApp *param_1, s32 param_2, u8 param_3);

s32 ov83_0223FFD8(BattleCastleSelfApp *, Window *, s32, s32, s32, Pokemon *, s32, s32, s32);
void ov83_0223FF44(BattleCastleSelfApp *arg0, Window *arg1, void *arg2);

u8 ov83_0223FD14(BattleCastleSelfApp *param_1, s32 param_2, u8 param_3) {
    u8 uVar1;

    uVar1 = ov83_0223FC48(param_1, &param_1->windows[6], param_2, 1, 1, 0xff, 1, 2, 0xf, param_3);
    ScheduleWindowCopyToVram(&param_1->windows[6]);
    return uVar1;
}

u8 ov83_0223FF20(BattleCastleSelfApp *param_1, Window *param_2, s32 param_3, u32 param_4, u16 param_5);

void ov83_02240C48(BattleCastleSelfApp *arg0, u32 idx, s32 num, u32 numDigits, enum PrintingMode strconvmode);

void ov83_0223FD4C(BattleCastleSelfApp *param_1, Window *param_2, Pokemon *param_3);

void ov83_0223FD4C(BattleCastleSelfApp *param_1, Window *param_2, Pokemon *param_3) {
    u8 bVar1;
    u32 uVar2;

    FillWindowPixelBuffer(param_2, 0);
    uVar2 = GetMonData(param_3, 6, (void *)0x0);
    BufferItemName(param_1->strTemplate, 0, uVar2);
    ov83_0223FF20(param_1, param_2, 0x46, 1, 8);
    ov83_0223FF20(param_1, param_2, 0x47, 0x40, 8);
    bVar1 = GetMonNature(param_3);
    BufferNatureName(param_1->strTemplate, 0, (u32)bVar1);
    ov83_0223FF20(param_1, param_2, 0x48, 1, 0x18);
    ov83_0223FF20(param_1, param_2, 0x49, 0x40, 0x18);
    uVar2 = GetMonData(param_3, 10, (void *)0x0);
    BufferAbilityName(param_1->strTemplate, 0, uVar2);
    ov83_0223FF20(param_1, param_2, 0x4a, 1, 0x28);
    ov83_0223FF20(param_1, param_2, 0x4b, 0x40, 0x28);
    uVar2 = GetMonData(param_3, 0xa5, (void *)0x0);
    ov83_02240C48(param_1, 0, uVar2, 3, PRINTING_MODE_RIGHT_ALIGN);
    ov83_0223FF20(param_1, param_2, 0x4c, 1, 0x38);
    ov83_0223FF20(param_1, param_2, 0x4d, 0x38, 0x38);
    uVar2 = GetMonData(param_3, 0xa6, (void *)0x0);
    ov83_02240C48(param_1, 0, uVar2, 3, PRINTING_MODE_RIGHT_ALIGN);
    ov83_0223FF20(param_1, param_2, 0x4e, 0x58, 0x38);
    ov83_0223FF20(param_1, param_2, 0x4f, 0x90, 0x38);
    uVar2 = GetMonData(param_3, 0xa8, (void *)0x0);
    ov83_02240C48(param_1, 0, uVar2, 3, PRINTING_MODE_RIGHT_ALIGN);
    ov83_0223FF20(param_1, param_2, 0x50, 1, 0x48);
    ov83_0223FF20(param_1, param_2, 0x51, 0x38, 0x48);
    uVar2 = GetMonData(param_3, 0xa9, (void *)0x0);
    ov83_02240C48(param_1, 0, uVar2, 3, PRINTING_MODE_RIGHT_ALIGN);
    ov83_0223FF20(param_1, param_2, 0x52, 0x58, 0x48);
    ov83_0223FF20(param_1, param_2, 0x53, 0x90, 0x48);
    uVar2 = GetMonData(param_3, 0xa7, (void *)0x0);
    ov83_02240C48(param_1, 0, uVar2, 3, PRINTING_MODE_RIGHT_ALIGN);
    ov83_0223FF20(param_1, param_2, 0x54, 1, 0x58);
    ov83_0223FF20(param_1, param_2, 0x55, 0x38, 0x58);
    ScheduleWindowCopyToVram(param_2);
    return;
}

u8 ov83_0223FCB4(BattleCastleSelfApp *app, Window *window, int entryID, u32 xOffset, u32 yOffset, u32 renderDelay, u8 fgColor, u8 shadowColor, u8 bgColor, u8 font);

u8 ov83_0223FF20(BattleCastleSelfApp *param_1, Window *param_2, s32 param_3, u32 param_4, u16 param_5) {
    u8 uVar1;

    uVar1 = ov83_0223FCB4(param_1, param_2, param_3, param_4, (u32)param_5, 0xff, 1, 2, 0, 0);
    return uVar1;
}

void ov83_0223FF44(BattleCastleSelfApp *arg0, Window *window, void *arg2) {
    FillWindowPixelBuffer(window, 0U);
    ov83_0223FFD8(arg0, window, 0, 0x60, 0x64, arg2, 0x36, 0x3A, 0x42);
    ov83_0223FFD8(arg0, window, 1, 0x61, 0x64, arg2, 0x37, 0x3B, 0x43);
    ov83_0223FFD8(arg0, window, 2, 0x62, 0x64, arg2, 0x38, 0x3C, 0x44);
    ov83_0223FFD8(arg0, window, 3, 0x63, 0x64, arg2, 0x39, 0x3D, 0x45);
    ScheduleWindowCopyToVram(window);
}
