#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_alpha_fade_context.h"
#include "overlay_07_grudge_sprite_context.h"

struct BattleAnimSystem;

int ov07_0221FAE8(BattleAnimSystem *system);

enum XYTransformParam {
    XY_PARAM_STEPS = 0,
    XY_PARAM_STEP_SIZE,
    XY_PARAM_STEP_SIZE_X = 1,
    XY_PARAM_STEP_SIZE_Y,
    XY_PARAM_CUR_X,
    XY_PARAM_CUR_Y,

    XY_PARAM_SHAKE_COUNTER = 0,
    XY_PARAM_SHAKE_INTERVAL,
    XY_PARAM_SHAKE_REMAINING,
    XY_PARAM_SHAKE_CYCLE,
    XY_PARAM_SHAKE_EXTENT_X,
    XY_PARAM_SHAKE_EXTENT_Y,
    XY_PARAM_SHAKE_PREV_X,
    XY_PARAM_SHAKE_PREV_Y,

    XY_PARAM_REV_STEPS = 0,
    XY_PARAM_REV_CUR_X,
    XY_PARAM_REV_RADIUS_X,
    XY_PARAM_REV_CUR_Y,
    XY_PARAM_REV_RADIUS_Y,
    XY_PARAM_REV_STEP_SIZE_X,
    XY_PARAM_REV_STEP_SIZE_Y,
};

BOOL ov07_02222180(XYTransformContext *ctx);

void ov07_02231300(GrudgeSpriteContext *ctx);

void ov07_02231300(GrudgeSpriteContext *ctx) {
    for (int i = 0; i < 6; i++) {
        ov07_02222180(&ctx->revs[i]);
        ManagedSprite_SetPositionXY(ctx->flameSprites[i], ctx->attackerX + ctx->revs[i].x, ctx->attackerY);

        if (ctx->revs[i].data[XY_PARAM_REV_CUR_X] >= (((90) * 0xFFFF) / 360)
            && ctx->revs[i].data[XY_PARAM_REV_CUR_X] <= (((269) * 0xFFFF) / 360)) {
            ManagedSprite_SetPriority(ctx->flameSprites[i], 1);
        } else {
            ManagedSprite_SetPriority(ctx->flameSprites[i], ov07_0221FAE8(ctx->battleAnimSys) + 1);
        }
    }
}
