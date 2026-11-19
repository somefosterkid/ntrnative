#include "global.h"

#include "options.h"
#include "sys_task.h"
#include "bg_window.h"
#include "constants/heap.h"
#include "list_menu_items.h"
#include "player_avatar.h"
#include "player_data.h"
#include "pm_string.h"
#include "save.h"
#include "save_frontier.h"
#include "unk_02030A98.h"
#include "unk_0205BFF0.h"
#include "overlay_83_battle_castle_self_app.h"
#include "battle_castle_app_sprite.h"
#include "font.h"
#include "string_util.h"

s32 ov83_02242CAC(BattleCastleSelfApp *);

s32 ov83_02242E88(void *);

s32 ov83_02247944(void *, u32);

void ov83_02240384(BattleCastleSelfApp *arg0);

void ov83_02242D5C(BattleCastleSelfApp *arg0);
void ov83_022403B8(void *arg0);

s32 ov83_0223FB6C(void *);
s32 ov83_02240B54(BattleCastleSelfApp *, s32);
void ov83_02241354(Window *window);
void ov83_022477EC(s32 arg0, s32 arg1, s32 *arg2);
void ov83_022403C0(BattleCastleSelfApp *arg0, s32 arg1);

void ov83_0224755C(BattleCastleAppSprite *sprite, BOOL flag);
void ov83_0224759C(BattleCastleAppSprite *obj, int x, int y);
void ov83_02247A18(struct GridInputHandler *inputHandler);
void ov83_0224042C(BattleCastleSelfApp *arg0);
void ov83_02247864(BattleCastleYesNoPrompt *param_1, BgConfig *param_2);
void ov83_02240514(BattleCastleSelfApp *param_1);
void ov83_02240C48(BattleCastleSelfApp *arg0, u32 idx, s32 num, u32 numDigits, enum PrintingMode strconvmode);
void ov83_02241DD8(BattleCastleSelfApp *arg0, Window *window, MsgData *msgData, s32 strno, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
void ov83_02247998(Window *window, String *string, u32 arg2, u32 y, enum Font fontId, u32 color, s32 arg6);
void ov83_022479E4(Window *, MsgData *, s32, u32, u32, u32, u32, s32);
void ov83_02240528(BattleCastleSelfApp *param_1, s32 param_2);

void ov83_02240384(BattleCastleSelfApp *arg0) {
    ov83_02247944(&arg0->windows[6], Options_GetFrame(arg0->options));
    arg0->listSelection = -2;
    ov83_02242CAC(arg0);
    ov83_02242E88(arg0);
}

void ov83_022403B8(void *arg0) {
    ov83_02242D5C(arg0);
}

void ov83_022403C0(BattleCastleSelfApp *arg0, s32 arg1) {
    arg0->itemSelectIsOpen = 1;
    ov83_02241354(&arg0->windows[6]);
    ClearWindowTilemapAndScheduleTransfer(&arg0->windows[6]);
    ClearWindowTilemapAndScheduleTransfer(&arg0->windows[9]);
    ov83_0223FB6C(arg0);
    ov83_022477EC(2, 1, &arg0->unk_868);
    ov83_022477EC(6, 1, &arg0->unk_868);
    ov83_022477EC(7, 1, &arg0->unk_868);
    arg0->unk_860 = 0;
    arg0->page = 0;
    ov83_02240B54(arg0, arg1);
}

void ov83_0224042C(BattleCastleSelfApp *arg0) {
    u32 v0;
    u32 v2;

    if (arg0->itemSelectIsOpen == 1) {
        v0 = 0;
        do {
            ClearWindowTilemapAndScheduleTransfer(&arg0->windows[v0 + 0x30]);
            v0 += 1;
        } while (v0 < 6U);
        ClearWindowTilemapAndScheduleTransfer(&arg0->windows[54]);
        ClearWindowTilemapAndScheduleTransfer(&arg0->windows[55]);
        ClearWindowTilemapAndScheduleTransfer(&arg0->windows[58]);
        ClearWindowTilemapAndScheduleTransfer(&arg0->windows[59]);
        ClearWindowTilemapAndScheduleTransfer(&arg0->windows[60]);
        ov83_022477EC(2, 0, &arg0->unk_868);
        ov83_022477EC(6, 0, &arg0->unk_868);
        ov83_022477EC(7, 0, &arg0->unk_868);
        ov83_02247A18(arg0->gridInput);
        ListMenuItems_Delete(arg0->listItems);
        ov83_0224755C(arg0->itemSelectCursorSprite, 0);
        ov83_0224755C(arg0->subScreenSprite, 0);
        v2 = 0;
        do {
            ov83_0224755C(arg0->listItemSprites[v2], 0);
            v2 += 1;
        } while (v2 < 6U);
        ov83_0224759C(arg0->selectedMonSprite, 0x30, 0x28);
        arg0->itemSelectIsOpen = 0;
    }
}

void ov83_02240514(BattleCastleSelfApp *param_1) {
    ov83_02247864(&param_1->yesNoPrompt, param_1->bgConfig);
    return;
}

void ov83_02240528(BattleCastleSelfApp *param_1, s32 param_2) {
    u8 bVar1;
    u16 uVar2;
    PlayerProfile *profile;
    String *string;
    u16 *src;
    u32 uVar3;
    u32 uVar4;
    u32 uVar5;

    FillWindowPixelBuffer(&param_1->windows[58], 0);
    FillWindowPixelBuffer(&param_1->windows[59], 0);
    FillWindowPixelBuffer(&param_1->windows[60], 0);
    if (param_2 == 6) {
        ov83_022479E4(&param_1->windows[58], param_1->msgLoader, 0x6a, 0, 0, 0, 0x10200, 0);
    } else {
        ov83_022479E4(&param_1->windows[58], param_1->msgLoader, 0x6b, 0, 0, 0, 0x10200, 0);
    }
    profile = Save_PlayerData_GetProfile(param_1->saveData);
    string = String_New(8, HEAP_ID_107);
    src = (unsigned short *)(PlayerProfile_GetNamePtr(profile));
    CopyU16ArrayToString(string, src);
    uVar3 = PlayerProfile_GetTrainerGender(profile);
    if (uVar3 == 0) {
        uVar4 = 0x70800;
    } else {
        uVar4 = 0x30400;
    }
    ov83_02247998(&param_1->windows[59], string, 0, 0, FONT_SYSTEM, uVar4, 0);
    String_Delete(string);
    uVar3 = sub_0205C1F0(param_1->challengeType);
    uVar5 = sub_0205C1F0(param_1->challengeType);
    uVar5 = sub_0205C268(uVar5);
    uVar2 = FrontierSave_GetStat(param_1->frontier, uVar3, uVar5);
    ov83_02240C48(param_1, 0, uVar2, 4, PRINTING_MODE_RIGHT_ALIGN);
    bVar1 = GetWindowWidth(&param_1->windows[60]);
    ov83_02241DD8(param_1, &param_1->windows[60], param_1->msgLoader, 2, (u32)bVar1 << 3, 0, 0, 0x10200, 1);
    ScheduleWindowCopyToVram(&param_1->windows[58]);
    ScheduleWindowCopyToVram(&param_1->windows[59]);
    ScheduleWindowCopyToVram(&param_1->windows[60]);
    return;
}
