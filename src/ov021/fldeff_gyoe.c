// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov036/fldeff_gyoe.c, ported by tools/port_file.py.
// The emotion bubbles that pop up over a field actor's head: an exclamation mark, a question mark, a music note and an ellipsis, by kind. The name is the
// ROM's own, from GFL_HeapAllocate's file argument ("gyoe" is a cry of surprise).
#include "types.h"
#include "constants/sound.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_camera.h"
#include "field/field_effect.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"
#include "gfl/sound.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_vec.h"
#include "nitro/fx/fx_mtx.h"
#include "nitro/mi.h"
#include "nnsys/g3d.h"

enum {
    GYOE_KIND_COUNT = 4,
};

typedef struct {
    FieldEffects *effects;
    void *resources[GYOE_KIND_COUNT];
    G3DModel *models[GYOE_KIND_COUNT];
    G3DActor *actors[GYOE_KIND_COUNT];
    // How far the bubbles are drawn toward the camera, so that nothing hides them
    u16 depthOffset;
} FieldEffectGyoe;

// What a bubble starts from
typedef struct {
    u32 kind;
    BOOL playSE;
    FieldEffectGyoe *gyoe;
    FieldActor *actor;
    FieldActorIdentity identity;
    Field *field;
} GyoeTaskParams;

typedef struct {
    GyoeTaskParams params;
    u32 state;
    int timer;
    BOOL done;
    // The bubble's hop above its resting height, and how fast it rises
    fx32 height;
    fx32 speed;
} GyoeTask;

static void func_ov021_021bb4bc(FieldEffectGyoe *gyoe);
static void func_ov021_021bb504(FieldEffectGyoe *gyoe);
static void func_ov021_021bb5dc(FieldEffectTask *task, void *work);
static void func_ov021_021bb608(FieldEffectTask *task, void *work);
static void func_ov021_021bb60c(FieldEffectTask *task, void *work);
static void func_ov021_021bb648(FieldEffectTask *task, void *work);
static void func_ov021_021bb68c(FieldEffectTask *task, void *work);
static void func_ov021_021bb778(u32 kind);
static void func_ov021_021bb7b4(G3DCamera *camera, const VecFx32 *src, VecFx32 *dest);
static void func_ov021_021bb808(GyoeTask *gyoeTask, VecFx32 *pos);

// The models of the bubbles in the field effects' archive, by kind
// pokebw2's GYOE_MODEL_FILES
static const u32 data_ov021_021d9a6c[GYOE_KIND_COUNT] = { 50, 51, 52, 53 };

// A bubble that stays when it is done, until its owner ends it
// pokebw2's GYOE_TASK_VTABLE
static const FieldEffectTaskVTable data_ov021_021d9a7c = {
    sizeof(GyoeTask),
    func_ov021_021bb5dc,
    func_ov021_021bb608,
    func_ov021_021bb60c,
    func_ov021_021bb68c,
};

// A bubble that ends itself when it is done
// pokebw2's GYOE_AUTO_END_TASK_VTABLE
static const FieldEffectTaskVTable data_ov021_021d9a90 = {
    sizeof(GyoeTask),
    func_ov021_021bb5dc,
    func_ov021_021bb608,
    func_ov021_021bb648,
    func_ov021_021bb68c,
};


void *func_ov021_021bb484(FieldEffects *effects, HeapID heapId) {
    FieldEffectGyoe *gyoe = GFL_HeapAllocate(heapId, sizeof(FieldEffectGyoe), TRUE, "fldeff_gyoe.c", 111);

    gyoe->effects = effects;
    func_ov021_021bb4bc(gyoe);
    return gyoe;
}

void func_ov021_021bb4a8(FieldEffects *effects, void *data) {
    func_ov021_021bb504(data);
    GFL_HeapFree(data);
}

static void func_ov021_021bb4bc(FieldEffectGyoe *gyoe) {
    int i;
    ArcTool *arc = FieldEffects_GetArc(gyoe->effects);

    for (i = 0; i < GYOE_KIND_COUNT; i++) {
        gyoe->resources[i] = GFL_G3DSysReadArcToolResource(arc, data_ov021_021d9a6c[i]);
        GFL_G3DResUploadTexData(gyoe->resources[i]);
        gyoe->models[i] = GFL_G3DMdlCreate(gyoe->resources[i], 0, gyoe->resources[i]);
        gyoe->actors[i] = GFL_G3DActorCreate(gyoe->models[i], NULL, 0);
    }
    gyoe->depthOffset = 4;
}

static void func_ov021_021bb504(FieldEffectGyoe *gyoe) {
    int i;

    for (i = 0; i < GYOE_KIND_COUNT; i++) {
        GFL_G3DActorFree(gyoe->actors[i]);
        GFL_G3DMdlFree(gyoe->models[i]);
        GFL_G3DResFree(gyoe->resources[i]);
    }
}

FieldEffectTask *func_ov021_021bb528(FieldEffects *effects, FieldActor *actor, u32 kind, BOOL playSE) {
    GyoeTaskParams params;

    params.kind = kind;
    params.playSE = playSE;
    params.gyoe = FieldEffects_GetHandleData(effects, 4);
    params.actor = actor;
    func_ov010_0216e1a4(actor, &params.identity);
    params.field = FieldEffects_GetField(effects);
    if (playSE == TRUE) {
        func_ov021_021bb778(kind);
    }
    return FieldEffects_TCBCreate(effects, &data_ov021_021d9a7c, NULL, 0, &params, 0);
}

