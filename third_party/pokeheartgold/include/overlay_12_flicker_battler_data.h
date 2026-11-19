#ifndef POKEHEARTGOLD_OVERLAY_12_FLICKER_BATTLER_DATA_H
#define POKEHEARTGOLD_OVERLAY_12_FLICKER_BATTLER_DATA_H

// FlickerBattlerData: merged by tools/hg/promote.py from 1 m2c local copies (1 variants, base
// src/overlay_12_battle_controller_opponent_0225A414.c) and 10 accesses; fields at every accessed offset, pads elsewhere.

struct BattleSystem;
struct PokemonSprite;

typedef struct FlickerBattlerData {
    struct BattleSystem *battleSys;
    struct PokemonSprite *monSprite;
    u8 battler;
    u8 counter;
    u8 delay;
    u8 unused;
} FlickerBattlerData;

// sizeof(FlickerBattlerData) == 0xc

#endif // POKEHEARTGOLD_OVERLAY_12_FLICKER_BATTLER_DATA_H
