// Black's code at ov021 0x0219ef44-0x021a006c: the rail data of a map, loaded from the rail archive, and the scene nodes
// of the rail maps' billboard actors and 3D objects (17 of data_ov021_021d770c's tables, each after its update
// function). The name is the file name its heap allocation passes, as pokebw2's src/ov036/field_rail_loader.c has it.

#include "types.h"
#include "gfl/arc_util.h"
#include "gfl/heap.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_actor_internal.h"
#include "field/field_rail_loader.h"
#include "field/zone.h"
#include "gfl/blact.h"
#include "gfl/std.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_vec.h"
#include "nitro/fx/fx_mtx.h"
#include "struct_decls.h"
#include "field/field_script_event.h"
#include "field/field_g3dobj.h"
#include "field/ov021_02197c4c.h"
#include "field/fldmmdl_blact.h"
#include "field/fldmmdl_g3dobj.h"
#include "field/field_camera.h"

// The 4 bytes of a rail billboard actor's animation that func_ov021_0219feb0 updates
typedef struct RailBlActAnim {
    u8 dir;
    u8 state;
    u8 frame;
    u8 unk_03;
} RailBlActAnim;

// The BlAct work of the rail maps' billboard actors (FieldActor.blActWork): their init functions clear 8 bytes of it
typedef struct RailBlActWork {
    u16 actId; // 0xffff: none
    u8 unk_02;
    u8 unk_03;
    RailBlActAnim anim;
    u32 unk_08;
} RailBlActWork;

void func_ov021_0219fe9c(RailBlActAnim *anim);
BOOL func_ov021_0219c814(FieldActor *param_1, u16 *param_2);
void func_ov021_0219feb0(RailBlActAnim *param_1, FieldActor *param_2, BlActSys *param_3, u32 param_4);
void func_ov021_0219ffbc(FieldActor *param_1, VecFx32 *pos);
void func_ov021_021a000c(FieldActor *param_1, BlActSys *param_2, u32 param_3, s32 param_4, s32 param_5);
u8 func_ov021_0219d738(FieldActor *param_1, s32 param_2);
u16 func_ov021_0219d634(FieldActor *param_1);
void func_ov021_0219d680(FieldActor *param_1, u16 param_2);

void func_ov021_0219c8d4(FieldActor *param_1, u32 param_2);

u8 func_ov021_0219ffd4(FieldActor *param_1);
void func_ov021_0219f990(FieldActor *param_1, RailBlActWork *param_2, u8 param_3);
BOOL func_ov021_02188cbc(Field *field);
u32 func_ov021_021a3080(FieldPlayer *player);
u32 func_ov021_0219fadc(RailBlActWork *param_1, s32 param_2, const FieldActorConfig *param_3);
void func_ov021_0219fab4(FieldActor *param_1, s32 param_2, const FieldActorConfig *param_3, fx32 *param_4);
u32 func_ov021_0219f17c(FieldActor *param_1, u32 param_2);
u32 func_ov021_0219f484(FieldActor *param_1, u32 param_2);
u32 func_ov021_0219fb20(FieldActor *param_1, u32 param_2);
u32 func_ov021_0219fc5c(FieldActor *param_1, u32 param_2);

void RailDataHandle_Reset(RailDataHandle *handle);

RailLoader *FieldRailLoader_Create(u32 heapId) {
    RailLoader *loader = GFL_HeapAllocate((HeapID)heapId, sizeof(RailLoader), 1, "field_rail_loader.c", 0xc0);

    RailDataHandle_Reset(&loader->handle);
    return loader;
}

void FieldRailLoader_Free(RailLoader *loader) {
    FieldRailLoader_Reset(loader);
    GFL_HeapFree(loader);
}

void FieldRailLoader_LoadFile(RailLoader *loader, u32 fileId, u32 heapId) {
    u32 size;

    loader->data = GFL_ArcSysReadHeapNewLZGetLen(0x4f, fileId, 0, (HeapID)heapId, &size);
    loader->size = size;
    loader->handle.points = (RailPoint *)((u8 *)loader->data + loader->data->pointsOffset);
    loader->handle.pointCount = (loader->data->linesOffset - loader->data->pointsOffset) / sizeof(RailPoint);
    loader->handle.lines = (RailLine *)((u8 *)loader->data + loader->data->linesOffset);
    loader->handle.lineCount = (loader->data->camerasOffset - loader->data->linesOffset) / sizeof(RailLine);
    loader->handle.cameras = (RailCamera *)((u8 *)loader->data + loader->data->camerasOffset);
    loader->handle.cameraCount = (loader->data->curvesOffset - loader->data->camerasOffset) / sizeof(RailCamera);
    loader->handle.curves = (RailCurve *)((u8 *)loader->data + loader->data->curvesOffset);
    loader->handle.curveCount = (loader->data->endOffset - loader->data->curvesOffset) / sizeof(RailCurve);
    loader->handle.tileWidth = *(fx32 *)((u8 *)loader->data + loader->data->tileWidthOffset);
}

void FieldRailLoader_Reset(RailLoader *loader) {
    if (loader->data != NULL) {
        GFL_HeapFree(loader->data);
        loader->data = NULL;
    }
    RailDataHandle_Reset(&loader->handle);
}

RailDataHandle *FieldRailLoader_GetRailData(RailLoader *loader) {
    return &loader->handle;
}

void CreatePlayerXActorScriptCall(GameSystem *param_1, u32 param_2, u32 param_3, u32 param_4, FieldActor *param_5, HeapID param_6) {
    EventScriptCall_Create(param_1, param_3, param_5, param_6);
}


void func_ov021_0219f030(FieldActor *param_1) {
    SetActorHidden(param_1, 1);
    SetActorMovementFlag(param_1, 0x8000);
}

