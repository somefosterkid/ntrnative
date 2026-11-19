#include "global.h"
#include "bg_window.h"
#include "heap.h"
#include "palette.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "pokepic.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_move_battler_x_context.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_blink_attacker_context.h"

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
void ov07_0221C448(BattleAnimSystem *system, SysTask *task);
void ov07_02222268(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, u16 steps);
BOOL ov07_022222B4(XYTransformContext *ctx);
enum MoveBattlerXState {
    MOVE_BATTLER_X_STATE_INIT = 0,
    MOVE_BATTLER_X_STATE_MOVE,
};
void ov07_02224998(SysTask *task, void *param);

void ov07_02224948(BattleAnimSystem *system);

void ov07_02224948(BattleAnimSystem *system) {
    BlinkAttackerContext *ctx;

    ctx = ov07_022324D8(system, sizeof(BlinkAttackerContext));
    ctx->battleAnimSys = system;
    ctx->count = ov07_0221C4A8(system, 0) * 2;
    ctx->interval = ov07_0221C4A8(system, 1);
    ov07_02231FE4(system, &ctx->common);
    ctx->spriteInfo.monSprite = ov07_0221FA48(system, ov07_0221C468(system));
    ov07_0221C410(ctx->battleAnimSys, ov07_022248F4, ctx);
}

void ov07_02224998(SysTask *task, void *param) {
    MoveBattlerXContext *ctx = param;

    switch (ctx->common.state) {
    case MOVE_BATTLER_X_STATE_INIT:
        ov07_02222268(
            &ctx->pos,
            ctx->spriteInfo.pos.x,
            ctx->spriteInfo.pos.x + ctx->offset,
            ctx->spriteInfo.pos.y,
            ctx->spriteInfo.pos.y,
            ctx->frames);
        ctx->common.state++;
        break;
    case MOVE_BATTLER_X_STATE_MOVE:
        if (ov07_022222B4(&ctx->pos) == FALSE) {
            ctx->common.state++;
        }

        Pokepic_SetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_X_CENTER, ctx->pos.x);
        break;
    default:
        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        break;
    }
}
