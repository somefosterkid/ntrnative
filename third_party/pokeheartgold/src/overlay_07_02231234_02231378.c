#include "global.h"

#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_alpha_fade_context.h"
#include "overlay_07_grudge_sprite_context.h"

struct BattleAnimSystem;

void ov07_02231E08(BattleAnimSystem *system, int spriteAlpha, int bgAlpha);

enum GrudgeSpriteState {
    GRUDGE_SPRITE_STATE_INIT = 0,
    GRUDGE_SPRITE_STATE_REVOLVE_AND_FADE_IN,
    GRUDGE_SPRITE_STATE_REVOLVE,
    GRUDGE_SPRITE_STATE_FADE_OUT,
    GIUDGE_SPRITE_STATE_CLEANUP,
};

void ov07_022312A8(GrudgeSpriteContext *ctx, int dir);

void ov07_02231300(GrudgeSpriteContext *ctx);

void ov07_02231378(SysTask *task, void *param);

void ov07_02231378(SysTask *task, void *param) {
    GrudgeSpriteContext *ctx = param;

    switch (ctx->state) {
    case GRUDGE_SPRITE_STATE_INIT:
        ov07_02231E08(
            ctx->battleAnimSys,
            1,
            16 - 1);
        ov07_02222AC4(
            &ctx->alpha,
            1,
            16,
            16 - 1,
            16 - 16,
            10);
        ov07_022312A8(ctx, ctx->dir);
        ov07_02231300(ctx);
        ctx->state++;
        break;
    case GRUDGE_SPRITE_STATE_REVOLVE_AND_FADE_IN:
        ov07_02231300(ctx);

        if (ov07_02222AF4(&ctx->alpha)) {
            ctx->state++;
            ctx->delay = 48 - (2 * 10);
        }
        break;
    case GRUDGE_SPRITE_STATE_REVOLVE:
        ov07_02231300(ctx);
        ctx->delay--;

        if (ctx->delay < 0) {
            ctx->state++;
            ov07_02222AC4(
                &ctx->alpha,
                16,
                1,
                16 - 16,
                16 - 1,
                10);
        }
        break;
    case GRUDGE_SPRITE_STATE_FADE_OUT:
        ov07_02231300(ctx);

        if (ov07_02222AF4(&ctx->alpha)) {
            ctx->state++;
        }
        break;
    case GIUDGE_SPRITE_STATE_CLEANUP:
        for (int i = 0; i < 6; i++) {
            Sprite_DeleteAndFreeResources(ctx->flameSprites[i]);
        }

        Heap_Free(ctx);
        ov07_0221C448(ctx->battleAnimSys, task);
        return;
    }

    SpriteSystem_DrawSprites(ctx->spriteMan);
}
