// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov167/battle_action.c, which is the same code
// in Black 2/White 2's overlay 167. The named functions have swan's names, the rest Black's dsd names.
#include "battle/btl_action.h"

void BattleAction_SetFightParam(BattleAction *action, u16 move, u8 target) {
    action->raw = 0;
    action->bits.action = 1;
    action->bits.target = target;
    action->bits.move = move;
}

void BattleAction_ChangeFightTargetPos(BattleAction *action, u8 target) {
    if (action->bits.action == 1 && target != 6) {
        action->bits.target = target;
    }
}

void func_ov093_021d8318(BattleAction *action) {
    if (action->bits.action == 1) {
        action->bits.unk26 = 1;
    }
}

BOOL func_ov093_021d832c(const BattleAction *action) {
    if (action->bits.action == 1) {
        return action->bits.unk26;
    }
    return FALSE;
}

u16 func_ov093_021d8340(const BattleAction *action) {
    if (action->bits.action == 1) {
        return action->bits.move;
    }
    return 0;
}

void func_ov093_021d8358(BattleAction *action, u16 item, u8 target, u8 param) {
    action->raw = 0;
    action->item.action = 2;
    action->item.item = item;
    action->item.target = target;
    action->item.param = param;
}

void func_ov093_021d8394(BattleAction *action, u8 pos, u8 slot) {
    action->raw = 0;
    action->change.action = 3;
    action->change.unk4 = pos;
    action->change.slot = slot;
    action->change.unk10 = 0;
}

void func_ov093_021d83c4(BattleAction *action) {
    action->raw = 0;
    action->change.action = 3;
    action->change.slot = 0;
    action->change.unk10 = 1;
}

BOOL func_ov093_021d83e0(const BattleAction *action) {
    if (action->change.action == 3 && action->change.unk10 == 1) {
        return TRUE;
    }
    return FALSE;
}

void func_ov093_021d83fc(BattleAction *action, u8 target) {
    action->raw = 0;
    action->bits.action = 6;
    action->bits.target = target;
}

void func_ov093_021d8414(BattleAction *action) {
    action->bits.action = 4;
}

void func_ov093_021d8424(BattleAction *action) {
    action->bits.action = 5;
}

void BattleAction_SetNull(BattleAction *action) {
    action->bits.action = 0;
    action->raw &= 0xf;
}

void BattleAction_SetSkip(BattleAction *action) {
    action->bits.action = 7;
}

u32 BattleAction_GetAction(const BattleAction *action) {
    return action->bits.action;
}

void func_ov093_021d845c(BattleAction *action) {
    action->bits.action = 8;
    action->raw &= 0xf;
}

void func_ov093_021d8470(BattleAction *action) {
    action->bits.action = 9;
    action->raw &= 0xf;
}
