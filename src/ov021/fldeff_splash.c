// Black's code at ov021 0x021c4978-0x021c4d14. The name is the file name its asserts pass (tools/name_files.py): two
// effects, the second pokebw2's fldeff_splash.c, which is between fldeff_ripple.c and sound_obj.c as here.

#include "types.h"
#include "field/field_actor.h"
#include "field/field_effect.h"
#include "field/field_g3dobj.h"
#include "gfl/blact.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"

typedef struct FieldEffectSplash FieldEffectSplash;
typedef struct FieldEffectSplashObj FieldEffectSplashObj;

void func_ov021_021c49c0(FieldEffectSplash *splash);
void func_ov021_021c4a00(FieldEffectSplash *splash);
void func_ov021_021c4b64(FieldEffectSplashObj *splashObj);
void func_ov021_021c4bc4(FieldEffectSplashObj *splashObj);

// What handle 8 holds: the effects and the blact system, then the material
struct FieldEffectSplash {
    FieldEffects *effects;
    BlActSys *blact;
    u16 mat;
};

// What handle 0x11 holds: the effects, the resource group and the object
struct FieldEffectSplashObj {
    FieldEffects *effects;
    u16 resGroup;
    u16 obj;
};

typedef struct {
    FieldEffectSplash *splash;
    FieldActor *actor;
} SplashTaskParams;

// The task's copy of its params first, then its blact actor
typedef struct {
    SplashTaskParams params;
    u16 actor;
} SplashTask;

typedef struct {
    FieldEffectSplashObj *obj;
    FieldActor *actor;
} SplashObjTaskParams;

// The task's copy of its params first
typedef struct {
    SplashObjTaskParams params;
    FieldActorIdentity identity;
    u16 unk_e;
    u16 objIdx;
} SplashObjTask;

void func_ov021_021c4a3c(FieldEffectTask *task, void *work);
void func_ov021_021c4ae8(FieldEffectTask *task, void *work);
void func_ov021_021c4b04(FieldEffectTask *task, void *work);
void func_ov021_021c4b24(FieldEffectTask *task, void *work);
void func_ov021_021c4c0c(FieldEffectTask *task, void *work);
void func_ov021_021c4c50(FieldEffectTask *task, void *work);
void func_ov021_021c4cdc(FieldEffectTask *task, void *work);

static const FieldEffectTaskVTable data_ov021_021daa2c = {
    sizeof(SplashTask),
    func_ov021_021c4a3c,
    func_ov021_021c4ae8,
    func_ov021_021c4b04,
    func_ov021_021c4b24,
};

void func_ov021_021c4c54(FieldEffectTask *param_1, void *work);

static const FieldEffectTaskVTable data_ov021_021daa18 = {
    sizeof(SplashObjTask),
    func_ov021_021c4c0c,
    func_ov021_021c4c50,
    func_ov021_021c4c54,
    func_ov021_021c4cdc,
};

static const BlActAnimStep data_ov021_021daa40[] = {
    { 0, 0, 0, 3 },
    { 1, 0, 0, 3 },
    { 2, 0, 0, 3 },
    { 3, 0, 0, 3 },
    { 0x3fff, 0, 0, 0 },
};

static const BlActAnimStep *data_ov021_021de478[] = {
    data_ov021_021daa40,
};

void *func_ov021_021c4978(FieldEffects *param_1, HeapID param_2) {
    FieldEffectSplash *splash;
    Field *field;

    splash = GFL_HeapAllocate(param_2, sizeof(FieldEffectSplash), 1, "fldeff_splash.c", 0x7d);
    splash->effects = param_1;
    field = FieldEffects_GetField(param_1);
    splash->blact = Field_GetWildEffectBlAct(field);
    func_ov021_021c49c0(splash);
    return splash;
}

void func_ov021_021c49ac(FieldEffects *param_1, void *param_2) {
    func_ov021_021c4a00(param_2);
    GFL_HeapFree(param_2);
}

