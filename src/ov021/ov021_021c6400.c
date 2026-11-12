// Black code at ov021 0x021c6400-0x021c6b18. No name is known: the module and address.

#include "types.h"
#include "field/field_g3dobj.h"
#include "gfl/g3d.h"
#include "nitro/mi.h"
#include "struct_decls.h"
#include "system/palanm.h"
#include "nitro/fx/fx_mtx.h"

void FieldG3DObjSystem_Free(FieldG3DObjSystem *sys) {
    u32 i;
    FieldG3DObj *obj;
    FieldG3DObjResGroup *group;
    sys->unk_02 = 0;
    obj = sys->objs;
    for (i = 0; i < sys->objCount; i++, obj++) {
        if (obj->state == 1 && obj->actor != NULL) {
            FieldG3DObj_Free(obj, &sys->resGroups[obj->resGroupIdx]);
        }
    }
    GFL_HeapFree(sys->objs);
    for (i = 0, group = sys->resGroups; i < sys->resGroupCount; i++, group++) {
        FieldG3DObjResGroup_Free(group);
    }
    GFL_HeapFree(sys->resGroups);
    GFL_HeapFree(sys);
}

void FieldG3DObjSystem_SetupResources(FieldG3DObjSystem *sys) {
    FieldG3DObj *obj;
    FieldG3DObjResGroup *group;
    FieldG3DObjResGroup *groups;
    s32 i;
    s32 j;
    if (sys->unk_02 == 1) {
        i = 0;
        obj = sys->objs;
        group = sys->resGroups;
        for (; i < sys->resGroupCount; i++) {
            if (group->texState == 2) {
                FieldG3DObjResGroup_SetupTextures(group);
            }
            group++;
        }
        j = 0;
        groups = sys->resGroups;
        for (; j < sys->objCount; j++) {
            if (obj->state == 2) {
                FieldG3DObj_Setup(obj, &groups[obj->resGroupIdx], obj->resGroupIdx, obj->modelIdx, sys->unk_00, NULL, FALSE);
            }
            obj++;
        }
    }
}

void func_ov021_021c64e0(FieldG3DObjSystem *sys) {
    u32 i;
    FieldG3DObj *obj = sys->objs;
    for (i = 0; i < sys->objCount; i++) {
        if (obj->state == 1) {
            if (obj->manualDraw == 0 && obj->hidden == 0) {
                if (obj->bboxCull == 1) {
                    GFL_G3DSysDrawObjBBoxCull(obj->actor, &obj->transform);
                } else {
                    GFL_G3DSysDrawObj(obj->actor, &obj->transform);
                }
            }
            obj->unk_5 = 0;
        }
        obj++;
    }
}

void func_ov021_021c6534(void *sys, void *luminanceTable) {
    ((FieldG3DObjSystem *)sys)->luminanceTable = luminanceTable;
}

u16 FieldG3DObjSystem_AddResGroupCore(FieldG3DObjSystem *sys, FieldG3DObjResRequest *req, BOOL preloadTextures, u8 *luminanceTable) {
    u16 i;
    FieldG3DObjResGroup *group = sys->resGroups;
    for (i = 0; i < sys->resGroupCount; i++) {
        if (group->mdlRes == NULL) {
            FieldG3DObjResGroup_LoadResources(group, sys->unk_00, req, preloadTextures, luminanceTable);
            return i;
        }
        group++;
    }
    return 0;
}

u16 FieldG3DObjSystem_AddResGroup(FieldG3DObjSystem *sys, FieldG3DObjResRequest *req, BOOL preloadTextures) {
    return FieldG3DObjSystem_AddResGroupCore(sys, req, preloadTextures, sys->luminanceTable);
}

void FieldG3DObjSystem_FreeResGroup(FieldG3DObjSystem *sys, u16 resGroupIdx) {
    FieldG3DObjResGroup_Free(&sys->resGroups[resGroupIdx]);
}

u16 FieldG3DObjSystem_AddObj(FieldG3DObjSystem *sys, u16 resGroupIdx, u16 modelIdxInRes, VecFx32 *pos) {
    FieldG3DObj *obj = sys->objs;
    u16 i;
    for (i = 0; i < sys->objCount; i++) {
        if (obj->state == 0 && obj->actor == NULL) {
            FieldG3DObjResGroup *group = &sys->resGroups[resGroupIdx];
            if (group->texState == 2) {
                obj->resGroupIdx = resGroupIdx;
                obj->modelIdx = modelIdxInRes;
                obj->state = 2;
                obj->bboxCull = TRUE;
                obj->transform.scale.x = FX32_ONE;
                obj->transform.scale.y = FX32_ONE;
                obj->transform.scale.z = FX32_ONE;
                if (pos != NULL) {
                    obj->transform.translation = *pos;
                }
                MTX_Identity33_(&obj->transform.rotation);
            } else {
                FieldG3DObj_Setup(obj, group, resGroupIdx, modelIdxInRes, sys->unk_00, pos, TRUE);
            }
            return i;
        }
        obj++;
    }
    return 0;
}

