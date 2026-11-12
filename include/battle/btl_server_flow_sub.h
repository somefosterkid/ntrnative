// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btl_server_flow_sub.h: the lines in
// use so far.
#ifndef BWPORT_BATTLE_BTL_SERVER_FLOW_SUB_H
#define BWPORT_BATTLE_BTL_SERVER_FLOW_SUB_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"

u8 func_ov093_021ca7f4(BtlServerFlow *flow, BattleMon *mon, u8 target, BtlFlowMoveParam *param, void *targets);
void func_ov093_021cc8a0(u32 *table);

void func_ov093_021cc8b0(u32 *table, u8 monId, u32 value);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
u8 func_ov093_021cb730(BtlServerFlow *flow, BattleMon *mon, u16 item, u8 param, u8 slot);
u32 func_ov093_021cc8c0(u32 *table, u8 monId);
u32 func_ov093_021cc8b8(u32 *table, u8 monId);
u32 func_ov093_021cc668(BtlServerFlow *flow);
void AddExpAndEVs(BtlServerFlow *flow, BattleParty *party, BattleMon *defeated, BtlFlowExpEntry *entries);

BOOL data_ov095_0689c8fd(u16 item);
u8 data_ov095_0689d1d5(BtlFaintRec *faintRec);

BOOL data_ov095_0689c9a1(u16 background);
BattleEventItem *data_ov095_06898c11(u8 side, u32 effect, BattleCondition cont);


void data_ov095_0689cb3d(BtlMonSet *set);
BattleMon *data_ov095_0689cc71(BtlMonSet *set, u32 index);
u8 data_ov095_0689d5f5(BtlPosPoke *posPoke, u8 monId);

u32 data_ov095_0689c9e5(u32 index);


void data_ov095_0689cb49(BtlMonSet *set, BattleMon *mon);

BOOL func_ov093_021cc558(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param);

BOOL func_ov093_021cc5f4(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param);

BOOL func_ov093_021ba534(BtlMainModule *mainModule);

u32 CalcBaseExpGain(BattleMon *mon);

u32 data_ov095_0689cd39(BtlMonSet *set);
void data_ov095_0689cc15(BtlMonSet *set, BattleMon *mon);

#endif // BWPORT_BATTLE_BTL_SERVER_FLOW_SUB_H
