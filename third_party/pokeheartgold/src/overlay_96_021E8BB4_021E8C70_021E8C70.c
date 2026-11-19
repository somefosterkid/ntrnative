#include "global.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "filesystem.h"
#include "filesystem_files_def.h"
#include "heap.h"
#include "overlay_96_sprite_res_man.h"
#include "sprite_transfer.h"
#include "unk_0200A090.h"
#include "unk_02013FDC.h"

s32 ov96_021E91B8(s32 param_1, s32 param_2, s32 param_3);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov96_021E8C70(Ov96SpriteResMan *man, Ov96SpriteResEntry *entry, Ov96SpriteResDesc *desc, s32 param_4, u8 param_5, s32 param_6) {
    u32 offs;
    s32 lim;
    u8 *srcp;
    s32 iStack_1c;
    s32 iStack_20;
    int iStack_24;
    s32 iStack_28;
    NNSG3dResFileHeader *header;
    u8 *texData;
    NNSG3dResTex *pNVar5;
    NNSG2dImagePaletteProxy *pImg;
    NNSG2dImageProxy *imageProxy;
    NNSG2dCharacterData *pNVar4;
    BOOL bVar1;
    BOOL bVar2;
    int uStack_4c;
    SpriteResource *pSVar3;
    s32 iVar6;
    u32 uVar7;
    u32 uVar8;
    s32 iVar9;
    void *startAddr;
    if (param_6 != 0) {
        if (desc->isLarge != 0) {
            iStack_1c = 10;
            iVar9 = 0xb;
            iStack_20 = 9;
        } else {
            iStack_1c = 7;
            iVar9 = 8;
            iStack_20 = 6;
        }
    } else if (desc->isLarge != 0) {
        iStack_1c = 4;
        iVar9 = 5;
        iStack_20 = 3;
    } else {
        iStack_1c = 1;
        iVar9 = 2;
        iStack_20 = 0;
    }
    pSVar3 = AddCharResObjFromNarc(man->resMan[0], NARC_a_1_4_9, iVar9, 0, (u32)param_5, param_4, man->heapID);
    entry->charRes = pSVar3;
    pSVar3 = AddPlttResObjFromNarc(man->resMan[1], NARC_a_0_4_9, 0x1e, 0, (u32)param_5, param_4, 1, man->heapID);
    entry->plttRes = pSVar3;
    pSVar3 = AddCellOrAnimResObjFromNarc(man->resMan[2], NARC_a_1_4_9, iStack_1c, 0, (u32)param_5, GF_GFX_RES_TYPE_CELL, man->heapID);
    entry->cellRes = pSVar3;
    pSVar3 = AddCellOrAnimResObjFromNarc(man->resMan[3], NARC_a_1_4_9, iStack_20, 0, (u32)param_5, GF_GFX_RES_TYPE_ANIM, man->heapID);
    entry->animRes = pSVar3;
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(entry->charRes);
    SpriteTransfer_CreateExtPlttTransferTask(entry->plttRes);
    bVar1 = FALSE;
    bVar2 = FALSE;
    imageProxy = SpriteTransfer_GetCharProxy(entry->charRes);
    pImg = SpriteTransfer_GetPaletteProxy(entry->plttRes, imageProxy);
    pNVar4 = GF2DGfxResObj_GetCharDataPtr(entry->charRes);
    if (param_4 == 3) {
        bVar1 = TRUE;
        bVar2 = TRUE;
    } else if (param_4 == 1) {
        bVar1 = TRUE;
    } else {
        bVar2 = TRUE;
    }
    iVar9 = ov96_021E91B8(desc->species, desc->unk_2, desc->unk_7);
    header = AllocAndReadWholeNarcMemberByIdPair(NARC_data_mmodel_mmodel, iVar9, man->heapID);
    if (desc->isLarge != 0) {
        uStack_4c = 0x800;
        iStack_24 = 8;
    } else {
        uStack_4c = 0x200;
        iStack_24 = 4;
    }
    pNVar5 = NNS_G3dGetTex(header);
    texData = (u8 *)pNVar5 + (pNVar5->texInfo).ofsTex;
    iStack_28 = 0;
    offs = 0;
    lim = 0x800 - iStack_24 * 0x20;
    srcp = man->texBuf + iStack_24 * 0x20;
    for (; iStack_28 < 8; iStack_28 = iStack_28 + 1) {
        sub_020145B4(texData + offs, iStack_24, 0, 0, iStack_24, iStack_24, man->texBuf);
        if (param_6 == 0) {
            if ((man->mode != 0) && (desc->isLarge != 0)) {
                iVar6 = 0;
                do {
                    if (iVar6 < lim) {
                        man->texBuf[iVar6] = srcp[iVar6];
                    } else {
                        man->texBuf[iVar6] = 0;
                    }
                    iVar6 = iVar6 + 1;
                } while (iVar6 < 0x800);
            }
            DC_FlushRange(man->texBuf, uStack_4c);
            if (bVar1) {
                uVar7 = NNS_G2dGetImageLocation(imageProxy, NNS_G2D_VRAM_TYPE_2DMAIN);
                GX_LoadOBJ(man->texBuf, uVar7 + offs, uStack_4c);
            }
            if (bVar2) {
                uVar7 = NNS_G2dGetImageLocation(imageProxy, NNS_G2D_VRAM_TYPE_2DSUB);
                GXS_LoadOBJ(man->texBuf, uVar7 + offs, uStack_4c);
            }
        }
        MIi_CpuCopyFast(man->texBuf, (void *)((u8 *)pNVar4->pRawData + offs), uStack_4c);
        offs = offs + uStack_4c;
    }
    startAddr = (void *)((u8 *)pNVar5 + (pNVar5->plttInfo).ofsPlttData);
    if (desc->unk_6 != 0) {
        startAddr = (void *)((u8 *)startAddr + 0x20);
    }
    DC_FlushRange(startAddr, 0x20);
    if (bVar1) {
        uVar8 = NNS_G2dGetImagePaletteLocation(pImg, NNS_G2D_VRAM_TYPE_2DMAIN);
        GX_LoadOBJPltt(startAddr, uVar8, 0x20);
    }
    if (bVar2) {
        uVar8 = NNS_G2dGetImagePaletteLocation(pImg, NNS_G2D_VRAM_TYPE_2DSUB);
        GXS_LoadOBJPltt(startAddr, uVar8, 0x20);
    }
    Heap_Free(header);
    return;
}
#else
void ov96_021E8C70(Ov96SpriteResMan *man, Ov96SpriteResEntry *entry, Ov96SpriteResDesc *desc, s32 param_4, u8 param_5, s32 param_6) {
    enum HeapID *param_1 = (enum HeapID *)man;
    u32 *param_2 = (u32 *)entry;
    u16 *param_3 = (u16 *)desc;
    u32 offs;
    s32 lim;
    u8 *srcp;
    s32 iStack_1c;
    s32 iStack_20;
    int iStack_24;
    s32 iStack_28;
    NNSG3dResFileHeader *header;
    u8 *texData;
    NNSG3dResTex *pNVar5;
    NNSG2dImagePaletteProxy *pImg;
    NNSG2dImageProxy *imageProxy;
    NNSG2dCharacterData *pNVar4;
    BOOL bVar1;
    BOOL bVar2;
    int uStack_4c;
    SpriteResource *pSVar3;
    s32 iVar6;
    u32 uVar7;
    u32 uVar8;
    s32 iVar9;
    void *startAddr;
    if (param_6 != 0) {
        if (param_3[2] != 0) {
            iStack_1c = 10;
            iVar9 = 0xb;
            iStack_20 = 9;
        } else {
            iStack_1c = 7;
            iVar9 = 8;
            iStack_20 = 6;
        }
    } else if (param_3[2] != 0) {
        iStack_1c = 4;
        iVar9 = 5;
        iStack_20 = 3;
    } else {
        iStack_1c = 1;
        iVar9 = 2;
        iStack_20 = 0;
    }
    pSVar3 = AddCharResObjFromNarc((GF_2DGfxResMan *)param_1[0x51], NARC_a_1_4_9, iVar9, 0, (u32)param_5, param_4, *param_1);
    *param_2 = (u32)(pSVar3);
    pSVar3 = AddPlttResObjFromNarc((GF_2DGfxResMan *)param_1[0x52], NARC_a_0_4_9, 0x1e, 0, (u32)param_5, param_4, 1, *param_1);
    param_2[1] = (u32)(pSVar3);
    pSVar3 = AddCellOrAnimResObjFromNarc((GF_2DGfxResMan *)param_1[0x53], NARC_a_1_4_9, iStack_1c, 0, (u32)param_5, GF_GFX_RES_TYPE_CELL, *param_1);
    param_2[2] = (u32)(pSVar3);
    pSVar3 = AddCellOrAnimResObjFromNarc((GF_2DGfxResMan *)param_1[0x54], NARC_a_1_4_9, iStack_20, 0, (u32)param_5, GF_GFX_RES_TYPE_ANIM, *param_1);
    param_2[3] = (u32)(pSVar3);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd((SpriteResource *)*param_2);
    SpriteTransfer_CreateExtPlttTransferTask((SpriteResource *)param_2[1]);
    bVar1 = FALSE;
    bVar2 = FALSE;
    imageProxy = SpriteTransfer_GetCharProxy((SpriteResource *)*param_2);
    pImg = SpriteTransfer_GetPaletteProxy((SpriteResource *)param_2[1], imageProxy);
    pNVar4 = GF2DGfxResObj_GetCharDataPtr((SpriteResource *)*param_2);
    if (param_4 == 3) {
        bVar1 = TRUE;
        bVar2 = TRUE;
    } else if (param_4 == 1) {
        bVar1 = TRUE;
    } else {
        bVar2 = TRUE;
    }
    iVar9 = ov96_021E91B8((u32)(u16)*param_3, (u32)(u16)param_3[1], (u32) * (u8 *)((u8 *)param_3 + 7));
    header = AllocAndReadWholeNarcMemberByIdPair(NARC_data_mmodel_mmodel, iVar9, *param_1);
    if (param_3[2] != 0) {
        uStack_4c = 0x800;
        iStack_24 = 8;
    } else {
        uStack_4c = 0x200;
        iStack_24 = 4;
    }
    pNVar5 = NNS_G3dGetTex(header);
    texData = (u8 *)pNVar5 + (pNVar5->texInfo).ofsTex;
    iStack_28 = 0;
    offs = 0;
    lim = 0x800 - iStack_24 * 0x20;
    srcp = (u8 *)param_1 + iStack_24 * 0x20;
    for (; iStack_28 < 8; iStack_28 = iStack_28 + 1) {
        sub_020145B4(texData + offs, iStack_24, 0, 0, iStack_24, iStack_24, param_1 + 0x56);
        if (param_6 == 0) {
            if ((param_1[2] != HEAP_ID_DEFAULT) && (param_3[2] != 0)) {
                iVar6 = 0;
                do {
                    if (iVar6 < lim) {
#if defined(SDK_PORT) || defined(NONMATCHING)
                        *(u8 *)((u8 *)param_1 + iVar6 + 0x158) = srcp[iVar6 + 0x158];
#else
                        *(u8 *)((s32)param_1 + iVar6 + 0x158) = srcp[iVar6 + 0x158];
#endif
                    } else {
#if defined(SDK_PORT) || defined(NONMATCHING)
                        *(u8 *)((u8 *)param_1 + iVar6 + 0x158) = 0;
#else
                        *(u8 *)((s32)param_1 + iVar6 + 0x158) = 0;
#endif
                    }
                    iVar6 = iVar6 + 1;
                } while (iVar6 < 0x800);
            }
            DC_FlushRange(param_1 + 0x56, uStack_4c);
            if (bVar1) {
                uVar7 = NNS_G2dGetImageLocation(imageProxy, NNS_G2D_VRAM_TYPE_2DMAIN);
                GX_LoadOBJ(param_1 + 0x56, uVar7 + offs, uStack_4c);
            }
            if (bVar2) {
                uVar7 = NNS_G2dGetImageLocation(imageProxy, NNS_G2D_VRAM_TYPE_2DSUB);
                GXS_LoadOBJ(param_1 + 0x56, uVar7 + offs, uStack_4c);
            }
        }
        MIi_CpuCopyFast(param_1 + 0x56, (void *)((u8 *)pNVar4->pRawData + offs), uStack_4c);
        offs = offs + uStack_4c;
    }
    startAddr = (void *)((u8 *)pNVar5 + (pNVar5->plttInfo).ofsPlttData);
    if (*((u8 *)param_3 + 6) != 0) {
        startAddr = (void *)((u8 *)startAddr + 0x20);
    }
    DC_FlushRange(startAddr, 0x20);
    if (bVar1) {
        uVar8 = NNS_G2dGetImagePaletteLocation(pImg, NNS_G2D_VRAM_TYPE_2DMAIN);
        GX_LoadOBJPltt(startAddr, uVar8, 0x20);
    }
    if (bVar2) {
        uVar8 = NNS_G2dGetImagePaletteLocation(pImg, NNS_G2D_VRAM_TYPE_2DSUB);
        GXS_LoadOBJPltt(startAddr, uVar8, 0x20);
    }
    Heap_Free(header);
    return;
}
#endif
