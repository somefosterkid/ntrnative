// Black's code at ov021 0x021be2a8-0x021be89c. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "field/field_exp_obj.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_mtx.h"
#include "nnsys/g3d.h"
#include "struct_decls.h"
#include "gfl/arc.h"

void FieldExpObj_AllocAnmSet(FieldExpObjActorAnms *actorAnms, s32 count, u32 heapId);

FieldExpObjSystem *func_ov021_021be2a8(u32 resourceLimit, u32 actorLimit, u32 heapId) {
    FieldExpObjSystem *system = GFL_HeapAllocate(heapId, sizeof(FieldExpObjSystem), TRUE, "fld_exp_obj.c", 0x4c);

    system->g3dManager = GFL_G3DMgrCreate(resourceLimit, actorLimit, heapId);
    system->heapId = heapId;
    return system;
}

void func_ov021_021be2e0(FieldExpObjSystem *system) {
    u16 scene;

    scene = 0;
    do {
        if (system->scenes[scene].active != 0) {
            FieldExpObj_FreeScene(system, scene);
        }
        scene++;
    } while (scene < 4);
    GFL_G3DMgrFree(system->g3dManager);
    GFL_HeapFree(system);
}

void FieldExpObj_AddScene(FieldExpObjSystem *system, const G3DSceneSetup *setup, u8 scene) {
    u16 sceneId;
    u16 actorCount;
    FieldExpObjActorSlot *slots;
    s32 j;
    FieldExpObjActorAnms *a;
    G3DActor *actor;
    u16 i;

    sceneId = GFL_G3DMgrNewSceneFast(system->g3dManager, setup);
    actorCount = setup->actorCount;
    system->scenes[scene].actorCount = actorCount;
    system->scenes[scene].active = 1;
    system->scenes[scene].sceneId = sceneId;
    slots = GFL_HeapAllocate(system->heapId, actorCount * sizeof(FieldExpObjActorSlot), TRUE, "fld_exp_obj.c", 0x8b);
    system->scenes[scene].slots = slots;
    system->scenes[scene].actorAnms = GFL_HeapAllocate(system->heapId, actorCount * sizeof(FieldExpObjActorAnms), TRUE, "fld_exp_obj.c", 0x8e);
    for (i = 0; i < actorCount; i++) {
        FieldExpObj_AllocAnmSet(&system->scenes[scene].actorAnms[i], setup->actors[i].animationCount, system->heapId);
        slots[i].mtx.scale.x = 0x1000;
        slots[i].mtx.scale.y = 0x1000;
        slots[i].mtx.scale.z = 0x1000;
        MTX_Identity33_(&slots[i].mtx.rotation);
        slots[i].mtx.translation.x = 0;
        slots[i].mtx.translation.y = 0;
        slots[i].mtx.translation.z = 0;
        slots[i].unk_00 = 0;
        slots[i].unk_04 = 0;
        actor = FieldExpObj_GetActor(system, scene, i);
        a = &system->scenes[scene].actorAnms[i];
        for (j = 0; j < a->count; j++) {
            a->anms[j].unk_0C = GFL_G3DActorGetAnm(actor, j);
        }
    }
}

void FieldExpObj_FreeScene(FieldExpObjSystem *system, u16 scene) {
    u16 i;

    if (system->scenes[scene].active != 0 && scene < 4) {
        for (i = 0; i < system->scenes[scene].actorCount; i++) {
            GFL_HeapFree(system->scenes[scene].actorAnms[i].anms);
        }
        GFL_HeapFree(system->scenes[scene].actorAnms);
        GFL_HeapFree(system->scenes[scene].slots);
        GFL_G3DMgrDeleteScene(system->g3dManager, system->scenes[scene].sceneId);
        system->scenes[scene].active = 0;
    }
}

SRTMatrix *FieldExpObj_GetActorMatrixPtr(FieldExpObjSystem *system, u16 scene, u16 actor) {
    if (system->scenes[scene].active == 0) {
        return NULL;
    }
    if (scene >= 4) {
        return NULL;
    }
    return &system->scenes[scene].slots[actor].mtx;
}

void func_ov021_021be4a4(FieldExpObjSystem *system, u16 scene, u16 actor, BOOL value) {
    system->scenes[scene].slots[actor].unk_00 = value;
}

void FieldExpObj_SetActorHidden(FieldExpObjSystem *system, u16 scene, u16 actor, BOOL value) {
    system->scenes[scene].slots[actor].unk_04 = value;
}

