// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btlv_gauge.h: the lines in use so
// far.
#ifndef BWPORT_BATTLE_BTLV_GAUGE_H
#define BWPORT_BATTLE_BTLV_GAUGE_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"
#include "gfl/clact.h"
#include "gfl/heap.h"

// Black 2's, not checked against Black
// The battle view's positions, which index the gauges: 0 and 1 in single battles, 2 to 5 in double battles and 2 to 7
// in triple and rotation battles, the player's even. An enum, which MWCC doesn't propagate constants into, so some
// loops over the gauges test their bound before the first pass
typedef enum {
    BTLV_GAUGE_POS_MAX = 8,
} BtlvGaugePos;

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BtlvGauge_HideStatus
void func_ov094_022089bc(BtlvGauge *gauge, int index);
// pokebw2's BtlvGauge_Update
void func_ov094_02206eb4(BtlvGauge *gauge);
// pokebw2's BtlvGauge_RequestNumberToggle
void func_ov094_022089ac(BtlvGauge *gauge);
// pokebw2's BtlvGauge_SetStatus
void func_ov094_02208814(BtlvGauge *gauge, u32 status, int index);
// pokebw2's BtlvGauge_StartShake
// index 8 stops every gauge's shake without starting one
void func_ov094_02208870(BtlvGauge *gauge, int index);
// pokebw2's BtlvGauge_GetStatus
BOOL func_ov094_02208958(BtlvGauge *gauge, int index, u32 *color, u32 *status);
// pokebw2's BtlvGauge_IsPinch
u32 func_ov094_022088f4(BtlvGauge *gauge);
// pokebw2's BtlvGauge_SetPinch
void func_ov094_02208900(BtlvGauge *gauge, u32 value);
// pokebw2's BtlvGauge_SetNoPinchBgm
void func_ov094_0220893c(BtlvGauge *gauge, u32 value);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BtlvGauge_CheckChanged
BOOL func_ov094_02207d78(BtlvGauge *gauge, int index);
// Plays the SE unless the battle's input is in the rule and screen the caller checks; takes BtlvInput
void func_ov094_02206b58(struct BtlvInput *input);
// Plays an SE unless the battle's input is in the state the caller checks; takes BtlvInput
void func_ov094_02206b74(struct BtlvInput *input);
// Plays an SE unless the battle's input is in the state the caller checks; takes BtlvInput
void func_ov094_02206b90(struct BtlvInput *input);
void func_ov094_02206bac(struct BtlvInput *input);
// pokebw2's BtlvGauge_Create, which takes the battle's heap and font
BtlvGauge *func_ov094_02206bc8(Font *font, HeapID heapId);
// pokebw2's BtlvGauge_SetBattleMon
// type is the battle's style, BTL_STYLE_*; index the gauge, by view position, even for the player's side
void func_ov094_022070c0(BtlvGauge *gauge, BtlMainModule *mainModule, BattleMon *mon, u32 type, int index);
// pokebw2's BtlvGauge_SetPartyPkm
void func_ov094_02207288(BtlvGauge *gauge, PokeDexSave *pokedex, PartyPkm *pkm, u32 type, int index);
// Frees the gauges; the pinch music stops first
void func_ov094_02206e28(BtlvGauge *gauge);
// pokebw2's BtlvGauge_Hide
void func_ov094_02207960(BtlvGauge *gauge, int index);
// pokebw2's BtlvGauge_SetPos
void func_ov094_022079c0(BtlvGauge *gauge, int index, const ClActorPos *offset);
// pokebw2's BtlvGauge_StartHpChange
void func_ov094_02207b4c(BtlvGauge *gauge, int index, s32 hp);
// pokebw2's BtlvGauge_SetHpAtOnce
void func_ov094_02207b68(BtlvGauge *gauge, int index, s32 hp);
// pokebw2's BtlvGauge_StartExpGain
void func_ov094_02207b84(BtlvGauge *gauge, int index, s32 exp);
// pokebw2's BtlvGauge_StartLevelUp
void func_ov094_02207bac(BtlvGauge *gauge, BattleMon *mon, int index);
// pokebw2's BtlvGauge_IsBusy
BOOL func_ov094_02207c94(BtlvGauge *gauge);
// pokebw2's BtlvGauge_SetSideVisible
// side 2 is both sides
void func_ov094_02207cc4(BtlvGauge *gauge, BOOL visible, u32 side);
// pokebw2's BtlvGauge_SetVisible
void func_ov094_02207d04(BtlvGauge *gauge, BOOL visible, int index);
// pokebw2's BtlvGauge_IsShown
BOOL func_ov094_02207d60(BtlvGauge *gauge, int index);

#endif // BWPORT_BATTLE_BTLV_GAUGE_H

