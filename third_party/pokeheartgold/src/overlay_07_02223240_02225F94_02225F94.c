#include "global.h"

#include "bg_window.h"
#include "overlay_07_battle_anim_funcs.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"

struct BattleAnimSystem;

typedef struct PixelatePokemonSpriteContext {
    BattleAnimScriptFuncCommon common;
    ManagedSprite *sprite;
    s16 scaleStep;
    int scaleX;
    int scaleY;
    int targetScale;
} PixelatePokemonSpriteContext;

void ov07_02225F18(SysTask *task, void *param);

void ov07_02225F94(BattleAnimSystem *system);

void ov07_02225F94(BattleAnimSystem *system) {
    PixelatePokemonSpriteContext *ctx = ov07_022324D8(system, sizeof(PixelatePokemonSpriteContext));
    ov07_02231FE4(system, &ctx->common);

    ctx->scaleStep = ov07_0221C4A8(ctx->common.battleAnimSys, 1);
    ctx->scaleX = ov07_0221C4A8(ctx->common.battleAnimSys, 2);
    ctx->scaleY = ov07_0221C4A8(ctx->common.battleAnimSys, 3);
    ctx->sprite = ov07_0221C4E8(
        ctx->common.battleAnimSys,
        ov07_0221C4A8(ctx->common.battleAnimSys, 0));

    ManagedSprite_SetDrawFlag(ctx->sprite, 1);

    if (ctx->scaleStep < 0) {
        ctx->targetScale = 0;
    } else {
        ctx->targetScale = 15;
    }

    ManagedSprite_SetMosaicFlag(ctx->sprite, 1);

    SysTask *task = ov07_0221C410(ctx->common.battleAnimSys, ov07_02225F18, ctx);
    ov07_02225F18(task, ctx);
}
