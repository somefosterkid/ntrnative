// Black's code at ov021 0x021c12c4-0x021c1a08. No name is known: the module and address.

#include "types.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_effect.h"
#include "field/field_move_tcb.h"
#include "field/fieldmap_tcb_camera_zoom.h"
#include "field/field_player.h"
#include "gfl/heap.h"
#include "gfl/sound.h"
#include "gfl/tcb.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "struct_decls.h"
#include "field/field_player_grid.h"

typedef struct FieldWaterfallWork {
    u16 unk_00;
    s16 state;
    u16 direction;
    u16 done;
    u16 frame;
    u8 unk_0a[6];
    s32 unk_10;
    u8 unk_14[4];
    VecFx32 offset;
    VecFx32 target;
    VecFx32 step;
    FieldPlayer *player;
    Field *field;
    u8 unk_44[4];
    FieldEffectTask *effect;
} FieldWaterfallWork;

void FieldEffects_FreeEffect(s32 *param_1, s32 param_2);
void func_ov021_021c16ac(FieldWaterfallWork *work);

BOOL FieldWaterfallTCB_CheckEnd(void *tcb) {
    FieldWaterfallWork *work = GFL_TCBGetData(tcb);

    return work->done;
}

void FieldWaterfallTCB_Free(void *tcb) {
    void *ptr;

    ptr = GFL_TCBGetData(tcb);
    GFL_HeapFree(ptr);
    GFL_TCBRemove(tcb);
}

void func_ov021_021c12e4(FieldActor *actor, const VecFx32 *pos) {
    VecFx32 vec = *pos;
    vec.y += 0x1000;
    vec.z += 0x5000;
    func_ov010_0216d8e0(actor, &vec);
}

u32 func_ov021_021c1318(FieldWaterfallWork *work) {
    FieldActor *actor;
    FieldEffectTask *pFVar1;
    FieldEffects *effects;
    u32 kind;
    u32 uStack_4c;
    VecFx32 pos;
    VecFx32 cell;
    VecFx32 pos2;

    actor = FieldPlayer_GetActor(work->player);
    func_ov010_0216db4c(actor, 1);
    CopyActorWPos(actor, &pos);
    pos2 = (cell = pos);
    ExpandVecInGridDir(work->direction, &cell, 0x10000);
    ExpandVecInGridDir(work->direction, &pos2, 0x20000);
    ConvGXZToVector((cell.x >> 4) / 4096, (cell.z >> 4) / 4096, &cell);
    ConvGXZToVector((pos2.x >> 4) / 4096, (pos2.z >> 4) / 4096, &work->target);
    *&work->target = pos2;
    GetHeightFromMap(actor, &pos2, &work->target.y);
    work->step.x = (cell.x - pos.x) / 2 / 16;
    work->step.z = (cell.z - pos.z) / 2 / 16;
    work->step.y = 0x6000;
    work->offset.y = 0x1000;
    pFVar1 = func_ov021_021a2f98(work->player);
    func_ov021_021acbb8(pFVar1, 2);
    kind = 1;
    if ((u16)(work->direction + 0xfffe) <= 1) {
        kind = 3;
    }
    effects = Field_GetFieldEffects(work->field);
    uStack_4c = 9;
    FieldEffects_Load(effects, &uStack_4c, 1);
    pFVar1 = func_ov021_021ad564(effects, kind, pFVar1);
    work->effect = pFVar1;
    FieldCameraZoomTCB_Create(work->field, 0x10, 0x20000);
    GFL_SndSEPlay(0x6e0);
    work->state = work->state + 1;
    return 1;
}

u32 func_ov021_021c146c(FieldWaterfallWork *work) {
    FieldActor *actor;
    FieldEffectTask *task;
    FieldEffects *effects;
    u32 kind;
    VecFx32 pos;

    actor = FieldPlayer_GetActor(work->player);
    CopyActorWPos(actor, &pos);
    pos.x += work->step.x;
    pos.z += work->step.z;
    SetActorWPosValue(actor, &pos);
    work->offset.y = -work->offset.y;
    work->frame = work->frame + 1;
    if (work->frame >= 0x10) {
        work->frame = 0;
        work->offset.y = 0;
        work->state = work->state + 1;
        func_ov021_021ac154(work->effect);
        task = func_ov021_021a2f98(work->player);
        kind = 2;
        if ((u16)(work->direction + 0xfffe) <= 1) {
            kind = 2;
        }
        effects = Field_GetFieldEffects(work->field);
        task = func_ov021_021ad564(effects, kind, task);
        work->effect = task;
    }
    func_ov021_021c12e4(actor, &work->offset);
    return 0;
}

