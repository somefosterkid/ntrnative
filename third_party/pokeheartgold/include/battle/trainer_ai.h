#ifndef POKEHEARTGOLD_TRAINER_AI_H
#define POKEHEARTGOLD_TRAINER_AI_H

#include "battle.h"

void ov10_0221BE20(BattleSystem *battleSystem, BattleContext *ctx, u8 battlerId, u8 a3);
u8 ov10_0221BEF4(BattleSystem *battleSystem, u8 battlerId);

// the following functions are in reality static but need to be decompiled still or are used by non-decompiled functions
u8 ov10_0221BF44(BattleSystem *battleSystem, BattleContext *ctx);
u8 ov10_0221C038(BattleSystem *battleSystem, BattleContext *ctx);

// AI command handlers (referenced from the AI command table in asm)
void ov10_0221C7D8(BattleSystem *battleSystem, BattleContext *ctx);
void ov10_0221C828(BattleSystem *battleSystem, BattleContext *ctx);
void ov10_0221C878(BattleSystem *battleSys, BattleContext *ctx);
void ov10_0221C8A8(BattleSystem *battleSys, BattleContext *ctx);
void ov10_0221C8D8(BattleSystem *battleSys, BattleContext *ctx);
void ov10_0221C908(BattleSystem *battleSys, BattleContext *ctx);
void ov10_0221C938(BattleSystem *battleSys, BattleContext *ctx);
void ov10_0221C968(BattleSystem *battleSys, BattleContext *ctx);
void ov10_0221C998(BattleSystem *battleSys, BattleContext *ctx);
void ov10_0221C9C8(BattleSystem *battleSys, BattleContext *ctx);
void ov10_0221CEA4(BattleSystem *battleSys, BattleContext *ctx);
void ov10_0221CED4(BattleSystem *battleSys, BattleContext *ctx);
void ov10_0221DEF0(BattleSystem *battleSys, BattleContext *ctx);

// AI script engine helpers shared by the command handlers (defined in overlay_10_trainer_ai_0221ED10.c)
int ov10_0221EEF0(BattleContext *battleCtx);
void ov10_0221EF24(BattleContext *battleCtx, int i);
u8 ov10_0221EF34(BattleContext *battleCtx, u8 inBattler);
BOOL ov10_0221EE60(BattleSystem *battleSys, BattleContext *battleCtx);
BOOL ov10_0221FD34(BattleSystem *battleSys, BattleContext *battleCtx, int battler, BOOL flag);
s32 ov10_0221EF7C(BattleSystem *battleSys, BattleContext *battleCtx, s32 attacker, u16 *moves, s32 *damageVals, u16 heldItem, u8 *ivs, s32 ability, s32 embargoTurns, s32 varyDamage);

#endif
