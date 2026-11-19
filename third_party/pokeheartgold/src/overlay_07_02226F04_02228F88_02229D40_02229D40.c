#include "global.h"

#include "overlay_07_battle_anim_common.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_taunt_sprite_context.h"

#include "heap.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"

struct BattleAnimSystem;

enum TauntSpriteState {
    TAUNT_SPRITE_STATE_FADE_IN = 0,
    TAUNT_SPRITE_STATE_FADE_OUT,
};

extern const u8 ov07_02236778[3][2];

void ov07_02229D40(SysTask *task, void *param);

void ov07_02229D40(SysTask *task, void *param) {
    int i;
    TauntSpriteContext *ctx = param;

    switch (ctx->state) {
    case TAUNT_SPRITE_STATE_FADE_IN:
        if (ctx->spriteAlpha < 15) {
            ctx->spriteAlpha++;
        }

        if (ctx->bgAlpha > 0) {
            ctx->bgAlpha--;
        }

        G2_ChangeBlendAlpha(ctx->spriteAlpha, ctx->bgAlpha);
        ctx->fadeInFrames++;

        for (i = 0; i < ctx->handCount; i++) {
            s16 x, y;
            ManagedSprite_GetPositionXY(ctx->handSprites[i], &x, &y);

            if (ctx->fadeInFrames >= ov07_02236778[i][0]) {
                ctx->fadeInCycles[i]++;

                if (ctx->fadeInCycles[i] >= ov07_02236778[i][1]) {
                    ctx->fadeInCycles[i] = 0;
                }
            }
        }

        if (ctx->fadeInFrames >= 45) {
            ctx->state++;
        }
        break;
    case TAUNT_SPRITE_STATE_FADE_OUT:
        if (ctx->spriteAlpha > 0) {
            ctx->spriteAlpha--;
        }

        if (ctx->bgAlpha < 15) {
            ctx->bgAlpha++;
        }

        if (ctx->spriteAlpha == 0 && ctx->bgAlpha == 15) {
            ctx->state++;
        }

        G2_ChangeBlendAlpha(ctx->spriteAlpha, ctx->bgAlpha);
        break;
    default:
        for (i = 0; i < ctx->handCount; i++) {
            Sprite_DeleteAndFreeResources(ctx->handSprites[i]);
        }

        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);

        return;
    }

    for (i = 0; i < ctx->handCount; i++) {
        Sprite_TickFrame(ctx->handSprites[i]->sprite);
    }

    SpriteSystem_DrawSprites(ctx->spriteMan);
}
