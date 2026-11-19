#ifndef POKEHEARTGOLD_OVERLAY_07_BATTLE_ANIM_COMMON_H
#define POKEHEARTGOLD_OVERLAY_07_BATTLE_ANIM_COMMON_H

// Types shared by the per-move battle animation script contexts (overlay 7). Pointers are `struct X *` so that this
// header can be included before the typedefs of BattleAnimSystem / PokemonSprite / ... are visible.

struct BattleAnimSystem;
struct BgConfig;
struct ManagedSprite;
struct PaletteData;
struct Pokepic;
struct SpriteManager;
struct SpriteSystem;

typedef struct Point2D {
    s16 x;
    s16 y;
} Point2D;

// sizeof == 0x24 on the DS
typedef struct XYTransformContext {
    s16 x;
    s16 y;
    s32 data[8];
} XYTransformContext;

// sizeof == 0x1C on the DS
typedef struct BattleAnimScriptFuncCommon {
    u8 state;
    u8 counter1;
    u8 counter2;
    u8 unused_03;
    struct BattleAnimSystem *battleAnimSys;
    struct SpriteSystem *spriteSystem;
    struct SpriteManager *pokemonSpriteManager;
    struct SpriteManager *primarySpriteManager;
    struct BgConfig *bgConfig;
    struct PaletteData *paletteData;
} BattleAnimScriptFuncCommon;

// sizeof == 0x14 on the DS
typedef struct BattleAnimSpriteInfo {
    Point2D basePos;
    Point2D pos;
    struct Pokepic *monSprite;
    struct ManagedSprite *hwSprite;
    int battler;
} BattleAnimSpriteInfo;

#endif // POKEHEARTGOLD_OVERLAY_07_BATTLE_ANIM_COMMON_H