void func_ov021_0219f048(FieldActor *param_1) {
}

void func_ov021_0219f04c(FieldActor *param_1) {
}

void func_ov021_0219f050(FieldActor *param_1) {
}

void func_ov021_0219f054(FieldActor *param_1) {
}

void func_ov021_0219f058(FieldActor *param_1, u16 *param_2, u16 *param_3) {
    RailBlActWork *work;
    BlActSys *sys;
    if (GetActorMdlInfo(param_1)->entityType == 1) {
        work = FldAct_GetBlActWorkPtr(param_1);
        if (work->actId != 0xffff) {
            sys = FieldBlAct_GetBlActSys(FldAct_GetFieldBlAct(param_1));
            *param_2 = func_02050848(sys, work->actId);
            *param_3 = func_02050880(sys, work->actId);
            return;
        }
        *param_2 = 0;
        *param_3 = 0;
        return;
    }
    *param_2 = 0;
    *param_3 = 0;
}

const FieldActorSceneNodeVTable data_ov021_021d86c0 = {
    func_ov021_0219f030, func_ov021_0219f048, func_ov021_0219f04c,
    func_ov021_0219f050, func_ov021_0219f054, NULL,
};


void func_ov021_0219f0b0(FieldActor *actor, u16 a1, u16 a2) {
    const FieldActorConfig *pFVar1;
    RailBlActWork *blActWorkPtr;
    BlActSys *sys;

    pFVar1 = GetActorMdlInfo(actor);
    if ((pFVar1->entityType == 1) && (blActWorkPtr = FldAct_GetBlActWorkPtr(actor), blActWorkPtr->actId != 0xffff))
    {
        sys = FieldBlAct_GetBlActSys(FldAct_GetFieldBlAct(actor));
        func_0205082c(sys, (u32)blActWorkPtr->actId, a1);
        func_0205085c(sys, (u32)blActWorkPtr->actId, a2);
    }
}

void func_ov021_0219f0f8(FieldActor *param_1) {
    RailBlActWork *ptr;

    ptr = FldAct_ResetBlActWork(param_1, 8);
    func_ov021_0219fe9c(&ptr->anim);
    func_ov021_0219c814(param_1, &ptr->actId);
}

void func_ov021_0219f114(FieldActor *param_1) {
    RailBlActWork *blActWorkPtr;

    blActWorkPtr = FldAct_GetBlActWorkPtr(param_1);
    func_ov021_0219c8d4(param_1, (u32)blActWorkPtr->actId);
}

void func_ov021_0219f128(FieldActor *param_1) {
    RailBlActWork *blActWorkPtr;
    BlActSys *sys;
    BlActScene *scene;
    VecFx32 VStack_20;

    blActWorkPtr = FldAct_GetBlActWorkPtr(param_1);
    if (blActWorkPtr->actId != 0xffff) {
        sys = FieldBlAct_GetBlActSys(FldAct_GetFieldBlAct(param_1));
        func_ov021_0219feb0(&blActWorkPtr->anim, param_1, sys, (u32)blActWorkPtr->actId);
        CopyActorPosAllAdd(param_1, &VStack_20);
        func_ov021_0219ffbc(param_1, &VStack_20);
        scene = BlActSys_GetScene(sys);
        BlActScene_SetActorPos(scene, (u32)blActWorkPtr->actId, &VStack_20);
    }
}

const FieldActorSceneNodeVTable data_ov021_021d85b8 = {
    func_ov021_0219f0f8, func_ov021_0219f128, func_ov021_0219f114,
    func_ov021_0219f114, func_ov021_0219f0f8, func_ov021_0219f17c,
};
const FieldActorSceneNodeVTable data_ov021_021d85d0 = {
    func_ov021_0219f0f8, func_ov021_0219f128, func_ov021_0219f114,
    func_ov021_0219f114, func_ov021_0219f0f8, func_ov021_0219f17c,
};

u32 func_ov021_0219f17c(FieldActor *param_1, u32 param_2) {
    RailBlActWork *blActWorkPtr;

    blActWorkPtr = FldAct_GetBlActWorkPtr(param_1);
    return blActWorkPtr->actId;
}

void func_ov021_0219f188(FieldActor *param_1) {
    int changed = 0;
    RailBlActWork *work;
    u8 anim;
    u16 kind;
    BlActSys *sys;
    BlActScene *scene;
    VecFx32 pos;
    work = FldAct_GetBlActWorkPtr(param_1);
    if (work->actId != 0xffff) {
        sys = FieldBlAct_GetBlActSys(FldAct_GetFieldBlAct(param_1));
        anim = func_ov021_0219ffd4(param_1);
        kind = func_ov010_0216d678(param_1);
        if (work->anim.dir != anim) {
            work->anim.dir = anim;
            work->anim.state = kind;
            func_0205082c(sys, work->actId, anim);
            changed = 1;
        } else if (work->anim.state != kind) {
            func_0205082c(sys, work->actId, anim);
            func_0205085c(sys, work->actId, changed);
            work->anim.state = kind;
        }
        func_ov021_021a000c(param_1, sys, work->actId, changed, 0);
        CopyActorPosAllAdd(param_1, &pos);
        func_ov021_0219ffbc(param_1, &pos);
        scene = BlActSys_GetScene(sys);
        BlActScene_SetActorPos(scene, work->actId, &pos);
    }
}

const FieldActorSceneNodeVTable data_ov021_021d8678 = {
    func_ov021_0219f0f8, func_ov021_0219f188, func_ov021_0219f114,
    func_ov021_0219f114, func_ov021_0219f0f8, func_ov021_0219f17c,
};


void func_ov021_0219f22c(FieldActor *param_1) {
    RailBlActWork *ptr;

    ptr = FldAct_ResetBlActWork(param_1, 8);
    func_ov021_0219fe9c(&ptr->anim);
    func_ov021_0219c814(param_1, &ptr->actId);
}

