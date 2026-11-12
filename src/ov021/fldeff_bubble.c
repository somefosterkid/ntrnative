// Black's code at ov021 0x021d05d8-0x021d0750. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "field/field_actor.h"
#include "field/field_effect.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"


// What a bubble's models and animations are made from: the effects, the three resources, then the model, its two
// animations and the actor
typedef struct {
    FieldEffects *effects;
    void *resources[3];
    G3DModel *model;
    void *anims[2];
    G3DActor *actor;
} FieldEffectBubbleRes;

void func_ov021_021d0648(FieldEffectBubbleRes *param_1);
void func_ov021_021d06e0(FieldEffectBubbleRes *param_1);
void func_ov021_021d072c(FieldEffectTask *task, void *work);
void func_ov021_021d074c(FieldEffectTask *task, void *work);
void func_ov021_021d0750(FieldEffectTask *task, void *work);
void func_ov021_021d07d8(FieldEffectTask *task, void *work);

// What a bubble starts from: its handle data, then the actor it rises from
typedef struct {
    void *handle;
    FieldActor *actor;
} FieldEffectBubbleParams;

// A bubble's task work: the params it starts from, then the state its init sets
typedef struct {
    FieldEffectBubbleParams params;
    u8 finished;
    u8 unk_9;
    u8 unk_a;
    u8 unk_b[0x79];
} FieldEffectBubble;

static const FieldEffectTaskVTable data_ov021_021db968 = {
    sizeof(FieldEffectBubble),
    func_ov021_021d072c,
    func_ov021_021d074c,
    func_ov021_021d0750,
    func_ov021_021d07d8,
};


void func_ov021_021d05d8(FieldActor *actor, FieldEffects *effects) {
    FieldEffectBubbleParams params;
    VecFx32 pos;

    params.handle = FieldEffects_GetHandleData(effects, 0x10);
    params.actor = actor;
    CopyActorWPos(actor, &pos);
    FieldEffects_TCBCreate(effects, &data_ov021_021db968, &pos, 0, &params, 0);
}

void *func_ov021_021d0610(FieldEffects *param_1, HeapID param_2) {
    FieldEffectBubbleRes *ptr;

    ptr = GFL_HeapAllocate(param_2, sizeof(FieldEffectBubbleRes), 1, "fldeff_bubble.c", 0xd5);
    ptr->effects = param_1;
    func_ov021_021d0648(ptr);
    return ptr;
}

void func_ov021_021d0634(FieldEffects *param_1, void *param_2) {
    func_ov021_021d06e0(param_2);
    GFL_HeapFree(param_2);
}



const u32 data_ov021_021db950[1] = { 0 };
static const u32 data_ov021_021db954[2] = { 1, 2 };
static const u32 data_ov021_021db95c[3] = { 0x53, 0x96, 0x83 };

void func_ov021_021d0648(FieldEffectBubbleRes *param_1) {
    FieldEffectBubbleRes *res;
    ArcTool *arc;
    s32 i;

    res = param_1;
    arc = FieldEffects_GetArc(res->effects);
    for (i = 0; i < 3; i++) {
        res->resources[i] = GFL_G3DSysReadArcToolResource(arc, data_ov021_021db95c[i]);
        if (GFL_G3DResCheckType(res->resources[i], 2)) {
            GFL_G3DResUploadTexData(res->resources[i]);
        }
    }
    res->model = GFL_G3DMdlCreate(res->resources[data_ov021_021db950[0]], 0, res->resources[data_ov021_021db950[0]]);
    for (i = 0; i < 2; i++) {
        res->anims[i] = GFL_G3DAnmCreate(res->model, res->resources[data_ov021_021db954[i]], 0);
    }
    res->actor = GFL_G3DActorCreate(res->model, res->anims, 2);
    for (i = 0; i < 2; i++) {
        GFL_G3DActorBindAnm(res->actor, i);
    }
}


void func_ov021_021d06e0(FieldEffectBubbleRes *param_1) {
    s32 i;

    GFL_G3DActorFree(param_1->actor);
    i = 0;
    do {
        GFL_G3DAnmFree(param_1->anims[i]);
        i = i + 1;
    } while (i < 2);
    GFL_G3DMdlFree(param_1->model);
    i = 0;
    do {
        if (GFL_G3DResCheckType(param_1->resources[i], 2) != 0) {
            GFL_G3DResFreeTexData(param_1->resources[i]);
        }
        GFL_G3DResFree(param_1->resources[i]);
        i = i + 1;
    } while (i < 3);
}

void func_ov021_021d072c(FieldEffectTask *task, void *work) {
    void *handle;
    FieldEffectBubble *bubble;
    FieldEffectBubbleParams *params;

    bubble = work;
    params = func_ov021_021ac1ac(task);
    handle = params->handle;
    bubble->params.actor = params->actor;
    bubble->params.handle = handle;
    bubble->unk_a = 0x78;
    bubble->finished = IsAllActorAcmdFinished(bubble->params.actor);
    bubble->unk_9 = 5;
}


void func_ov021_021d074c(FieldEffectTask *task, void *work) {
}
