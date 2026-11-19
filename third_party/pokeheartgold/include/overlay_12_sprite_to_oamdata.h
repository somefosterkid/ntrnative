#ifndef POKEHEARTGOLD_OVERLAY_12_SPRITE_TO_OAMDATA_H
#define POKEHEARTGOLD_OVERLAY_12_SPRITE_TO_OAMDATA_H

// SpriteToOAMData: merged by tools/hg/promote.py from 1 m2c local copies (1 variants, base
// src/overlay_12_battle_controller_opponent_0225AE48_0225B200_0225B200.c) and 17 accesses; fields at every accessed offset, pads elsewhere.

struct BattleSystem;
struct OpponentData;
struct Pokepic;

typedef struct SpriteToOAMData {
    struct BattleSystem *battleSys;
    struct OpponentData *battlerData;
    struct Pokepic *monSprite;
    u8 command;
    u8 battler;
    u8 state;
    u8 unused[1];
} SpriteToOAMData;

// sizeof(SpriteToOAMData) == 0x10

#endif // POKEHEARTGOLD_OVERLAY_12_SPRITE_TO_OAMDATA_H
