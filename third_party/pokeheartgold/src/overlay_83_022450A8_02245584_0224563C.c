#include "global.h"

#include "unk_02005D10.h"
#include "unk_02035900.h"
#include "overlay_83_battle_castle_opponent_app.h"

void ov83_02245390(BattleCastleOpponentApp *arg0);

void ov83_02245824(BattleCastleOpponentApp *arg0, s32 arg1);

void ov80_02237FA4(struct FrontierSave *frontier, u8 challengeType, int castlePoints);

void ov83_02244ABC(BattleCastleOpponentApp *arg0, u32);

u8 ov83_02245068(u8 adjustment);

void ov83_02245288(BattleCastleOpponentApp *arg0, u32);

void ov83_02245318(BattleCastleOpponentApp *arg0, u32);

void ov83_022453DC(BattleCastleOpponentApp *arg0, Window *arg1);

void ov83_02245838(BattleCastleOpponentApp *param_1, u32, s32);

u32 ov83_0224776C(u32 param_1, u32 param_2);

void ov83_022477C4(struct MessageFormat *template, u32 idx);

void ov83_0224563C(BattleCastleOpponentApp *param_1, u32 param_2, u32 param_3);

void ov83_0224563C(BattleCastleOpponentApp *param_1, u32 param_2, u32 param_3) {
    u16 uVar1;
    s32 uVar2;
    u32 uVar3;
    u8 unaff_r5;

    PlaySE(0x5e3);
    uVar2 = param_1->exitSlot;
    uVar3 = ov83_0224776C(uVar2, param_2);
    switch (param_3) {
    case 0:
        unaff_r5 = 1;
        break;
    case 1:
        unaff_r5 = ov83_02245068(param_1->selectedLevelAdjustment);
        break;
    case 3:
        unaff_r5 = 2;
        break;
    case 4:
        unaff_r5 = 5;
    }
    uVar1 = sub_0203769C();
    if (uVar1 == 0) {
        if (param_2 < uVar2) {
            ov83_02244ABC(param_1, 5);
            ov80_02237FA4(param_1->frontier, param_1->challengeType, unaff_r5);
        } else {
            ov83_022477C4(param_1->strTemplate, 5);
            param_1->partnersCP -= unaff_r5;
        }
    } else if (param_2 < uVar2) {
        ov83_022477C4(param_1->strTemplate, 5);
        param_1->partnersCP -= unaff_r5;
    } else {
        ov83_02244ABC(param_1, 5);
        ov80_02237FA4(param_1->frontier, param_1->challengeType, unaff_r5);
    }
    ov83_022453DC(param_1, &param_1->windows[0]);
    ov83_02245390(param_1);
    switch (param_3) {
    case 0:
        ov83_02245824(param_1, uVar3);
        return;
    case 1:
        ov83_02245838(param_1, uVar3, param_1->selectedLevelAdjustment);
        return;
    case 3:
        ov83_02245288(param_1, uVar3);
        return;
    case 4:
        ov83_02245318(param_1, uVar3);
    }
    return;
}
