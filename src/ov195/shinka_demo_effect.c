// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov284/shinka_demo_effect.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/arc.h"
#include "demo/demo_particle.h"
#include "demo/shinka_demo.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"
#include "gfl/particle.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/fx/fx_mtx.h"

// The effects around the evolving Pokémon: particles that play from the start, then a 3D model's animations that play
// forward and back

// The steps of the effect
enum {
    EFFECT_WAIT_START,
    EFFECT_PARTICLES,
    EFFECT_START_3D,
    EFFECT_WAIT_3D,
    EFFECT_HOLD_3D,
    EFFECT_WAIT_3D_REVERSE,
    EFFECT_DONE,
    // Created as already played
    EFFECT_PLAYED,
};

// What the 3D model's animations are doing
enum {
    EFFECT_3D_IDLE,
    EFFECT_3D_FORWARD,
    EFFECT_3D_END,
    EFFECT_3D_REVERSE,
};

typedef struct {
    u16 actor;
    SRTMatrix srt;
    BOOL visible;
} EffectActor;

struct ShinkaDemoEffect {
    HeapID heapId;
    BOOL played;
    s32 state;
    u32 wait;
    DemoParticle *particle;
    G3DManager *g3dManager;
    u16 scene;
    u16 modelActor;
    u16 actorCount;
    EffectActor *actors;
    u32 frame3D;
    u32 state3D;
};

static EffectActor *func_ov195_022086c8(HeapID heapId, u16 count);
static void func_ov195_02208714(EffectActor *actors);
static DemoParticle *func_ov195_0220887c(HeapID heapId, u16 count, const DemoParticleEvent *events);
static void func_ov195_02208994(DemoParticle *particle);
static void func_ov195_022089a4(DemoParticle *particle);
static void func_ov195_02208a24(DemoParticle *particle);
static void func_ov195_02208a2c(DemoParticle *particle);
static void func_ov195_02208a38(DemoParticle *particle, s32 stopTimer);
static void func_ov195_02208a48(DemoParticle *particle, s32 stopTimer);
static void func_ov195_02208a54(ShinkaDemoEffect *effect);
static void func_ov195_02208a6c(ShinkaDemoEffect *effect);
static void func_ov195_02208a78(ShinkaDemoEffect *effect);
static void func_ov195_02208ab0(ShinkaDemoEffect *effect);
static void func_ov195_02208b54(ShinkaDemoEffect *effect);
static void func_ov195_02208b6c(ShinkaDemoEffect *effect);
static void func_ov195_02208c10(ShinkaDemoEffect *effect);
static BOOL func_ov195_02208c18(ShinkaDemoEffect *effect);
static void func_ov195_02208c28(ShinkaDemoEffect *effect);
static BOOL func_ov195_02208c30(ShinkaDemoEffect *effect);

// pokebw2's sModelResources
static const G3DSceneResourceSetup data_ov195_02208ef4[] = {
#include "asset/shinka_demo_effect_data_ov195_02208ef4.inc"
};

// pokebw2's sModelAnimations
static const G3DSceneAnimationSetup data_ov195_02208ea4[] = { { 1, 0 }, { 2, 0 }, { 3, 0 } };

// pokebw2's sModelActors
static const G3DSceneActorSetup data_ov195_02208eb0[] = {
    { 0, 0, 0, 0, data_ov195_02208ea4, NELEMS(data_ov195_02208ea4) },
};

// pokebw2's sModelScene
static const G3DSceneSetup data_ov195_02208ec0 = { data_ov195_02208ef4, NELEMS(data_ov195_02208ef4), data_ov195_02208eb0,
                                           NELEMS(data_ov195_02208eb0) };

// Not referenced. Its value, FX32_ONE, is the step of the model's looping animation and the scale of its actor
const fx32 SHINKA_DEMO_EFFECT_UNK_FX32 = FX32_ONE;

// pokebw2's sParticleResources
static const DemoParticleResource data_ov195_02208e9c[DEMO_PARTICLE_UNIT_COUNT] = { { 0xb6, 10 } };

// pokebw2's sParticleEvents
static const DemoParticleEvent data_ov195_02208ed0[] = {
#include "asset/shinka_demo_effect_data_ov195_02208ed0.inc"
};