G3DActor *FieldExpObj_GetActor(FieldExpObjSystem *system, u32 scene, u16 actor) {
    u16 firstIdx;

    if (system->scenes[scene].active == 0) {
        return NULL;
    }
    if (scene >= 4) {
        return NULL;
    }
    firstIdx = GFL_G3DMgrGetSceneFirstActorIdx(system->g3dManager, system->scenes[scene].sceneId);
    return GFL_G3DMgrGetActor(system->g3dManager, firstIdx + actor);
}

void func_ov021_021be4f8(FieldExpObjSystem *system) {
    u8 scene;
    u8 i;
    u16 count;
    u16 firstIdx;
    G3DActor *actor;

    for (scene = 0; scene < 4; scene++) {
        if (system->scenes[scene].active == 1) {
            firstIdx = GFL_G3DMgrGetSceneFirstActorIdx(system->g3dManager, system->scenes[scene].sceneId);
            count = GFL_G3DSceneGetNodeActorCount(system->g3dManager, system->scenes[scene].sceneId);
            for (i = 0; i < count; i++) {
                if (system->scenes[scene].slots[i].unk_04 == 0) {
                    actor = GFL_G3DMgrGetActor(system->g3dManager, firstIdx + i);
                    if (system->scenes[scene].slots[i].unk_00 != 0) {
                        GFL_G3DSysDrawObjBBoxCull(actor, &system->scenes[scene].slots[i].mtx);
                    } else {
                        GFL_G3DSysDrawObj(actor, &system->scenes[scene].slots[i].mtx);
                    }
                }
            }
        }
    }
}

void FieldExpObj_StepAllAnimations(FieldExpObjSystem *system) {
    u8 scene;
    u8 actorIdx;
    u8 anmIdx;
    u16 nodeCount;
    s32 anmCount;
    G3DActor *actor;
    FieldExpObjAnm *anm;
    BOOL ret;

    for (scene = 0; scene < 4; scene++) {
        if (system->scenes[scene].active == 1) {
            nodeCount = GFL_G3DSceneGetNodeActorCount(system->g3dManager, system->scenes[scene].sceneId);
            for (actorIdx = 0; actorIdx < nodeCount; actorIdx++) {
                actor = FieldExpObj_GetActor(system, scene, actorIdx);
                anmCount = system->scenes[scene].actorAnms[actorIdx].count;
                for (anmIdx = 0; anmIdx < anmCount; anmIdx++) {
                    anm = &system->scenes[scene].actorAnms[actorIdx].anms[anmIdx];
                    if (anm->unk_02 != 0 && anm->unk_00 == 0) {
                        if (anm->unk_01 != 0) {
                            ret = GFL_G3DActorStepAnmFrameLoop(actor, anmIdx, anm->unk_04);
                        } else {
                            ret = GFL_G3DActorStepAnmFrame(actor, anmIdx, anm->unk_04);
                        }
                        anm->unk_08 = ret;
                    }
                }
            }
        }
    }
}

void FieldExpObj_AllocAnmSet(FieldExpObjActorAnms *actorAnms, s32 count, u32 heapId) {
    FieldExpObjAnm *anms;
    s32 i;

    anms = GFL_HeapAllocate(heapId, count * sizeof(FieldExpObjAnm), TRUE, "fld_exp_obj.c", 0x19d);
    actorAnms->anms = anms;
    actorAnms->count = count;
    for (i = 0; i < count; i++) {
        anms[i].unk_00 = 0;
        anms[i].unk_01 = 1;
        anms[i].unk_02 = 0;
        anms[i].unk_04 = 0x1000;
    }
}

FieldExpObjAnm *FieldExpObj_GetAnmInfo(FieldExpObjSystem *system, u32 scene, u16 actor, u16 anm) {
    FieldExpObjAnm *anms;

    if (system->scenes[scene].active == 0) {
        return NULL;
    }
    if (actor >= system->scenes[scene].actorCount) {
        return NULL;
    }
    anms = system->scenes[scene].actorAnms[actor].anms;
    if (anm >= system->scenes[scene].actorAnms[actor].count) {
        return NULL;
    }
    return &anms[anm];
}

void FieldExpObjAnm_SetPaused(FieldExpObjAnm *anm, u8 paused) {
    anm->unk_00 = paused;
}

