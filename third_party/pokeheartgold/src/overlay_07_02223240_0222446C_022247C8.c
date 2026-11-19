#include "global.h"

#include "heap.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_alpha_fade_sprite_context.h"
#include "overlay_07_alpha_fade_context.h"

void ov07_022247C8(SysTask *task, void *param) {
    AlphaFadePokemonSpriteContext *ctx = param;

    if (ov07_02222AF4(&ctx->alpha) != 0) {
#if defined(SDK_PORT) || defined(NONMATCHING)
        BattleAnimSystem *system = ctx->battleAnimSys;
        Heap_Free(ctx);
        ov07_0221C448(system, task);
#else
        Heap_Free(ctx);
        ov07_0221C448(ctx->battleAnimSys, task);
#endif
        return;
    }
    SpriteSystem_DrawSprites(ctx->pokemonSpriteManager);
    return;
}
