#include "global.h"

#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "sprite_system.h"
#include "bg_window.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_value_lerp_context.h"

struct BattleAnimScriptFuncCommon;

struct BattleAnimSystem;

struct BgConfig;

struct FollowMeSpriteContext;

struct ManagedSprite;

struct PaletteData;

struct SpriteManager;

struct SpriteSystem;

struct SysTask;

struct SysTaskQueue;

struct ValueLerpContext;

enum AffineOverwriteMode {
    AFFINE_OVERWRITE_MODE_NONE = 0,
    AFFINE_OVERWRITE_MODE_NORMAL,
    AFFINE_OVERWRITE_MODE_DOUBLE,
};

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

int ov07_0221FAE8(BattleAnimSystem *system);

int ov07_0221FB04(BattleAnimSystem *system, int bg);

void ov07_022223F0(ValueLerpContext *ctx, s32 start, s32 end, u32 steps);

BOOL ov07_02222440(ValueLerpContext *ctx);

typedef struct FollowMeSpriteContext {
    BattleAnimScriptFuncCommon common;
    ManagedSprite *sprite;
    ValueLerpContext tilt;
    int state;
} FollowMeSpriteContext;

enum FollowMeSpriteState {
    FOLLOW_ME_SPRITE_STATE_OSCILLATE = 0,
    FOLLOW_ME_SPRITE_STATE_TILT,
};

extern const s16 ov07_02236670[43];

void ov07_02226880(SysTask *task, void *param);

void ov07_02226880(SysTask *task, void *param) {
    FollowMeSpriteContext *ctx = param;

    switch (ctx->common.state) {
    case FOLLOW_ME_SPRITE_STATE_OSCILLATE: {
        s16 offsetX = ov07_02236670[ctx->state];

        ctx->state++;

        if (offsetX != 255) {
            if (offsetX == 0) {
                int explicitPrio = ManagedSprite_GetPriority(ctx->sprite);
                if (explicitPrio == ov07_0221FAE8(ctx->common.battleAnimSys)) {
                    int bgPrio = ov07_0221FB04(ctx->common.battleAnimSys, 2);
                    ManagedSprite_SetPriority(ctx->sprite, bgPrio);
                } else {
                    int monPrio = ov07_0221FAE8(ctx->common.battleAnimSys);
                    ManagedSprite_SetPriority(ctx->sprite, monPrio);
                }
            }

            ManagedSprite_OffsetPositionXY(ctx->sprite, offsetX, 0);
        } else {
            ManagedSprite_SetAffineOverwriteMode(ctx->sprite, AFFINE_OVERWRITE_MODE_DOUBLE);
            ov07_022223F0(&ctx->tilt, (((0) * 0xFFFF) / 360), +(((20) * 0xFFFF) / 360), 4);
            ManagedSprite_SetAffineTranslation(ctx->sprite, -8, 16);
            ctx->state = 0;
            ctx->common.state++;
        }
        break;
    }
    case FOLLOW_ME_SPRITE_STATE_TILT: {
        if (ov07_02222440(&ctx->tilt) == 1) {
            ManagedSprite_SetAffineZRotation(ctx->sprite, ctx->tilt.value);
        } else {
            if (ctx->state > 5) {
                ctx->common.state++;
            } else {
                ctx->state++;

                switch (ctx->state) {
                case 1:
                    ov07_022223F0(&ctx->tilt, +(((20) * 0xFFFF) / 360), -(((20) * 0xFFFF) / 360), 4);
                    break;
                case 2:
                    ov07_022223F0(&ctx->tilt, -(((20) * 0xFFFF) / 360), +(((20) * 0xFFFF) / 360), 4);
                    break;
                case 3:
                    ov07_022223F0(&ctx->tilt, +(((20) * 0xFFFF) / 360), -(((20) * 0xFFFF) / 360), 4);
                    break;
                case 4:
                    ov07_022223F0(&ctx->tilt, -(((20) * 0xFFFF) / 360), +(((20) * 0xFFFF) / 360), 4);
                    break;
                case 5:
                    ov07_022223F0(&ctx->tilt, +(((20) * 0xFFFF) / 360), (((0) * 0xFFFF) / 360), 4 / 2);
                    break;
                }
            }
        }
        break;
    }
    default:
        Sprite_DeleteAndFreeResources(ctx->sprite);
        ov07_0221C448(ctx->common.battleAnimSys, task);
        Heap_Free(ctx);
        return;
    }

    ManagedSprite_TickFrame(ctx->sprite);
    SpriteSystem_DrawSprites(ctx->common.primarySpriteManager);
}
