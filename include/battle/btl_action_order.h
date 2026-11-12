// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btl_action_order.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_BATTLE_BTL_ACTION_ORDER_H
#define BWPORT_BATTLE_BTL_ACTION_ORDER_H

#include "types.h"
#include "struct_decls.h"
#include "battle/btl_action.h"

ActionOrderEntry *ActionOrder_SearchByMonID(BtlServerFlow *flow, u8 monId);
ActionOrderEntry *ActionOrder_SearchByMoveID(BtlServerFlow *flow, u16 moveId, u8 start);
s32 ActionOrderTool_Interrupt(BtlServerFlow *flow, ActionOrderEntry *entry, s32 start);
void ActionOrderTool_SendToLast(BtlServerFlow *flow, ActionOrderEntry *entry);
BOOL ActionOrder_InterruptReserve(BtlServerFlow *flow, u8 monId);
BOOL ActionOrder_InterruptReserveByMove(BtlServerFlow *flow, u16 moveId);
void ActionOrder_ForceDone(BtlServerFlow *flow, u8 monId);

// Black 2's; twinned code confirms the offsets of mon, action, key, clientId and done in Black
struct ActionOrderEntry {
    BattleMon *mon;
    BattleAction action;
    // The speed in bits 0-12, then three fields of 3, 6 and 3 bits
    u32 key;
    u8 clientId;
    u8 done;
    // Set while a Pursuit waits to strike a mon that is switching out
    u8 interrupting;
    u8 unk0F;
};

BOOL ActionOrder_SendToLast(BtlServerFlow *flow, u8 monId);

#endif // BWPORT_BATTLE_BTL_ACTION_ORDER_H