FieldEffectTask *func_ov021_021bb578(FieldEffects *effects, FieldActor *actor, u32 kind, BOOL playSE) {
    GyoeTaskParams params;

    params.kind = kind;
    params.playSE = playSE;
    params.gyoe = FieldEffects_GetHandleData(effects, 4);
    params.actor = actor;
    func_ov010_0216e1a4(actor, &params.identity);
    params.field = FieldEffects_GetField(effects);
    if (playSE == TRUE) {
        func_ov021_021bb778(kind);
    }
    return FieldEffects_TCBCreate(effects, &data_ov021_021d9a90, NULL, 0, &params, 0);
}

BOOL func_ov021_021bb5c8(FieldEffectTask *task) {
    if (task != NULL) {
        GyoeTask *gyoeTask = func_ov021_021ac1e0(task);

        return gyoeTask->done;
    }
    return TRUE;
}

static void func_ov021_021bb5dc(FieldEffectTask *task, void *work) {
    GyoeTask *gyoeTask = work;
    GyoeTaskParams *params = func_ov021_021ac1ac(task);

    gyoeTask->params = *params;
    gyoeTask->speed = FX32_CONST(6);
    func_ov021_021ac178(task);
}

static void func_ov021_021bb608(FieldEffectTask *task, void *work) {
}

// Follow the actor; when it is gone, the bubble is done
static void func_ov021_021bb60c(FieldEffectTask *task, void *work) {
    GyoeTask *gyoeTask = work;
    VecFx32 pos;

    if (!func_ov010_0216e1c4(gyoeTask->params.actor, &gyoeTask->params.identity)) {
        gyoeTask->done = TRUE;
        return;
    }
    CopyActorWPos(gyoeTask->params.actor, &pos);
    func_ov021_021bb808(gyoeTask, &pos);
    func_ov021_021ac1cc(task, &pos);
}

// Follow the actor, and end once the bubble is done or the actor is gone
static void func_ov021_021bb648(FieldEffectTask *task, void *work) {
    GyoeTask *gyoeTask = work;
    VecFx32 pos;

    if (gyoeTask->done == TRUE || !func_ov010_0216e1c4(gyoeTask->params.actor, &gyoeTask->params.identity)) {
        func_ov021_021ac154(task);
        return;
    }
    CopyActorWPos(gyoeTask->params.actor, &pos);
    func_ov021_021bb808(gyoeTask, &pos);
    func_ov021_021ac1cc(task, &pos);
}

// Draw the bubble moved toward the camera by its depth offset, through the projection matrix
static void func_ov021_021bb68c(FieldEffectTask *task, void *work) {
    GyoeTask *gyoeTask = work;
    SRTMatrix status = { { 0, 0, 0 }, { FX32_ONE, FX32_ONE, FX32_ONE } };
    const MtxFx44 *proj = NNS_G3dGlbGetProjectionMtx();
    MtxFx44 saved;
    MtxFx44 mtx;
    fx32 offset = FX32_CONST(gyoeTask->params.gyoe->depthOffset);

    saved = *proj;
    mtx = saved;
    mtx.m[3][2] += FX_Mul(mtx.m[2][2], offset);
    NNS_G3dGlbSetProjectionMtx(&mtx);
    NNS_G3dGlbFlushP();
    NNS_G3dGeFlushBuffer();
    MTX_Identity33_(&status.rotation);
    func_ov021_021ac1b8(task, &status.translation);
    GFL_G3DSysDrawObjBBoxCull(gyoeTask->params.gyoe->actors[gyoeTask->params.kind], &status);
    NNS_G3dGlbSetProjectionMtx(&saved);
    NNS_G3dGlbFlushP();
    NNS_G3dGeFlushBuffer();
}

static void func_ov021_021bb778(u32 kind) {
    u32 se = 0;

    switch (kind) {
    case 0:
        se = SEQ_SE_FLD_07;
        break;
    case 1:
        se = SEQ_SE_SYS_62;
        break;
    case 2:
        se = SEQ_SE_SYS_63;
        break;
    case 3:
        return;
    }
    GFL_SndSEPlay(se);
}

// Turns src from the camera's view space into the world's
static void func_ov021_021bb7b4(G3DCamera *camera, const VecFx32 *src, VecFx32 *dest) {
    VecFx32 camPos;
    VecFx32 camUp;
    VecFx32 target;
    MtxFx43 lookAt;
    MtxFx33 rot;

    GFL_G3DCameraGetLookatPos(camera, &camPos);
    GFL_G3DCameraGetLookatUpVector(camera, &camUp);
    GFL_G3DCameraGetLookatTarget(camera, &target);
    MTX_LookAt(&camPos, &camUp, &target, &lookAt);
    MI_Copy36B(&lookAt, &rot);
    MTX_Inverse33(&rot, &rot);
    MTX_MultVec33(src, &rot, dest);
}

// The bubble hops up and comes back down, then stays for 30 frames, above the actor's head as the camera sees it
static void func_ov021_021bb808(GyoeTask *gyoeTask, VecFx32 *pos) {
    VecFx32 vec;
    G3DCamera *camera;

    switch (gyoeTask->state) {
    case 0:
        gyoeTask->height += gyoeTask->speed;
        if (gyoeTask->height != 0) {
            gyoeTask->speed -= FX32_CONST(2);
        } else {
            gyoeTask->speed = 0;
            gyoeTask->state++;
        }
        break;
    case 1:
        gyoeTask->timer++;
        if (gyoeTask->timer >= 30) {
            gyoeTask->state++;
            gyoeTask->done = TRUE;
        }
        break;
    }
    camera = FieldCamera_GetG3DCamera(Field_GetCameraSystem(gyoeTask->params.field));
    {
        VecFx32 offset = { 0, gyoeTask->height + FX32_CONST(22), 0 };

        vec = offset;
    }
    func_ov021_021bb7b4(camera, &vec, &vec);
    VEC_Add(&vec, pos, pos);
}