u32 func_ov021_021c14f8(FieldWaterfallWork *work) {
    FieldActor *actor;
    VecFx32 pos;
    s32 speed;

    actor = FieldPlayer_GetActor(work->player);
    CopyActorWPos(actor, &pos);
    pos.y += work->step.y;
    if (pos.y >= work->target.y) {
        pos.y = work->target.y;
        work->state = work->state + 1;
        work->step.x = (work->target.x - pos.x) / 16;
        speed = -0x8000;
        work->step.z = (work->target.z - pos.z) / 16;
        work->unk_10 = speed;
        work->step.y = 0x6000;
        work->unk_10 = work->unk_10 / 11;
        FieldCameraZoomTCB_Create(work->field, 0x10, speed << 2);
        func_ov021_021ac154(work->effect);
    }
    SetActorWPosValue(actor, &pos);
    return 0;
}

u32 func_ov021_021c157c(FieldWaterfallWork *work) {
    FieldActor *actor;
    VecFx32 pos;

    actor = FieldPlayer_GetActor(work->player);
    CopyActorWPos(actor, &pos);
    if (pos.x < work->target.x) {
        pos.x += work->step.x;
        if (pos.x > work->target.x) {
            pos.x = work->target.x;
        }
    } else {
        pos.x += work->step.x;
        if (pos.x < work->target.x) {
            pos.x = work->target.x;
        }
    }
    if (pos.z < work->target.z) {
        pos.z += work->step.z;
        if (pos.z > work->target.z) {
            pos.z = work->target.z;
        }
    } else {
        pos.z += work->step.z;
        if (pos.z < work->target.z) {
            pos.z = work->target.z;
        }
    }
    pos.y += work->step.y;
    work->step.y += work->unk_10;
    work->frame = work->frame + 1;
    if (work->frame >= 0x10) {
        pos = *&work->target;
        work->offset.x = 0;
        work->offset.y = 0;
        work->offset.z = 0;
        work->state = work->state + 1;
    }
    SetActorWPosValue(actor, &pos);
    func_ov021_021c12e4(actor, &work->offset);
    return 0;
}






u32 func_ov021_021c161c(FieldWaterfallWork *work) {
    FieldActor *actor;
    FieldEffectTask *pFVar1;
    FieldEffects *effects;

    actor = FieldPlayer_GetActor(work->player);
    func_ov010_0216db4c(actor, 0);
    SetActorWPosAll(actor, &work->target, work->direction);
    pFVar1 = func_ov021_021a2f98(work->player);
    func_ov021_021acbb8(pFVar1, 1);
    effects = Field_GetFieldEffects(work->field);
    pFVar1 = func_ov021_021ad564(effects, 0, pFVar1);
    work->effect = pFVar1;
    GFL_SndSEPlay(0x6e2);
    work->state = work->state + 1;
    return 0;
}

u32 func_ov021_021c1670(FieldWaterfallWork *work) {
    BOOL BVar1;
    s32 *fieldEffects;

    if ((work->effect != (FieldEffectTask *)0x0) &&
          (BVar1 = func_ov021_021ad5cc(work->effect), BVar1 == 1)) {
        fieldEffects = Field_GetFieldEffects(work->field);
        func_ov021_021ac154(work->effect);
        FieldEffects_FreeEffect(fieldEffects, 9);
        func_ov021_021a35a0(work->player, 1);
        work->effect = 0;
        work->done = 1;
    }
    return 0;
}

