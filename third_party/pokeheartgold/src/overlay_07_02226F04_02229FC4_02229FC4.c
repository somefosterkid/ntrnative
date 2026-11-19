#include "global.h"

#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"

struct BattleAnimSystem;

struct HelpingHandSpriteContext;

struct ManagedSprite;

struct Point2D;

struct Sprite;

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

typedef struct HelpingHandSpriteContext {
    u8 state;
    u8 unused0[6];
    u8 sLotoId;
    u8 handCount;
    u8 spriteAlpha;
    u8 bgAlpha;
    BattleAnimSystem *battleAnimSys;
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    ManagedSprite *handSprites[6];
    int currentPair;
    ManagedSprite *hand1;
    ManagedSprite *hand2;
    XYTransformContext hand1Pos;
    XYTransformContext hand2Pos;
} HelpingHandSpriteContext;

enum HelpingHandSpriteState {
    HELPING_HAND_SPRITE_STATE_FADE_IN = 0,
    HELPING_HAND_SPRITE_STATE_SETUP_MOVE,
    HELPING_HAND_SPRITE_STATE_MOVE,
    HELPING_HAND_SPRITE_STATE_FADE_OUT,
};

extern const Point2D ov07_022367AE[4][2];

extern const u8 ov07_02236774[4];

s32 ov07_022222F0(void *arg0, void *arg1);

void ov07_02229FC4(SysTask *task, void *param);

void ov07_02229FC4(SysTask *task, void *param) {
    int i;
    BOOL spriteAnimsDone = 0;
    HelpingHandSpriteContext *ctx = param;

    switch (ctx->state) {
    case HELPING_HAND_SPRITE_STATE_FADE_IN:
        if (ctx->spriteAlpha < 15) {
            ctx->spriteAlpha++;
        }

        if (ctx->bgAlpha > 0) {
            ctx->bgAlpha--;
        }

        if (ctx->spriteAlpha == 15 && ctx->bgAlpha == 0) {
            ctx->state++;
        }

        G2_ChangeBlendAlpha(ctx->spriteAlpha, ctx->bgAlpha);
        break;
    case HELPING_HAND_SPRITE_STATE_SETUP_MOVE: {
        switch (ctx->currentPair) {
        case 0:
            ctx->hand1 = ctx->handSprites[0];
            ctx->hand2 = ctx->handSprites[3];
            ManagedSprite_SetDrawFlag(ctx->hand1, 1);
            ManagedSprite_SetDrawFlag(ctx->hand2, 1);
            break;
        case 1:
            ctx->hand1 = ctx->handSprites[1];
            ctx->hand2 = ctx->handSprites[2];
            ManagedSprite_SetDrawFlag(ctx->hand1, 1);
            ManagedSprite_SetDrawFlag(ctx->hand2, 1);
            break;
        case 2:
        case 3:
            ctx->hand1 = ctx->handSprites[4];
            ctx->hand2 = ctx->handSprites[5];
            ManagedSprite_SetDrawFlag(ctx->hand1, 1);
            ManagedSprite_SetDrawFlag(ctx->hand2, 1);
            break;
        }

        s16 x, y;
        ManagedSprite_GetPositionXY(ctx->hand1, &x, &y);
        ov07_02222268(
            &ctx->hand1Pos,
            x,
            ov07_022367AE[ctx->currentPair][0].x,
            y,
            ov07_022367AE[ctx->currentPair][0].y,
            ov07_02236774[ctx->currentPair]);

        ManagedSprite_GetPositionXY(ctx->hand2, &x, &y);
        ov07_02222268(
            &ctx->hand2Pos,
            x,
            ov07_022367AE[ctx->currentPair][1].x,
            y,
            ov07_022367AE[ctx->currentPair][1].y,
            ov07_02236774[ctx->currentPair]);

        ctx->currentPair++;
        ctx->state++;
        break;
    }
    case HELPING_HAND_SPRITE_STATE_MOVE: {
        int doneCount = 0;
        BOOL active = (s32)(ov07_022222F0((u8 *)(&ctx->hand1Pos), (u8 *)(ctx->hand1)));
        if (active == 0) {
            doneCount++;
        }

        active = (s32)(ov07_022222F0((u8 *)(&ctx->hand2Pos), (u8 *)(ctx->hand2)));
        if (active == 0) {
            doneCount++;
        }

        if (doneCount >= 2) {
            if (ctx->currentPair <= 3) {
                ManagedSprite_SetDrawFlag(ctx->hand1, 0);
                ManagedSprite_SetDrawFlag(ctx->hand2, 0);
                ctx->state--;
            } else {
                ctx->state++;
            }
        }
        break;
    }
    case HELPING_HAND_SPRITE_STATE_FADE_OUT:
        if (ctx->spriteAlpha > 0) {
            ctx->spriteAlpha--;
        }

        if (ctx->bgAlpha < 15) {
            ctx->bgAlpha++;
        }

        if (ctx->bgAlpha >= 15 / 2) {
            spriteAnimsDone = 1;
        }

        if (ctx->spriteAlpha == 0 && ctx->bgAlpha == 15) {
            ctx->state++;
        }

        G2_ChangeBlendAlpha(ctx->spriteAlpha, ctx->bgAlpha);
        break;
    default:
        for (i = 0; i < ctx->handCount; i++) {
            Sprite_DeleteAndFreeResources(ctx->handSprites[i]);
        }

        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        return;
    }

    if (ctx->state < HELPING_HAND_SPRITE_STATE_FADE_OUT && spriteAnimsDone == 0) {
        for (i = 0; i < ctx->handCount; i++) {
            Sprite_TickFrame(ctx->handSprites[i]->sprite);
        }
    }

    SpriteSystem_DrawSprites(ctx->spriteMan);
}
