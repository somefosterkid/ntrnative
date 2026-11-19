#include "global.h"

#include "system.h"
#include "unk_02005D10.h"
#include "battle/battle_bag_context.h"
#include "overlay_08_battle_bag.h"
#include "overlay_08_battle_sub_menu_cursor.h"

void ov08_0222417C(BattleBag *battleBag);

u8 ov08_02224B88(BattleSubMenuCursor *cursor);

s32 ov08_02223368(BattleBag *, u8 *, u32, u32);

u16 ov08_02223CD4(BattleBag *, s32);

void ov08_02224938(BattleBag *, u8, u8);

s32 ov08_02224B8C(BattleSubMenuCursor *);

s32 ov08_02224C94(BattleSubMenuCursor *);

extern u32 ov08_02225B68;

u32 ov08_02222918(BattleBag *battleBag, u32 param_2, u32 param_3, u32 param_4);

u32 ov08_02222918(BattleBag *battleBag, u32 param_2, u32 param_3, u32 param_4) {
    u8 uVar1;
    u8 uVar2;
    s32 iVar3;
    s32 iVar4;

    iVar3 = ov08_02223368(battleBag, (u8 *)(&ov08_02225B68), param_3, param_4);
    if (iVar3 == -1) {
        iVar4 = ov08_02224B8C(battleBag->cursor);
        uVar1 = ov08_02224B88(battleBag->cursor);
        iVar3 = ov08_02224C94(battleBag->cursor);
        uVar2 = ov08_02224B88(battleBag->cursor);
        if (iVar3 == -2) {
            iVar3 = 6;
        } else if ((iVar4 == 1) && (uVar1 == uVar2)) {
            if (((gSystem.newAndRepeatedKeys & 0x20) != 0) && ((uVar2 == 0 || (uVar2 == 2)) || (uVar2 == 4))) {
                iVar3 = 7;
            }
            if (((gSystem.newAndRepeatedKeys & 0x10) != 0) && ((uVar2 == 1 || (uVar2 == 3)) || (uVar2 == 5))) {
                iVar3 = 8;
            }
        }
    } else {
        ov08_0222417C(battleBag);
    }
    switch (iVar3) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        iVar4 = ov08_02223CD4(battleBag, iVar3);
        if (iVar4 != 0) {
            PlaySE(0x5dd);
            battleBag->context->pocketCurrentPagePositions[battleBag->currentBattlePocket] = (s8)iVar3;
            battleBag->queuedState = 6;
            ov08_02224938(battleBag, iVar3 + 6U & 0xff, 0);
            return 0xb;
        }
        break;
    case 6:
        PlaySE(0x5dd);
        battleBag->queuedState = 4;
        ov08_02224938(battleBag, 0xe, 0);
        return 0xb;
    case 7:
        if (battleBag->numBattlePocketPages[battleBag->currentBattlePocket] != 0) {
            PlaySE(0x5dd);
            battleBag->queuedState = 7;
            battleBag->queuedBattlePocketPageChange = -1;
            ov08_02224938(battleBag, 0xc, 0);
            return 0xb;
        }
        break;
    case 8:
        if (battleBag->numBattlePocketPages[battleBag->currentBattlePocket] != 0) {
            PlaySE(0x5dd);
            battleBag->queuedState = 7;
            battleBag->queuedBattlePocketPageChange = 1;
            ov08_02224938(battleBag, 0xd, 0);
            return 0xb;
        }
    }
    return 2;
}