void func_ov021_0219f248(FieldActor *param_1) {
    RailBlActWork *blActWorkPtr;

    blActWorkPtr = FldAct_GetBlActWorkPtr(param_1);
    func_ov021_0219c8d4(param_1, (u32)blActWorkPtr->actId);
}

void func_ov021_0219f25c(FieldActor *param_1) {
    RailBlActWork *blActWorkPtr;
    BlActSys *sys;
    BlActScene *scene;
    VecFx32 VStack_20;

    blActWorkPtr = FldAct_GetBlActWorkPtr(param_1);
    if (blActWorkPtr->actId != 0xffff) {
        sys = FieldBlAct_GetBlActSys(FldAct_GetFieldBlAct(param_1));
        func_ov021_0219feb0(&blActWorkPtr->anim, param_1, sys, (u32)blActWorkPtr->actId);
        CopyActorPosAllAdd(param_1, &VStack_20);
        func_ov021_0219ffbc(param_1, &VStack_20);
        scene = BlActSys_GetScene(sys);
        BlActScene_SetActorPos(scene, (u32)blActWorkPtr->actId, &VStack_20);
    }
}

const FieldActorSceneNodeVTable data_ov021_021d8660 = {
    func_ov021_0219f22c, func_ov021_0219f25c, func_ov021_0219f248,
    func_ov021_0219f248, func_ov021_0219f22c, func_ov021_0219f484,
};

void func_ov021_0219f2b0(FieldActor *param_1) {
    int changed = 0;
    u32 flag = 0;
    RailBlActWork *work;
    BlActSys *sys;
    BlActScene *scene;
    const FieldActorConfig *cfg;
    s32 offset;
    u8 anim;
    u16 idx;
    VecFx32 pos;
    work = FldAct_GetBlActWorkPtr(param_1);
    if (work->actId != 0xffff) {
        sys = FieldBlAct_GetBlActSys(FldAct_GetFieldBlAct(param_1));
        anim = func_ov021_0219ffd4(param_1);
        if (FldAct_GetObjCode(param_1) == 0xcc) {
            flag = 1;
        }
        cfg = GetActorMdlInfo(param_1);
        if (cfg->sceneNodeType == 0xf) {
            offset = 4;
        } else if (cfg->sceneNodeType == 5) {
            offset = 8;
        } else {
            offset = 0x10;
        }
        idx = offset + anim;
        if (work->anim.dir != anim) {
            work->anim.dir = anim;
            func_0205082c(sys, work->actId, idx);
            changed = 1;
        }
        func_ov021_021a000c(param_1, sys, work->actId, changed, flag);
        CopyActorPosAllAdd(param_1, &pos);
        func_ov021_0219ffbc(param_1, &pos);
        scene = BlActSys_GetScene(sys);
        BlActScene_SetActorPos(scene, work->actId, &pos);
    }
}

const FieldActorSceneNodeVTable data_ov021_021d8648 = {
    func_ov021_0219f22c, func_ov021_0219f2b0, func_ov021_0219f248,
    func_ov021_0219f248, func_ov021_0219f22c, func_ov021_0219f484,
};


void func_ov021_0219f354(FieldActor *param_1) {
    RailBlActWork *work;
    u16 kind;
    BlActSys *sys;
    BlActScene *scene;
    u8 changed;
    u32 anim;
    VecFx32 pos;
    work = FldAct_GetBlActWorkPtr(param_1);
    if (work->actId != 0xffff) {
        sys = FieldBlAct_GetBlActSys(FldAct_GetFieldBlAct(param_1));
        kind = func_ov010_0216d678(param_1);
        if (kind >= 10) {
            changed = 0;
            anim = func_ov021_0219ffd4(param_1);
            if (kind != work->anim.state) {
                changed = 1;
                work->anim.dir = anim;
                work->anim.state = kind;
                work->anim.frame = 0;
                func_0205082c(sys, work->actId, kind + 0x1e);
            }
            func_ov021_021a000c(param_1, sys, work->actId, changed, 0);
        } else {
            func_ov021_0219feb0(&work->anim, param_1, sys, work->actId);
        }
        CopyActorPosAllAdd(param_1, &pos);
        func_ov021_0219ffbc(param_1, &pos);
        scene = BlActSys_GetScene(sys);
        BlActScene_SetActorPos(scene, work->actId, &pos);
    }
}

const FieldActorSceneNodeVTable data_ov021_021d8630 = {
    func_ov021_0219f22c, func_ov021_0219f354, func_ov021_0219f248,
    func_ov021_0219f248, func_ov021_0219f22c, func_ov021_0219f484,
};


void func_ov021_0219f3f0(FieldActor *param_1) {
    u16 result;
    RailBlActWork *blActWorkPtr;
    BlActSys *sys;
    u32 result2;
    BlActScene *scene;
    VecFx32 VStack_24;
    blActWorkPtr = FldAct_GetBlActWorkPtr(param_1);
    if (blActWorkPtr->actId != 0xffff) {
        sys = FieldBlAct_GetBlActSys(FldAct_GetFieldBlAct(param_1));
        result = func_ov010_0216d678(param_1);
        if (result >= 10) {
            result2 = func_ov021_0219ffd4(param_1);
            if (result != blActWorkPtr->anim.state) {
                blActWorkPtr->anim.dir = (s8)result2;
                blActWorkPtr->anim.state = (s8)result;
                blActWorkPtr->anim.frame = 0;
                func_0205082c(sys, (u32)blActWorkPtr->actId, result + 0x1e);
            }
            func_ov021_021a000c(param_1, sys, (u32)blActWorkPtr->actId, 1, 0);
        } else {
            func_ov021_0219feb0(&blActWorkPtr->anim, param_1, sys, (u32)blActWorkPtr->actId);
        }
        CopyActorPosAllAdd(param_1, &VStack_24);
        func_ov021_0219ffbc(param_1, &VStack_24);
        scene = BlActSys_GetScene(sys);
        BlActScene_SetActorPos(scene, (u32)blActWorkPtr->actId, &VStack_24);
    }
}

