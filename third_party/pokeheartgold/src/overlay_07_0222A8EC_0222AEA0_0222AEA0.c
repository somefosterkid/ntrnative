#include "global.h"

#include "overlay_07_battle_anim_common.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_shadow_punch_context.h"

#include "bg_window.h"
#include "palette.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"

struct BattleAnimSystem;

enum AffineOverwriteMode {
    AFFINE_OVERWRITE_MODE_NONE = 0,
    AFFINE_OVERWRITE_MODE_NORMAL,
    AFFINE_OVERWRITE_MODE_DOUBLE,
};

u8 ov07_0221FA04(BattleAnimSystem *system, int battler);

int ov07_0221FAE8(BattleAnimSystem *system);

void ov07_02222590(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, s16 ref, u32 steps);

enum {
    DEFENDER = 0,
    DEFENDER_PARTNER,
    DEFENDER_COUNT,
};

typedef struct MimicContext {
    BattleAnimScriptFuncCommon common;
    BattleAnimSpriteInfo defenderSprite;
    BattleAnimSpriteInfo opponentSprites[DEFENDER_COUNT];
    s16 defenderY;
    s16 defenderHeight;
    s16 unused;
    XYTransformContext scale;
} MimicContext;

void ov07_0222AE14(SysTask *task, void *param);

void ov07_0222AEA0(BattleAnimSystem *system);

void ov07_0222AEA0(BattleAnimSystem *system) {
    MimicContext *ctx = ov07_022324D8(system, sizeof(MimicContext));

    ov07_02231FE4(system, &ctx->common);

    ctx->defenderSprite.monSprite = ov07_0221FA48(ctx->common.battleAnimSys, ov07_0221C470(ctx->common.battleAnimSys));
    ctx->defenderY = Pokepic_GetAttr(ctx->defenderSprite.monSprite, POKEPIC_Y);
    ctx->unused = ctx->defenderY;
    ctx->defenderHeight = ov07_0221FAA0(ctx->common.battleAnimSys, ov07_0221C470(ctx->common.battleAnimSys));
    ctx->defenderSprite.hwSprite = ov07_0221C4E8(system, 0);
    ctx->opponentSprites[DEFENDER].hwSprite = ov07_0221C4E8(system, 1);
    ctx->opponentSprites[DEFENDER_PARTNER].hwSprite = ov07_0221C4E8(system, 2);

    ManagedSprite_SetAffineOverwriteMode(ctx->defenderSprite.hwSprite, AFFINE_OVERWRITE_MODE_DOUBLE);
    ManagedSprite_SetPriority(ctx->defenderSprite.hwSprite, ov07_0221FAE8(system));
    ManagedSprite_SetPriority(ctx->opponentSprites[DEFENDER].hwSprite, ov07_0221FAE8(system));
    ManagedSprite_SetPriority(ctx->opponentSprites[DEFENDER_PARTNER].hwSprite, ov07_0221FAE8(system));

    int defenderType = ov07_0221FA04(ctx->common.battleAnimSys, ov07_0221C470(ctx->common.battleAnimSys));
    if (defenderType == 3 || defenderType == 4) {
        ManagedSprite_SetDrawPriority(ctx->defenderSprite.hwSprite, 30);
        ManagedSprite_SetDrawPriority(ctx->opponentSprites[DEFENDER].hwSprite, 50);
        ManagedSprite_SetDrawPriority(ctx->opponentSprites[DEFENDER_PARTNER].hwSprite, 70);
    } else {
        ManagedSprite_SetDrawPriority(ctx->defenderSprite.hwSprite, 60);
        ManagedSprite_SetDrawPriority(ctx->opponentSprites[DEFENDER].hwSprite, 70);
        ManagedSprite_SetDrawPriority(ctx->opponentSprites[DEFENDER_PARTNER].hwSprite, 50);
    }

    ov07_02222590(
        &ctx->scale,
        100,
        20,
        100,
        20,
        100,
        10);

    ov07_0221C410(ctx->common.battleAnimSys, ov07_0222AE14, ctx);
}

void ov07_02222268(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, u16 steps);

BOOL ov07_022222F0(XYTransformContext *ctx, ManagedSprite *sprite);

BOOL ov07_02222314(XYTransformContext *ctx, Pokepic *sprite);

void ov07_02232508(void *ptr);

enum ShadowPunchState {
    SHADOW_PUNCH_STATE_INIT = 0,
    SHADOW_PUNCH_STATE_MOVE,
};

void ov07_0222AFAC(SysTask *task, void *param);

void ov07_0222AFAC(SysTask *task, void *param) {
    ShadowPunchContext *ctx = param;

    switch (ctx->common.state) {
    case SHADOW_PUNCH_STATE_INIT:
        if (ctx->iteration == 0) {
            ov07_02222268(
                &ctx->mainPos,
                ctx->spriteInfo.pos.x,
                ctx->spriteInfo.pos.x + 48,
                ctx->spriteInfo.pos.y,
                ctx->spriteInfo.pos.y,
                3);
            ov07_02222268(
                &ctx->afterimage1Pos,
                ctx->spriteInfo.pos.x,
                ctx->spriteInfo.pos.x + 48,
                ctx->spriteInfo.pos.y + ctx->attackerHeight,
                ctx->spriteInfo.pos.y + ctx->attackerHeight,
                6);
            ov07_02222268(
                &ctx->afterimage2Pos,
                ctx->spriteInfo.pos.x,
                ctx->spriteInfo.pos.x + 48,
                ctx->spriteInfo.pos.y + ctx->attackerHeight,
                ctx->spriteInfo.pos.y + ctx->attackerHeight,
                8);
        } else {
            ov07_02222268(
                &ctx->mainPos,
                ctx->spriteInfo.pos.x + 48,
                ctx->spriteInfo.pos.x,
                ctx->spriteInfo.pos.y,
                ctx->spriteInfo.pos.y,
                3);
            ov07_02222268(
                &ctx->afterimage1Pos,
                ctx->spriteInfo.pos.x + 48,
                ctx->spriteInfo.pos.x,
                ctx->spriteInfo.pos.y + ctx->attackerHeight,
                ctx->spriteInfo.pos.y + ctx->attackerHeight,
                6);
            ov07_02222268(
                &ctx->afterimage2Pos,
                ctx->spriteInfo.pos.x + 48,
                ctx->spriteInfo.pos.x,
                ctx->spriteInfo.pos.y + ctx->attackerHeight,
                ctx->spriteInfo.pos.y + ctx->attackerHeight,
                9);
        }

        ctx->iteration++;
        ctx->common.state++;
        break;
    case SHADOW_PUNCH_STATE_MOVE: {
        int done = 0;

        if (ov07_022222F0(&ctx->afterimage1Pos, ctx->spriteInfo.hwSprite) == 0) {
            done++;
        }

        if (ov07_02222314(&ctx->mainPos, ctx->spriteInfo.monSprite) == 0) {
            done++;
        }

        if (ov07_022222F0(&ctx->afterimage2Pos, ctx->sprite) == 0) {
            done++;
        }

        if (done >= 3) {
            if (ctx->iteration == 1) {
                ctx->common.state--;
            } else {
                ctx->common.state++;
            }
        }
        break;
    }
    default:
        ov07_0221C448(ctx->common.battleAnimSys, task);
        ov07_02232508(ctx);
        ctx = 0L;
        return;
    }

    ManagedSprite_TickFrame(ctx->spriteInfo.hwSprite);
    SpriteSystem_DrawSprites(ctx->common.pokemonSpriteManager);
}
