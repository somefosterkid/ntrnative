#ifndef POKEHEARTGOLD_OVERLAY_12_PLAY_LEVEL_UP_ANIMATION_DATA_H
#define POKEHEARTGOLD_OVERLAY_12_PLAY_LEVEL_UP_ANIMATION_DATA_H

// PlayLevelUpAnimationData: merged by tools/hg/promote.py from 1 m2c local copies (1 variants, base
// src/overlay_12_battle_controller_opponent_0225AE48_0225AE48.c) and 7 accesses; fields at every accessed offset, pads elsewhere.

struct BattleSystem;

typedef struct PlayLevelUpAnimationData {
    struct BattleSystem *battleSys;
    void *healthbox;
    u8 command;
    u8 battler;
    u8 state;
    u8 flashComplete;
} PlayLevelUpAnimationData;

// sizeof(PlayLevelUpAnimationData) == 0xc

#endif // POKEHEARTGOLD_OVERLAY_12_PLAY_LEVEL_UP_ANIMATION_DATA_H