const FieldActorSceneNodeVTable data_ov021_021d8618 = {
    func_ov021_0219f22c, func_ov021_0219f3f0, func_ov021_0219f248,
    func_ov021_0219f248, func_ov021_0219f22c, func_ov021_0219f484,
};

u32 func_ov021_0219f484(FieldActor *param_1, u32 param_2) {
    RailBlActWork *blActWorkPtr;

    blActWorkPtr = FldAct_GetBlActWorkPtr(param_1);
    return blActWorkPtr->actId;
}

void func_ov021_0219f490(FieldActor *param_1) {
    RailBlActWork *work;
    u16 kind;
    BlActSys *sys;
    BlActScene *scene;
    VecFx32 pos;
    work = FldAct_GetBlActWorkPtr(param_1);
    if (work->actId != 0xffff) {
        sys = FieldBlAct_GetBlActSys(FldAct_GetFieldBlAct(param_1));
        kind = func_ov010_0216d678(param_1);
        if (work->anim.state >= 2) {
            work->anim.state = 0;
            func_0205082c(sys, work->actId, work->anim.state);
            func_0205085c(sys, work->actId, 0);
        }
        if (kind < 2 && work->anim.state != kind) {
            work->anim.state = kind;
            func_0205082c(sys, work->actId, work->anim.state);
            func_0205085c(sys, work->actId, 0);
        }
        func_ov021_021a000c(param_1, sys, work->actId, 0, 0);
        CopyActorPosAllAdd(param_1, &pos);
        func_ov021_0219ffbc(param_1, &pos);
        scene = BlActSys_GetScene(sys);
        BlActScene_SetActorPos(scene, work->actId, &pos);
    }
}

const FieldActorSceneNodeVTable data_ov021_021d8600 = {
    func_ov021_0219f0f8, func_ov021_0219f490, func_ov021_0219f114,
    func_ov021_0219f114, func_ov021_0219f0f8, func_ov021_0219f17c,
};

void func_ov021_0219f52c(FieldActor *param_1) {
    RailBlActWork *work;
    u16 kind;
    BlActSys *sys;
    BlActScene *scene;
    VecFx32 pos;
    work = FldAct_GetBlActWorkPtr(param_1);
    if (work->actId != 0xffff) {
        sys = FieldBlAct_GetBlActSys(FldAct_GetFieldBlAct(param_1));
        kind = func_ov010_0216d678(param_1);
        if (work->anim.state >= 3) {
            work->anim.state = 0;
            func_0205082c(sys, work->actId, work->anim.state);
            func_0205085c(sys, work->actId, 0);
        }
        if (kind < 3 && work->anim.state != kind) {
            work->anim.state = kind;
            func_0205082c(sys, work->actId, work->anim.state);
            func_0205085c(sys, work->actId, 0);
        }
        func_ov021_021a000c(param_1, sys, work->actId, 0, 0);
        CopyActorPosAllAdd(param_1, &pos);
        func_ov021_0219ffbc(param_1, &pos);
        scene = BlActSys_GetScene(sys);
        BlActScene_SetActorPos(scene, work->actId, &pos);
    }
}

const FieldActorSceneNodeVTable data_ov021_021d85e8 = {
    func_ov021_0219f0f8, func_ov021_0219f52c, func_ov021_0219f114,
    func_ov021_0219f114, func_ov021_0219f0f8, func_ov021_0219f17c,
};



void func_ov021_0219f5c8(FieldActor *param_1, u32 param_2) {
    ((RailBlActWork *)FldAct_GetBlActWorkPtr(param_1))->unk_02 = param_2;
}

void TransformVecByCamera(G3DCamera *param_1, VecFx32 *param_2, VecFx32 *param_3) {
    VecFx32 VStack_24;
    VecFx32 VStack_30;
    VecFx32 VStack_3c;
    MtxFx43 MStack_6c;
    MtxFx33 MStack_90;


    GFL_G3DCameraGetLookatPos(param_1, &VStack_24);
    GFL_G3DCameraGetLookatUpVector(param_1, &VStack_30);
    GFL_G3DCameraGetLookatTarget(param_1, &VStack_3c);
    MTX_LookAt(&VStack_24, &VStack_30, &VStack_3c, &MStack_6c);
    MI_Copy36B(&MStack_6c, &MStack_90);
    MTX_Inverse33(&MStack_90, &MStack_90);
    MTX_MultVec33(param_2, &MStack_90, param_3);
}

void func_ov021_0219f628(FieldActor *param_1, u32 param_2, u32 param_3, VecFx32 *out) {
    out->x = 0;
    out->y = 0;
    out->z = 0;
}

