#include "global.h"

#include "struct_defs/resdat_narc_structs.h"

#include "pokemon_types_def.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "heap.h"
#include "pokepic.h"
#include "palette.h"
#include "unk_02009D48.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_torment_sprite_context.h"
#include "overlay_07_alpha_fade_context.h"
#include "overlay_07_memento_context.h"
#include "overlay_07_battle_anim_funcs.h"

struct BattleAnimSystem;

SysTask *ov07_0221C410(BattleAnimSystem *system, SysTaskFunc func, void *param);

u16 ov07_0221C468(BattleAnimSystem *system);

ManagedSpriteTemplate ov07_0221F9E8(BattleAnimSystem *system);

void ov07_0222EF88(SysTask *task, void *param);

void ov07_0222F0B0(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite);

void ov07_0221C448(BattleAnimSystem *system, SysTask *task);
void ov07_02222268(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, u16 steps);
BOOL ov07_022222B4(XYTransformContext *ctx);
BOOL ov07_02222314(XYTransformContext *ctx, Pokepic *sprite);
void ov07_02222508(XYTransformContext *ctx, s32 startScale, s32 refScale, s32 endScale, u32 steps);
BOOL ov07_02222768(XYTransformContext *ctx, Pokepic *sprite);
typedef struct BatonPassSpriteContext {
    BattleAnimSystem *battleAnimSys;
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    int state;
    Pokepic *attackerSprite;
    XYTransformContext attackerScale;
    XYTransformContext attackerPos;
    ManagedSprite *ballSprite;
    XYTransformContext ballPos;
    int unused;
    s16 attackerX;
    s16 attackerY;
} BatonPassSpriteContext;
enum BatonPassSpriteState {
    BATON_PASS_SPRITE_STATE_INIT = 0,
    BATON_PASS_SPRITE_STATE_ANIMATE_BALL,
    BATON_PASS_SPRITE_STATE_ANIMATE_ATTACKER,
    BATON_PASS_SPRITE_STATE_WAIT_BALL_ANIM,
    BATON_PASS_SPRITE_STATE_MOVE_BALL_OFF_SCREEN,
    BATON_PASS_SPRITE_STATE_CLEANUP,
};
void ov07_0222F210(SysTask *task, void *param);

enum BattleAnimPositionType {
    BATTLE_ANIM_POSITION_MON_X = 0,
    BATTLE_ANIM_POSITION_MON_Y,
    BATTLE_ANIM_POSITION_PARTICLE_X,
    BATTLE_ANIM_POSITION_PARTICLE_Y,
};
s16 ov07_02221F80(BattleAnimSystem *system, int battler, enum BattleAnimPositionType posType);
void ov07_0222F378(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite);

u8 ov07_02221FF0(ManagedSprite *sprite);
void ov07_0222F408(MementoContext *ctx, ManagedSprite *sprite);

void ov07_0222F0B0(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite) {
    ManagedSpriteTemplate template;

    TormentSpriteContext *ctx = ov07_022324D8(system, sizeof(TormentSpriteContext));

    ctx->battleAnimSys = system;
    ctx->spriteSys = spriteSys;
    ctx->spriteMan = spriteMan;
    ctx->attackerSprite = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));

    s16 attackerX = Pokepic_GetAttr(ctx->attackerSprite, POKEPIC_X);
    s16 attackerY = Pokepic_GetAttr(ctx->attackerSprite, POKEPIC_Y);

    ctx->attackerY = attackerY;
    ctx->attackerOffset = ov07_0221FAA0(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));

    template = ov07_0221F9E8(ctx->battleAnimSys);

    for (int i = 0; i < 6; i++) {
        if (i == 0) {
            ctx->speechBubbleSprites[i] = sprite;
        } else {
            ctx->speechBubbleSprites[i] = SpriteSystem_NewSprite(spriteSys, spriteMan, &template);
        }

        ManagedSprite_SetDrawFlag(ctx->speechBubbleSprites[i], 0);

        s16 offsetX = FX_MulInline(FX_CosIdx(((((90) * 0xFFFF) / 360) / (6 / 2)) * (i / 2)), ((fx32)(((48) > 0) ? ((48) * (1 << 12) + 0.5f) : ((48) * (1 << 12) - 0.5f)))) >> 12;
        s16 offsetY = FX_MulInline(FX_SinIdx(((((90) * 0xFFFF) / 360) / (6 / 2)) * (i / 2)), ((fx32)(((48) > 0) ? ((48) * (1 << 12) + 0.5f) : ((48) * (1 << 12) - 0.5f)))) >> 12;

        if ((i % 2) == 0) {
            offsetX = attackerX + offsetX;
            ManagedSprite_SetFlipMode(ctx->speechBubbleSprites[i], 1);
        } else {
            offsetX = attackerX - offsetX;
        }

        offsetY = attackerY - offsetY;
        ManagedSprite_SetPositionXY(ctx->speechBubbleSprites[i], offsetX, offsetY);

        if (i < 2) {
            ctx->speechBubbleEndDelays[i] = 8;
        } else {
            ctx->speechBubbleEndDelays[i] = 0;
        }
    }

    ov07_0221C410(ctx->battleAnimSys, ov07_0222EF88, ctx);
}