void FieldG3DObjSystem_FreeObj(FieldG3DObjSystem *sys, u16 objIdx) {
    FieldG3DObj *obj = &sys->objs[objIdx];
    sys->unk_02 = 0;
    if (obj->state == 1) {
        FieldG3DObj_Free(obj, &sys->resGroups[obj->resGroupIdx]);
    }
    sys->unk_02 = 1;
}

SRTMatrix *FieldG3DObjSystem_GetObjTransform(FieldG3DObjSystem *sys, u16 objIdx) {
    return &sys->objs[objIdx].transform;
}

void FieldG3DObjSystem_SetObjPosition(FieldG3DObjSystem *sys, u16 objIdx, VecFx32 *src) {
    sys->objs[objIdx].transform.translation = *src;
}

void func_ov021_021c6678(FieldG3DObjSystem *sys, u16 objIdx, BOOL hidden) {
    sys->objs[objIdx].hidden = hidden;
}

BOOL FieldG3DObjSystem_StepObjAnm(FieldG3DObjSystem *sys, u16 objIdx, fx32 addend) {
    FieldG3DObj *obj;
    BOOL result = FALSE;
    s32 i;
    obj = &sys->objs[objIdx];
    if (obj->state == 1) {
        s32 count = sys->resGroups[obj->resGroupIdx].anmCount;
        for (i = 0; i < count; i++) {
            if (GFL_G3DActorStepAnmFrame(obj->actor, i, addend)) {
                result = TRUE;
            }
        }
    } else if (obj->state == 2) {
        result = TRUE;
    }
    return result;
}

BOOL FieldG3DObjSystem_StepObjAnmLoop(FieldG3DObjSystem *sys, u16 objIdx, fx32 addend) {
    FieldG3DObj *obj;
    BOOL result = FALSE;
    s32 i;
    obj = &sys->objs[objIdx];
    if (obj->state == 1) {
        s32 count = sys->resGroups[obj->resGroupIdx].anmCount;
        for (i = 0; i < count; i++) {
            if (GFL_G3DActorStepAnmFrameLoop(obj->actor, i, addend)) {
                result = TRUE;
            }
        }
    } else if (obj->state == 2) {
        result = TRUE;
    }
    return result;
}

void FieldG3DObjSystem_SetAnmFrame(FieldG3DObjSystem *sys, u16 objIdx, s8 anmIdx, fx32 frame) {
    FieldG3DObj *obj = &sys->objs[objIdx];
    if (obj->state == 1) {
        s32 count = sys->resGroups[obj->resGroupIdx].anmCount;
        if (anmIdx == 8) {
            s32 i;
            for (i = 0; i < count; i++) {
                GFL_G3DActorSetAnmFrame(obj->actor, i, &frame);
            }
        } else if (anmIdx < count) {
            GFL_G3DActorSetAnmFrame(obj->actor, anmIdx, &frame);
        }
    }
}

fx32 FieldG3DObjSystem_GetAnmFrame(FieldG3DObjSystem *sys, u16 objIdx, u32 anmIdx) {
    fx32 frame = 0;
    FieldG3DObj *obj = &sys->objs[objIdx];
    if (obj->state == 1) {
        if (!GFL_G3DActorGetAnmFrame(obj->actor, anmIdx, &frame)) {
            frame = 0;
        }
    }
    return frame;
}

void func_ov021_021c67d8(FieldG3DObjSystem *sys, u16 objIdx, BOOL flag) {
    sys->objs[objIdx].manualDraw = flag;
}

void FieldG3DObjSystem_DrawObj(FieldG3DObjSystem *sys, u16 objIdx) {
    FieldG3DObj *obj = &sys->objs[objIdx];
    if (obj->state == 1 && obj->hidden == 0) {
        if (obj->bboxCull == 1) {
            GFL_G3DSysDrawObjBBoxCull(obj->actor, &obj->transform);
        } else {
            GFL_G3DSysDrawObj(obj->actor, &obj->transform);
        }
    }
}

void func_ov021_021c6828(FieldG3DObjSystem *sys, u16 objIdx, u32 flag) {
    sys->objs[objIdx].unk_5 = flag;
}

u32 func_ov021_021c6844(FieldG3DObjSystem *sys, u16 objIdx) {
    return sys->objs[objIdx].unk_5;
}

void FieldG3DObjResGroup_SetupTextures(FieldG3DObjResGroup *group) {
    group->texState = 0;
    if (group->texRes != NULL) {
        GFL_G3DResUploadTexData(group->texRes);
    } else {
        GFL_G3DResUploadTexData(group->mdlRes);
    }
    group->texState = 1;
}