// pokebw2's ShinkaDemoEffect_CreateActors
static EffectActor *func_ov195_022086c8(HeapID heapId, u16 count) {
    EffectActor *actors = GFL_HeapAllocate(heapId, sizeof(EffectActor) * count, TRUE, "shinka_demo_effect.c", 232);
    u16 i;

    for (i = 0; i < count; i++) {
        actors[i].srt.translation.x = 0;
        actors[i].srt.translation.y = 0;
        actors[i].srt.translation.z = 0;
        actors[i].srt.scale.x = FX32_ONE;
        actors[i].srt.scale.y = FX32_ONE;
        actors[i].srt.scale.z = FX32_ONE;
        MTX_Identity33_(&actors[i].srt.rotation);
    }
    return actors;
}

// pokebw2's ShinkaDemoEffect_FreeActors
static void func_ov195_02208714(EffectActor *actors) {
    GFL_HeapFree(actors);
}

// pokebw2's ShinkaDemoEffect_Create
ShinkaDemoEffect *func_ov195_0220871c(HeapID heapId, BOOL played) {
    ShinkaDemoEffect *effect = GFL_HeapAllocate(heapId, sizeof(ShinkaDemoEffect), TRUE, "shinka_demo_effect.c", 336);

    effect->heapId = heapId;
    effect->played = played;
    if (played == FALSE) {
        effect->state = EFFECT_WAIT_START;
    } else {
        effect->state = EFFECT_PLAYED;
    }
    effect->wait = 0;
    effect->particle = func_ov195_0220887c(effect->heapId, NELEMS(data_ov195_02208ed0), data_ov195_02208ed0);
    func_ov195_02208a54(effect);
    func_ov195_02208ab0(effect);
    return effect;
}

// pokebw2's ShinkaDemoEffect_Free
void func_ov195_02208770(ShinkaDemoEffect *effect) {
    func_ov195_02208b54(effect);
    func_ov195_02208a6c(effect);
    func_ov195_02208994(effect->particle);
    GFL_HeapFree(effect);
}

// pokebw2's ShinkaDemoEffect_Update
void func_ov195_0220878c(ShinkaDemoEffect *effect) {
    switch (effect->state) {
    case EFFECT_WAIT_START:
    case EFFECT_PARTICLES:
        break;
    case EFFECT_START_3D:
        func_ov195_02208c10(effect);
        effect->state = EFFECT_WAIT_3D;
        break;
    case EFFECT_WAIT_3D:
        if (func_ov195_02208c18(effect)) {
            effect->state = EFFECT_HOLD_3D;
            effect->wait = 3;
            func_ov195_02208a48(effect->particle, 0);
        }
        break;
    case EFFECT_HOLD_3D:
        if (effect->wait == 0) {
            func_ov195_02208c28(effect);
            effect->state = EFFECT_WAIT_3D_REVERSE;
        } else {
            effect->wait--;
        }
        break;
    case EFFECT_WAIT_3D_REVERSE:
        if (func_ov195_02208c30(effect)) {
            effect->state = EFFECT_DONE;
        }
        break;
    case EFFECT_DONE:
    case EFFECT_PLAYED:
        break;
    }
    func_ov195_02208b6c(effect);
    func_ov195_022089a4(effect->particle);
}

// pokebw2's ShinkaDemoEffect_Draw
void func_ov195_02208804(ShinkaDemoEffect *effect) {
    func_ov195_02208a78(effect);
    func_ov195_02208a24(effect->particle);
}

// Stops the particles and plays the 3D model when the evolution is cancelled
// pokebw2's ShinkaDemoEffect_Cancel
void func_ov195_02208814(ShinkaDemoEffect *effect) {
    func_ov195_02208a38(effect->particle, 30);
    if (effect->state == EFFECT_PARTICLES) {
        effect->state = EFFECT_START_3D;
    }
}

// pokebw2's ShinkaDemoEffect_Start
void func_ov195_0220882c(ShinkaDemoEffect *effect) {
    func_ov195_02208a2c(effect->particle);
    effect->state = EFFECT_PARTICLES;
    effect->wait = 620;
}

// pokebw2's ShinkaDemoEffect_Start3D
void func_ov195_02208844(ShinkaDemoEffect *effect) {
    if (effect->state == EFFECT_PARTICLES) {
        effect->state = EFFECT_START_3D;
    }
}

// pokebw2's ShinkaDemoEffect_Is3DHeld
BOOL func_ov195_02208850(ShinkaDemoEffect *effect) {
    return effect->state == EFFECT_HOLD_3D;
}

