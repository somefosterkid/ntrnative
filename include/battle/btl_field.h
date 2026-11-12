// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btl_field.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_BATTLE_BTL_FIELD_H
#define BWPORT_BATTLE_BTL_FIELD_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"

void FieldStatusSetWeather(u8 weather, u8 duration);
BOOL FieldStatusAddEffect(u32 effect, BattleCondition value);
BOOL FieldStatusaddEffectCore(BtlField *field, u32 effect, BattleCondition value, BOOL addEvent);
BOOL CheckImprison(BtlField *field, BtlPokeCon *pokeCon, BattleMon *mon, u16 move);
BOOL CheckFieldEffect(BtlField *field, u32 effect);

// The battle in progress, whose field status btl_field.c keeps
void func_ov093_021ef874(u32 weather);
BOOL func_ov093_021ef91c(BtlPokeCon *pokeCon, BattleMon *mon, u16 move);
// A field status of the main module's, which the clients keep up to date
BtlField *func_ov093_021ef958(HeapID heapId);
u8 func_ov093_021ef9a8(BtlField *field);
void func_ov093_021ef9c0(BtlField *field, u8 weather, u16 turns);
void func_ov093_021ef9c8(BtlField *field);
BOOL func_ov093_021efa94(BtlField *field, u32 effect);
BOOL func_ov093_021efad8(BtlField *field, u32 effect, u8 monId);
void func_ov093_021efc78(BtlField *field, void (*callback)(u32 effect, BtlServerFlow *flow), BtlServerFlow *flow);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
BOOL FieldStatusRemoveEffect(u32 effect);
BOOL FieldStatusAddDependPoke(u32 effect, u8 monId);
// btl_server_flow.c's
u32 GetWeather(BtlServerFlow *serverFlow);
// pokebw2's GetFieldWeather
u8 func_ov093_021ef884(void);
u32 func_ov093_021ef894(void);
u8 func_ov093_021ef8b8(void);
void func_ov093_021ef90c(u8 monId);
void func_ov093_021ef934(void (*callback)(u32 effect, BtlServerFlow *flow), BtlServerFlow *flow);
// pokebw2's IsFieldEffectActive
u32 func_ov093_021ef948(u32 fieldEffect);
void func_ov093_021ef980(BtlField *field);
void func_ov093_021efb34(BtlField *field, u8 monId);

#endif // BWPORT_BATTLE_BTL_FIELD_H
