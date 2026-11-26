#include "global.h"
#include "overlay_83_battle_castle_opponent_app.h"
#include "list_menu_items.h"
#include "touchscreen_list_menu.h"

void ov83_022447E0(BattleCastleOpponentApp *app, Window *window, u32, u32, u32, u32, u8, u8, u8, u8);

void ov83_02246CC0(BattleCastleOpponentApp *param_1);

void ov83_02246CC0(BattleCastleOpponentApp *param_1) {
    u32 param_3;
    u32 uVar1;
    uVar1 = param_1->touchMenu->cursorPos;
    if (param_1->listSelection != uVar1) {
        uVar1 = param_1->listMenuItems[uVar1].value;
        switch (uVar1) {
        case 0:
            param_3 = 0xc;
            break;
        case 1:
            param_3 = 0xd;
            break;
        case 2:
            param_3 = 0xe;
            break;
        case 0xfffffffe:
            param_3 = 0xf;
            break;
        }
        ov83_022447E0(param_1, &param_1->windows[7], param_3, 1, 1, 0xff, 1, 2, 0xf, 1);
        param_1->listSelection = param_1->touchMenu->cursorPos;
    }
}

u8 ov83_0224777C(SaveData *, u8, u8);

extern u32 ov83_02248024;

extern u32 ov83_02248028;

extern u32 ov83_0224802C;

extern u32 ov83_02248030;

void ov83_02246D40(BattleCastleOpponentApp *param_1);

void ov83_02246D40(BattleCastleOpponentApp *param_1) {
    s32 iVar1;
    u32 uVar2;
    u32 uVar3;
    u32 unaff_r4;
    if (param_1->listSelection != param_1->touchMenu->cursorPos) {
        iVar1 = ov83_0224777C(param_1->saveData, param_1->challengeType, 2);
        uVar2 = (u32)(iVar1 != 1);
        uVar3 = param_1->listMenuItems[param_1->touchMenu->cursorPos].value;
        switch (uVar3) {
        case 3:
            unaff_r4 = *((u16 *)(((u8 *)(&ov83_02248024)) + (uVar2 * 2)));
            break;
        case 4:
            unaff_r4 = (u32)(*((u16 *)(((u8 *)(&ov83_02248028)) + (uVar2 * 2))));
            break;
        case 5:
            unaff_r4 = (u32)(*((u16 *)(((u8 *)(&ov83_0224802C)) + (uVar2 * 2))));
            break;
        case 0xfffffffe:
            unaff_r4 = (u32)(*((u16 *)(((u8 *)(&ov83_02248030)) + (uVar2 * 2))));
            break;
        }
        ov83_022447E0(param_1, &param_1->windows[7], unaff_r4, 1, 1, 0xff, 1, 2, 0xf, 1);
        param_1->listSelection = param_1->touchMenu->cursorPos;
    }
    return;
}
