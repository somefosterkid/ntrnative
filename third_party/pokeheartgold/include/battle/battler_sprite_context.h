#ifndef POKEHEARTGOLD_BATTLE_BATTLER_SPRITE_CONTEXT_H
#define POKEHEARTGOLD_BATTLE_BATTLER_SPRITE_CONTEXT_H

#include "global.h"

// Per-battler sprite data record, BattleSystem.unk1D0[4]
typedef struct PokemonSpriteData {
    u8 *tiles;
    int narcID;
    int palette;
    int yOffset;
} PokemonSpriteData; // size: 0x10 (DS)

// Filled by ov12_02261CA8 for move animations, faints and transforms
typedef struct BattlerSpriteContext {
    int targetBattler;
    int sourceBattler;
    PokemonSpriteData *pokemonSpriteData[4];
    struct Pokepic *sprites[4];
    u16 species[4];
    u8 genders[4];
    u8 shinyFlags[4];
    u8 forms[4];
    u32 personalities[4];
    u8 types[4];
} BattlerSpriteContext; // size: 0x50 (DS)

#endif // POKEHEARTGOLD_BATTLE_BATTLER_SPRITE_CONTEXT_H