// Whether the 3D model is about to or has started reversing
// pokebw2's ShinkaDemoEffect_Is3DReversing
BOOL func_ov195_02208860(ShinkaDemoEffect *effect) {
    if (effect->state == EFFECT_HOLD_3D && effect->wait <= 1) {
        return TRUE;
    }
    return effect->state == EFFECT_WAIT_3D_REVERSE;
}

// pokebw2's ShinkaDemoParticle_Create
static DemoParticle *func_ov195_0220887c(HeapID heapId, u16 count, const DemoParticleEvent *events) {
    VecFx32 cameraPosition = { 0, 0, FX32_CONST(13) };
    VecFx32 cameraUp = { 0, FX32_ONE, 0 };
    VecFx32 cameraTarget = { 0, 0, 0 };
    G3DCameraProjection projection;
    DemoParticle *particle;
    void *resource;
    u32 i;

    projection.type = G3DCAM_PROJECTION_PERSPECTIVE;
    projection.param1 = FX_SinIdx(DEG_TO_IDX(20));
    projection.param2 = FX_CosIdx(DEG_TO_IDX(20));
    projection.param3 = FX32_CONST(4.0 / 3.0);
    projection.param4 = 0;
    projection.near = FX32_ONE;
    projection.far = FX32_CONST(1024);
    projection.ndcRangeOverride = 0;
    particle = GFL_HeapAllocate(heapId, sizeof(DemoParticle), TRUE, "shinka_demo_effect.c", 741);
    particle->frame = 0;
    particle->index = 0;
    particle->count = count;
    particle->events = events;
    particle->active = FALSE;
    particle->stopTimer = -1;
    func_02050b80(heapId);
    for (i = 0; i < DEMO_PARTICLE_UNIT_COUNT; i++) {
        particle->units[i].system =
            func_02050c30(particle->units[i].buffer, DEMO_PARTICLE_BUFFER_SIZE, TRUE, 5, 6, 0x3b, heapId);
        func_02051758(particle->units[i].system);
        func_02051648(particle->units[i].system, &projection, FX32_CONST(2), &cameraPosition, &cameraUp, &cameraTarget,
                      heapId);
        resource = func_0205121c(data_ov195_02208e9c[i].arcId, data_ov195_02208e9c[i].fileId, heapId);
        particle->units[i].resourceCount = func_02051b04(resource);
        func_02051230(particle->units[i].system, resource, TRUE, FALSE);
    }
    return particle;
}

// pokebw2's ShinkaDemoParticle_Free
static void func_ov195_02208994(DemoParticle *particle) {
    func_02050ed4();
    GFL_HeapFree(particle);
}

// Emits the particles of the events at the current frame
// pokebw2's ShinkaDemoParticle_Update
static void func_ov195_022089a4(DemoParticle *particle) {
    if (particle->active) {
        VecFx32 pos = { 0, 0, 0 };

        while (particle->index < particle->count) {
            if (particle->frame != particle->events[particle->index].frame) {
                break;
            }
            func_020515b4(particle->units[particle->events[particle->index].unit].system,
                          particle->events[particle->index].emitter, &pos);
            particle->index++;
        }
        particle->frame++;
    }
    if (particle->stopTimer >= 0) {
        if (particle->stopTimer == 0) {
            func_02051620(particle->units[0].system);
        }
        particle->stopTimer--;
    }
}

// pokebw2's ShinkaDemoParticle_Draw
static void func_ov195_02208a24(DemoParticle *particle) {
    func_02050be8();
}

// pokebw2's ShinkaDemoParticle_Start
static void func_ov195_02208a2c(DemoParticle *particle) {
    particle->active = TRUE;
}

// pokebw2's ShinkaDemoParticle_Stop
static void func_ov195_02208a38(DemoParticle *particle, s32 stopTimer) {
    particle->active = FALSE;
    particle->stopTimer = stopTimer;
}

// pokebw2's ShinkaDemoParticle_SetStopTimer
static void func_ov195_02208a48(DemoParticle *particle, s32 stopTimer) {
    particle->stopTimer = stopTimer;
}

// pokebw2's ShinkaDemoEffect_Init3D
static void func_ov195_02208a54(ShinkaDemoEffect *effect) {
    effect->g3dManager = GFL_G3DMgrCreate(4, 1, effect->heapId);
    effect->actorCount = 0;
}

// pokebw2's ShinkaDemoEffect_Free3D
static void func_ov195_02208a6c(ShinkaDemoEffect *effect) {
    GFL_G3DMgrFree(effect->g3dManager);
}

