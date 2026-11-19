#include "global.h"

#include "heap.h"
#include "pokepic.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_confusion_context.h"

void ov07_02222590(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, s16 ref, u32 steps);

BOOL ov07_0222260C(XYTransformContext *ctx);

BOOL ov07_022227D8(XYTransformContext *ctx);

void ov07_0222C6A8(SysTask *task, void *param) {
    ConfusionContext *ctx = param;
    s32 iVar1;

    iVar1 = ctx->state;
    switch (iVar1) {
    case 0:
        ov07_022227D8(&ctx->shake);
        iVar1 = ov07_0222260C(&ctx->scale);
        ov07_0222C610(ctx);
        if (iVar1 == 0) {
            ov07_02222590(&ctx->scale, 0xc, 10, 0xf, 10, 10, 4);
            ctx->state++;
        }
        break;
    case 1:
        ov07_022227D8(&ctx->shake);
        iVar1 = ov07_0222260C(&ctx->scale);
        ov07_0222C610(ctx);
        if (iVar1 == 0) {
            Pokepic_SetAttr(ctx->defenderSprite, 0, ctx->defenderX);
            Pokepic_SetAttr(ctx->defenderSprite, 1, ctx->defenderY);
            ManagedSprite_SetOamMode(ctx->monSprite, GX_OAM_MODE_NORMAL);
            ManagedSprite_SetPositionXY(ctx->monSprite, ctx->defenderX, ctx->defenderY - ctx->defenderShadowHeight);
            ManagedSprite_SetAffineScale(ctx->monSprite, 1.0, 1.0);
            ctx->state++;
        }
        break;
    case 2:
        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        return;
    }
    SpriteSystem_DrawSprites(ctx->pokemonSpriteManager);
    return;
}
