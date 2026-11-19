#include "global.h"

#include "overlay_07_battle_anim_common.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_string_shot_sprite_context.h"

#include "heap.h"
#include "sprite_system.h"
#include "sys_task.h"

struct BattleAnimSystem;

void ov07_02222590(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, s16 ref, u32 steps);

BOOL ov07_0222260C(XYTransformContext *ctx);

void ov07_02222644(XYTransformContext *ctx, f32 *px, f32 *py);

enum StringShotSpriteState {
    STRING_SHOT_SPRITE_STATE_BLINK = 0,
    STRING_SHOT_SPRITE_STATE_SCALE,
    STRING_SHOT_SPRITE_STATE_WAIT_FOR_SCALE,
    STRING_SHOT_SPRITE_STATE_FADE_OUT,
};

extern const u8 ov07_022366D2[3][2];

void ov07_02227300(SysTask *task, void *param);

void ov07_02227300(SysTask *task, void *param) {
    int i;
    StringShotSpriteContext *ctx = param;

    switch (ctx->state) {
    case STRING_SHOT_SPRITE_STATE_BLINK:
        ctx->delay++;

        for (i = 0; i < ov07_0221C4A8(ctx->battleAnimSystem, 0); i++) {
            s16 unused0, sLotoId;
            ManagedSprite_GetPositionXY(ctx->sprites[i], &unused0, &sLotoId);

            if (ctx->delay >= ov07_022366D2[i][0]) {
                ctx->spriteCounters[i]++;

                if (ctx->spriteCounters[i] >= ov07_022366D2[i][1]) {
                    ctx->spriteCounters[i] = 0;

                    if (ManagedSprite_GetDrawFlag(ctx->sprites[i]) == 1) {
                        ManagedSprite_SetDrawFlag(ctx->sprites[i], 0);
                    } else {
                        ManagedSprite_SetDrawFlag(ctx->sprites[i], 1);
                    }
                }
            } else {
                ManagedSprite_SetDrawFlag(ctx->sprites[i], 0);
            }
        }

        if (ctx->delay >= 45) {
            for (i = 0; i < ov07_0221C4A8(ctx->battleAnimSystem, 0); i++) {
                ManagedSprite_SetDrawFlag(ctx->sprites[i], 1);
            }

            ctx->delay = 0;
            ctx->state++;
        }
        break;
    case STRING_SHOT_SPRITE_STATE_SCALE:
        ov07_02222590(&ctx->scale,
            100,
            60,
            100,
            100,
            100,
            10);
        ctx->state++;
        break;
    case STRING_SHOT_SPRITE_STATE_WAIT_FOR_SCALE:
        if (ov07_0222260C(&ctx->scale) == 1) {
            for (i = 0; i < ov07_0221C4A8(ctx->battleAnimSystem, 0); i++) {
                f32 affineX, affineY;
                ov07_02222644(&ctx->scale, &affineX, &affineY);
                ManagedSprite_SetAffineScale(ctx->sprites[i], affineX, affineY);
            }
        } else {
            if (++ctx->delay >= 45) {
                ctx->state++;
            }
        }
        break;
    case STRING_SHOT_SPRITE_STATE_FADE_OUT:
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
        for (i = 0; i < ov07_0221C4A8(ctx->battleAnimSystem, 0); i++) {
            Sprite_DeleteAndFreeResources(ctx->sprites[i]);
        }

        ov07_0221C448(ctx->battleAnimSystem, task);
        Heap_Free(ctx);
        return;
    }

    for (i = 0; i < ov07_0221C4A8(ctx->battleAnimSystem, 0); i++) {
        Sprite_TickFrame(ctx->sprites[i]->sprite);
    }

    SpriteSystem_DrawSprites(ctx->spriteMan);
}