static const VecFx32 data_ov021_021d854c = { 0x00000000, -0xd000, 0x00000000 };
void func_ov021_0219f634(FieldActor *param_1) {
    RailBlActWork *work;
    BlActSys *sys;
    BlActScene *scene;
    MMSys *mmSys;
    Field *field;
    FieldCamera *camera;
    G3DCamera *g3d;
    int changed = 0;
    u32 anim;
    u16 kind;
    u16 idx;
    u16 a;
    u16 b;
    VecFx32 pos;
    VecFx32 delta;
    VecFx32 off;
    work = FldAct_GetBlActWorkPtr(param_1);
    if (work->actId != 0xffff) {
        sys = FieldBlAct_GetBlActSys(FldAct_GetFieldBlAct(param_1));
        anim = func_ov021_0219ffd4(param_1);
        kind = func_ov010_0216d678(param_1);
        idx = anim + kind * 4;
        if (anim != work->anim.dir || work->anim.state != kind) {
            work->anim.dir = anim;
            changed = 1;
            work->anim.state = kind;
            func_0205082c(sys, work->actId, idx);
            func_0205085c(sys, work->actId, 0);
        }
        func_ov021_021a000c(param_1, sys, work->actId, changed, 1);
        CopyActorPosAllAdd(param_1, &pos);
        func_ov021_0219ffbc(param_1, &pos);
        a = func_02050848(sys, work->actId);
        b = func_02050880(sys, work->actId);
        func_ov021_0219f628(param_1, a, b, &delta);
        pos.x += delta.x;
        pos.y += delta.y;
        pos.z += delta.z;
        if (work->unk_02 == 0) {
            off = data_ov021_021d854c;
            mmSys = GetActorMModelSystem(param_1);
            field = GetMMSysField(mmSys);
            camera = Field_GetCameraSystem(field);
            g3d = FieldCamera_GetG3DCamera(camera);
            TransformVecByCamera(g3d, &off, &off);
            VEC_Add(&pos, &off, &pos);
        } else {
            pos.y -= 0xc000;
            pos.z += 0x8000;
        }
        scene = BlActSys_GetScene(sys);
        BlActScene_SetActorPos(scene, work->actId, &pos);
    }
}

const FieldActorSceneNodeVTable data_ov021_021d8690 = {
    func_ov021_0219f0f8, func_ov021_0219f634, func_ov021_0219f114,
    func_ov021_0219f114, func_ov021_0219f0f8, func_ov021_0219f17c,
};


void func_ov021_0219f750(FieldActor *param_1) {
    RailBlActWork *work;
    BlActSys *sys;
    BlActScene *scene;
    u8 anim;
    VecFx32 pos;
    work = FldAct_GetBlActWorkPtr(param_1);
    if (work->actId != 0xffff) {
        sys = FieldBlAct_GetBlActSys(FldAct_GetFieldBlAct(param_1));
        anim = func_ov021_0219ffd4(param_1);
        if (work->anim.dir != anim) {
            func_0205082c(sys, work->actId, anim);
            func_0205085c(sys, work->actId, 0);
            work->anim.dir = anim;
        }
        func_ov021_021a000c(param_1, sys, work->actId, 0, 0);
        CopyActorPosAllAdd(param_1, &pos);
        func_ov021_0219ffbc(param_1, &pos);
        scene = BlActSys_GetScene(sys);
        BlActScene_SetActorPos(scene, work->actId, &pos);
    }
}

const FieldActorSceneNodeVTable data_ov021_021d85a0 = {
    func_ov021_0219f0f8, func_ov021_0219f750, func_ov021_0219f114,
    func_ov021_0219f114, func_ov021_0219f0f8, func_ov021_0219f17c,
};


BOOL func_ov021_0219f7cc(FieldActor *actor, u16 time) {
    int idx;
    BlActSys *sys;
    RailBlActWork *work;
    work = FldAct_GetBlActWorkPtr(actor);
    if (work->actId == 0xffff) {
        return 0;
    }
    if (time < 0x18) {
        if (time < 6) {
            idx = 0;
        } else if (time < 0xc) {
            idx = 1;
        } else if (time < 0x12) {
            idx = 2;
        } else {
            idx = 3;
        }
    } else if (time < 0x28) {
        if (time < 0x1c) {
            idx = 4;
        } else if (time < 0x20) {
            idx = 5;
        } else if (time < 0x24) {
            idx = 6;
        } else {
            idx = 7;
        }
    } else if (time < 0x30) {
        if (time < 0x2a) {
            idx = 8;
        } else if (time < 0x2c) {
            idx = 9;
        } else if (time < 0x2e) {
            idx = 10;
        } else {
            idx = 11;
        }
    } else {
        idx = 12;
    }
    sys = FieldBlAct_GetBlActSys(FldAct_GetFieldBlAct(actor));
    u16 cur = func_02050880(sys, work->actId);
    if (cur != idx) {
        func_0205085c(sys, work->actId, 0);
    }
    return 1;
}


void func_ov021_0219f870(FieldActor *param_1) {
    RailBlActWork *blActWorkPtr;
    BlActSys *sys;
    BlActScene *scene;
    VecFx32 VStack_24;

    blActWorkPtr = FldAct_GetBlActWorkPtr(param_1);
    if (blActWorkPtr->actId != 0xffff) {
        sys = FieldBlAct_GetBlActSys(FldAct_GetFieldBlAct(param_1));
        func_ov021_021a000c(param_1, sys, (u32)blActWorkPtr->actId, 0, 0);
        CopyActorPosAllAdd(param_1, &VStack_24);
        func_ov021_0219ffbc(param_1, &VStack_24);
        scene = BlActSys_GetScene(sys);
        BlActScene_SetActorPos(scene, (u32)blActWorkPtr->actId, &VStack_24);
    }
}

const FieldActorSceneNodeVTable data_ov021_021d8588 = {
    func_ov021_0219f22c, func_ov021_0219f870, func_ov021_0219f248,
    func_ov021_0219f248, func_ov021_0219f22c, func_ov021_0219f484,
};

void func_ov021_0219f8c8(FieldActor *param_1) {
    RailBlActWork *ptr;

    ptr = FldAct_ResetBlActWork(param_1, 8);
    ptr->unk_02 = 9;
    func_ov021_0219c814(param_1, &ptr->actId);
}

void func_ov021_0219f8e0(FieldActor *param_1) {
    RailBlActWork *blActWorkPtr;

    blActWorkPtr = FldAct_GetBlActWorkPtr(param_1);
    func_ov021_0219c8d4(param_1, (u32)blActWorkPtr->actId);
}

