#include "global.h"

#include "constants/heap.h"

#include "filesystem_files_def.h"
#include "gf_3d_render.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "overlay_106.h"
#include "overlay_106_legendary_cinematic.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov106_021E5AA8(LegendaryCinematic *cinematic, NarcId narcId, const LegendaryCinematicModelDesc *descs, u32 count) {
    const LegendaryCinematicModelDesc *desc;
    NNSG3dResFileHeader *pNVar2;
    u16 uStack_20;
    void *pvVar3;
    NNSG3dAnmObj *pAnmObj;
    u32 fileId;
    LegendaryCinematicModel *model;
    u32 uVar4;
    NNSG3dResTex *pResTex;
    HeapExp_FndInitAllocator(&cinematic->allocator, HEAP_ID_153, 4);
    uStack_20 = 0;
    if (uStack_20 < count) {
        do {
            model = &cinematic->models[uStack_20];
            desc = &descs[uStack_20];
            pNVar2 = GfGfxLoader_LoadFromNarc(narcId, desc->modelFileId, 1, HEAP_ID_153, 1);
            model->fileHeader = pNVar2;
            GF3dRender_InitObjFromHeader(&model->renderObj, &model->resMdl, &model->fileHeader);
            pResTex = NNS_G3dGetTex(model->fileHeader);
            if (desc->resetMatColors == 1) {
                NNSi_G3dModifyMatFlag(model->resMdl, 0, NNS_G3D_MATFLAG_DIFFUSE);
                NNSi_G3dModifyMatFlag(model->resMdl, 0, NNS_G3D_MATFLAG_AMBIENT);
                NNSi_G3dModifyMatFlag(model->resMdl, 0, NNS_G3D_MATFLAG_SPECULAR);
                NNSi_G3dModifyMatFlag(model->resMdl, 0, NNS_G3D_MATFLAG_EMISSION);
            }
            uVar4 = 0;
            do {
                fileId = desc->anmFileIds[uVar4];
                if (fileId == 0xff) {
                    model->anmData[uVar4] = NULL;
                    model->anmObj[uVar4] = NULL;
                } else {
                    pvVar3 = GfGfxLoader_LoadFromNarc(narcId, fileId, 1, HEAP_ID_153, 1);
                    model->anmData[uVar4] = pvVar3;
                    pvVar3 = NNS_G3dGetAnmByIdx(pvVar3, 0);
                    pAnmObj = NNS_G3dAllocAnmObj(&cinematic->allocator, pvVar3, model->resMdl);
                    model->anmObj[uVar4] = pAnmObj;
                    NNS_G3dAnmObjInit(pAnmObj, pvVar3, model->resMdl, pResTex);
                    NNS_G3dRenderObjAddAnmObj(&model->renderObj, model->anmObj[uVar4]);
                }
                uVar4 = (uVar4 + 1) & 0xffff;
            } while (uVar4 < 4);
            uStack_20 = (uStack_20 + 1) & 0xffff;
        } while (uStack_20 < count);
    }
    cinematic->numModels = count;
    return;
}
#else
void ov106_021E5AA8(LegendaryCinematic *param_1, NarcId param_2, const LegendaryCinematicModelDesc *param_3, u32 param_4) {
    u8 *iVar1;
    NNSG3dResMdl *pNVar2;
    u16 uStack_20;
    void *pvVar3;
    NNSG3dAnmObj *pAnmObj;
    u32 fileId;
    NNSG3dRenderObj *obj;
    u32 uVar4;
    u8 *base;
    NNSG3dResTex *pResTex;
    HeapExp_FndInitAllocator(&param_1->allocator, HEAP_ID_153, 4);
    uStack_20 = 0;
    if (uStack_20 < param_4) {
        do {
            obj = (NNSG3dRenderObj *)(((u8 *)&param_1->models[0]) + (uStack_20 * 0x7C));
            iVar1 = (u8 *)param_3 + (uStack_20 * 0xc);
            pNVar2 = GfGfxLoader_LoadFromNarc(param_2, *((s32 *)((u8 *)param_3 + (uStack_20 * 0xc))), 1, HEAP_ID_153, 1);
            obj[1].resMdl = pNVar2;
            GF3dRender_InitObjFromHeader(obj, (NNSG3dResMdl **)(obj + 1), (NNSG3dResFileHeader **)(&obj[1].resMdl));
            pResTex = NNS_G3dGetTex((NNSG3dResFileHeader *)obj[1].resMdl);
            if ((*((s32 *)(iVar1 + 8))) == 1) {
                NNSi_G3dModifyMatFlag((NNSG3dResMdl *)obj[1].flag, 0, NNS_G3D_MATFLAG_DIFFUSE);
                NNSi_G3dModifyMatFlag((NNSG3dResMdl *)obj[1].flag, 0, NNS_G3D_MATFLAG_AMBIENT);
                NNSi_G3dModifyMatFlag((NNSG3dResMdl *)obj[1].flag, 0, NNS_G3D_MATFLAG_SPECULAR);
                NNSi_G3dModifyMatFlag((NNSG3dResMdl *)obj[1].flag, 0, NNS_G3D_MATFLAG_EMISSION);
            }
            uVar4 = 0;
            do {
                fileId = (u32)(*((u8 *)((iVar1 + uVar4) + 4)));
                if (fileId == 0xff) {
                    obj[1].hintMatAnmExist[uVar4 - 0xd] = 0;
                    obj[1].hintMatAnmExist[uVar4 - 9] = 0;
                } else {
                    base = (u8 *)obj + uVar4 * 4;
                    pvVar3 = GfGfxLoader_LoadFromNarc(param_2, fileId, 1, HEAP_ID_153, 1);
                    *(void **)(base + 0x5c) = (u8 *)pvVar3;
                    pvVar3 = NNS_G3dGetAnmByIdx(pvVar3, 0);
                    pAnmObj = NNS_G3dAllocAnmObj(&param_1->allocator, pvVar3, (NNSG3dResMdl *)obj[1].flag);
                    *(void **)(base + 0x6c) = (u8 *)pAnmObj;
                    NNS_G3dAnmObjInit(pAnmObj, pvVar3, (NNSG3dResMdl *)obj[1].flag, pResTex);
                    NNS_G3dRenderObjAddAnmObj(obj, (NNSG3dAnmObj *)*(NNSG3dAnmObj **)(base + 0x6c));
                }
                uVar4 = (uVar4 + 1) & 0xffff;
            } while (uVar4 < 4);
            uStack_20 = (uStack_20 + 1) & 0xffff;
        } while (uStack_20 < param_4);
    }
    param_1->numModels = param_4;
    return;
}
#endif
