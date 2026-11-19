#include "global.h"

#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "pokemon_types_def.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_megahorn_defender_context.h"
#include "pokepic.h"

struct BattleAnimSystem;

struct Pokepic;

struct PokepicDrawParam;

enum PokemonSpriteAttribute {
    MON_SPRITE_X_CENTER = 0,
    MON_SPRITE_Y_CENTER = 1,
    MON_SPRITE_Z_CENTER = 2,
    MON_SPRITE_X_OFFSET = 3,
    MON_SPRITE_Y_OFFSET = 4,
    MON_SPRITE_Z_OFFSET = 5,
    MON_SPRITE_HIDE = 6,
    MON_SPRITE_ROTATION_X = 7,
    MON_SPRITE_ROTATION_Y = 8,
    MON_SPRITE_ROTATION_Z = 9,
    MON_SPRITE_X_PIVOT = 10,
    MON_SPRITE_Y_PIVOT = 11,
    MON_SPRITE_SCALE_X = 12,
    MON_SPRITE_SCALE_Y = 13,
    MON_SPRITE_PARTIAL_DRAW = 14,
    MON_SPRITE_DRAW_X_OFFSET = 15,
    MON_SPRITE_DRAW_Y_OFFSET = 16,
    MON_SPRITE_DRAW_WIDTH = 17,
    MON_SPRITE_DRAW_HEIGHT = 18,
    MON_SPRITE_SHADOW_X = 19,
    MON_SPRITE_SHADOW_Y = 20,
    MON_SPRITE_SHADOW_X_OFFSET = 21,
    MON_SPRITE_SHADOW_Y_OFFSET = 22,
    MON_SPRITE_ALPHA = 23,
    MON_SPRITE_DIFFUSE_R = 24,
    MON_SPRITE_DIFFUSE_G = 25,
    MON_SPRITE_DIFFUSE_B = 26,
    MON_SPRITE_AMBIENT_R = 27,
    MON_SPRITE_AMBIENT_G = 28,
    MON_SPRITE_AMBIENT_B = 29,
    MON_SPRITE_FADE_ACTIVE = 30,
    MON_SPRITE_FADE_TARGET_COLOR = 31,
    MON_SPRITE_FADE_INIT_ALPHA = 32,
    MON_SPRITE_FADE_TARGET_ALPHA = 33,
    MON_SPRITE_FADE_DELAY_COUNTER = 34,
    MON_SPRITE_FLIP_H = 35,
    MON_SPRITE_FLIP_V = 36,
    MON_SPRITE_HIDE_2 = 37,
    MON_SPRITE_CURR_SPRITE_FRAME = 38,
    MON_SPRITE_DUMMY = 39,
    MON_SPRITE_MOSAIC_INTENSITY = 40,
    MON_SPRITE_SHADOW_HEIGHT = 41,
    MON_SPRITE_SHADOW_PLTT_SLOT = 42,
    MON_SPRITE_SHADOW_SHOULD_FOLLOW_X = 43,
    MON_SPRITE_SHADOW_SHOULD_FOLLOW_Y = 44,
    MON_SPRITE_SHADOW_IS_AFFINE = 45,
    MON_SPRITE_SHADOW_SIZE = 46,
};

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);



void ov07_02222268(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, u16 steps);

BOOL ov07_02222314(XYTransformContext *ctx, Pokepic *sprite);

void ov07_022227A8(XYTransformContext *ctx, s16 extentX, s16 extentY, u32 interval, u32 amount);

BOOL ov07_0222283C(XYTransformContext *ctx, s32 cx, s32 cy, Pokepic *sprite);

enum MegahornDefenderState {
    MEGAHORN_DEFENDER_STATE_INIT = 0,
    MEGAHORN_DEFENDER_STATE_MOVE,
    MEGAHORN_DEFENDER_STATE_SHAKE,
    MEGAHORN_DEFENDER_STATE_MOVE_BACK,
    MEGAHORN_DEFENDER_STATE_CLEANUP,
};

void ov07_02230BF8(SysTask *task, void *param);

void ov07_02230BF8(SysTask *task, void *param) {
    MegahornDefenderContext *ctx = param;
    s16 baseX, baseY;

    switch (ctx->state) {
    case MEGAHORN_DEFENDER_STATE_INIT:
        ov07_02222268(
            &ctx->pos,
            ctx->x,
            ctx->x + ((-40) * ctx->dirX),
            ctx->y,
            ctx->y + (16 * ctx->dirY),
            4);
        ctx->state++;
        break;
    case MEGAHORN_DEFENDER_STATE_MOVE:
        if (ov07_02222314(&ctx->pos, ctx->sprite) == 0) {
            ctx->state++;
            ov07_022227A8(
                &ctx->shake,
                4,
                0,
                1,
                4);
        }
        break;
    case MEGAHORN_DEFENDER_STATE_SHAKE:
        baseX = Pokepic_GetAttr(ctx->sprite, MON_SPRITE_X_CENTER);
        baseY = Pokepic_GetAttr(ctx->sprite, MON_SPRITE_Y_CENTER);

        if (ov07_0222283C(&ctx->shake, baseX, baseY, ctx->sprite) == 0) {
            ctx->state++;
            ov07_02222268(
                &ctx->pos,
                ctx->x + ((-40) * ctx->dirX),
                ctx->x,
                ctx->y + (16 * ctx->dirY),
                ctx->y,
                4);
        }
        break;
    case MEGAHORN_DEFENDER_STATE_MOVE_BACK:
        if (ov07_02222314(&ctx->pos, ctx->sprite) == 0) {
            ctx->state++;
        }
        break;
    case MEGAHORN_DEFENDER_STATE_CLEANUP:
        Pokepic_SetAttr(ctx->sprite, MON_SPRITE_X_CENTER, ctx->x);
        Pokepic_SetAttr(ctx->sprite, MON_SPRITE_Y_CENTER, ctx->y);
        Heap_Free(ctx);
        ov07_0221C448(ctx->battleAnimSys, task);
        break;
    }
}
