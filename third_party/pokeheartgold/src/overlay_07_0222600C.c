#include "global.h"

#include "error_handling.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_battle_anim_system.h"
#include "particle_system.h"
#include "overlay_07_emitter_animation_context.h"
#include "library/spl_particle.h"
#include "library/spl_resource.h"
#include "library/spl_emitter.h"

void ov07_0222600C(BattleAnimSystem *system, EmitterAnimationContext *ctx);

struct SPLEmitter;
struct SPLResource;
enum SPLUpdateCallbackType {
    SPL_CALLBACK_PRE_UPDATE = 0,
    SPL_CALLBACK_POST_UPDATE = 1,
};
typedef void (*SPLEmitterUpdateCallback)(struct SPLEmitter *emitter, enum SPLUpdateCallbackType type);
typedef struct SPLEmitterState {
    union {
        u32 all;
        struct
        {
            u32 terminate : 1;
            u32 emissionPaused : 1;
            u32 paused : 1;
            u32 renderingDisabled : 1;
            u32 started : 1;
            u32 : 27;
        };
    };
} SPLEmitterState;
BOOL ov07_022260DC(SPLEmitter *emitter);

void ov07_0222600C(BattleAnimSystem *system, EmitterAnimationContext *ctx) {
    s32 v0;
    s32 v2;
    u32 v1;
    u32 v3;

    ctx->emitterID = ov07_0221C4A8(system, 0);
    ctx->offset.x = (s16)ov07_0221C4A8(system, 1);
    ctx->offset.y = (s16)ov07_0221C4A8(system, 2);
    ctx->startDelay = (s16)ov07_0221C4A8(system, 3);
    ctx->frames = (s16)ov07_0221C4A8(system, 4);
    ctx->radius = ov07_0221C4A8(system, 5);
    ctx->mode = ov07_0221C4A8(system, 6);
    ctx->curve = ov07_0221C4A8(system, 8);
    v0 = ov07_0221C4A8(system, 7);
    v3 = 0xFFFF0000 & v0;
    v1 = v0 << 0x10;
    ctx->frame = 0;
    ctx->skipFrames = (u32)(v3 >> 0x10);
    ctx->maxFrames = (u32)(v1 >> 0x10);
    if (ctx->skipFrames == 0) {
        ctx->skipFrames = 0U;
    }
    if (ctx->maxFrames == 0) {
        ctx->maxFrames = 0xFFU;
    }
    ctx->emitter = ov07_0221C494(system, ctx->emitterID);
    ctx->particleSys = ov07_0221C478(system);
    v2 = ctx->mode;
    if (v2 == 0) {
        ctx->startBattler = ov07_0221C468(system);
        ctx->endBattler = ov07_0221C470(system);
    } else if (v2 == 1) {
        ctx->startBattler = ov07_0221C470(system);
        ctx->endBattler = ov07_0221C468(system);
    }
    if (ctx->emitter == 0) {
        GF_AssertFail();
    }
}

BOOL ov07_022260DC(SPLEmitter *emitter) {
    BOOL active = 0;
    SPLParticle *particle = emitter->act_ptcl_list.p_begin;

    int age;
    int lifeTime;
    while (particle != 0L) {
        age = particle->age;
        lifeTime = particle->life;
        particle = particle->p_next;
    }

    if (age != lifeTime) {
        active = 1;
    }

    return active;
}
