// Black's code at ov021 0x021bbfb8-0x021bc550. No name is known: the module and address.

#include "types.h"
#include "struct_decls.h"
#include "field/field_actor.h"
#include "field/field_effect.h"
#include "gfl/bg_sys.h"
#include "field/fldmmdl_blact.h"
#include "field/field_place_name.h"
#include "field/field.h"
#include "field/ov021_021bd068.h"
#include "field/field_map.h"
#include "field/field_camera.h"
#include "field/fldeff_reflect.h"

// What func_ov021_021bbfb8 gives the reflection task (pokebw2's ReflectTaskParams)
typedef struct {
    FieldEffectReflect *effect;
    u32 type;
    FieldActor *actor;
    MMSys *mmSys;
    BlActSys *blActSys;
} ReflectTaskParams;

// The reflection of an actor on water and shiny floors (pokebw2's ReflectTask)
typedef struct {
    // The swing of the reflection's width, around base
    s32 base;
    u32 scaleY;
    s32 swing;
    s32 swingSpeed;
    u8 created;
    u8 initialized;
    ReflectTaskParams params;
    // The material's flags and the reflection's billboard actor, as func_ov021_0219c928 gives them
    u16 blAct[2];
    FieldActorIdentity identity;
} ReflectTask;

void func_ov021_021bbff4(FieldEffectTask *task, void *data);
void func_ov021_021bc030(FieldEffectTask *task, void *data);
void func_ov021_021bc048(FieldEffectTask *task, void *data);
void func_ov021_021bc10c(FieldEffectTask *task, void *data);
void func_ov021_021bc138(u32 param_1, void *data);

const FieldEffectTaskVTable data_ov021_021d9b64 = {
    sizeof(ReflectTask),
    func_ov021_021bbff4,
    func_ov021_021bc030,
    func_ov021_021bc048,
    func_ov021_021bc10c,
};

s32 func_ov021_021bc340(s32 param_1, s32 param_2);
u32 func_ov021_021bc338(u16 param_1);

void func_ov021_021bbfb8(MMSys *system, FieldActor *actor, FieldEffects *effects, u32 kind) {
    ReflectTaskParams params;

    params.type = kind;
    params.actor = actor;
    params.mmSys = system;
    params.blActSys = FieldBlAct_GetBlActSys(func_ov010_0216d520(system));
    params.effect = FieldEffects_GetHandleData(effects, 6);
    FieldEffects_TCBCreate(effects, &data_ov021_021d9b64, NULL, 0, &params, 0);
}

void func_ov021_021bbff4(FieldEffectTask *task, void *data) {
    ReflectTask *work = data;
    ReflectTaskParams *params = func_ov021_021ac1ac(task);
    work->params = *params;
    work->swingSpeed = 0x40;
    work->base = 0;
    work->swing = 0;
    if (work->params.type == 2) {
        work->swingSpeed = 0;
    }
    func_ov010_0216e1a4(params->actor, &work->identity);
}

void func_ov021_021bc030(FieldEffectTask *task, void *data) {
    ReflectTask *work = data;

    if (work->created != 0) {
        work->created = 0;
        func_ov021_0219c98c(work->params.mmSys, work->blAct);
    }
}

void func_ov021_021bc048(FieldEffectTask *task, void *data) {
    ReflectTask *work = data;
    u32 blActIdx;
    s32 next;
    u16 zone[2];

    if (func_ov010_0216e1c4(work->params.actor, &work->identity) == 0 ||
        FldAct_CheckReflectingFlag(work->params.actor) == 0) {
        func_ov021_021ac154(task);
        return;
    }
    blActIdx = FldAct_GetBlActIdx(work->params.actor, 0);
    if (work->created == 0) {
        if (blActIdx == 0xffff) {
            return;
        }
        VecFx32 pos = { 0, 0, 0 };
        func_ov021_0219c928(work->params.mmSys, GetActorMdlInfo(work->params.actor), work->blAct, &pos,
                            func_ov021_021bc138, work);
        work->created = 1;
    }
    if (work->blAct[1] != 0xffff) {
        if (work->initialized == 0) {
            func_ov021_0219c9e0(work->params.actor, &zone[1], &zone[0]);
            work->base = zone[1];
            work->scaleY = zone[0];
            work->swing = work->base;
            work->initialized = 1;
        }
        next = work->swing + work->swingSpeed;
        work->swing = next;
        if (next >= work->base + 0x400) {
            work->swing = work->base + 0x400;
            work->swingSpeed = -work->swingSpeed;
            return;
        }
        if (next <= work->base - 0x400) {
            work->swing = work->base - 0x400;
            work->swingSpeed = -work->swingSpeed;
        }
    }
}


