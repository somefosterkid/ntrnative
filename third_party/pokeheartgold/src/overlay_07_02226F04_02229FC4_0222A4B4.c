#include "global.h"

#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"

struct BattleAnimSystem;

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

typedef struct AssistSpriteContext {
    u8 state;
    u8 unused0;
    u8 pawCount;
    u8 sLotoId;
    u8 unused2;
    BattleAnimSystem *battleAnimSys;
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    AssistPawSprite paws[20];
    int pawStates[20];
} AssistSpriteContext;

enum AssistSpriteState {
    ASSIST_SPRITE_STATE_WAIT_FOR_SCALE = 0,
    ASSIST_SPRITE_STATE_BEGIN_MOVE,
    ASSIST_SPRITE_STATE_WAIT_FOR_DONE,
    ASSIST_SPRITE_STATE_CLEANUP,
};

void ov07_0222A4B4(SysTask *task, void *param);

void ov07_0222A4B4(SysTask *task, void *param) {
    int i;
    AssistSpriteContext *ctx = param;

    switch (ctx->state) {
    case ASSIST_SPRITE_STATE_WAIT_FOR_SCALE: {
        BOOL scaleDone = TRUE;

        for (i = 0; i < ctx->pawCount; i++) {
            if (ctx->pawStates[i] != ASSIST_PAW_SPRITE_OUTER_STATE_SCALE_DONE) {
                scaleDone = FALSE;
                break;
            }
        }

        if (scaleDone == TRUE) {
            ctx->state++;
        }
        break;
    }
    case ASSIST_SPRITE_STATE_BEGIN_MOVE:
        for (i = 0; i < ctx->pawCount; i++) {
            ctx->pawStates[i] = ASSIST_PAW_SPRITE_OUTER_STATE_MOVE;
        }

        ctx->state++;
        break;
    case ASSIST_SPRITE_STATE_WAIT_FOR_DONE: {
        BOOL done = TRUE;

        for (i = 0; i < ctx->pawCount; i++) {
            if (ctx->pawStates[i] != ASSIST_PAW_SPRITE_OUTER_STATE_DONE) {
                done = FALSE;
                break;
            }
        }

        if (done == TRUE) {
            ctx->state++;
        }
        break;
    }
    case ASSIST_SPRITE_STATE_CLEANUP:
        for (i = 0; i < ctx->pawCount; i++) {
            Sprite_DeleteAndFreeResources(ctx->paws[i].sprite);
        }

        ctx->state++;
        break;
    default:
        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        return;
    }

    SpriteSystem_DrawSprites(ctx->spriteMan);
}