void func_ov021_0219f8f4(FieldActor *param_1) {
    RailBlActWork *work;
    BlActScene *scene;
    u16 face;
    s32 changed;
    BlActSys *sys;
    VecFx32 pos;
    work = FldAct_GetBlActWorkPtr(param_1);
    if (work->actId != 0xffff) {
        changed = 0;
        sys = FieldBlAct_GetBlActSys(FldAct_GetFieldBlAct(param_1));
        face = GetActorFaceDir(param_1);
        if (face != work->unk_02) {
            changed = 1;
            work->unk_02 = face;
            work->unk_03 = 0;
            work->unk_08 = 0;
            func_0205082c(sys, work->actId, work->unk_02);
            {
                VecFx32 zero = { 0, 0, 0 };
                func_ov010_0216d8b0(param_1, &zero);
            }
        }
        func_ov021_0219f990(param_1, work, face);
        CopyActorPosAllAdd(param_1, &pos);
        func_ov021_0219ffbc(param_1, &pos);
        scene = BlActSys_GetScene(sys);
        BlActScene_SetActorPos(scene, work->actId, &pos);
        func_ov021_021a000c(param_1, sys, work->actId, changed, 0);
    }
}

const FieldActorSceneNodeVTable data_ov021_021d8570 = {
    func_ov021_0219f8c8, func_ov021_0219f8f4, func_ov021_0219f8e0,
    func_ov021_0219f8e0, func_ov021_0219f8c8, func_ov021_0219fb20,
};

void func_ov021_0219f990(FieldActor *param_1, RailBlActWork *param_2, u8 param_3) {
    BOOL walking;
    BOOL isPlayer;
    VecFx32 offset;
    VecFx32 base;
    Field *field;
    BOOL hidden;
    u16 zone;
    s16 gx;
    const FieldActorConfig *cfg;
    BOOL isPuppet;
    u16 uid;
    u8 rem;
    MMSys *mmSys;
    FieldPlayer *player;

    walking = TRUE;
    cfg = GetActorMdlInfo(param_1);
    isPlayer = FALSE;
    offset.x = 0;
    offset.y = 0;
    offset.z = 0;
    hidden = func_ov021_02197c9c(param_1);
    func_ov010_0216d8b0(param_1, &base);
    gx = GetGPosX(param_1);
    zone = GetActorZoneID(param_1);
    uid = GetActorUID(param_1);
    if (uid == 0xff) {
        isPlayer = TRUE;
    }
    isPuppet = uid == 0xfc ? TRUE : FALSE;
    if (isPlayer) {
        mmSys = GetActorMModelSystem(param_1);
        field = GetMMSysField(mmSys);
        if (func_ov021_02188cbc(field)) {
            player = Field_GetPlayer(field);
            if (player != NULL) {
                walking = func_ov021_021a3080(player) != 0;
            }
        }
    }
    if (hidden != 0 || (base.y > 0x1800 && cfg->sceneNodeType != 0xb)) {
        offset.y = base.y;
    } else if (func_ov021_0219fadc(param_2, param_3, cfg) == 0 && walking) {
        offset.y += 0x1800;
    }
    if (GetZoneIsEntreeForest(zone) && !isPlayer) {
        rem = gx % 3;
        if (rem == 0) {
            offset.z -= 0x219a;
        } else if (rem == 2) {
            offset.z += 0x219a;
        }
        offset.z -= 2 << 12;
    }
    func_ov021_0219fab4(param_1, param_3, cfg, &offset.x);
    if (isPuppet) {
        offset.x = 0;
        offset.y = 0;
        offset.z = 0;
    }
    SetActorWPosOffset(param_1, &offset);
    param_2->unk_03 += 1;
}



void func_ov021_0219fab4(FieldActor *param_1, s32 param_2, const FieldActorConfig *param_3, fx32 *param_4) {
    if (param_3->billboardSize == 0) {
        switch (param_2) {
        case 2:
            *param_4 -= 2 << 12;
            break;
        case 3:
            *param_4 += 2 << 12;
            break;
        }
    }
}

u32 func_ov021_0219fadc(RailBlActWork *param_1, s32 param_2, const FieldActorConfig *param_3) {
    s32 rem = param_1->unk_03 % 20;
    param_1->unk_03 = rem;
    if (param_3->sceneNodeType == 11) {
        if (param_2 == 1) {
            if (rem < 5 || rem >= 15) {
                return 1;
            }
        } else if (rem < 10) {
            return 1;
        }
    } else if ((rem >= 5 && rem < 10) || rem >= 15) {
        return 1;
    }
    return 0;
}



u32 func_ov021_0219fb20(FieldActor *param_1, u32 param_2) {
    RailBlActWork *blActWorkPtr;

    blActWorkPtr = FldAct_GetBlActWorkPtr(param_1);
    return blActWorkPtr->actId;
}

void func_ov021_0219fb2c(FieldActor *param_1) {
    RailBlActWork *ptr;

    ptr = FldAct_ResetBlActWork(param_1, 8);
    func_ov021_0219fe9c(&ptr->anim);
    func_ov021_0219c814(param_1, &ptr->actId);
}

void func_ov021_0219fb48(FieldActor *param_1) {
    RailBlActWork *blActWorkPtr;

    blActWorkPtr = FldAct_GetBlActWorkPtr(param_1);
    func_ov021_0219c8d4(param_1, (u32)blActWorkPtr->actId);
}

