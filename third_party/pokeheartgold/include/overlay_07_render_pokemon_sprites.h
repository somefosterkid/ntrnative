#ifndef POKEHEARTGOLD_OVERLAY_07_RENDER_POKEMON_SPRITES_H
#define POKEHEARTGOLD_OVERLAY_07_RENDER_POKEMON_SPRITES_H

#include "global.h"
#include "overlay_07_battle_anim_common.h"

struct ManagedSprite;

// Heap object of the "render Pokemon sprites" script command (ov07_022271DC allocs it, ov07_0222719C ticks it).
// sizeof(RenderPokemonSpritesContext) == 0x30 on the DS.
typedef struct RenderPokemonSpritesContext {
    BattleAnimScriptFuncCommon common;
    struct ManagedSprite *sprites[4];
    int frames;
    int unused;
} RenderPokemonSpritesContext;

#endif // POKEHEARTGOLD_OVERLAY_07_RENDER_POKEMON_SPRITES_H
