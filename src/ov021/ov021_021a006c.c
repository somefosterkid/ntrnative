// Black's code at ov021 0x021a006c-0x021a0104: the scene node of the actors drawn as a 3D model rather than a billboard
// (data_ov021_021d770c's twelfth). The ROM gives the file no name (pokebw2 guesses fldmmdl_g3dobj_draw.c); it is a file
// of its own because the size sort of the file before (ov021_0219f018.c) lays out that file's 17 tables in Black's
// order only without this one, which lies after them.

#include "types.h"
#include "field/field_actor.h"
#include "field/field_actor_internal.h"
#include "field/field_g3dobj.h"
#include "field/fldmmdl_g3dobj.h"
#include "field/ov021_02197c4c.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"

u16 func_ov021_0219d634(FieldActor *param_1);
void func_ov021_0219d680(FieldActor *param_1, u16 param_2);

void func_ov021_021a006c(FieldActor *param_1) {
    u16 *ptr;

    ptr = FldAct_ResetBlActWork(param_1, 2);
    *ptr = func_ov021_0219d634(param_1);
}

void func_ov021_021a0084(FieldActor *param_1) {
    VecFx32 pos;
    s32 hidden;
    u16 *work;
    FieldG3DObjSystem *sys;

    work = FldAct_GetBlActWorkPtr(param_1);
    sys = (FieldG3DObjSystem *)FldAct_GetG3DObjSystem(param_1);
    CopyActorPosAllAdd(param_1, &pos);
    FieldG3DObjSystem_SetObjPosition(sys, *work, &pos);
    hidden = 0;
    if (func_ov010_0216dab4(param_1) == 1) {
        hidden = 1;
    }
    func_ov021_021c6678(sys, *work, hidden);
    if (func_ov021_02197c9c(param_1) == 0) {
        FieldG3DObjSystem_StepObjAnmLoop(sys, *work, 0x1000);
    }
}


void func_ov021_021a00e0(FieldActor *param_1) {
    u16 *blActWorkPtr;

    blActWorkPtr = FldAct_GetBlActWorkPtr(param_1);
    func_ov021_0219d680(param_1, *blActWorkPtr);
}

void func_ov021_021a00f4(FieldActor *param_1) {
    func_ov021_021a00e0(param_1);
}

void func_ov021_021a00fc(FieldActor *param_1) {
    func_ov021_021a006c(param_1);
}

const FieldActorSceneNodeVTable data_ov021_021d86f0 = {
    func_ov021_021a006c, func_ov021_021a0084, func_ov021_021a00e0,
    func_ov021_021a00f4, func_ov021_021a00fc, NULL,
};