u8 func_ov021_021be6b4(u8 *param_1) {
    return *param_1;
}

void FieldExpObjAnm_SetLooped(FieldExpObjAnm *anm, u8 looped) {
    anm->unk_01 = looped;
}

BOOL FieldExpObjAnm_IsPlaybackFinished(FieldExpObjAnm *anm) {
    if (anm->unk_08 == 0) {
        return 1;
    }
    return 0;
}

void FieldExpObj_SetAnmFrame(FieldExpObjSystem *system, u16 scene, u16 actor, u16 anm, fx32 frame) {
    G3DActor *actor_00;
    fx32 fStack_10;
    u32 uStack_c;

    uStack_c = (u32)anm;
    fStack_10 = frame;
    actor_00 = FieldExpObj_GetActor(system, scene, actor);
    GFL_G3DActorSetAnmFrame(actor_00, anm, &fStack_10);
}

fx32 func_ov021_021be6e8(FieldExpObjSystem *system, u16 scene, u16 actor, u16 anm) {
    fx32 frame;

    GFL_G3DActorGetAnmFrame(FieldExpObj_GetActor(system, scene, actor), anm, &frame);
    return frame;
}

void FieldExpObj_SetAnm(FieldExpObjSystem *system, u16 scene, u16 actor, u16 anm, BOOL value) {
    G3DActor *obj = FieldExpObj_GetActor(system, scene, actor);
    FieldExpObjAnm *info = FieldExpObj_GetAnmInfo(system, scene, actor, anm);

    if (info != NULL) {
        if (!(value == 0)) {
            if (info->unk_02 != 0) {
                return;
            }
            GFL_G3DActorBindAnm(obj, anm);
        } else {
            if (info->unk_02 == 0) {
                return;
            }
            GFL_G3DActorUnbindAnm(obj, anm);
        }
        info->unk_02 = value;
    }
}



fx32 func_ov021_021be748(FieldExpObjAnm *anm) {
    NNSG3dAnmObj *pNVar1;

    pNVar1 = GFL_G3DAnmGetRenderObj(anm->unk_0C);
    return (u32)((const NNSG3dResAnmCommon *)pNVar1->resAnm)->numFrame * 0x1000 + -0x1000;
}

void LoadFieldExpandObjData(FieldExpObjSystem *system, const G3DSceneSetup *setup, u16 scene) {
    u16 actorCount;
    ArcTool *arc;
    u16 sceneId;
    FieldExpObjActorSlot *slots;
    s32 j;
    FieldExpObjActorAnms *a;
    G3DActor *actor;
    u16 i;

    arc = GFL_ArcSysCreateFileHandle(setup->resources->arcId, (u16)system->heapId & 0x7fff | 0x8000);
    sceneId = GFL_G3DMgrNewSceneFastEx(system->g3dManager, setup, arc);
    GFL_ArcToolFree(arc);
    actorCount = setup->actorCount;
    system->scenes[scene].actorCount = actorCount;
    system->scenes[scene].active = 1;
    system->scenes[scene].sceneId = sceneId;
    slots = GFL_HeapAllocate(system->heapId, actorCount * sizeof(FieldExpObjActorSlot), TRUE, "fld_exp_obj.c", 0x23e);
    system->scenes[scene].slots = slots;
    system->scenes[scene].actorAnms = GFL_HeapAllocate(system->heapId, actorCount * sizeof(FieldExpObjActorAnms), TRUE, "fld_exp_obj.c", 0x241);
    for (i = 0; i < actorCount; i++) {
        FieldExpObj_AllocAnmSet(&system->scenes[scene].actorAnms[i], setup->actors[i].animationCount, system->heapId);
        slots[i].mtx.scale.x = 0x1000;
        slots[i].mtx.scale.y = 0x1000;
        slots[i].mtx.scale.z = 0x1000;
        MTX_Identity33_(&slots[i].mtx.rotation);
        slots[i].mtx.translation.x = 0;
        slots[i].mtx.translation.y = 0;
        slots[i].mtx.translation.z = 0;
        slots[i].unk_00 = 0;
        slots[i].unk_04 = 0;
        actor = FieldExpObj_GetActor(system, scene, i);
        a = &system->scenes[scene].actorAnms[i];
        for (j = 0; j < a->count; j++) {
            a->anms[j].unk_0C = GFL_G3DActorGetAnm(actor, j);
        }
    }
}
