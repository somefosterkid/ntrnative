// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov036/fldeff_kemuri.c, ported by tools/port_file.py.
// The puff of dust where a field actor lands or steps. The name is the ROM's own, from GFL_HeapAllocate's file
// argument ("kemuri" is smoke). Function names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)
#include "types.h"
#include "field/field_actor.h"
#include "field/field_effect.h"
#include "field/field_g3dobj.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"

typedef struct {
    FieldEffects *effects;
    u16 resGroup;
} FieldEffectSmoke;

// What a puff starts from
typedef struct {
    FieldEffectSmoke *smoke;
    VecFx32 pos;
} SmokeTaskParams;

typedef struct {
    FieldEffectSmoke *smoke;
    FieldG3DObjSystem *objSys;
    u16 objIdx;
} SmokeTask;

static void func_ov021_021ac4cc(FieldEffectSmoke *smoke);
static void func_ov021_021ac514(FieldEffectSmoke *smoke);
void func_ov021_021ac62c(FieldEffectTask *task, void *work);
void func_ov021_021ac660(FieldEffectTask *task, void *work);
void func_ov021_021ac66c(FieldEffectTask *task, void *work);
void func_ov021_021ac688(FieldEffectTask *task, void *work);

// pokebw2's SMOKE_TASK_VTABLE
static const FieldEffectTaskVTable data_ov021_021d90ac = {
    sizeof(SmokeTask),
    func_ov021_021ac62c,
    func_ov021_021ac660,
    func_ov021_021ac66c,
    func_ov021_021ac688,
};

void *playSmokeEffect(FieldEffects *effects, HeapID heapId) {
    FieldEffectSmoke *smoke = GFL_HeapAllocate(heapId, sizeof(FieldEffectSmoke), TRUE, "fldeff_kemuri.c", 82);

    smoke->effects = effects;
    func_ov021_021ac4cc(smoke);
    return smoke;
}

void freeSmokeEffect(FieldEffects *effects, void *data) {
    func_ov021_021ac514(data);
    GFL_HeapFree(data);
}

static void func_ov021_021ac4cc(FieldEffectSmoke *smoke) {
    FieldG3DObjResRequest request;
    FieldG3DObjSystem *objSys = func_ov021_021abe18(smoke->effects);
    ArcTool *arc = FieldEffects_GetArc(smoke->effects);

    FieldG3DObjResRequest_Clear(&request);
    FieldG3DObjResRequest_SetModel(&request, arc, 48);
    FieldG3DObjResRequest_SetAnmArc(&request, arc);
    FieldG3DObjResRequest_AddAnm(&request, 96);
    smoke->resGroup = FieldG3DObjSystem_AddResGroup(objSys, &request, FALSE);
}

static void func_ov021_021ac514(FieldEffectSmoke *smoke) {
    FieldG3DObjSystem_FreeResGroup(func_ov021_021abe18(smoke->effects), smoke->resGroup);
}

// A puff of dust under the actor
void func_ov021_021ac528(FieldActor *actor, FieldEffects *effects) {
    VecFx32 pos;
    SmokeTaskParams params;

    CopyActorWPos(actor, &pos);
    pos.y += FX32_ONE;
    pos.z += FX32_CONST(6);
    params.smoke = FieldEffects_GetHandleData(effects, 1);
    params.pos = pos;
    FieldEffects_TCBCreate(effects, &data_ov021_021d90ac, NULL, 0, &params, 0);
}

// A puff of dust in front of the actor
void func_ov021_021ac578(FieldActor *actor, FieldEffects *effects) {
    VecFx32 pos;
    SmokeTaskParams params;
    u16 angle = func_ov010_0216d538(GetActorMModelSystem(actor));

    CopyActorWPos(actor, &pos);
    pos.y += FX32_ONE;
    pos.z += FX_Mul(FX_CosIdx(angle), FX32_CONST(6));
    pos.x += FX_Mul(FX_SinIdx(angle), FX32_CONST(6));
    params.smoke = FieldEffects_GetHandleData(effects, 1);
    params.pos = pos;
    FieldEffects_TCBCreate(effects, &data_ov021_021d90ac, NULL, 0, &params, 0);
}

void func_ov021_021ac62c(FieldEffectTask *task, void *work) {
    SmokeTask *smokeTask = work;
    SmokeTaskParams *params = func_ov021_021ac1ac(task);

    smokeTask->smoke = params->smoke;
    smokeTask->objSys = func_ov021_021abe18(smokeTask->smoke->effects);
    func_ov021_021ac1cc(task, &params->pos);
    smokeTask->objIdx = FieldG3DObjSystem_AddObj(smokeTask->objSys, smokeTask->smoke->resGroup, 0, &params->pos);
}

void func_ov021_021ac660(FieldEffectTask *task, void *work) {
    SmokeTask *smokeTask = work;

    FieldG3DObjSystem_FreeObj(smokeTask->objSys, smokeTask->objIdx);
}

// End when the animation does
void func_ov021_021ac66c(FieldEffectTask *task, void *work) {
    SmokeTask *smokeTask = work;

    if (!FieldG3DObjSystem_StepObjAnm(smokeTask->objSys, smokeTask->objIdx, FX32_ONE)) {
        func_ov021_021ac154(task);
    }
}

void func_ov021_021ac688(FieldEffectTask *task, void *work) {
}
