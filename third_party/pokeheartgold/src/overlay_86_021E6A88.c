#include "global.h"

#include "bg_window.h"
#include "overlay_86_frontier_records_app.h"
#include "player_avatar.h"
#include "save_frontier.h"
#include "unk_02030A98.h"
struct FrontierRecordsApp;

u32 ov86_021E6DD8(struct FrontierRecordsApp *param_1, u32 param_2);

void ov86_021E6A88(FrontierRecordsApp *param_1) {
    u8 bVar1;
    u16 uVar2;
    u32 uVar3;
    FillWindowPixelBuffer(param_1->windows, 0);
    FillWindowPixelBuffer(&param_1->windows[2], 0);
    FillWindowPixelBuffer(&param_1->windows[3], 0);
    ov86_021E6024(param_1, 0, 0x15, 0, 0, FONT_SYSTEM, 0xf0200, 0);
    if (((param_1->challengeType) & 0xFF) == 0) {
        uVar3 = 0x1a;
    } else {
        uVar3 = 0x1b;
    }
    ov86_021E6024(param_1, 0, uVar3, 0xe0, 0, FONT_SYSTEM, 0xf0200, 1);
    uVar3 = ov86_021E6DD8(param_1, (u32)(param_1->challengeType));
    ov86_021E6024(param_1, 2, uVar3, 0, 0, FONT_SYSTEM, 0x10200, 0);
    bVar1 = sub_0205C310((u32)(param_1->challengeType));
    uVar2 = FrontierSave_GetStat(param_1->frontierSave, (u32)bVar1, 0xff);
    ov86_021E5FBC(param_1, 0, uVar2);
    ov86_021E6064(param_1, 2, 0x32, 0x70, 0, FONT_SYSTEM, 0x10200, 2);
    ov86_021E6024(param_1, 3, 0x2b, 0, 0, FONT_SYSTEM, 0x10200, 0);
    bVar1 = sub_0205C350((u32)(param_1->challengeType));
    uVar2 = FrontierSave_GetStat(param_1->frontierSave, (u32)bVar1, 0xff);
    ov86_021E5FBC(param_1, 0, uVar2);
    ov86_021E6064(param_1, 3, 0x32, 0x70, 0, FONT_SYSTEM, 0x10200, 2);
    ScheduleWindowCopyToVram(param_1->windows);
    ScheduleWindowCopyToVram(&param_1->windows[2]);
    ScheduleWindowCopyToVram(&param_1->windows[3]);
    return;
}

void ov86_021E6BA8(FrontierRecordsApp *param_1) {
    u8 bVar1;
    u16 uVar2;
    u32 uVar3;

    FillWindowPixelBuffer(param_1->windows, 0);
    FillWindowPixelBuffer(&param_1->windows[1], 0);
    FillWindowPixelBuffer(&param_1->windows[4], 0);
    FillWindowPixelBuffer(&param_1->windows[5], 0);
    FillWindowPixelBuffer(&param_1->windows[6], 0);
    FillWindowPixelBuffer(&param_1->windows[7], 0);
    FillWindowPixelBuffer(&param_1->windows[8], 0);
    ov86_021E6024(param_1, 0, 0x15, 0, 0, FONT_SYSTEM, 0xf0200, 0);
    ov86_021E6024(param_1, 0, 0x1c, 0xe0, 0, FONT_SYSTEM, 0xf0200, 1);
    ov86_021E6024(param_1, 1, 0x2c, 0, 0, FONT_SYSTEM, 0x10200, 0);
    uVar3 = ov86_021E6DD8(param_1, (u32) * &param_1->challengeType);
    ov86_021E6024(param_1, 4, uVar3, 0, 0, FONT_SYSTEM, 0x10200, 0);
    bVar1 = sub_0205C310((u32) * &param_1->challengeType);
    uVar2 = FrontierSave_GetStat(param_1->frontierSave, (u32)bVar1, 0xff);
    ov86_021E5FBC(param_1, 0, uVar2);
    ov86_021E6064(param_1, 4, 0x32, 0x70, 0, FONT_SYSTEM, 0x10200, 2);
    ov86_021E6024(param_1, 5, 0x2b, 0, 0, FONT_SYSTEM, 0x10200, 0);
    bVar1 = sub_0205C350((u32) * &param_1->challengeType);
    uVar2 = FrontierSave_GetStat(param_1->frontierSave, (u32)bVar1, 0xff);
    ov86_021E5FBC(param_1, 0, uVar2);
    ov86_021E6064(param_1, 5, 0x32, 0x70, 0, FONT_SYSTEM, 0x10200, 2);
    ov86_021E6024(param_1, 6, 0x2d, 0, 0, FONT_SYSTEM, 0x10200, 0);
    uVar3 = ov86_021E6DD8(param_1, 3);
    ov86_021E6024(param_1, 7, uVar3, 0, 0, FONT_SYSTEM, 0x10200, 0);
    bVar1 = sub_0205C310(3);
    uVar2 = FrontierSave_GetStat(param_1->frontierSave, (u32)bVar1, 0xff);
    ov86_021E5FBC(param_1, 0, uVar2);
    ov86_021E6064(param_1, 7, 0x32, 0x70, 0, FONT_SYSTEM, 0x10200, 2);
    ov86_021E6024(param_1, 8, 0x2b, 0, 0, FONT_SYSTEM, 0x10200, 0);
    bVar1 = sub_0205C350(3);
    uVar2 = FrontierSave_GetStat(param_1->frontierSave, (u32)bVar1, 0xff);
    ov86_021E5FBC(param_1, 0, uVar2);
    ov86_021E6064(param_1, 8, 0x32, 0x70, 0, FONT_SYSTEM, 0x10200, 2);
    ScheduleWindowCopyToVram(param_1->windows);
    ScheduleWindowCopyToVram(&param_1->windows[1]);
    ScheduleWindowCopyToVram(&param_1->windows[4]);
    ScheduleWindowCopyToVram(&param_1->windows[5]);
    ScheduleWindowCopyToVram(&param_1->windows[6]);
    ScheduleWindowCopyToVram(&param_1->windows[7]);
    ScheduleWindowCopyToVram(&param_1->windows[8]);
    return;
}
