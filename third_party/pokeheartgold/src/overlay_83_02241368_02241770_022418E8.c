#include "global.h"

#include "bg_window.h"
#include "options.h"
#include "unk_02005D10.h"
#include "unk_02035900.h"
#include "overlay_83_battle_castle_self_app.h"
struct BattleCastleSelfApp;

void ov83_0224042C(BattleCastleSelfApp *arg0);

u16 ov83_02240EF8(u32 itemID);

void ov83_02241730(struct BattleCastleSelfApp *arg0);

void ov83_02241B18(BattleCastleSelfApp *arg0);

void ov80_02237FA4(void *, u8, u32);

void ov83_02240C6C(void *, u32);

void ov83_02241354(void *);

void ov83_022415F4(struct BattleCastleSelfApp *, u32, s32);

void ov83_022416A0(void *, u32, u16);

void ov83_02241770(void *param_1, Window *param_2);

void ov83_02241A60(BattleCastleSelfApp *, u32);

void ov83_02241ABC(struct BattleCastleSelfApp *, u32);

u32 ov83_0224776C(u32, u32);

void ov83_022477C4(void *, s32 arg1);

void ov83_022477EC(u32, u32, void *);

void ov83_02247944(Window *window, u32);

extern u32 ov83_02247D18;

void ov83_022418E8(BattleCastleSelfApp *param_1, u32 param_2, s32 param_3);

void ov83_022418E8(BattleCastleSelfApp *param_1, u32 param_2, s32 param_3) {
    u16 uVar1;
    u32 uVar3;
    u32 uVar2;
    u32 uVar4;
    u32 unaff_r6;
    u32 uVar5;

    uVar5 = param_2;
    PlaySE(0x5e3);
    uVar2 = (u32) * &param_1->exitSlot;
    uVar3 = ov83_0224776C(uVar2, uVar5);
    switch (param_3) {
    case 1:
    case 2:
    case 3:
        unaff_r6 = (u32) * (u16 *)(((u8 *)&ov83_02247D18) + (param_3 + -1) * 2);
        break;
    case 6:
    case 7:
        uVar1 = ov83_02240EF8(param_1->selectedRentalItem);
        unaff_r6 = (u32)uVar1;
        break;
    case 9:
    case 10:
        unaff_r6 = 0;
    }
    uVar1 = sub_0203769C();
    if (uVar1 == 0) {
        if (uVar5 < uVar2) {
            ov83_02240C6C(param_1, 5);
            ov80_02237FA4(param_1->frontier, param_1->challengeType, unaff_r6);
        } else {
            ov83_022477C4(param_1->strTemplate, 5);
            param_1->partnersCP = param_1->partnersCP - unaff_r6;
        }
    } else if (uVar5 < uVar2) {
        ov83_022477C4(param_1->strTemplate, 5);
        param_1->partnersCP = param_1->partnersCP - unaff_r6;
    } else {
        ov83_02240C6C(param_1, 5);
        ov80_02237FA4(param_1->frontier, param_1->challengeType, unaff_r6);
    }
    ov83_02241770((u8 *)param_1, param_1->windows);
    ov83_02241730(param_1);
    ov83_02241B18(param_1);
    ov83_0224042C(param_1);
    ov83_022477EC(2, 0, &param_1->unk_868);
    ov83_02241354(&param_1->windows[6]);
    switch (param_3) {
    case 1:
    case 2:
    case 3:
        uVar4 = Options_GetFrame(param_1->options);
        ov83_02247944(&param_1->windows[6], uVar4);
        ov83_022415F4(param_1, uVar3, param_3);
        return;
    case 6:
    case 7:
        uVar4 = Options_GetFrame(param_1->options);
        ov83_02247944(&param_1->windows[6], uVar4);
        ov83_022416A0(param_1, uVar3, param_1->selectedRentalItem);
        return;
    case 9:
        ov83_02241A60(param_1, uVar3);
        return;
    case 10:
        ov83_02241ABC(param_1, uVar3);
    }
    return;
}
