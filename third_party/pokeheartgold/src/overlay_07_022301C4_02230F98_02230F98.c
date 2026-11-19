#include "global.h"

#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "struct_defs/resdat_narc_structs.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_alpha_fade_context.h"

struct BattleAnimSystem;

struct ManagedSprite;

struct SpriteManager;

struct SpriteSystem;

struct SysTask;

struct SysTaskQueue;

void ManagedSprite_SetDrawFlag(ManagedSprite *managedSprite, BOOL draw);

void ManagedSprite_SetPriority(ManagedSprite *managedSprite, int priority);

void ManagedSprite_SetOamMode(ManagedSprite *managedSprite, GXOamMode mode);

typedef struct FreezeSpriteContext {
    BattleAnimSystem *battleAnimSys;
    int state;
    int paletteOffset;
    SpriteManager *spriteMan;
    ManagedSprite *sprite;
    AlphaFadeContext alphaFade;
} FreezeSpriteContext;

static void ov07_02230FE8(SysTask *task, void *param);

void ov07_02230F98(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite);

void ov07_02230F98(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite) {
    FreezeSpriteContext *ctx = ov07_022324D8(system, sizeof(FreezeSpriteContext));

    ctx->battleAnimSys = system;
    ctx->spriteMan = spriteMan;
    ctx->sprite = sprite;

    ManagedSprite_SetDrawFlag(ctx->sprite, 0);
    ManagedSprite_SetOamMode(ctx->sprite, GX_OAM_MODE_XLU);
    ManagedSprite_SetDrawPriority(ctx->sprite, 100);
    ManagedSprite_SetPriority(ctx->sprite, 1);

    ctx->paletteOffset = ManagedSprite_GetPaletteOverrideOffset(ctx->sprite);

    ov07_0221C410(ctx->battleAnimSys, ov07_02230FE8, ctx);
}

void SpriteSystem_DrawSprites(SpriteManager *spriteMan);

void Sprite_DeleteAndFreeResources(ManagedSprite *managedSprite);

void ManagedSprite_SetAnimateFlag(ManagedSprite *managedSprite, BOOL animate);

void ManagedSprite_SetAnimSpeed(ManagedSprite *managedSprite, fx32 speed);

BOOL ManagedSprite_IsAnimated(ManagedSprite *managedSprite);

u16 ManagedSprite_GetAnimationFrame(ManagedSprite *managedSprite);

void ov07_02231E08(BattleAnimSystem *system, int spriteAlpha, int bgAlpha);

enum FreezeSpriteState {
    FREEZE_SPRITE_STATE_INIT = 0,
    FREEZE_SPRITE_STATE_FADEIN,
    FREEZE_SPRITE_STATE_ROTATE_PALS,
    FREEZE_SPRITE_STATE_FADEOUT,
    FREEZE_SPRITE_STATE_CLEANUP,
};

static void ov07_02230FE8(SysTask *task, void *param) {
    FreezeSpriteContext *ctx = param;

    switch (ctx->state) {
    case FREEZE_SPRITE_STATE_INIT:
        ov07_02222AC4(
            &ctx->alphaFade,
            0,
            16,
            20 - 0,
            20 - 16,
            10);
        ManagedSprite_SetDrawFlag(ctx->sprite, 1);
        ov07_02231E08(
            ctx->battleAnimSys,
            0,
            20 - 0);
        ctx->state++;
        break;
    case FREEZE_SPRITE_STATE_FADEIN:
        if (ov07_02222AF4(&ctx->alphaFade)) {
            ctx->state++;

            ManagedSprite_SetAnimateFlag(ctx->sprite, 1);
            ManagedSprite_SetAnimSpeed(ctx->sprite, ((fx32)0x0000000000001000L));
        }
        break;
    case FREEZE_SPRITE_STATE_ROTATE_PALS: {
        int animFrame = ManagedSprite_GetAnimationFrame(ctx->sprite);
        animFrame %= 3;

        ManagedSprite_SetPaletteOverrideOffset(ctx->sprite, ctx->paletteOffset + animFrame);

        if (!ManagedSprite_IsAnimated(ctx->sprite)) {
            ctx->state++;
            ov07_02222AC4(
                &ctx->alphaFade,
                16,
                0,
                20 - 16,
                20 - 0,
                8);
        }
        break;
    }
    case FREEZE_SPRITE_STATE_FADEOUT:
        if (ov07_02222AF4(&ctx->alphaFade)) {
            ctx->state++;
            ManagedSprite_SetDrawFlag(ctx->sprite, 0);
        }
        break;
    case FREEZE_SPRITE_STATE_CLEANUP:
        Sprite_DeleteAndFreeResources(ctx->sprite);
        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        return;
    }

    SpriteSystem_DrawSprites(ctx->spriteMan);
}

ManagedSprite *SpriteSystem_NewSprite(SpriteSystem *spriteSys, SpriteManager *spriteMan, const ManagedSpriteTemplate *template);

void ManagedSprite_SetPositionXY(ManagedSprite *managedSprite, s16 x, s16 y);

ManagedSpriteTemplate ov07_0221F9E8(BattleAnimSystem *system);

enum BattleAnimPositionType {
    BATTLE_ANIM_POSITION_MON_X = 0,
    BATTLE_ANIM_POSITION_MON_Y,
    BATTLE_ANIM_POSITION_PARTICLE_X,
    BATTLE_ANIM_POSITION_PARTICLE_Y,
};

s16 ov07_02221F80(BattleAnimSystem *system, int battler, enum BattleAnimPositionType posType);

typedef struct BurnSpriteContext {
    BattleAnimSystem *battleAnimSys;
    int state;
    SpriteManager *spriteMan;
    ManagedSprite *sprites[6];
    int visibleFrames[6];
    int delays[6];
} BurnSpriteContext;

void ov07_02231234(SysTask *task, void *param);

void ov07_022311B0(ManagedSprite *sprite, int *outVisibleFrames, int *outDelay, int idx, int dir);

void ov07_022310E0(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite);

void ov07_022310E0(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite) {
    BurnSpriteContext *ctx = ov07_022324D8(system, sizeof(BurnSpriteContext));

    ctx->battleAnimSys = system;
    ctx->spriteMan = spriteMan;

    int attacker = ov07_0221C468(ctx->battleAnimSys);
    int dir = ov07_02222004(system, attacker);

    ManagedSpriteTemplate template;
    template = ov07_0221F9E8(ctx->battleAnimSys);
    template.x = ov07_02221F80(system, attacker, BATTLE_ANIM_POSITION_MON_X);
    template.y = ov07_02221F80(system, attacker, BATTLE_ANIM_POSITION_MON_Y);

    for (int i = 6 - 1; i >= 0; i--) {
        if (i == 6 - 1) {
            ctx->sprites[i] = sprite;
            ManagedSprite_SetPositionXY(sprite, template.x, template.y);
        } else {
            ctx->sprites[i] = SpriteSystem_NewSprite(spriteSys, spriteMan, &template);
        }

        ManagedSprite_SetDrawPriority(ctx->sprites[i], 100);
        ManagedSprite_SetPriority(ctx->sprites[i], 1);
        ManagedSprite_SetDrawFlag(ctx->sprites[i], 0);

        ov07_022311B0(ctx->sprites[i], &ctx->visibleFrames[i], &ctx->delays[i], i, dir);
    }

    ov07_0221C410(ctx->battleAnimSys, ov07_02231234, ctx);
}
