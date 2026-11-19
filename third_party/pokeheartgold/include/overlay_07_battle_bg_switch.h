#ifndef POKEHEARTGOLD_OVERLAY_07_BATTLE_BG_SWITCH_H
#define POKEHEARTGOLD_OVERLAY_07_BATTLE_BG_SWITCH_H

#include "global.h"

struct BattleAnimSystem;

// Heap object of the battle animation background switch task (heap alloc 0x4C, ov07_0221DF1C).
typedef struct BattleBgSwitch {
    int unread_00;
    u8 unused_04;
    u8 state;
    u8 unused_06;
    u8 unused_07;
    u8 unused_08;
    u8 blendCoeffA;
    u8 blendCoeffB;
    u8 targetBlendCoeffA;
    u8 targetBlendCoeffB;
    u8 fadeType;
    s8 unk_0E;
    s8 unk_0F;
    int bgID;
    int mode;
    int flags;
    int scriptVars[10];
    u16 bgMoveAnimActive : 1;
    u16 bgWaveAnimActive : 1;
    u16 bgExtraAnimActive : 1;
    u16 unused_44_3 : 13;
    struct BattleAnimSystem *battleAnimSystem;
} BattleBgSwitch;

#endif // POKEHEARTGOLD_OVERLAY_07_BATTLE_BG_SWITCH_H
