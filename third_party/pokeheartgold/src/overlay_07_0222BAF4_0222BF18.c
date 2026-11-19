#include "global.h"

#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_drill_peck_context.h"
#include "pokepic.h"
#include "sys_task.h"

enum PokemonSpriteAttribute {
    MON_SPRITE_X_CENTER = 0,
    MON_SPRITE_Y_CENTER = 1,
    MON_SPRITE_ROTATION_Z = 9,
};

void ov07_022220B8(XYTransformContext *ctx, Pokepic *sprite, s16 cx, s16 cy);

void ov07_02222268(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, u16 steps);

BOOL ov07_022222B4(XYTransformContext *ctx);

void ov07_022223F0(ValueLerpContext *ctx, s32 start, s32 end, u32 steps);

BOOL ov07_02222440(ValueLerpContext *ctx);

void ov07_0222BF18(SysTask *task, void *param);

void ov07_0222BF18(SysTask *task, void *param) {
    DrillPeckContext *ctx = param;

    switch (ctx->state) {
    case DRILL_PECK_STATE_MOVE_BWD:
        if (ov07_022222B4(&ctx->pos)) {
            ov07_022220B8(&ctx->pos, ctx->attackerSprite, ctx->attackerX, ctx->attackerY);
            return;
        }
        ov07_02222268(&ctx->pos, ctx->pos.x, 0, 0, 0, 2);
        ctx->state++;
        return;
    case DRILL_PECK_STATE_ROTATE_FWD:
        if (ov07_02222440(&ctx->angle)) {
            Pokepic_SetAttr(ctx->attackerSprite, MON_SPRITE_ROTATION_Z, ctx->angle.value);
            return;
        }
        ov07_022223F0(&ctx->angle, ctx->angle.value, 0, 4);
        ctx->state++;
        ctx->delay = 2;
        return;
    case DRILL_PECK_STATE_DELAY_1:
        ctx->delay--;
        if (ctx->delay < 0) {
            ctx->state++;
            return;
        }
        return;
    case DRILL_PECK_STATE_MOVE_FWD:
        if (ov07_022222B4(&ctx->pos)) {
            ov07_022220B8(&ctx->pos, ctx->attackerSprite, ctx->attackerX, ctx->attackerY);
            return;
        }
        ctx->state++;
        ctx->delay = 32;
        return;
    case DRILL_PECK_STATE_DELAY_2:
        ctx->delay--;
        if (ctx->delay < 0) {
            ctx->state++;
            return;
        }
        break;
    case DRILL_PECK_STATE_ROTATE_BWD:
        if (ov07_02222440(&ctx->angle)) {
            Pokepic_SetAttr(ctx->attackerSprite, MON_SPRITE_ROTATION_Z, ctx->angle.value);
            return;
        }
        ctx->state++;
        return;
    case DRILL_PECK_STATE_CLEANUP:
        Pokepic_SetAttr(ctx->attackerSprite, MON_SPRITE_X_CENTER, ctx->attackerX);
        Pokepic_SetAttr(ctx->attackerSprite, MON_SPRITE_Y_CENTER, ctx->attackerY);
        Pokepic_SetAttr(ctx->attackerSprite, MON_SPRITE_ROTATION_Z, 0);
        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        break;
    }
}
