#include "global.h"

#include "bg_window.h"
#include "overlay_86_frontier_records_app.h"
#include "player_avatar.h"
#include "save.h"
#include "save_frontier.h"
#include "unk_02030A98.h"
struct FrontierRecordsApp;

void ov86_021E5FBC(struct FrontierRecordsApp *, u32, s32);

void ov86_021E6024(struct FrontierRecordsApp *, u32, u32, u32, u32, u32, u32, u32);

void ov86_021E6064(struct FrontierRecordsApp *, u32, u32, u32, u32, u32, u32, u32);

u32 ov86_021E6A34(FrontierRecordsApp *param_1);

void ov86_021E690C(FrontierRecordsApp *param_1);

void ov86_021E690C(FrontierRecordsApp *param_1) {
    u8 bVar1;
    u16 uVar2;
    u32 uVar3;

    FillWindowPixelBuffer(param_1->windows, 0);
    FillWindowPixelBuffer(&param_1->windows[1], 0);
    FillWindowPixelBuffer(&param_1->windows[2], 0);
    ov86_021E6024(param_1, 0, 0x19, 0, 0, 0, 0xf0200, 0);
    if (param_1->challengeType == 0) {
        uVar3 = 0x1a;
    } else if (param_1->challengeType == 1) {
        uVar3 = 0x1b;
    } else {
        uVar3 = 0x1c;
    }
    ov86_021E6024(param_1, 0, uVar3, 0xe0, 0, 0, 0xf0200, 1);
    uVar3 = ov86_021E6A34(param_1);
    ov86_021E6024(param_1, 1, uVar3, 0, 0, 0, 0x10200, 0);
    bVar1 = sub_0205C2C0((u32) * &param_1->challengeType);
    uVar2 = FrontierSave_GetStat(param_1->frontierSave, (u32)bVar1, 0xff);
    ov86_021E5FBC(param_1, 0, uVar2);
    ov86_021E6064(param_1, 1, 0x37, 0x70, 0, 0, 0x10200, 2);
    ov86_021E6024(param_1, 2, 0x2b, 0, 0, 0, 0x10200, 0);
    bVar1 = sub_0205C2E8((u32) * &param_1->challengeType);
    uVar2 = FrontierSave_GetStat(param_1->frontierSave, (u32)bVar1, 0xff);
    ov86_021E5FBC(param_1, 0, uVar2);
    ov86_021E6064(param_1, 2, 0x37, 0x70, 0, 0, 0x10200, 2);
    ScheduleWindowCopyToVram(param_1->windows);
    ScheduleWindowCopyToVram(&param_1->windows[1]);
    ScheduleWindowCopyToVram(&param_1->windows[2]);
    return;
}

u32 ov86_021E6A34(FrontierRecordsApp *param_1) {
    u8 uVar1;
    u16 uVar2;
    FrontierSave *pFVar3;
    u32 uVar4;
    void *pvVar5;
    u32 uVar6;

    uVar1 = param_1->challengeType;
    if (uVar1 == 3) {
        pFVar3 = Save_Frontier_GetStatic(param_1->saveData);
        uVar4 = sub_0205C268(0x6e);
        uVar2 = FrontierSave_GetStat(pFVar3, 0x6e, uVar4);
        uVar6 = (u32)uVar2;
    } else {
        pvVar5 = sub_02030FA0(param_1->saveData);
        uVar6 = sub_02030FE4(pvVar5, 8, uVar1, 0, 0);
    }
    if ((uVar6 & 0xff) == 1) {
        return 0x1e;
    }
    return 0x1d;
}
