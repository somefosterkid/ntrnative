#ifndef POKEHEARTGOLD_BATTLE_OVERLAY_12_0225A2A0_H
#define POKEHEARTGOLD_BATTLE_OVERLAY_12_0225A2A0_H

#include "battle/battle.h"

// the opponent's ball-throw task data: ov12_0225A2A0 allocates it, ov12_0225D138 / ov12_0225D644 run it

typedef struct UnkStruct_ov12_0225A2A0 {
    BattleSystem *battleSystem;
    OpponentData *unk4;
    u8 unk8;
    u8 unk9;
    u8 unkA;
    u8 unkB;
    s32 unkC;
    u32 unk10;
    s32 unk14;
    s32 unk18;
} UnkStruct_ov12_0225A2A0; // size 0x1c

#endif // POKEHEARTGOLD_BATTLE_OVERLAY_12_0225A2A0_H