// pokebw2's ShinkaDemoEffect_Draw3D
static void func_ov195_02208a78(ShinkaDemoEffect *effect) {
    u16 i;
    EffectActor *actor;

    for (i = 0; i < effect->actorCount; i++) {
        actor = &effect->actors[i];
        if (actor->visible) {
            GFL_G3DSysDrawObj(GFL_G3DMgrGetActor(effect->g3dManager, actor->actor), &actor->srt);
        }
    }
}

// pokebw2's ShinkaDemoEffect_AddScene
static void func_ov195_02208ab0(ShinkaDemoEffect *effect) {
    u16 i;
    u16 first;
    EffectActor *actor;
    G3DActor *model;
    fx32 frame;

    effect->scene = GFL_G3DMgrNewScene(effect->g3dManager, &data_ov195_02208ec0);
    effect->actorCount = 1;
    effect->actors = func_ov195_022086c8(effect->heapId, effect->actorCount);
    first = GFL_G3DMgrGetSceneFirstActorIdx(effect->g3dManager, effect->scene);
    effect->modelActor = 0;
    actor = &effect->actors[0];
    actor->actor = first;
    actor->srt.translation.x = 0;
    actor->srt.translation.y = 0;
    actor->srt.translation.z = 0;
    actor->visible = TRUE;
    for (i = 0; i < effect->actorCount; i++) {
        GFL_G3DActorBindAnm(GFL_G3DMgrGetActor(effect->g3dManager, effect->actors[i].actor), 0);
    }
    effect->frame3D = 0;
    model = GFL_G3DMgrGetActor(effect->g3dManager, effect->actors[effect->modelActor].actor);
    frame = 0;
    effect->state3D = EFFECT_3D_IDLE;
    GFL_G3DActorBindAnm(model, 1);
    GFL_G3DActorBindAnm(model, 2);
    GFL_G3DActorSetAnmFrame(model, 1, &frame);
    GFL_G3DActorSetAnmFrame(model, 2, &frame);
}

// pokebw2's ShinkaDemoEffect_DeleteScene
static void func_ov195_02208b54(ShinkaDemoEffect *effect) {
    GFL_G3DMgrDeleteScene(effect->g3dManager, effect->scene);
    func_ov195_02208714(effect->actors);
    effect->actorCount = 0;
}

// pokebw2's ShinkaDemoEffect_Update3D
static void func_ov195_02208b6c(ShinkaDemoEffect *effect) {
    u16 i;
    G3DActor *model;
    BOOL running1;
    BOOL running2;

    for (i = 0; i < effect->actorCount; i++) {
        GFL_G3DActorStepAnmFrameLoop(GFL_G3DMgrGetActor(effect->g3dManager, effect->actors[i].actor), 0, FX32_ONE);
    }
    effect->frame3D++;
    model = GFL_G3DMgrGetActor(effect->g3dManager, effect->actors[effect->modelActor].actor);
    if (effect->state3D == EFFECT_3D_FORWARD) {
        running1 = GFL_G3DActorStepAnmFrame(model, 1, FX32_CONST(5));
        running2 = GFL_G3DActorStepAnmFrame(model, 2, FX32_CONST(5));
        if (running1 == FALSE && running2 == FALSE) {
            effect->state3D = EFFECT_3D_END;
        }
    } else if (effect->state3D == EFFECT_3D_REVERSE) {
        running1 = GFL_G3DActorStepAnmFrame(model, 1, -FX32_CONST(2));
        running2 = GFL_G3DActorStepAnmFrame(model, 2, -FX32_CONST(2));
        if (running1 == FALSE && running2 == FALSE) {
            effect->state3D = EFFECT_3D_IDLE;
        }
    }
}

// pokebw2's ShinkaDemoEffect_Play3D
static void func_ov195_02208c10(ShinkaDemoEffect *effect) {
    effect->state3D = EFFECT_3D_FORWARD;
}

// pokebw2's ShinkaDemoEffect_Is3DAtEnd
static BOOL func_ov195_02208c18(ShinkaDemoEffect *effect) {
    return effect->state3D == EFFECT_3D_END;
}

// pokebw2's ShinkaDemoEffect_Reverse3D
static void func_ov195_02208c28(ShinkaDemoEffect *effect) {
    effect->state3D = EFFECT_3D_REVERSE;
}

// pokebw2's ShinkaDemoEffect_Is3DIdle
static BOOL func_ov195_02208c30(ShinkaDemoEffect *effect) {
    return effect->state3D == EFFECT_3D_IDLE;
}
