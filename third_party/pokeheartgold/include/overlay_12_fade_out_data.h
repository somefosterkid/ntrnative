#ifndef POKEHEARTGOLD_OVERLAY_12_FADE_OUT_DATA_H
#define POKEHEARTGOLD_OVERLAY_12_FADE_OUT_DATA_H

// FadeOutData: merged by tools/hg/promote.py from 1 m2c local copies (1 variants, base
// src/overlay_12_battle_controller_opponent_0225A414.c) and 7 accesses; fields at every accessed offset, pads elsewhere.

struct BattleSystem;

typedef struct FadeOutData {
    struct BattleSystem *battleSys;
    u8 command;
    u8 battler;
    u8 state;
    u8 unused;
} FadeOutData;

// sizeof(FadeOutData) == 0x8

#endif // POKEHEARTGOLD_OVERLAY_12_FADE_OUT_DATA_H
