#include "global.h"

#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_torment_sprite_context.h"

struct BattleAnimSystem;

struct Pokepic;

struct PokepicDrawParam;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

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

void ov07_0222EDF0(TormentSpriteContext *ctx);

BOOL ov07_0222EE14(TormentSpriteContext *ctx);

void ov07_0222EF0C(ManagedSprite *sprite, int *outState, fx32 animSpeed);

BOOL ov07_0222EF58(ManagedSprite *speechBubble, int *state);

void ov07_0222EF88(SysTask *task, void *param);

void ov07_0222EF88(SysTask *task, void *param) {
    TormentSpriteContext *ctx = param;
    BOOL attackerScaleDone, speechBubbleDone;
    int i;

    switch (ctx->state) {
    case TORMENT_SPRITE_STATE_INIT:
        ov07_0222EDF0(ctx);
        ov07_0222EF0C(
            ctx->speechBubbleSprites[ctx->curSpeechBubble],
            &ctx->speechBubbleStates[ctx->curSpeechBubble],
            ((fx32)(((2) > 0) ? ((2) * (1 << 12) + 0.5f) : ((2) * (1 << 12) - 0.5f))));
        ctx->state++;
        break;
    case TORMENT_SPRITE_STATE_ANIMATE:
        attackerScaleDone = ov07_0222EE14(ctx);
        speechBubbleDone = ov07_0222EF58(
            ctx->speechBubbleSprites[ctx->curSpeechBubble],
            &ctx->speechBubbleStates[ctx->curSpeechBubble]);

        if (attackerScaleDone == speechBubbleDone && attackerScaleDone == 1) {
            ManagedSprite_SetAnim(ctx->speechBubbleSprites[ctx->curSpeechBubble], 1);
            ctx->state++;
        }
        break;
    case TORMENT_SPRITE_STATE_DELAY:
        ctx->speechBubbleEndDelays[ctx->curSpeechBubble]--;

        if (ctx->speechBubbleEndDelays[ctx->curSpeechBubble] < 0) {
            ctx->curSpeechBubble++;

            if (ctx->curSpeechBubble < 6) {
                ctx->state = 0;
            } else {
                ctx->state++;
            }
        }
        break;
    case TORMENT_SPRITE_STATE_REINIT:
        for (i = 0; i < 6; i++) {
            ov07_0222EF0C(
                ctx->speechBubbleSprites[i],
                &ctx->speechBubbleStates[i],
                ((fx32)(((2) > 0) ? ((2) * (1 << 12) + 0.5f) : ((2) * (1 << 12) - 0.5f))));
        }

        ctx->state++;
        break;
    case TORMENT_SPRITE_STATE_ANIMATE2:
        for (i = 0; i < 6; i++) {
            attackerScaleDone = ov07_0222EF58(
                ctx->speechBubbleSprites[i],
                &ctx->speechBubbleStates[i]);
        }

        if (attackerScaleDone == 1) {
            ctx->state++;
        }
        break;
    case TORMENT_SPRITE_STATE_CLEANUP:
        for (i = 0; i < 6; i++) {
            Sprite_DeleteAndFreeResources(ctx->speechBubbleSprites[i]);
        }

        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        return;
    }

    SpriteSystem_DrawSprites(ctx->spriteMan);
}
