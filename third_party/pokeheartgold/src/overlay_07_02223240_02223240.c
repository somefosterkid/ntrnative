#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "pokepic.h"
#include "sprite_system.h"
#include "overlay_07_shake_context.h"

void ov07_022227A8(XYTransformContext *ctx, s16 extentX, s16 extentY, u32 interval, u32 amount);
BOOL ov07_022227D8(XYTransformContext *ctx);

void ov07_02223240(SysTask *arg0, void *ptr) {
    ShakeContext *ctx = ptr;
    ManagedSprite *managedSprite;
    Pokepic *pokepic;
    s32 v1;
    s32 v4;
    s32 v5;
    u8 v0;

    v0 = ctx->common.state;
    switch (v0) {
    case 0:
        ov07_022227A8(&ctx->shake, ctx->shakeInfo.extentX, ctx->shakeInfo.extentY, ctx->shakeInfo.interval, (s32)ctx->shakeInfo.amount);
        ctx->common.state = (u8)(ctx->common.state + 1);
        return;
    case 1:
        if (ov07_022227D8(&ctx->shake) == 0) {
            ctx->common.state = (u8)(ctx->common.state + 1);
            return;
        }
        v1 = ctx->shakeInfo.targets;
        if ((v1 & 0x100) == 0x100) {
            v4 = 0;
            if ((s32)ctx->spriteCount > 0) {
                do {
                    pokepic = ctx->sprites[v4].monSprite;
                    if (pokepic != NULL) {
                        Pokepic_SetAttr(pokepic, 0, ctx->shake.x + ctx->sprites[v4].pos.x);
                        Pokepic_SetAttr(ctx->sprites[v4].monSprite, 1, ctx->shake.y + ctx->sprites[v4].pos.y);
                    }
                    v4 += 1;
                } while (v4 < (s32)ctx->spriteCount);
            }
        } else if ((v1 & 0x200) == 0x200) {
            v5 = 0;
            if ((s32)ctx->spriteCount > 0) {
                do {
                    managedSprite = ctx->sprites[v5].hwSprite;
                    if (managedSprite != NULL) {
                        ManagedSprite_SetPositionXY(managedSprite, (s16)(ctx->shake.x + ctx->sprites[v5].pos.x), (s16)(ctx->shake.y + ctx->sprites[v5].pos.y));
                        ManagedSprite_TickFrame(ctx->sprites[v5].hwSprite);
                    }
                    v5 += 1;
                } while (v5 < (s32)ctx->spriteCount);
            }
            SpriteSystem_DrawSprites(ctx->common.pokemonSpriteManager);
        }
        if ((ctx->shakeInfo.targets & 0x400) == 0x400) {
            BgSetPosTextAndCommit(ctx->common.bgConfig, 3U, BG_POS_OP_SET_ROT, (s32)ctx->shake.x);
            return;
        }
        return;
    default:
        ov07_0221C448(ctx->common.battleAnimSys, arg0);
        Heap_Free(ptr);
        break;
    }
}