void func_ov021_021c49c0(FieldEffectSplash *splash) {
    ArcTool *handle;
    BlActMatRequest BStack_18;

    handle = FieldEffects_GetArc(splash->effects);
    BStack_18.format = 2;
    BStack_18.size = 0x13;
    BStack_18.faceWidth = 0x10;
    BStack_18.faceHeight = 0x10;
    BStack_18.mode = 1;
    BStack_18.texResource = GFL_G3DSysReadArcToolResource(handle, 0x2d);
    FieldEffects_ApplyLuminanceTable(splash->effects, BStack_18.texResource);
    splash->mat = BlActSys_ExecMatLoadRequests(splash->blact, &BStack_18, 1);
}

void func_ov021_021c4a00(FieldEffectSplash *splash) {
    BlActSys_FreeMaterials(splash->blact, splash->mat, 1);
}

void func_ov021_021c4a10(FieldEffects *effects, FieldActor *actor) {
    SplashTaskParams params;

    params.splash = FieldEffects_GetHandleData(effects, 8);
    params.actor = actor;
    FieldEffects_TCBCreate(effects, &data_ov021_021daa2c, NULL, 0, &params, 0);
}

void func_ov021_021c4a3c(FieldEffectTask *task, void *work) {
    SplashTask *splashTask = work;
    SplashTaskParams *params = func_ov021_021ac1ac(task);
    BlActSys *blact;
    VecFx32 pos;
    BlActActorRequest request;
    u16 actorIndex;

    splashTask->params = *params;
    CopyActorWPos(splashTask->params.actor, &pos);
    blact = splashTask->params.splash->blact;
    pos.y -= 0xe000;
    pos.z -= 0xa000;
    request.texMat = splashTask->params.splash->mat;
    request.scaleX = 0x1000;
    request.scaleY = 0x1000;
    request.alpha = 0x1f;
    request.visible = TRUE;
    request.lights = 1;
    request.pos = pos;
    request.callback = NULL;
    request.callbackData = splashTask;
    splashTask->actor = BlActSys_ExecActorRequests(blact, 0, &request, 1, 3);
    if (splashTask->actor == BLACT_NONE) {
        func_ov021_021ac154(task);
        return;
    }
    func_02050804(blact, splashTask->actor, data_ov021_021de478, 1);
    actorIndex = splashTask->actor;
    func_0205082c(blact, actorIndex, 0);
    func_020507f0(blact, actorIndex, TRUE);
}



void func_ov021_021c4ae8(FieldEffectTask *task, void *work) {
    SplashTask *splashTask = work;
    BlActSys *blact = splashTask->params.splash->blact;

    if (splashTask->actor != BLACT_NONE) {
        BlActSys_DeleteActors(blact, splashTask->actor, 1);
    }
}

void func_ov021_021c4b04(FieldEffectTask *task, void *work) {
    SplashTask *splashTask = work;
    BlActSys *blact = splashTask->params.splash->blact;
    u16 cmd;

    if (func_020508ec(blact, splashTask->actor, &cmd) != 0) {
        func_ov021_021ac154(task);
    }
}



void func_ov021_021c4b24(FieldEffectTask *task, void *work) {
}

void *func_ov021_021c4b28(FieldEffects *param_1, HeapID param_2) {
    FieldEffectSplashObj *splashObj;

    splashObj = GFL_HeapAllocate(param_2, sizeof(FieldEffectSplashObj), 1, "fldeff_splash.c", 0x163);
    splashObj->effects = param_1;
    func_ov021_021c4b64(splashObj);
    return splashObj;
}

void func_ov021_021c4b50(FieldEffects *param_1, void *param_2) {
    func_ov021_021c4bc4(param_2);
    GFL_HeapFree(param_2);
}

