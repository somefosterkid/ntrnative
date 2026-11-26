#include "global.h"

#include "heap.h"
#include "pokepic.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "error_handling.h"
#include "overlay_07_lock_on_sprite_context.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_alpha_fade_context.h"
#include "overlay_07_battle_anim_funcs.h"

void ov07_0222207C(XYTransformContext *ctx, ManagedSprite *sprite, s16 cx, s16 cy);

BOOL ov07_022222B4(XYTransformContext *ctx);

typedef struct ForesightSpriteContext {
    BattleAnimSystem *battleAnimSys;
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    int state;
    Pokepic *defenderSprite;
    ManagedSprite *sprite;
    XYTransformContext pos;
    AlphaFadeContext alpha;
    s16 baseX;
    s16 baseY;
    int segment;
    int delay;
} ForesightSpriteContext;

enum ForesightSpriteState {
    FORESIGHT_SPRITE_STATE_DELAY = 0,
    FORESIGHT_SPRITE_STATE_MOVE,
    FORESIGHT_SPRITE_STATE_FADE_OUT,
    FORESIGHT_SPRITE_STATE_WAIT_FLASH,
    FORESIGHT_SPRITE_STATE_CLEANUP,
};

void ov07_0222DE30(ForesightSpriteContext *ctx);

void ov07_0222DEEC(SysTask *task, void *param);

s32 ov07_02222268(void *, s16, s16, s16, s16, u16);
void ov07_0222E07C(void *arg0);

void ov07_0222DEEC(SysTask *task, void *param) {
    ForesightSpriteContext *ctx = param;

    switch (ctx->state) {
    case FORESIGHT_SPRITE_STATE_DELAY:
        ctx->delay++;

        if (ctx->delay > 4) {
            ov07_0222DE30(ctx);
            ctx->state++;
            ctx->delay = 0;
        }
        break;
    case FORESIGHT_SPRITE_STATE_MOVE:
        if (ov07_022222B4(&ctx->pos)) {
            ov07_0222207C(&ctx->pos, ctx->sprite, ctx->baseX, ctx->baseY);
        } else {
            ctx->segment++;

            if (ctx->segment < 6) {
                ctx->state = FORESIGHT_SPRITE_STATE_DELAY;
                ctx->baseX += ctx->pos.x;
                ctx->baseY += ctx->pos.y;
            } else {
                ctx->state++;

                ManagedSprite_SetOamMode(ctx->sprite, GX_OAM_MODE_XLU);
                ov07_02222AC4(
                    &ctx->alpha,
                    16,
                    0,
                    0,
                    16,
                    16);

                Pokepic_StartPaletteFade(
                    ctx->defenderSprite,
                    0,
                    10,
                    0,
                    ((GXRgb)(((31) << (0)) | ((31) << (5)) | ((31) << (10)))));
            }
        }
        break;
    case FORESIGHT_SPRITE_STATE_FADE_OUT:
        if (ov07_02222AF4(&ctx->alpha)) {
            ManagedSprite_SetDrawFlag(ctx->sprite, 0);
        }

        if (Pokepic_ResumePaletteFade(ctx->defenderSprite) == 0) {
            ctx->state++;
            Pokepic_StartPaletteFade(
                ctx->defenderSprite,
                10,
                0,
                0,
                ((GXRgb)(((31) << (0)) | ((31) << (5)) | ((31) << (10)))));
        }
        break;
    case FORESIGHT_SPRITE_STATE_WAIT_FLASH:
        if (Pokepic_ResumePaletteFade(ctx->defenderSprite) == 0) {
            ctx->state++;
        }
        break;
    case FORESIGHT_SPRITE_STATE_CLEANUP:
        Sprite_DeleteAndFreeResources(ctx->sprite);
        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        return;
    }

    SpriteSystem_DrawSprites(ctx->spriteMan);
}

void ov07_0222E020(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite);

void ov07_0222E020(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite) {
    ForesightSpriteContext *ctx = ov07_022324D8(system, sizeof(ForesightSpriteContext));

    ctx->battleAnimSys = system;
    ctx->spriteSys = spriteSys;
    ctx->spriteMan = spriteMan;
    ctx->defenderSprite = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C470(system));
    ctx->sprite = sprite;

    ManagedSprite_SetDrawPriority(ctx->sprite, 100);
    ManagedSprite_SetPriority(ctx->sprite, 1);
    ManagedSprite_GetPositionXY(ctx->sprite, &ctx->baseX, &ctx->baseY);

    ov07_0221C410(ctx->battleAnimSys, ov07_0222DEEC, ctx);
}

void ov07_0222E07C(void *arg0) {
    u32 v0;

    v0 = *((u32 *)&((LockOnSpriteContext *)arg0)->segment);
    switch (v0) {
    case 0:
        ov07_02222268(&((LockOnSpriteContext *)arg0)->pos, 0, 0x50, 0, 0x50, 4);
        return;
    case 1:
        ov07_02222268(&((LockOnSpriteContext *)arg0)->pos, 0, 0, 0, -0x50, 4);
        return;
    case 2:
        ov07_02222268(&((LockOnSpriteContext *)arg0)->pos, 0, -0x50, 0, 0x50, 4);
        return;
    case 3:
        ov07_02222268(&((LockOnSpriteContext *)arg0)->pos, 0, 0x28, 0, -0x28, 4);
        return;
    default:
        GF_AssertFail();
        return;
    }
}
