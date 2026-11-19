#include "global.h"

#include "overlay_07_battle_anim_funcs.h"
#include "struct_defs/resdat_narc_structs.h"

#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "heap.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"

struct BattleAnimSystem;

ManagedSpriteTemplate ov07_0221F9E8(BattleAnimSystem *system);

enum BattleAnimPositionType {
    BATTLE_ANIM_POSITION_MON_X = 0,
    BATTLE_ANIM_POSITION_MON_Y,
    BATTLE_ANIM_POSITION_PARTICLE_X,
    BATTLE_ANIM_POSITION_PARTICLE_Y,
};

s16 ov07_02221F80(BattleAnimSystem *system, int battler, enum BattleAnimPositionType posType);

typedef struct SleepSpriteContext {
    BattleAnimSystem *battleAnimSys;
    int state;
    int delay;
    SpriteManager *spriteMan;
    int dir;
    ManagedSprite *sprites[2];
    XYTransformContext pos[2];
    XYTransformContext scale[2];
} SleepSpriteContext;

void ov07_02230E20(SysTask *task, void *param);

void ov07_02230D74(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite);

enum SleepSpriteState {
    SLEEP_SPRITE_STATE_INIT_FIRST = 0,
    SLEEP_SPRITE_STATE_DELAY,
    SLEEP_SPRITE_STATE_INIT_SECOND,
    SLEEP_SPRITE_STATE_ANIMATE,
    SLEEP_SPRITE_STATE_CLEANUP,
};
void ov07_02230F0C(ManagedSprite *sprite, XYTransformContext *pos, XYTransformContext *scale, int dir);
BOOL ov07_02230F6C(ManagedSprite *managedSprite, XYTransformContext *pos, XYTransformContext *scale);
enum AffineOverwriteMode {
    AFFINE_OVERWRITE_MODE_NONE = 0,
    AFFINE_OVERWRITE_MODE_NORMAL,
    AFFINE_OVERWRITE_MODE_DOUBLE,
};
void ov07_02222268(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, u16 steps);
void ov07_02222508(XYTransformContext *ctx, s32 startScale, s32 refScale, s32 endScale, u32 steps);

void ov07_02230D74(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite) {
    SleepSpriteContext *ctx = ov07_022324D8(system, sizeof(SleepSpriteContext));

    ctx->battleAnimSys = system;
    ctx->spriteMan = spriteMan;

    int attacker = ov07_0221C468(system);
    ctx->dir = ov07_02222004(system, attacker);

    ManagedSpriteTemplate template;
    template = ov07_0221F9E8(ctx->battleAnimSys);
    template.x = ov07_02221F80(system, attacker, BATTLE_ANIM_POSITION_MON_X);
    template.y = ov07_02221F80(system, attacker, BATTLE_ANIM_POSITION_MON_Y);

    for (int i = 0; i < 2; i++) {
        if (i == 0) {
            ctx->sprites[i] = sprite;
            ManagedSprite_SetPositionXY(ctx->sprites[i], template.x, template.y);
        } else {
            ctx->sprites[i] = SpriteSystem_NewSprite(spriteSys, spriteMan, &template);
        }

        ManagedSprite_SetDrawFlag(ctx->sprites[i], 0);
        ManagedSprite_SetDrawPriority(ctx->sprites[i], 100);
        ManagedSprite_SetPriority(ctx->sprites[i], 1);
    }

    ov07_0221C410(ctx->battleAnimSys, ov07_02230E20, ctx);
}

void ov07_02230E20(SysTask *task, void *param) {
    SleepSpriteContext *ctx = param;

    switch (ctx->state) {
    case SLEEP_SPRITE_STATE_INIT_FIRST:
        ov07_02230F0C(ctx->sprites[0], &ctx->pos[0], &ctx->scale[0], ctx->dir);
        ov07_02230F6C(ctx->sprites[0], &ctx->pos[0], &ctx->scale[0]);
        ctx->state++;
        ctx->delay = 8;
        break;
    case SLEEP_SPRITE_STATE_DELAY:
        ctx->delay--;
        ov07_02230F6C(ctx->sprites[0], &ctx->pos[0], &ctx->scale[0]);

        if (ctx->delay < 0) {
            ctx->state++;
        }
        break;
    case SLEEP_SPRITE_STATE_INIT_SECOND:
        ov07_02230F0C(ctx->sprites[1], &ctx->pos[1], &ctx->scale[1], ctx->dir);
        ov07_02230F6C(ctx->sprites[1], &ctx->pos[1], &ctx->scale[1]);
        ov07_02230F6C(ctx->sprites[0], &ctx->pos[0], &ctx->scale[0]);
        ctx->state++;
        break;
    case SLEEP_SPRITE_STATE_ANIMATE:
        ov07_02230F6C(ctx->sprites[0], &ctx->pos[0], &ctx->scale[0]);
        if (ov07_02230F6C(ctx->sprites[1], &ctx->pos[1], &ctx->scale[1])) {
            ctx->state++;
        }
        break;
    case SLEEP_SPRITE_STATE_CLEANUP:
        for (int i = 0; i < 2; i++) {
            Sprite_DeleteAndFreeResources(ctx->sprites[i]);
        }

        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        return;
    }

    SpriteSystem_DrawSprites(ctx->spriteMan);
}

void ov07_02230F0C(ManagedSprite *sprite, XYTransformContext *pos, XYTransformContext *scale, int dir) {
    ManagedSprite_SetAffineOverwriteMode(sprite, AFFINE_OVERWRITE_MODE_DOUBLE);
    ManagedSprite_SetDrawFlag(sprite, 1);

    s16 startX, startY;
    ManagedSprite_GetPositionXY(sprite, &startX, &startY);

    ov07_02222268(
        pos,
        startX,
        startX + 16 * dir,
        startY,
        startY + (-32),
        32);

    ov07_02222508(
        scale,
        2,
        10,
        10,
        32);
}