void ov07_0222F210(SysTask *task, void *param) {
    BatonPassSpriteContext *ctx = param;

    switch (ctx->state) {
    case BATON_PASS_SPRITE_STATE_INIT:
        ManagedSprite_SetAnimateFlag(ctx->ballSprite, 1);
        ctx->state++;
        break;
    case BATON_PASS_SPRITE_STATE_ANIMATE_BALL:
        if (ManagedSprite_IsAnimated(ctx->ballSprite) == 0) {
            ManagedSprite_SetAnimateFlag(ctx->ballSprite, 0);
            ctx->state++;
            ov07_02222508(
                &ctx->attackerScale,
                10,
                10,
                0,
                8);
            int cx = Pokepic_GetAttr(ctx->attackerSprite, POKEPIC_X);
            int cy = Pokepic_GetAttr(ctx->attackerSprite, POKEPIC_Y);
            ov07_02222268(
                &ctx->attackerPos,
                cx,
                ctx->attackerX,
                cy,
                ctx->attackerY,
                8);
        }
        break;
    case BATON_PASS_SPRITE_STATE_ANIMATE_ATTACKER:
        ov07_02222314(&ctx->attackerPos, ctx->attackerSprite);

        if (ov07_02222768(&ctx->attackerScale, ctx->attackerSprite) == 0) {
            Pokepic_SetAttr(ctx->attackerSprite, POKEPIC_VANISHED, 1);
            Pokepic_SetAttr(ctx->attackerSprite, POKEPIC_AFFINEW, (1 << 8));
            Pokepic_SetAttr(ctx->attackerSprite, POKEPIC_AFFINEH, (1 << 8));
            ManagedSprite_SetAnim(ctx->ballSprite, 1);
            ManagedSprite_SetAnimSpeed(ctx->ballSprite, ((fx32)0x0000000000001000L));
            ManagedSprite_SetAnimateFlag(ctx->ballSprite, 1);
            ctx->state++;
        }
        break;
    case BATON_PASS_SPRITE_STATE_WAIT_BALL_ANIM:
        if (ManagedSprite_IsAnimated(ctx->ballSprite) == 0) {
            ManagedSprite_SetAnimateFlag(ctx->ballSprite, 0);
            ctx->state++;

            ov07_02222268(
                &ctx->ballPos,
                0,
                0,
                ctx->attackerY,
                0,
                8);
        }
        break;
    case BATON_PASS_SPRITE_STATE_MOVE_BALL_OFF_SCREEN:
        if (ov07_022222B4(&ctx->ballPos)) {
            ManagedSprite_SetPositionXY(ctx->ballSprite, ctx->attackerX, ctx->ballPos.y);
        } else {
            ctx->state++;
        }
        break;
    case BATON_PASS_SPRITE_STATE_CLEANUP:
        Sprite_DeleteAndFreeResources(ctx->ballSprite);
        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        return;
    }

    SpriteSystem_DrawSprites(ctx->spriteMan);
}

void ov07_0222F378(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite) {
    BatonPassSpriteContext *ctx = ov07_022324D8(system, sizeof(BatonPassSpriteContext));
    ctx->battleAnimSys = system;
    ctx->spriteSys = spriteSys;
    ctx->spriteMan = spriteMan;
    ctx->attackerSprite = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C468(system));

    Pokepic_SetAttr(ctx->attackerSprite, POKEPIC_VANISHED, 0);

    ctx->attackerX = ov07_02221F80(system, ov07_0221C468(system), BATTLE_ANIM_POSITION_MON_X);
    ctx->attackerY = ov07_02221F80(system, ov07_0221C468(system), BATTLE_ANIM_POSITION_MON_Y);
    ctx->ballSprite = sprite;

    ManagedSprite_SetDrawPriority(ctx->ballSprite, 100);
    ManagedSprite_SetPriority(ctx->ballSprite, 1);
    ManagedSprite_SetPositionXY(ctx->ballSprite, ctx->attackerX, ctx->attackerY);
    ov07_0221C410(ctx->battleAnimSys, ov07_0222F210, ctx);
}

void ov07_0222F408(MementoContext *ctx, ManagedSprite *sprite) {
    int paletteMask = 1 << ov07_02221FF0(sprite);
    PaletteData_BlendPalettes(
        ctx->paletteData,
        PLTTBUF_MAIN_OBJ,
        paletteMask,
        8,
        (((0) << 10) | ((0) << 5) | (0)));
}
