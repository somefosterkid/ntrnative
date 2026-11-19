#include "global.h"

#include "overlay_07_battle_anim_funcs.h"
#include "sprite_system.h"
#include "bg_window.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_render_pokemon_sprites.h"

struct BattleAnimScriptFuncCommon;

struct BattleAnimSystem;

struct BgConfig;

struct ManagedSprite;

struct PaletteData;

struct RenderPokemonSpritesContext;

struct SpriteManager;

struct SpriteSystem;

struct SysTask;

struct SysTaskQueue;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

void ov07_0222719C(SysTask *task, void *param);

void ov07_022271DC(BattleAnimSystem *system);

void ov07_022271DC(BattleAnimSystem *system) {
    RenderPokemonSpritesContext *ctx = ov07_022324D8(system, sizeof(RenderPokemonSpritesContext));
    ov07_02231FE4(system, &ctx->common);

    if (ov07_0221C4A8(system, 0) == 0) {
        ctx->frames = 3;
    } else {
        ctx->frames = ov07_0221C4A8(system, 0);
    }

    extern int ov07_022378B0[4];

    for (int battler = 0; battler < 4; battler++) {
        ctx->sprites[battler] = ov07_0221C4E8(ctx->common.battleAnimSys, battler);

        if (ctx->sprites[battler] != 0L) {
            ManagedSprite_SetDrawPriority(ctx->sprites[battler], ov07_022378B0[battler]);
        }
    }

    SysTask *task = ov07_0221C410(ctx->common.battleAnimSys, ov07_0222719C, ctx);
    ov07_0222719C(task, ctx);
}