void func_ov021_021bc10c(FieldEffectTask *task, void *data) {
    ReflectTask *work = data;
    BOOL BVar1;

    BVar1 = func_ov010_0216e1c4(work->params.actor, &work->identity);
    if (((BVar1 == 1) && (work->created != 0)) && (work->blAct[1] != 0xffff)) {
        func_ov021_021bc138(work->blAct[1], work);
    }
}

const s32 data_ov021_021d9b58[3] = { 0xfffe7000, 0xfffdf000, 0xfffe7000 };
const s32 data_ov021_021d9b4c[3] = { 0, 0, 0 };
const s32 data_ov021_021d9b40[3] = { 0xc000, 0x6000, 0x19000 };

void func_ov021_021bc138(u32 param_1, void *data) {
    ReflectTask *work = data;
    u16 zone;
    BlActSys *sys;
    u32 blActIdx;
    FieldActor *actor;
    BlActScene *scene;
    u16 visible;
    u32 matId;
    u32 matId2 = 0;
    Field *field;
    const FieldActorConfig *mdlInfo;
    VecFx32 pos;
    VecFx32 tmp;
    VecFx32 look;
    fx32 height;
    BOOL flag;
    s16 arr[4];

    sys = work->params.blActSys;
    scene = BlActSys_GetScene(sys);
    blActIdx = FldAct_GetBlActIdx(work->params.actor, 0);
    if (param_1 != 0xffff) {
        field = FieldEffects_GetField(work->params.effect->effects);
        zone = Field_GetPlayerStateZoneID(field);
        visible = func_020507d0(sys, (u16)blActIdx);
        func_020507a0(sys, param_1, visible);
        func_020507f0(sys, param_1, matId2);
        actor = work->params.actor;
        mdlInfo = GetActorMdlInfo(actor);
        CopyActorWPos(actor, &pos);
        pos.z += data_ov021_021d9b40[mdlInfo->billboardSize];
        func_ov010_0216d8b0(actor, &tmp);
        pos.x += tmp.x;
        pos.z -= tmp.z;
        if (GetHeightFromMap(actor, &pos, &height) == 1) {
            pos.y = height;
        }
        pos.y += data_ov021_021d9b58[work->params.type];
        func_ov010_0216d8d0(actor, &tmp);
        pos.x += tmp.x;
        pos.y -= tmp.y;
        pos.z -= tmp.z;
        pos.y += data_ov021_021d9b4c[mdlInfo->billboardSize];
        if (mdlInfo->billboardSize == 2) {
            if (GetTileFlags(GetCachedTileUnderActor(actor)) & 0x40) {
                pos.y -= 0x3800;
            } else {
                pos.y -= 0x8000;
            }
        }
        pos.y += func_ov021_021bc340(zone, mdlInfo->billboardSize);
        field = FieldEffects_GetField(work->params.effect->effects);
        GFL_G3DCameraGetLookatTarget(FieldCamera_GetG3DCamera(Field_GetCameraSystem(field)), &look);
        pos.x = pos.x + (pos.x / 4096 - look.x / 4096) * 0x12a;
        BlActScene_SetActorPos(scene, param_1, &pos);
        flag = TRUE;
        matId = func_020508c4(sys, blActIdx & 0xffff);
        matId2 = func_020508c4(sys, param_1);
        arr[3] = work->swing;
        arr[2] = work->scaleY;
        arr[2] += func_ov021_021bc338(zone);
        arr[1] = 0;
        arr[0] = 0;
        func_0204f5e4(scene, matId, (u16 *)&arr[1]);
        func_0204f604(scene, matId2, (u16 *)&arr[1]);
        func_0204f654(scene, matId, (u16 *)&arr[0]);
        func_0204f66c(scene, matId2, (u16 *)&arr[0]);
        func_0204f684(scene, matId2, &arr[3], &arr[2]);
        func_0204f794(scene, matId2, &flag);
        flag = func_0204f748(scene, matId);
        func_0204f764(scene, matId2, &flag);
    }
}