void func_ov021_0219fb5c(FieldActor *param_1) {
    int changed = 0;
    BlActSys *sys;
    u16 objCode;
    u16 kind;
    RailBlActWork *work;
    BlActScene *scene;
    u16 anim;
    u32 dir;
    VecFx32 pos;
    work = FldAct_GetBlActWorkPtr(param_1);
    if (work->actId != 0xffff) {
        sys = FieldBlAct_GetBlActSys(FldAct_GetFieldBlAct(param_1));
        objCode = FldAct_GetObjCode(param_1);
        kind = func_ov010_0216d678(param_1);
        if (objCode == 0xd3 || objCode == 0xd5 || objCode == 0x79) {
            anim = 0;
            if (kind != work->anim.state) {
                switch (kind) {
                case 10:
                    anim = 1;
                    break;
                case 11:
                    anim = 2;
                    break;
                case 12:
                    anim = 3;
                    break;
                }
                work->anim.state = kind;
                func_0205082c(sys, work->actId, anim);
                changed = 1;
            }
        } else if (objCode == 0xd4 || objCode == 0xd6) {
            dir = func_ov021_0219ffd4(param_1);
            anim = 0;
            if (kind < 10) {
                if (dir != work->anim.dir || kind != work->anim.state) {
                    if (dir == 0) {
                        anim = 0;
                    } else if (dir == 1) {
                        anim = 1;
                    }
                    work->anim.dir = dir;
                    work->anim.state = kind;
                    func_0205082c(sys, work->actId, anim);
                    changed = 1;
                }
            } else if (kind != work->anim.state) {
                switch (kind) {
                case 10:
                    anim = 2;
                    break;
                case 11:
                    anim = 3;
                    break;
                }
                work->anim.state = kind;
                func_0205082c(sys, work->actId, anim);
                changed = 1;
            }
        } else {
            if (kind != work->anim.state) {
                work->anim.state = kind;
                func_0205082c(sys, work->actId, changed);
                changed = 1;
            }
        }
        func_ov021_021a000c(param_1, sys, work->actId, changed, 1);
        CopyActorPosAllAdd(param_1, &pos);
        func_ov021_0219ffbc(param_1, &pos);
        scene = BlActSys_GetScene(sys);
        BlActScene_SetActorPos(scene, work->actId, &pos);
    }
}

const FieldActorSceneNodeVTable data_ov021_021d86a8 = {
    func_ov021_0219fb2c, func_ov021_0219fb5c, func_ov021_0219fb48,
    func_ov021_0219fb48, func_ov021_0219fb2c, func_ov021_0219fc5c,
};


u32 func_ov021_0219fc5c(FieldActor *param_1, u32 param_2) {
    RailBlActWork *blActWorkPtr;

    blActWorkPtr = FldAct_GetBlActWorkPtr(param_1);
    return blActWorkPtr->actId;
}

u32 func_ov021_0219fc68(FieldActor *param_1, u16 *param_2, u16 *param_3) {
    u16 objCode = FldAct_GetObjCode(param_1);
    RailBlActWork *work;
    BlActSys *sys;
    if (objCode == 0xd3 || objCode == 0xd5) {
        work = FldAct_GetBlActWorkPtr(param_1);
        if (work->actId != 0xffff) {
            sys = FieldBlAct_GetBlActSys(FldAct_GetFieldBlAct(param_1));
            *param_2 = func_02050848(sys, work->actId);
            *param_3 = func_02050880(sys, work->actId);
            return 1;
        }
        return 0;
    }
    *param_2 = 0;
    *param_3 = 0;
    return 1;
}

u32 func_ov021_0219fcc0(FieldActor *param_1, u16 *param_2, u16 *param_3) {
    BlActSys *sys;
    RailBlActWork *work;

    if ((u16)(FldAct_GetObjCode(param_1) + 0xFF23) <= 1) {
        work = FldAct_GetBlActWorkPtr(param_1);
        if (work->actId != 0xFFFF) {
            sys = FieldBlAct_GetBlActSys(FldAct_GetFieldBlAct(param_1));
            *param_2 = func_02050848(sys, work->actId);
            *param_3 = func_02050880(sys, work->actId);
            return 1;
        }
        return 0;
    }
    *param_2 = 0;
    *param_3 = 0;
    return 1;
}



void func_ov021_0219fd1c(FieldActor *param_1) {
    s32 flag;
    RailBlActWork *blActWorkPtr;
    BlActSys *sys;
    u16 anim;
    VecFx32 VStack_24;
    BlActScene *scene;


    flag = 0;
    blActWorkPtr = FldAct_GetBlActWorkPtr(param_1);
    if (blActWorkPtr->actId != 0xffff) {
        sys = FieldBlAct_GetBlActSys(FldAct_GetFieldBlAct(param_1));
        anim = func_ov010_0216d678(param_1);
        if ((anim != blActWorkPtr->anim.state) && (anim < 2)) {
            blActWorkPtr->anim.state = (s8)anim;
            func_0205082c(sys, (u32)blActWorkPtr->actId, anim);
            flag = 1;
        }
        func_ov021_021a000c(param_1, sys, (u32)blActWorkPtr->actId, flag, 0);
        CopyActorPosAllAdd(param_1, &VStack_24);
        func_ov021_0219ffbc(param_1, &VStack_24);
        scene = BlActSys_GetScene(sys);
        BlActScene_SetActorPos(scene, (u32)blActWorkPtr->actId, &VStack_24);
    }
}

const FieldActorSceneNodeVTable data_ov021_021d8558 = {
    func_ov021_0219f22c, func_ov021_0219fd1c, func_ov021_0219f248,
    func_ov021_0219f248, func_ov021_0219f22c, func_ov021_0219f484,
};