void FieldG3DObjResGroup_LoadResources(FieldG3DObjResGroup *group, u32 unk, FieldG3DObjResRequest *req, BOOL preloadTextures, u8 *luminanceTable) {
    void *tex;
    s32 i;
    if (req->mdlArc != NULL) {
        group->mdlRes = GFL_G3DSysReadArcToolResource(req->mdlArc, req->mdlDatID);
    }
    if (req->texArc != NULL) {
        group->texRes = GFL_G3DSysReadArcToolResource(req->texArc, req->texDatID);
    }
    if (luminanceTable != NULL) {
        tex = NULL;
        if (group->mdlRes != NULL) {
            tex = group->mdlRes;
        }
        if (group->texRes != NULL) {
            tex = group->texRes;
        }
        if (GFL_G3DResCheckType(tex, 2)) {
            NNSG3dResTex *nnsTex = NNS_G3dGetTex(GFL_G3DResGetResData(tex));
            ColorFilter_ApplyLUT((u16 *)((u8 *)nnsTex + nnsTex->plttInfo.ofsPlttData),
                                 (nnsTex->plttInfo.sizePltt << 3) / sizeof(u16), luminanceTable);
        }
    }
    if (group->mdlRes != NULL || group->texRes != NULL) {
        if (preloadTextures == 1) {
            FieldG3DObjResGroup_SetupTextures(group);
        } else {
            group->texState = 2;
        }
    }
    if (req->anmCount != 0) {
        group->anmCount = req->anmCount;
        MIi_CpuClear32(0, group->anmRes, sizeof(group->anmRes));
        for (i = 0; i < group->anmCount; i++) {
            group->anmRes[i] = GFL_G3DSysReadArcToolResource(req->anmArc, req->anmDatIDs[i]);
        }
    }
}

void FieldG3DObjResGroup_Free(FieldG3DObjResGroup *group) {
    u8 state = group->texState;
    s32 i;
    group->texState = 0;
    if (state == 1) {
        if (group->texRes != NULL) {
            GFL_G3DResFreeTexData(group->texRes);
        } else if (group->mdlRes != NULL) {
            GFL_G3DResFreeTexData(group->mdlRes);
        }
    }
    if (group->texRes != NULL) {
        GFL_G3DResFree(group->texRes);
    }
    if (group->mdlRes != NULL) {
        GFL_G3DResFree(group->mdlRes);
    }
    if (group->anmCount != 0) {
        for (i = 0; i < group->anmCount; i++) {
            GFL_G3DResFree(group->anmRes[i]);
        }
    }
    MI_CpuFill(group, 0, sizeof(FieldG3DObjResGroup));
    group->texState = 1;
}

void FieldG3DObj_Setup(FieldG3DObj *obj, FieldG3DObjResGroup *group, u16 resGroupIdx, u16 modelIdx, u16 unk, VecFx32 *pos, BOOL initTransform) {
    G3DModel *model;
    void *tex = NULL;
    s32 i;
    obj->resGroupIdx = resGroupIdx;
    obj->modelIdx = modelIdx;
    if (group->texState == 1) {
        tex = group->mdlRes;
        if (group->texRes != NULL) {
            tex = group->texRes;
        }
    }
    model = GFL_G3DMdlCreate(group->mdlRes, modelIdx, tex);
    if (group->anmCount != 0) {
        MIi_CpuClear32(0, obj->anms, sizeof(obj->anms));
        for (i = 0; i < group->anmCount; i++) {
            obj->anms[i] = GFL_G3DAnmCreate(model, group->anmRes[i], 0);
        }
    }
    obj->actor = GFL_G3DActorCreate(model, obj->anms, group->anmCount);
    if (group->anmCount != 0) {
        for (i = 0; i < group->anmCount; i++) {
            GFL_G3DActorBindAnm(obj->actor, i);
        }
    }
    if (pos != NULL) {
        obj->transform.translation = *pos;
    }
    if (initTransform) {
        obj->bboxCull = TRUE;
        obj->transform.scale.x = FX32_ONE;
        obj->transform.scale.y = FX32_ONE;
        obj->transform.scale.z = FX32_ONE;
        MTX_Identity33_(&obj->transform.rotation);
    }
    obj->state = 1;
}

void FieldG3DObj_Free(FieldG3DObj *obj, FieldG3DObjResGroup *group) {
    s32 i;
    if (obj->anms != NULL) {
        for (i = 0; i < group->anmCount; i++) {
            GFL_G3DAnmFree(obj->anms[i]);
        }
    }
    if (obj->actor != NULL) {
        G3DModel *model = GFL_G3DActorGetMdl(obj->actor);
        GFL_G3DActorFree(obj->actor);
        GFL_G3DMdlFree(model);
    }
    MI_CpuFill(obj, 0, sizeof(FieldG3DObj));
}

void FieldG3DObjResRequest_Clear(FieldG3DObjResRequest *req) {
    MIi_CpuClear32(0, req, sizeof(FieldG3DObjResRequest));
}

void FieldG3DObjResRequest_SetModel(FieldG3DObjResRequest *req, ArcTool *arc, u16 datId) {
    req->mdlArc = arc;
    req->mdlDatID = datId;
}

void FieldG3DObjResRequest_SetAnmArc(FieldG3DObjResRequest *req, ArcTool *arc) {
    req->anmArc = arc;
}

void FieldG3DObjResRequest_AddAnm(FieldG3DObjResRequest *req, u16 datId) {
    req->anmDatIDs[req->anmCount] = datId;
    req->anmCount = req->anmCount + 1;
}
