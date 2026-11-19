#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "overlay_86_frontier_records_app.h"
#include "player_avatar.h"
#include "save.h"
#include "save_frontier.h"
#include "unk_02030A98.h"
#include "unk_0205BFF0.h"
struct FrontierRecordsApp;

void ov86_021E5FBC(struct FrontierRecordsApp *, u32, u32);

void ov86_021E6024(struct FrontierRecordsApp *, u32, u32, u32, u32, u32, u32, u32);

void ov86_021E6064(struct FrontierRecordsApp *, u32, u32, u32, u32, u32, u32, u32);

void ov86_021E668C(void *, s32);

void ov86_021E64E0(FrontierRecordsApp *param_1);

void ov86_021E64E0(FrontierRecordsApp *param_1) {
    u16 uVar1;
    FrontierSave *pFVar2;
    s32 iVar3;
    u32 uVar4;
    u32 *a1;
    u32 uVar5;
    u32 auStack_18;

    FillWindowPixelBuffer(param_1->windows, 0);
    FillWindowPixelBuffer(&param_1->windows[1], 0);
    FillWindowPixelBuffer(&param_1->windows[2], 0);
    FillWindowPixelBuffer(&param_1->windows[3], 0);
    ov86_021E6024(param_1, 0, 0x18, 0, 0, 0, 0xf0200, 0);
    if (param_1->challengeType == 0) {
        uVar5 = 0x1a;
    } else if (param_1->challengeType == 1) {
        uVar5 = 0x1b;
    } else {
        uVar5 = 0x1c;
    }
    ov86_021E6024(param_1, 0, uVar5, 0xe0, 0, 0, 0xf0200, 1);
    ov86_021E668C(param_1, param_1->species);
    ov86_021E6064(param_1, 1, 0x33, 0, 0, 0, 0x10200, 0);
    ov86_021E6024(param_1, 2, 0x1d, 0, 0, 0, 0x10200, 0);
    pFVar2 = Save_Frontier_GetStatic(param_1->saveData);
    iVar3 = sub_0205C11C(param_1->challengeType);
    uVar4 = sub_0205C11C(param_1->challengeType);
    uVar4 = sub_0205C268(uVar4);
    uVar1 = FrontierSave_GetStat(pFVar2, iVar3, uVar4);
    if (param_1->species != uVar1) {
        uVar1 = 0;
    } else {
        uVar4 = sub_0205C0CC(param_1->challengeType);
        uVar1 = FrontierSave_GetStat(param_1->frontierSave, uVar4, 0xff);
    }
    ov86_021E5FBC(param_1, 0, uVar1);
    ov86_021E6064(param_1, 2, 0x32, 0x70, 0, 0, 0x10200, 2);
    ov86_021E6024(param_1, 3, 0x2b, 0, 0, 0, 0x10200, 0);
    a1 = sub_020312C4(param_1->saveData, 0xb, &auStack_18);
    if (auStack_18 != 1) {
        uVar4 = 0;
    } else {
        uVar4 = sub_0205C144(param_1->challengeType);
        uVar4 = sub_020312E0(param_1->saveData, a1, uVar4, (u32) * &param_1->species);
    }
    if (a1 != NULL) {
        Heap_Free(a1);
    }
    ov86_021E5FBC(param_1, 0, uVar4);
    ov86_021E6064(param_1, 3, 0x32, 0x70, 0, 0, 0x10200, 2);
    ScheduleWindowCopyToVram(param_1->windows);
    ScheduleWindowCopyToVram(&param_1->windows[1]);
    ScheduleWindowCopyToVram(&param_1->windows[2]);
    ScheduleWindowCopyToVram(&param_1->windows[3]);
    return;
}