void func_ov021_0219fd94(FieldActor *param_1) {
    RailBlActWork *work;
    BlActSys *sys;
    BlActScene *scene;
    u8 mode = 0;
    u16 kind;
    VecFx32 pos;
    work = FldAct_GetBlActWorkPtr(param_1);
    if (work->actId != 0xffff) {
        sys = FieldBlAct_GetBlActSys(FldAct_GetFieldBlAct(param_1));
        kind = func_ov010_0216d678(param_1);
        if (kind != work->anim.state) {
            switch (kind) {
            case 10:
                mode = 1;
                break;
            case 11:
                mode = 2;
                break;
            case 12:
                mode = 3;
                break;
            }
            work->anim.state = kind;
            func_0205082c(sys, work->actId, mode);
            mode = 1;
        }
        func_ov021_021a000c(param_1, sys, work->actId, mode, 0);
        CopyActorPosAllAdd(param_1, &pos);
        func_ov021_0219ffbc(param_1, &pos);
        scene = BlActSys_GetScene(sys);
        BlActScene_SetActorPos(scene, work->actId, &pos);
    }
}

const FieldActorSceneNodeVTable data_ov021_021d86d8 = {
    func_ov021_0219f22c, func_ov021_0219fd94, func_ov021_0219f248,
    func_ov021_0219f248, func_ov021_0219f22c, func_ov021_0219f484,
};


u32 func_ov021_0219fe20(FieldActor *param_1) {
    u16 objCode = FldAct_GetObjCode(param_1);
    u16 blActIdx;
    u16 cmd;
    BlActSys *sys;
    if (objCode != 0x7b) {
        return 1;
    }
    blActIdx = FldAct_GetBlActIdx(param_1, 0);
    if (blActIdx != 0xffff) {
        sys = FieldBlAct_GetBlActSys(FldAct_GetFieldBlAct(param_1));
        if (func_020508ec(sys, blActIdx, &cmd) == 1) {
            return 1;
        }
    }
    return 0;
}


void func_ov021_0219fe64(FieldActor *param_1) {
    u32 blActIdx;
    BlActSys *sys;

    if (FldAct_GetObjCode(param_1) == 0x7b) {
        blActIdx = FldAct_GetBlActIdx(param_1, 0);
        if ((blActIdx & 0xffff) != 0xffff) {
            sys = FieldBlAct_GetBlActSys(FldAct_GetFieldBlAct(param_1));
            func_0205085c(sys, blActIdx & 0xffff, 0);
        }
    }
}

void func_ov021_0219fe9c(RailBlActAnim *anim) {
    MI_CpuFill8(anim, 0, sizeof(RailBlActAnim));
    anim->dir = 8;
}

static const u8 data_ov021_021d8548[4] = { 0x00, 0x02, 0x02, 0x00 };
void func_ov021_0219feb0(RailBlActAnim *param_1, FieldActor *param_2, BlActSys *param_3, u32 param_4) {
    BOOL changed = FALSE;
    u16 animId;
    u16 dir = func_ov021_0219ffd4(param_2);
    u16 state = func_ov010_0216d678(param_2);

    animId = dir + state * 4;
    if (param_1->unk_03 != 0 && state == param_1->state) {
        param_1->unk_03 = 0;
    }
    if (dir != param_1->dir) {
        changed = TRUE;
        param_1->state = state;
        param_1->dir = dir;
        param_1->frame = 0;
        func_0205082c(param_3, param_4, animId);
    } else if (state != param_1->state) {
        changed = TRUE;
        if (state == 0) {
            if (param_1->state == 0) {
                func_0205082c(param_3, param_4, animId);
                param_1->state = state;
                param_1->unk_03 = 0;
            } else if (param_1->unk_03 == 0) {
                param_1->unk_03++;
                changed = FALSE;
            } else {
                u8 steps[4] = { 0, 2, 2, 0 };

                param_1->frame = func_02050880(param_3, param_4);
                if (param_1->frame >= 4) {
                    param_1->frame = 0;
                }
                param_1->frame = steps[param_1->frame];
                func_0205082c(param_3, param_4, animId);
                param_1->state = state;
                param_1->unk_03 = 0;
            }
        } else {
            if (param_1->state != 0) {
                param_1->frame = func_02050880(param_3, param_4);
                if (param_1->frame >= 2) {
                    param_1->frame = 0;
                } else {
                    param_1->frame = 2;
                }
            }
            func_0205082c(param_3, param_4, animId);
            func_0205085c(param_3, param_4, param_1->frame);
            param_1->state = state;
            param_1->unk_03 = 0;
        }
    }
    func_ov021_021a000c(param_2, param_3, param_4, changed, 0);
}


void func_ov021_0219ffbc(FieldActor *param_1, VecFx32 *pos) {
    MMSys *mmSys;
    Field *p;

    mmSys = GetActorMModelSystem(param_1);
    p = GetMMSysField(mmSys);
    pos->y = pos->y + func_ov021_02189a78(p);
}

u8 func_ov021_0219ffd4(FieldActor *param_1) {
    u16 angle;
    u16 dir;
    MMSys *system;
    system = GetActorMModelSystem(param_1);
    angle = func_ov010_0216d538(system);
    dir = GetActorFaceDir(param_1);
    if (CheckActorFlag(param_1, 0x2000)) {
        return func_ov021_0219d738(param_1, angle);
    }
    return func_ov010_021642d4(dir, angle);
}

void func_ov021_021a000c(FieldActor *param_1, BlActSys *param_2, u32 param_3, s32 param_4, s32 param_5) {
    BOOL animate = 1;
    u8 kind = func_ov021_0219c648(param_1);
    if (param_4 == 0) {
        if (param_5 == 1) {
            if (kind == 1 && CheckActorFlag(param_1, 0x10)) {
                animate = 0;
            }
        } else if (kind == 1 && func_ov021_02197c9c(param_1) == 1) {
            animate = 0;
        }
    }
    func_020507f0(param_2, param_3, animate);
    animate = 1;
    if (func_ov010_0216dab4(param_1) == 1) {
        animate = 0;
    }
    func_020507a0(param_2, param_3, animate);
}
