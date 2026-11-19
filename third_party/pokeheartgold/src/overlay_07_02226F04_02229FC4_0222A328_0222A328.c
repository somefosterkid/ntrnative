#include "global.h"

#include "math_util.h"
#include "overlay_07_battle_anim_funcs.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"

struct AssistPawSprite;

struct BattleAnimSystem;

struct ManagedSprite;

struct Point2D;

struct SpriteManager;

struct SpriteSystem;

struct SysTask;

struct SysTaskQueue;

struct XYTransformContext;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

void ov07_02222268(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, u16 steps);

typedef struct AssistPawSprite {
    BattleAnimSystem *battleAnimSys;
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    u8 state;
    u8 step;
    u8 delay;
    u8 destIndex;
    ManagedSprite *sprite;
    XYTransformContext pos;
    f32 scale;
    int *outerState;
} AssistPawSprite;

enum AssistPawSpriteState {
    ASSIST_PAW_SPRITE_STATE_DELAY_1 = 0,
    ASSIST_PAW_SPRITE_STATE_ENABLE_DRAW,
    ASSIST_PAW_SPRITE_STATE_GROW,
    ASSIST_PAW_SPRITE_STATE_SHRINK,
    ASSIST_PAW_SPRITE_STATE_ADVANCE_OUTER_1,
    ASSIST_PAW_SPRITE_STATE_DELAY_2,
    ASSIST_PAW_SPRITE_STATE_SETUP_MOVE,
    ASSIST_PAW_SPRITE_STATE_MOVE,

    ASSIST_PAW_SPRITE_OUTER_STATE_INIT = 0,
    ASSIST_PAW_SPRITE_OUTER_STATE_SCALE_DONE,
    ASSIST_PAW_SPRITE_OUTER_STATE_MOVE,
    ASSIST_PAW_SPRITE_OUTER_STATE_DONE,
};

extern const Point2D ov07_0223677E[6];

s32 ov07_022222F0(void *arg0, void *arg1);

void ov07_0222A328(SysTask *task, void *param);

void ov07_0222A328(SysTask *task, void *param) {
    AssistPawSprite *paw = param;

    switch (paw->state) {
    case ASSIST_PAW_SPRITE_STATE_DELAY_1:
        if (--paw->delay <= 10) {
            paw->state++;
        }
        break;
    case ASSIST_PAW_SPRITE_STATE_ENABLE_DRAW:
        if (++paw->step >= 3) {
            ManagedSprite_SetDrawFlag(paw->sprite, 1);
            paw->step = 0;
            paw->state++;
        }
        break;
    case ASSIST_PAW_SPRITE_STATE_GROW:
        if (++paw->step <= 3) {
            paw->scale += 0.1f;
            ManagedSprite_SetAffineScale(paw->sprite, paw->scale, paw->scale);
        } else {
            paw->step = 0;
            paw->state++;
        }
        break;
    case ASSIST_PAW_SPRITE_STATE_SHRINK:
        if (++paw->step <= 3) {
            paw->scale -= 0.1f;
            ManagedSprite_SetAffineScale(paw->sprite, paw->scale, paw->scale);
        } else {
            paw->step = 0;
            paw->state++;
        }
        break;
    case ASSIST_PAW_SPRITE_STATE_ADVANCE_OUTER_1:
        if (--paw->delay <= 10) {
            *paw->outerState = ASSIST_PAW_SPRITE_OUTER_STATE_SCALE_DONE;
            paw->step = 0;
            paw->state++;
        }
        break;
    case ASSIST_PAW_SPRITE_STATE_DELAY_2:
        if (++paw->step > 30) {
            paw->step = 0;
            paw->state++;
            break;
        }

        if (*paw->outerState == ASSIST_PAW_SPRITE_OUTER_STATE_MOVE) {
            paw->state++;
        }
        break;
    case ASSIST_PAW_SPRITE_STATE_SETUP_MOVE: {
        Point2D pos;
        int frames = 10 + (LCRandom() % 10);

        ManagedSprite_GetPositionXY(paw->sprite, &pos.x, &pos.y);
        ov07_02222268(
            &paw->pos,
            pos.x,
            ov07_0223677E[paw->destIndex].x,
            pos.y,
            ov07_0223677E[paw->destIndex].y,
            frames);
        paw->state++;
        break;
    }
    case ASSIST_PAW_SPRITE_STATE_MOVE: {
        BOOL active = (s32)(ov07_022222F0((u8 *)(&paw->pos), (u8 *)(paw->sprite)));
        if (active == 0) {
            ManagedSprite_SetDrawFlag(paw->sprite, 0);
            paw->state++;
        } else {
            break;
        }
    }
    default:
        *(paw->outerState) = ASSIST_PAW_SPRITE_OUTER_STATE_DONE;
        ov07_0221C448(paw->battleAnimSys, task);
        break;
    }

    ManagedSprite_TickFrame(paw->sprite);
}
