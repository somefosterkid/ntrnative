#ifndef POKEHEARTGOLD_OVERLAY_12_OPEN_CAPTURE_BALL_DATA_H
#define POKEHEARTGOLD_OVERLAY_12_OPEN_CAPTURE_BALL_DATA_H

// OpenCaptureBallData: merged by tools/hg/promote.py from 1 m2c local copies (1 variants, base
// src/overlay_12_battle_controller_opponent_0225A018.c) and 12 accesses; fields at every accessed offset, pads elsewhere.

struct BattleSystem;
struct PokemonSprite;

typedef struct OpenCaptureBallData {
    struct BattleSystem *battleSys;
    struct PokemonSprite *monSprite;
    u8 command;
    u8 battler;
    u8 battlerType;
    u8 state;
    u8 yOffset;
    u8 unused_0D;
    u16 unused_0E;
} OpenCaptureBallData;

// sizeof(OpenCaptureBallData) == 0x10

#endif // POKEHEARTGOLD_OVERLAY_12_OPEN_CAPTURE_BALL_DATA_H
