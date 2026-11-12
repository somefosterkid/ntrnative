// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov036/fldeff_iwakudaki.c, ported by tools/port_file.py.
// The rock that breaks into pieces when Rock Smash is used on it. The name is the ROM's own, from GFL_HeapAllocate's
// file argument ("iwakudaki" is Rock Smash). Function names from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)
#include "types.h"
#include "field/field_actor.h"
#include "field/field_effect.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"
#include "gfl/tcb.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_mtx.h"

#define ROCK_SMASH_ANM_COUNT 4

typedef struct {
    FieldEffects *effects;
    void *mdlRes;
    void *anmRes[ROCK_SMASH_ANM_COUNT];
    TCB *uploadTcb;
    // Set once the model's textures are in VRAM
    BOOL uploaded;
} FieldEffectRockSmash;

// What the breaking starts from
typedef struct {
    FieldEffectRockSmash *effect;
    VecFx32 pos;
} RockSmashTaskParams;

typedef struct {
    FieldEffectRockSmash *effect;
    G3DActor *actor;
    void *anms[ROCK_SMASH_ANM_COUNT];
    G3DModel *mdl;
} RockSmashTask;

static void func_ov021_021adcf0(FieldEffectRockSmash *effect);
static void func_ov021_021add38(FieldEffectRockSmash *effect);
static void func_ov021_021add64(TCB *tcb, void *data);
void func_ov021_021adde8(FieldEffectTask *task, void *work);
void func_ov021_021ade4c(FieldEffectTask *task, void *work);
void func_ov021_021ade70(FieldEffectTask *task, void *work);
void func_ov021_021adea0(FieldEffectTask *task, void *work);

// pokebw2's ROCK_SMASH_TASK_VTABLE
static const FieldEffectTaskVTable data_ov021_021d9290 = {
    sizeof(RockSmashTask),
    func_ov021_021adde8,
    func_ov021_021ade4c,
    func_ov021_021ade70,
    func_ov021_021adea0,
};

// pokebw2's ROCK_SMASH_ANM_IDS
static u32 data_ov021_021de0fc[ROCK_SMASH_ANM_COUNT] = { 113, 101, 148, 129 };

void *func_ov021_021adcb8(FieldEffects *effects, HeapID heapId) {
    FieldEffectRockSmash *effect = GFL_HeapAllocate(heapId, sizeof(FieldEffectRockSmash), TRUE, "fldeff_iwakudaki.c",
                                                    103);

    effect->effects = effects;
    func_ov021_021adcf0(effect);
    return effect;
}

void func_ov021_021adcdc(FieldEffects *effects, void *data) {
    func_ov021_021add38(data);
    GFL_HeapFree(data);
}

static void func_ov021_021adcf0(FieldEffectRockSmash *effect) {
    int i;
    ArcTool *arc = FieldEffects_GetArc(effect->effects);

    effect->mdlRes = GFL_G3DSysReadArcToolResource(arc, 77);
    GFL_G3DResSetupTexData(effect->mdlRes);
    effect->uploadTcb = GFL_VBlankTCBAdd(func_ov021_021add64, effect, 0);
    for (i = 0; i < ROCK_SMASH_ANM_COUNT; i++) {
        effect->anmRes[i] = GFL_G3DSysReadArcToolResource(arc, data_ov021_021de0fc[i]);
    }
}

static void func_ov021_021add38(FieldEffectRockSmash *effect) {
    int i;

    for (i = 0; i < ROCK_SMASH_ANM_COUNT; i++) {
        GFL_G3DResFree(effect->anmRes[i]);
    }
    GFL_G3DResFreeTexData(effect->mdlRes);
    GFL_G3DResFree(effect->mdlRes);
    GFL_TCBRemove(effect->uploadTcb);
}

// Upload the model's textures in the first VBlank
static void func_ov021_021add64(TCB *tcb, void *data) {
    FieldEffectRockSmash *effect = data;

    if (!effect->uploaded) {
        GFL_G3DResUploadTexDataCore(effect->mdlRes);
        effect->uploaded = TRUE;
    }
}

// Break the rock in front of the actor, loading the effect first if the map hasn't
void func_ov021_021add7c(FieldActor *actor, FieldEffects *effects) {
    VecFx32 pos;
    RockSmashTaskParams params;
    u32 id;

    CopyActorWPos(actor, &pos);
    pos.y += FX32_ONE;
    pos.z += FX32_CONST(12);
    if (!FieldEffects_IsLoaded(effects, 10)) {
        id = 10;
        FieldEffects_Load(effects, &id, 1);
    }
    params.effect = FieldEffects_GetHandleData(effects, 10);
    params.pos = pos;
    FieldEffects_TCBCreate(effects, &data_ov021_021d9290, NULL, 0, &params, 0);
}

void func_ov021_021adde8(FieldEffectTask *task, void *work) {
    int i;
    RockSmashTask *rockTask = work;
    RockSmashTaskParams *params = func_ov021_021ac1ac(task);

    rockTask->effect = params->effect;
    func_ov021_021ac1cc(task, &params->pos);
    rockTask->mdl = GFL_G3DMdlCreate(rockTask->effect->mdlRes, 0, rockTask->effect->mdlRes);
    for (i = 0; i < ROCK_SMASH_ANM_COUNT; i++) {
        rockTask->anms[i] = GFL_G3DAnmCreate(rockTask->mdl, rockTask->effect->anmRes[i], 0);
    }
    rockTask->actor = GFL_G3DActorCreate(rockTask->mdl, rockTask->anms, ROCK_SMASH_ANM_COUNT);
    for (i = 0; i < ROCK_SMASH_ANM_COUNT; i++) {
        GFL_G3DActorBindAnm(rockTask->actor, i);
    }
}

void func_ov021_021ade4c(FieldEffectTask *task, void *work) {
    int i;
    RockSmashTask *rockTask = work;

    for (i = 0; i < ROCK_SMASH_ANM_COUNT; i++) {
        GFL_G3DAnmFree(rockTask->anms[i]);
    }
    GFL_G3DActorFree(rockTask->actor);
    GFL_G3DMdlFree(rockTask->mdl);
}

// End once every animation has
void func_ov021_021ade70(FieldEffectTask *task, void *work) {
    int i;
    RockSmashTask *rockTask = work;
    BOOL done = TRUE;

    for (i = 0; i < ROCK_SMASH_ANM_COUNT; i++) {
        if (GFL_G3DActorStepAnmFrame(rockTask->actor, i, FX32_ONE) == TRUE) {
            done = FALSE;
        }
    }
    if (done) {
        func_ov021_021ac154(task);
    }
}

void func_ov021_021adea0(FieldEffectTask *task, void *work) {
    SRTMatrix srt = { { 0, 0, 0 }, { FX32_ONE, FX32_ONE, FX32_ONE } };
    RockSmashTask *rockTask = work;

    MTX_Identity33_(&srt.rotation);
    func_ov021_021ac1b8(task, &srt.translation);
    GFL_G3DSysDrawObjBBoxCull(rockTask->actor, &srt);
}
