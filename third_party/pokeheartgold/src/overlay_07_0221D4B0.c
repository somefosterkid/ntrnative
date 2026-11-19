#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_74_main_menu_util.h"
#include "sys_task.h"
#include "pokepic.h"
#include "overlay_07_battle_anim_tracking.h"

struct BgConfig;
struct Pokepic;
struct PokepicDrawParam;
struct SysTask;
struct SysTaskQueue;
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
enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};
enum BgLayer {
    BG_LAYER_MAIN_0 = 0,
    BG_LAYER_MAIN_1,
    BG_LAYER_MAIN_2,
    BG_LAYER_MAIN_3,
    BG_LAYER_SUB_0,
    BG_LAYER_SUB_1,
    BG_LAYER_SUB_2,
    BG_LAYER_SUB_3,

    BG_LAYER_MAX,
};
enum BgOffsetUpdateOp {
    BG_OFFSET_UPDATE_SET_X = 0,
    BG_OFFSET_UPDATE_ADD_X,
    BG_OFFSET_UPDATE_SUB_X,
    BG_OFFSET_UPDATE_SET_Y,
    BG_OFFSET_UPDATE_ADD_Y,
    BG_OFFSET_UPDATE_SUB_Y,
};
typedef struct BgConfig BgConfig;
void BgSetPosTextAndCommit(BgConfig *bgConfig, u8 bgLayer, u8 bgOffsetUpdateOp, int val);

void ov07_0221D4B0(SysTask *task, void *param);
void ov07_0221D4FC(SysTask *task, void *param);

void ov07_0221D4B0(SysTask *task, void *param) {
    SpriteTrackingTask *trackingTask = param;

    if (trackingTask->data.interval != 0) {
        trackingTask->data.frameCount++;

        if (trackingTask->data.frameCount != trackingTask->data.interval) {
            return;
        }

        trackingTask->data.frameCount = 0;
    }

    s16 x = Pokepic_GetAttr(trackingTask->data.sprite, MON_SPRITE_X_CENTER);
    s16 y = Pokepic_GetAttr(trackingTask->data.sprite, MON_SPRITE_Y_CENTER);
    y -= Pokepic_GetAttr(trackingTask->data.sprite, MON_SPRITE_SHADOW_HEIGHT);

    ManagedSprite_SetPositionXY(trackingTask->sprite, x, y);
}

void ov07_0221D4FC(SysTask *task, void *param) {
    BgTrackingTask *trackingTask = param;

    if (trackingTask->data.interval != 0) {
        trackingTask->data.frameCount++;

        if (trackingTask->data.frameCount != trackingTask->data.interval) {
            return;
        }

        trackingTask->data.frameCount = 0;
    }

    s16 x = Pokepic_GetAttr(trackingTask->data.sprite, MON_SPRITE_X_CENTER);
    s16 y = Pokepic_GetAttr(trackingTask->data.sprite, MON_SPRITE_Y_CENTER);
    y -= Pokepic_GetAttr(trackingTask->data.sprite, MON_SPRITE_SHADOW_HEIGHT);

    BgSetPosTextAndCommit(trackingTask->bg, BG_LAYER_MAIN_2, BG_OFFSET_UPDATE_SET_X, -(x - 40));
    BgSetPosTextAndCommit(trackingTask->bg, BG_LAYER_MAIN_2, BG_OFFSET_UPDATE_SET_Y, -(y - 40));
}
