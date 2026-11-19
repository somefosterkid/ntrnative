#ifndef POKEHEARTGOLD_UNK_0206793C_H
#define POKEHEARTGOLD_UNK_0206793C_H

#include "npc_trade.h"
#include "pokemon_types_def.h"
#include "script.h"
#include "trade_anim.h"

typedef struct UnkStruct_0206793C UnkStruct_0206793C;

typedef struct NPCTradeTaskEnv {
    NPCTradeAppData *tradeWork;
    u32 state;
    u32 slot;
    TRADE_ANIM_WORK tradeAnim;
    Pokemon *myMon;
    Pokemon *tradeMon;
} NPCTradeTaskEnv; // size: 0x2C on the DS

void FieldSystem_ClearFollowingTrainer(FieldSystem *fieldSystem);
void sub_02067AE4(FieldSystem *fieldSystem);
void sub_02067A88(FieldSystem *fieldSystem);
void sub_02067B88(FieldSystem *fieldSystem);
void sub_02067BA4(FieldSystem *fieldSystem);
void sub_02067BC0(FieldSystem *fieldSystem);
void sub_02067BE8(FieldSystem *fieldSystem);
void CallTask_NPCTrade(TaskManager *taskManager, NPCTradeAppData *tradeWork, u16 arg, enum HeapID heapID);

void sub_0206793C(TaskManager *arg0);

#endif // POKEHEARTGOLD_UNK_0206793C_H
