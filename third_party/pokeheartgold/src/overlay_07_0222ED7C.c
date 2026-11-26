#include "global.h"

#include "pokepic.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_07_torment_sprite_context.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_vital_throw_context.h"

enum TormentSpriteState {
    TORMENT_SPRITE_STATE_INIT = 0,
    TORMENT_SPRITE_STATE_ANIMATE,
    TORMENT_SPRITE_STATE_DELAY,
    TORMENT_SPRITE_STATE_REINIT,
    TORMENT_SPRITE_STATE_ANIMATE2,
    TORMENT_SPRITE_STATE_CLEANUP,

    TORMENT_SPRITE_ATTACKER_SCALE_STATE_SHRINK = 0,
    TORMENT_SPRITE_ATTACKER_SCALE_STATE_GROW,
    TORMENT_SPRITE_ATTACKER_SCALE_STATE_RESET,
    TORMENT_SPRITE_ATTACKER_SCALE_STATE_DONE,

    TORMENT_SPRITE_SPEECH_BUBBLE_STATE_ANIMATING = 0,
    TORMENT_SPRITE_SPEECH_BUBBLE_STATE_DONE,
};
void ov07_0222EF0C(ManagedSprite *sprite, int *outState, fx32 animSpeed);

s32 ov07_0222EF58(ManagedSprite *arg0, s32 *arg1);

void ov07_0222ED7C(BattleAnimSystem *system);

void ov07_0222ED7C(BattleAnimSystem *system) {
    Pokepic *pokepic;
    Pokepic *v1;
    VitalThrowContext *ctx;

    ctx = ov07_022324D8(system, sizeof(VitalThrowContext));
    ctx->battleAnimSys = system;
    ctx->dir = ov07_02222004(ctx->battleAnimSys, ov07_0221C468(system));
    pokepic = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C468(system));
    ctx->attackerSprite = pokepic;
    ctx->attackerX = Pokepic_GetAttr(pokepic, POKEPIC_X);
    ctx->attackerY = Pokepic_GetAttr(ctx->attackerSprite, POKEPIC_Y);
    v1 = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C470(system));
    ctx->defenderSprite = v1;
    ctx->defenderX = Pokepic_GetAttr(v1, POKEPIC_X);
    ctx->defenderY = Pokepic_GetAttr(ctx->defenderSprite, POKEPIC_Y);
    ov07_0221C410(ctx->battleAnimSys, ov07_0222EBB0, ctx);
}

s32 ov07_02222590(void *, s16, s16, s16, s16, s16, s32);

void ov07_0222EDF0(void *arg0);

void ov07_0222EDF0(void *arg0) {
    *((s32 *)&((TormentSpriteContext *)arg0)->attackerScaleState) = 0;
    ov07_02222590(&((TormentSpriteContext *)arg0)->scale, 0xA, 9, 0xA, 8, 0xA, 2);
}

s32 ov07_022226C4(Pokepic *, s16, s32, s32, s32);

s32 ov07_02222788(void *, Pokepic *);

s32 ov07_0222EE14(void *arg0);

s32 ov07_0222EE14(void *arg0) {
    s32 v1;
    u32 v0;

    v0 = *((u32 *)&((TormentSpriteContext *)arg0)->attackerScaleState);
    v1 = 0;
    switch (v0) {
    case 0:
        if (ov07_02222788(&((TormentSpriteContext *)arg0)->scale, ((Pokepic *)((TormentSpriteContext *)arg0)->attackerSprite)) != 0) {
            ov07_022226C4(((Pokepic *)((TormentSpriteContext *)arg0)->attackerSprite), ((TormentSpriteContext *)arg0)->attackerY, ((TormentSpriteContext *)arg0)->attackerOffset, *((s32 *)&((TormentSpriteContext *)arg0)->scale.data[4]), 0);
        } else {
            *((u32 *)&((TormentSpriteContext *)arg0)->attackerScaleState) = (u32)(*((u32 *)&((TormentSpriteContext *)arg0)->attackerScaleState) + 1);
            ov07_02222590(&((TormentSpriteContext *)arg0)->scale, 9, 0xB, 8, 0xF, 0xA, 2);
        }
        break;
    case 1:
        if (ov07_02222788(&((TormentSpriteContext *)arg0)->scale, ((Pokepic *)((TormentSpriteContext *)arg0)->attackerSprite)) != 0) {
            ov07_022226C4(((Pokepic *)((TormentSpriteContext *)arg0)->attackerSprite), ((TormentSpriteContext *)arg0)->attackerY, ((TormentSpriteContext *)arg0)->attackerOffset, *((s32 *)&((TormentSpriteContext *)arg0)->scale.data[4]), 0);
        } else {
            *((u32 *)&((TormentSpriteContext *)arg0)->attackerScaleState) = (u32)(*((u32 *)&((TormentSpriteContext *)arg0)->attackerScaleState) + 1);
            ov07_02222590(&((TormentSpriteContext *)arg0)->scale, 0xB, 0xA, 0xF, 0xA, 0xA, 2);
        }
        break;
    case 2:
        if (ov07_02222788(&((TormentSpriteContext *)arg0)->scale, ((Pokepic *)((TormentSpriteContext *)arg0)->attackerSprite)) != 0) {
            ov07_022226C4(((Pokepic *)((TormentSpriteContext *)arg0)->attackerSprite), ((TormentSpriteContext *)arg0)->attackerY, ((TormentSpriteContext *)arg0)->attackerOffset, *((s32 *)&((TormentSpriteContext *)arg0)->scale.data[4]), 0);
        } else {
            Pokepic_SetAttr(((Pokepic *)((TormentSpriteContext *)arg0)->attackerSprite), 0xC, 0x100);
            Pokepic_SetAttr(((Pokepic *)((TormentSpriteContext *)arg0)->attackerSprite), 0xD, 0x100);
            v1 = 1;
            *((u32 *)&((TormentSpriteContext *)arg0)->attackerScaleState) = (u32)(*((u32 *)&((TormentSpriteContext *)arg0)->attackerScaleState) + 1);
        }
        break;
    case 3:
        v1 = 1;
        break;
    }
    return v1;
}

void ov07_0222EF0C(ManagedSprite *sprite, int *outState, fx32 animSpeed) {
    *outState = TORMENT_SPRITE_SPEECH_BUBBLE_STATE_ANIMATING;

    ManagedSprite_SetDrawPriority(sprite, 100);
    ManagedSprite_SetPriority(sprite, 1);
    ManagedSprite_SetDrawFlag(sprite, 1);
    ManagedSprite_SetAnimateFlag(sprite, 1);
    ManagedSprite_SetAnimSpeed(sprite, animSpeed);
    ManagedSprite_SetAnim(sprite, 0);
    ManagedSprite_SetAnimationFrame(sprite, 0);
    ManagedSprite_TickNFrames(sprite, ((fx32)(((55) > 0) ? ((55) * (1 << 12) + 0.5f) : ((55) * (1 << 12) - 0.5f))));
}

s32 ov07_0222EF58(ManagedSprite *managedSprite, s32 *arg1) {
    s32 v0;
    s32 v1;

    v0 = *arg1;
    v1 = 0;
    switch (v0) {
    case 0:
        if (ManagedSprite_IsAnimated(managedSprite) == 0) {
            v1 = 1;
            *arg1 += 1;
        }
        break;
    case 1:
        v1 = 1;
        break;
    default:
        v1 = 1;
        break;
    }
    return v1;
}
