#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "overlay_86_frontier_records_app.h"
#include "save_frontier.h"
#include "unk_02030A98.h"
#include "unk_0205BFF0.h"
struct FrontierRecordsApp;

s32 ov86_021E6484(void *, u32);

s32 sub_0205C01C(u32, u32);

s32 sub_0205C074(u32, u32);

void ov86_021E6114(FrontierRecordsApp *param_1) {
    u16 uVar1;
    s32 iVar2;
    u32 sVar3;

    FillWindowPixelBuffer(param_1->windows, 0);
    FillWindowPixelBuffer(&param_1->windows[1], 0);
    FillWindowPixelBuffer(&param_1->windows[2], 0);
    FillWindowPixelBuffer(&param_1->windows[3], 0);
    ov86_021E6024(param_1, 0, 0x16, 0, 0, FONT_SYSTEM, 0xf0200, 0);
    if (param_1->challengeType == 0) {
        sVar3 = 0x1a;
    } else if (param_1->challengeType == 1) {
        sVar3 = 0x1b;
    } else {
        sVar3 = 0x1c;
    }
    ov86_021E6024(param_1, 0, sVar3, 0xe0, 0, FONT_SYSTEM, 0xf0200, 1);
    ov86_021E6024(param_1, 1, 0x2e, 0, 0, FONT_SYSTEM, 0x10200, 0);
    ov86_021E6024(param_1, 1, 0x30, 0xe0, 0, FONT_SYSTEM, 0x10200, 1);
    sVar3 = ov86_021E6484(param_1, 0);
    ov86_021E6024(param_1, 2, sVar3, 0, 0, FONT_SYSTEM, 0x10200, 0);
    iVar2 = sub_0205BFF0(0, param_1->challengeType);
    uVar1 = FrontierSave_GetStat(param_1->frontierSave, iVar2, 0xff);
    ov86_021E5FBC(param_1, 0, (u32)uVar1);
    ov86_021E6064(param_1, 2, 0x32, 0x70, 0, FONT_SYSTEM, 0x10200, 2);
    iVar2 = sub_0205C048(0, param_1->challengeType);
    uVar1 = FrontierSave_GetStat(param_1->frontierSave, iVar2, 0xff);
    ov86_021E5FBC(param_1, 0, (u32)uVar1);
    ov86_021E6064(param_1, 2, 0x34, 0xe0, 0, FONT_SYSTEM, 0x10200, 1);
    ov86_021E6024(param_1, 3, 0x2b, 0, 0, FONT_SYSTEM, 0x10200, 0);
    iVar2 = sub_0205C01C(0, param_1->challengeType);
    uVar1 = FrontierSave_GetStat(param_1->frontierSave, iVar2, 0xff);
    ov86_021E5FBC(param_1, 0, (u32)uVar1);
    ov86_021E6064(param_1, 3, 0x32, 0x70, 0, FONT_SYSTEM, 0x10200, 2);
    iVar2 = sub_0205C074(0, param_1->challengeType);
    uVar1 = FrontierSave_GetStat(param_1->frontierSave, iVar2, 0xff);
    ov86_021E5FBC(param_1, 0, (u32)uVar1);
    ov86_021E6064(param_1, 3, 0x34, 0xe0, 0, FONT_SYSTEM, 0x10200, 1);
    ScheduleWindowCopyToVram(param_1->windows);
    ScheduleWindowCopyToVram(&param_1->windows[1]);
    ScheduleWindowCopyToVram(&param_1->windows[2]);
    ScheduleWindowCopyToVram(&param_1->windows[3]);
    return;
}

void ov86_021E62F4(FrontierRecordsApp *param_1) {
    u16 uVar1;
    s32 sVar2;
    s32 iVar3;

    FillWindowPixelBuffer(&param_1->windows[4], 0);
    FillWindowPixelBuffer(&param_1->windows[5], 0);
    FillWindowPixelBuffer(&param_1->windows[6], 0);
    ov86_021E6024(param_1, 4, 0x2f, 0, 0, FONT_SYSTEM, 0x10200, 0);
    ov86_021E6024(param_1, 4, 0x30, 0xe0, 0, FONT_SYSTEM, 0x10200, 1);
    sVar2 = ov86_021E6484(param_1, 1);
    ov86_021E6024(param_1, 5, sVar2, 0, 0, FONT_SYSTEM, 0x10200, 0);
    iVar3 = sub_0205BFF0(1, param_1->challengeType);
    uVar1 = FrontierSave_GetStat(param_1->frontierSave, iVar3, 0xff);
    ov86_021E5FBC(param_1, 0, (u32)uVar1);
    ov86_021E6064(param_1, 5, 0x32, 0x70, 0, FONT_SYSTEM, 0x10200, 2);
    iVar3 = sub_0205C048(1, param_1->challengeType);
    uVar1 = FrontierSave_GetStat(param_1->frontierSave, iVar3, 0xff);
    ov86_021E5FBC(param_1, 0, (u32)uVar1);
    ov86_021E6064(param_1, 5, 0x34, 0xe0, 0, FONT_SYSTEM, 0x10200, 1);
    ov86_021E6024(param_1, 6, 0x2b, 0, 0, FONT_SYSTEM, 0x10200, 0);
    iVar3 = sub_0205C01C(1, param_1->challengeType);
    uVar1 = FrontierSave_GetStat(param_1->frontierSave, iVar3, 0xff);
    ov86_021E5FBC(param_1, 0, (u32)uVar1);
    ov86_021E6064(param_1, 6, 0x32, 0x70, 0, FONT_SYSTEM, 0x10200, 2);
    iVar3 = sub_0205C074(1, param_1->challengeType);
    uVar1 = FrontierSave_GetStat(param_1->frontierSave, iVar3, 0xff);
    ov86_021E5FBC(param_1, 0, (u32)uVar1);
    ov86_021E6064(param_1, 6, 0x34, 0xe0, 0, FONT_SYSTEM, 0x10200, 1);
    ScheduleWindowCopyToVram(&param_1->windows[4]);
    ScheduleWindowCopyToVram(&param_1->windows[5]);
    ScheduleWindowCopyToVram(&param_1->windows[6]);
    return;
}
