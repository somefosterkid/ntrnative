#include "global.h"

#include "bg_window.h"
#include "overlay_86_frontier_records_app.h"
#include "player_avatar.h"
#include "save.h"
#include "save_frontier.h"
#include "unk_02030A98.h"
#include "unk_0205BFF0.h"
struct FrontierRecordsApp;

u32 ov86_021E68B8(FrontierRecordsApp *param_1);

s32 sub_0205C1C8(u8);

s32 sub_0205C240(u8);

void ov86_021E66D0(FrontierRecordsApp *param_1) {
    u16 uVar1;
    u32 uVar2;
    FrontierSave *pFVar3;
    u32 uVar4;
    s32 iVar5;
    int new_var;
    FillWindowPixelBuffer(param_1->windows, 0);
    FillWindowPixelBuffer(&param_1->windows[1], 0);
    FillWindowPixelBuffer(&param_1->windows[2], 0);
    FillWindowPixelBuffer(&param_1->windows[3], 0);
    ov86_021E6024(param_1, 0, 0x17, 0, 0, FONT_SYSTEM, 0xf0200, 0);
    new_var = 0xf0200;
    if ((param_1->challengeType) == 0) {
        new_var = 0x1a;
    } else if ((param_1->challengeType) == 1) {
        new_var = 0x1b;
    } else {
        new_var = 0x1c;
    }
    ov86_021E6024(param_1, 0, new_var, 0xe0, 0, FONT_SYSTEM, 0xf0200, 1);
    ov86_021E6024(param_1, 1, 0x31, 0x70, 0, FONT_SYSTEM, 0x10200, 1);
    new_var = ov86_021E68B8(param_1);
    ov86_021E6024(param_1, 2, new_var, 0, 0, FONT_SYSTEM, 0x10200, 0);
    uVar2 = sub_0205C1A0(param_1->challengeType);
    uVar1 = FrontierSave_GetStat(param_1->frontierSave, uVar2, 0xff);
    ov86_021E5FBC(param_1, 0, uVar1);
    ov86_021E6064(param_1, 2, 0x36, 0x70, 0, FONT_SYSTEM, 0x10200, 2);
    pFVar3 = Save_Frontier_GetStatic(param_1->saveData);
    uVar2 = sub_0205C1F0(param_1->challengeType);
    uVar4 = sub_0205C1F0(param_1->challengeType);
    uVar4 = sub_0205C268(uVar4);
    uVar1 = FrontierSave_GetStat(pFVar3, uVar2, uVar4);
    ov86_021E5FBC(param_1, 0, uVar1);
    ov86_021E6064(param_1, 2, 0x35, 0xe0, 0, FONT_SYSTEM, 0x10200, 1);
    ov86_021E6024(param_1, 3, 0x2b, 0, 0, FONT_SYSTEM, 0x10200, 0);
    iVar5 = sub_0205C1C8(param_1->challengeType);
    uVar1 = FrontierSave_GetStat(param_1->frontierSave, iVar5, 0xff);
    ov86_021E5FBC(param_1, 0, uVar1);
    ov86_021E6064(param_1, 3, 0x36, 0x70, 0, FONT_SYSTEM, 0x10200, 2);
    pFVar3 = Save_Frontier_GetStatic(param_1->saveData);
    iVar5 = sub_0205C240(param_1->challengeType);
    uVar2 = sub_0205C240(param_1->challengeType);
    uVar2 = sub_0205C268(uVar2);
    uVar1 = FrontierSave_GetStat(pFVar3, iVar5, uVar2);
    ov86_021E5FBC(param_1, 0, uVar1);
    ov86_021E6064(param_1, 3, 0x35, 0xe0, 0, FONT_SYSTEM, 0x10200, 1);
    ScheduleWindowCopyToVram(param_1->windows);
    ScheduleWindowCopyToVram(&param_1->windows[1]);
    ScheduleWindowCopyToVram(&param_1->windows[2]);
    ScheduleWindowCopyToVram(&param_1->windows[3]);
    return;
}
