#ifndef POKEHEARTGOLD_OVERLAY_12_SET_MOSAIC_DATA_H
#define POKEHEARTGOLD_OVERLAY_12_SET_MOSAIC_DATA_H

// SetMosaicData: merged by tools/hg/promote.py from 1 m2c local copies (1 variants, base
// src/overlay_12_battle_controller_opponent_0225AE48_0225AF74_0225AF74.c) and 13 accesses; fields at every accessed offset, pads elsewhere.

struct BattleSystem;
struct Pokepic;

typedef struct SetMosaicData {
    struct BattleSystem *battleSys;
    struct Pokepic *monSprite;
    u8 command;
    u8 battler;
    u8 state;
    u8 intensity;
    u8 counter;
    u8 wait;
    u16 unused;
} SetMosaicData;

// sizeof(SetMosaicData) == 0x10

#endif // POKEHEARTGOLD_OVERLAY_12_SET_MOSAIC_DATA_H