void func_ov021_021c4b64(FieldEffectSplashObj *splashObj) {
    u16 result;
    FieldG3DObjSystem *sys;
    ArcTool *arc;
    FieldG3DObjResRequest FStack_38;

    sys = func_ov021_021abe18(splashObj->effects);
    arc = FieldEffects_GetArc(splashObj->effects);
    FieldG3DObjResRequest_Clear(&FStack_38);
    FieldG3DObjResRequest_SetModel(&FStack_38, arc, 0x5f);
    FieldG3DObjResRequest_SetAnmArc(&FStack_38, arc);
    FieldG3DObjResRequest_AddAnm(&FStack_38, 0x6a);
    result = FieldG3DObjSystem_AddResGroup(sys, &FStack_38, 0);
    splashObj->resGroup = result;
    result = FieldG3DObjSystem_AddObj(sys, splashObj->resGroup, 0, NULL);
    splashObj->obj = result;
    func_ov021_021c67d8(sys, splashObj->obj, TRUE);
}

void func_ov021_021c4bc4(FieldEffectSplashObj *splashObj) {
    FieldG3DObjSystem *sys;

    sys = func_ov021_021abe18(splashObj->effects);
    FieldG3DObjSystem_FreeObj(sys, splashObj->obj);
    FieldG3DObjSystem_FreeResGroup(sys, splashObj->resGroup);
}

void func_ov021_021c4be0(FieldEffects *effects, FieldActor *actor) {
    SplashObjTaskParams params;

    params.obj = FieldEffects_GetHandleData(effects, 0x11);
    params.actor = actor;
    FieldEffects_TCBCreate(effects, &data_ov021_021daa18, NULL, 0, &params, 0);
}

void func_ov021_021c4c0c(FieldEffectTask *task, void *work) {
    SplashObjTask *objTask = work;
    SplashObjTaskParams *params = func_ov021_021ac1ac(task);
    VecFx32 pos;

    objTask->params = *params;
    func_ov010_0216e1a4(objTask->params.actor, &objTask->identity);
    CopyActorWPos(objTask->params.actor, &pos);
    pos.z += 0x8000;
    func_ov021_021ac1cc(task, &pos);
    objTask->objIdx = objTask->params.obj->obj;
}



void func_ov021_021c4c50(FieldEffectTask *task, void *work) {
}

void func_ov021_021c4c54(FieldEffectTask *param_1, void *work) {
    SplashObjTask *objTask = work;
    u16 objIdx;
    BOOL BVar1;
    FieldG3DObjSystem *sys;
    VecFx32 VStack_20;

    BVar1 = func_ov010_0216e1c4(objTask->params.actor, &objTask->identity);
    if (BVar1 == 0) {
        func_ov021_021ac154(param_1);
        return;
    }
    BVar1 = func_ov010_0216dbe8(objTask->params.actor);
    if (BVar1 == 0) {
        func_ov021_021ac154(param_1);
        return;
    }
    objIdx = objTask->objIdx;
    sys = func_ov021_021abe18(objTask->params.obj->effects);
    FieldG3DObjSystem_GetObjTransform(sys, objIdx);
    if (func_ov021_021c6844(sys, objIdx) == 0) {
        FieldG3DObjSystem_StepObjAnmLoop(sys, objIdx, 0x1000);
        func_ov021_021c6828(sys, objIdx, 1);
    }
    CopyActorWPos(objTask->params.actor, &VStack_20);
    VStack_20.z = VStack_20.z + 0x8000;
    func_ov021_021ac1cc(param_1, &VStack_20);
}

void func_ov021_021c4cdc(FieldEffectTask *task, void *work) {
    SplashObjTask *objTask = work;
    u16 objIdx = objTask->objIdx;
    FieldG3DObjSystem *sys = func_ov021_021abe18(objTask->params.obj->effects);
    SRTMatrix *transform = FieldG3DObjSystem_GetObjTransform(sys, objIdx);
    VecFx32 pos;

    func_ov021_021ac1b8(task, &pos);
    transform->translation = pos;
    FieldG3DObjSystem_DrawObj(sys, objIdx);
}

