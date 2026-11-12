// Black's code at ov021 0x021d046c-0x021d05d8. No name is known: the module and address.

#include "types.h"
#include "field/field_actor.h"
#include "field/field_effect.h"
#include "gfl/sound.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"
#include "field/field_g3dobj.h"
#include "field/fldeff_hide.h"
#include "field/fldmmdl_movecode_pair.h"

// What a hiding object starts from (pokebw2's HideTaskParams)
typedef struct {
    FieldEffectHide *effect;
    FieldG3DObjSystem *objSys;
    FieldActor *actor;
    u32 kind;
} HideTaskParams;

// A hiding object's task work (pokebw2's HideTask)
typedef struct {
    HideTaskParams params;
    FieldActorIdentity identity;
    u16 pad;
    u16 objIdx;
    u16 soundPlayed;
    u16 animDone;
    u16 unk_1e;
    fx32 anmFrame;
} HideTask;

const u32 data_ov021_021db934[2] = {0x570, 0x7a6};
extern const FieldEffectTaskVTable data_ov021_021db93c;

SRTMatrix data_ov021_021de628 = {
    {0, 0, 0},
    {0x1000, 0x1000, 0x1000},
    {0x1000, 0, 0, 0, 0x1000, 0, 0, 0, 0x1000},
};

fx32 FieldG3DObjSystem_GetAnmFrame(FieldG3DObjSystem *sys, u16 objIdx, u32 anmIdx);


FieldEffectTask *func_ov021_021d046c(FieldEffects *effects, FieldActor *actor, u32 kind) {
    HideTaskParams params;

    params.effect = FieldEffects_GetHandleData(effects, 0xf);
    params.objSys = func_ov021_021abe18(effects);
    params.actor = actor;
    params.kind = kind;
    return FieldEffects_TCBCreate(effects, &data_ov021_021db93c, NULL, 0, &params, 0);
}

void func_ov021_021d04a4(FieldEffectTask *task) {
    HideTask *hideTask;

    if (task != NULL) {
        hideTask = func_ov021_021ac1e0(task);
        hideTask->soundPlayed = TRUE;
        GFL_SndSEPlay(data_ov021_021db934[hideTask->params.kind]);
    }
}

u16 func_ov021_021d04c4(FieldEffectTask *task) {
    if (task != NULL) {
        return ((HideTask *)func_ov021_021ac1e0(task))->animDone;
    }
    return 1;
}

void func_ov021_021d04d8(FieldEffectTask *task, void *work) {
    HideTask *hideTask = work;

    hideTask->params = *(HideTaskParams *)func_ov021_021ac1ac(task);
    func_ov010_0216e1a4(hideTask->params.actor, &hideTask->identity);
    hideTask->objIdx = hideTask->params.effect->objIdx[hideTask->params.kind];
}


void func_ov021_021d0504(FieldEffectTask *task, void *work) {
    HideTask *hideTask = work;
    BOOL BVar1;

    BVar1 = func_ov010_0216e1c4(hideTask->params.actor, &hideTask->identity);
    if (BVar1 == 1) {
        func_ov021_0219bf3c(hideTask->params.actor, NULL);
    }
}

void func_ov021_021d0520(FieldEffectTask *param_1, void *work) {
    HideTask *hideTask = work;
    BOOL BVar1;
    fx32 fStack_20;
    VecFx32 VStack_1c;

    BVar1 = func_ov010_0216e1c4(hideTask->params.actor, &hideTask->identity);
    if (BVar1 == 0) {
        func_ov021_021ac154(param_1);
        return;
    }
    CopyActorWPos(hideTask->params.actor, &VStack_1c);
    BVar1 = GetHeightFromMap(hideTask->params.actor, &VStack_1c, &fStack_20);
    if (BVar1 == 1) {
        VStack_1c.y = fStack_20;
    }
    func_ov021_021ac1cc(param_1, &VStack_1c);
}

void func_ov021_021d0568(FieldEffectTask *param_1, void *work) {
    HideTask *hideTask = work;
    BOOL BVar1;
    VecFx32 VStack_18;

    if ((hideTask->soundPlayed == 1) && (hideTask->animDone == 0)) {
        FieldG3DObjSystem_SetAnmFrame(hideTask->params.objSys, hideTask->objIdx, 0, hideTask->anmFrame);
        BVar1 = FieldG3DObjSystem_StepObjAnm(hideTask->params.objSys, hideTask->objIdx, 0x1000);
        if (BVar1 == 0) {
            hideTask->animDone = 1;
        }
        hideTask->anmFrame = FieldG3DObjSystem_GetAnmFrame(hideTask->params.objSys, hideTask->objIdx, 0);
    }
    else {
        FieldG3DObjSystem_SetAnmFrame(hideTask->params.objSys, hideTask->objIdx, 0, hideTask->anmFrame);
    }
    func_ov021_021ac1b8(param_1, &VStack_18);
    FieldG3DObjSystem_SetObjPosition(hideTask->params.objSys, hideTask->objIdx, &VStack_18);
    FieldG3DObjSystem_DrawObj(hideTask->params.objSys, hideTask->objIdx);
}

const FieldEffectTaskVTable data_ov021_021db93c = {
    sizeof(HideTask),
    func_ov021_021d04d8,
    func_ov021_021d0504,
    func_ov021_021d0520,
    func_ov021_021d0568,
};