void func_ov021_021c16ac(FieldWaterfallWork *work) {
    FieldActor *actor;
    FieldEffectTask *task;
    s32 *fieldEffects;
    u32 kind;
    VecFx32 pos;
    VecFx32 startPos;
    VecFx32 posB;
    VecFx32 posC;
    u32 effectId;
    FieldEffects *effects;

    actor = FieldPlayer_GetActor(work->player);
    switch (work->state) {
    case 0:
        func_ov010_0216db4c(actor, 1);
        CopyActorWPos(actor, &startPos);
        posB = startPos;
        posC = startPos;
        ExpandVecInGridDir(work->direction, &posB, FX32_CONST(16));
        ExpandVecInGridDir(work->direction, &posC, FX32_CONST(32));
        ConvGXZToVector((posB.x >> 4) / FX32_ONE, (posB.z >> 4) / FX32_ONE, &posB);
        ExpandVecInGridDir(work->direction, &posB, FX32_CONST(4));
        ConvGXZToVector((posC.x >> 4) / FX32_ONE, (posC.z >> 4) / FX32_ONE, &posC);
        work->target = posC;
        GetHeightFromMap(actor, &posC, &work->target.y);
        work->step.x = (posB.x - startPos.x) / 16;
        work->step.z = (posB.z - startPos.z) / 16;
        work->step.y = -0x6000;
        work->offset.y = FX32_ONE;
        task = func_ov021_021a2f98(work->player);
        func_ov021_021acbb8(task, 2);
        kind = 1;
        if ((u16)(work->direction + 0xfffe) <= 1) {
            kind = 3;
        }
        effects = Field_GetFieldEffects(work->field);
        effectId = 9;
        FieldEffects_Load(effects, &effectId, 1);
        task = func_ov021_021ad564(effects, kind, task);
        work->effect = task;
        FieldCameraZoomTCB_Create(work->field, 16, FX32_CONST(32));
        GFL_SndSEPlay(0x6e1);
        work->state = work->state + 1;
        /* fall through */
    case 1:
        CopyActorWPos(actor, &pos);
        pos.x += work->step.x;
        pos.z += work->step.z;
        SetActorWPosValue(actor, &pos);
        work->offset.y = -work->offset.y;
        work->frame = work->frame + 1;
        if (work->frame >= 16) {
            work->frame = 0;
            work->offset.y = 0;
            work->state = work->state + 1;
            func_ov021_021ac154(work->effect);
            task = func_ov021_021a2f98(work->player);
            kind = 2;
            if ((u16)(work->direction + 0xfffe) <= 1) {
                kind = 2;
            }
            effects = Field_GetFieldEffects(work->field);
            task = func_ov021_021ad564(effects, kind, task);
            work->effect = task;
        }
        func_ov021_021c12e4(actor, &work->offset);
        break;
    case 2:
        CopyActorWPos(actor, &pos);
        pos.y += work->step.y;
        if (pos.y <= work->target.y) {
            pos.y = work->target.y;
            work->state = work->state + 1;
            work->step.x = (work->target.x - pos.x) / 16;
            work->step.z = (work->target.z - pos.z) / 16;
            FieldCameraZoomTCB_Create(work->field, 16, -FX32_CONST(32));
            func_ov021_021ac154(work->effect);
            task = func_ov021_021a2f98(work->player);
            func_ov021_021acbb8(task, 1);
            effects = Field_GetFieldEffects(work->field);
            task = func_ov021_021ad564(effects, 0, task);
            work->effect = task;
        }
        SetActorWPosValue(actor, &pos);
        break;
    case 3:
        CopyActorWPos(actor, &pos);
        pos.x += work->step.x;
        pos.z += work->step.z;
        SetActorWPosValue(actor, &pos);
        work->frame = work->frame + 1;
        if (work->frame >= 16) {
            fieldEffects = Field_GetFieldEffects(work->field);
            func_ov010_0216db4c(actor, 0);
            SetActorWPosAll(actor, &work->target, work->direction);
            func_ov021_021ac154(work->effect);
            FieldEffects_FreeEffect(fieldEffects, 9);
            func_ov021_021a35a0(work->player, 1);
            work->effect = NULL;
            work->done = 1;
            GFL_SndSEPlay(0x6e2);
            work->state = work->state + 1;
        }
        break;
    }
}


u32 (*const data_ov021_021da128[])(FieldWaterfallWork *) = {
    func_ov021_021c1318,
    func_ov021_021c146c,
    func_ov021_021c14f8,
    func_ov021_021c157c,
    func_ov021_021c161c,
    func_ov021_021c1670,
};


void FieldWaterfallTCB_Callback(u32 param_1, FieldWaterfallWork *work) {
    if (work->unk_00 != 0) {
        do {
        } while ((data_ov021_021da128[work->state](work)) == 1);
    } else {
        func_ov021_021c16ac(work);
    }
}

FieldActor *FieldPlayerGrid_FindFrontActor(FieldPlayerGrid *grid, u32 direction) {
    FieldActor *actor;
    MMSys *system;
    s32 x;
    s32 z;
    fx32 y;

    actor = FieldPlayerGrid_GetPlayerActor(grid);
    system = GetActorMModelSystem(actor);
    x = GetGPosX(actor);
    x += GetDirectionVectorCompX(direction);
    z = GetGPosZ(actor);
    z += GetDirectionVectorCompZ(direction);
    y = GetActorPosY(actor);
    return FindActorByGPos(system, x, z, y, 1, 0);
}