u32 func_ov021_021bc338(u16 param_1) {
    return 0xffffff00;
}

s32 func_ov021_021bc340(s32 param_1, s32 param_2) {
    s32 result = 0;

    if (param_1 == 0x71 || param_1 == 0x159) {
        if (param_2 == 0) {
            result = -0x800;
        }
    } else if (param_1 == 0x8f) {
        if (param_2 == 0) {
            result = 2 << 10;
        }
    } else if (param_1 == 0x16b) {
        if (param_2 == 0) {
            result = -0x5800;
        }
    } else if (param_2 == 0) {
        result = 6 << 10;
    }
    return result;
}

BGSetup data_ov021_021de334 = { 0, 0, 2048, 0, 1, 0, 2, 1, 0x8000, 0, 1, 0, 0 };
PlaceName *func_ov021_021bc388(GameSystem *param_1, HeapID param_2, void *param_3) {
    PlaceName *placeName;

    placeName = func_ov021_021bcf88(param_2);
    func_ov021_021bcfa4(placeName, param_1);
    func_ov021_021bd0e0(placeName);
    func_ov021_021bc810(placeName);
    func_ov021_021bcfe0(placeName);
    func_ov021_021bcff0(placeName);
    func_ov021_021bd008(placeName);
    func_ov021_021bd098(placeName, param_2);
    func_ov021_021bd09c(placeName, func_ov021_021904ac(param_3));
    func_ov021_021bc730(placeName);
    func_ov021_021bc75c(placeName);
    func_ov021_021bc794(placeName);
    func_ov021_021bc7bc(placeName, 0);
    func_ov021_021bc998(placeName);
    func_ov021_021bc7d4(placeName);
    GFL_BGSysCreateBG(3, &data_ov021_021de334, 0);
    func_ov021_021bc664(placeName);
    func_ov021_021bc6e8(placeName);
    func_ov021_021bd01c(placeName);
    func_ov021_021bcb9c(placeName, 0);
    return placeName;
}



void func_ov021_021bc424(PlaceName *param_1) {
    func_ov021_021bd0fc(param_1);
    func_ov021_021bc6b4(param_1);
    func_ov021_021bc7f8(param_1);
    func_ov021_021bd068(param_1);
    func_ov021_021bc9ac(param_1);
    func_ov021_021bc7b0(param_1);
    func_ov021_021bc788(param_1);
    func_ov021_021bc748(param_1);
    func_ov021_021bd040(param_1);
    func_ov021_021bd054(param_1);
    func_ov021_021bd07c(param_1);
    func_ov021_021bc704(param_1);
    func_ov021_021bc824(param_1);
    GFL_BGSysReleaseBG(3);
    func_ov021_021bcfd8(param_1);
}

void func_ov021_021bc484(PlaceName *param_1) {
    func_ov021_021bd0b0(param_1);
    switch(func_ov021_021bd0a4(param_1)) {
    case 0:
        func_ov021_021bcc3c(param_1);
        break;
    case 1:
        func_ov021_021bcc80(param_1);
        break;
    case 2:
        func_ov021_021bcd34(param_1);
        break;
    case 3:
        func_ov021_021bcd70(param_1);
        break;
    case 4:
        func_ov021_021bcd84(param_1);
        break;
    case 5:
        func_ov021_021bcdd8(param_1);
        break;
    case 6:
        func_ov021_021bce88(param_1);
    }
    func_ov021_021bc970(param_1);
}

void func_ov021_021bc4f0(PlaceName *param_1) {
    u32 idx;

    idx = func_ov021_021bd0a4(param_1);
    switch (idx) {
    case 0:
        func_ov021_021bceac(param_1);
        return;
    case 1:
        func_ov021_021bceb0(param_1);
        return;
    case 2:
        func_ov021_021bceb4(param_1);
        return;
    case 3:
        func_ov021_021bcf18(param_1);
        return;
    case 4:
        func_ov021_021bcf1c(param_1);
        return;
    case 5:
        func_ov021_021bcf20(param_1);
        return;
    case 6:
        func_ov021_021bcf24(param_1);
        break;
    }
}

