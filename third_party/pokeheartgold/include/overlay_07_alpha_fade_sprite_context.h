#ifndef POKEHEARTGOLD_OVERLAY_07_ALPHA_FADE_SPRITE_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_ALPHA_FADE_SPRITE_CONTEXT_H

#include "global.h"
#include "sys_task.h"
#include "overlay_07_alpha_fade_context.h"

struct BattleAnimSystem;
struct SpriteManager;

// sizeof == 0x30 on the DS (ov07_022324D8 alloc in ov07_022247F4)
typedef struct AlphaFadePokemonSpriteContext {
    struct BattleAnimSystem *battleAnimSys;
    struct SpriteManager *pokemonSpriteManager;
    AlphaFadeContext alpha;
} AlphaFadePokemonSpriteContext;

void ov07_022247C8(SysTask *task, void *param);
void ov07_022247F4(struct BattleAnimSystem *system);

#endif // POKEHEARTGOLD_OVERLAY_07_ALPHA_FADE_SPRITE_CONTEXT_H
