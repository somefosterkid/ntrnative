// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btl_action.h, the lines in use so
// far.
#ifndef BWPORT_BATTLE_BTL_ACTION_H
#define BWPORT_BATTLE_BTL_ACTION_H

#include "types.h"
#include "struct_decls.h"

union BattleAction {
    u32 raw;
    struct {
        u32 action : 4;
        u32 target : 3;
        u32 move : 16;
        u32 unk23 : 3;
        u32 unk26 : 1;
        u32 unk27 : 5;
    } bits;
    // An item use
    struct {
        u32 action : 4;
        u32 target : 3;
        u32 item : 16;
        u32 param : 8;
        u32 unk31 : 1;
    } item;
    // A switch, to the party slot
    struct {
        u32 action : 4;
        u32 unk4 : 3;
        u32 slot : 3;
        u32 unk10 : 1;
        u32 unk11 : 21;
    } change;
};

void BattleAction_SetFightParam(BattleAction *action, u16 move, u8 target);
void BattleAction_ChangeFightTargetPos(BattleAction *action, u8 target);
void func_ov093_021d8318(BattleAction *action);
BOOL func_ov093_021d832c(const BattleAction *action);
u16 func_ov093_021d8340(const BattleAction *action);
void func_ov093_021d8358(BattleAction *action, u16 item, u8 target, u8 param);
void func_ov093_021d8394(BattleAction *action, u8 pos, u8 slot);
void func_ov093_021d83c4(BattleAction *action);
BOOL func_ov093_021d83e0(const BattleAction *action);
void func_ov093_021d83fc(BattleAction *action, u8 target);
void func_ov093_021d8414(BattleAction *action);
void func_ov093_021d8424(BattleAction *action);
void BattleAction_SetNull(BattleAction *action);
void BattleAction_SetSkip(BattleAction *action);
u32 BattleAction_GetAction(const BattleAction *action);
void func_ov093_021d845c(BattleAction *action);
void func_ov093_021d8470(BattleAction *action);

// Black 2's, not checked against Black
// The move of a fight action, 0 for any other action
static inline u16 BattleAction_GetMove(const BattleAction *action) {
    if (action->bits.action == 1) {
        return action->bits.move;
    }
    return 0;
}

#endif // BWPORT_BATTLE_BTL_ACTION_H
