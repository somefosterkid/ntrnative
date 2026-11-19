#include "global.h"

#include "heap.h"
#include "sprite_transfer.h"
#include "unk_0200A090.h"
#include "overlay_96_unk_struct_ov96_021e9a78.h"

void ov96_021E9C0C(UnkStruct_ov96_021E9A78 *res);

u8 ov96_021E9CF4(UnkStruct_ov96_021E9A78 *res, s32 flag);
GF_2DGfxResMan *ov96_021E9D08(UnkStruct_ov96_021E9A78 *res, s32 index);
void ov96_021E9D10(UnkStruct_ov96_021E9A78 *res);

void ov96_021E9C0C(UnkStruct_ov96_021E9A78 *res) {
    SpriteResource *plttResObj;
    s32 v1;

    if (1 & res->flags) {
        SpriteTransfer_DeleteCharTransferTask(res->charRes[0]);
    }
    if (0x4 & res->flags) {
        SpriteTransfer_DeleteCharTransferTask(res->charRes[1]);
    }
    if (0x2 & res->flags) {
        SpriteTransfer_DeleteCharTransferTask(res->charRes[2]);
    }
    if (0x8 & res->flags) {
        SpriteTransfer_DeleteCharTransferTask(res->charRes[3]);
    }
    if (0x10 & res->flags) {
        SpriteTransfer_DeleteCharTransferTask(res->charRes[4]);
    }
    if (0x20 & res->flags) {
        SpriteTransfer_DeleteCharTransferTask(res->charRes[5]);
    }
    if (0x40 & res->flags) {
        SpriteTransfer_DeleteCharTransferTask(res->charRes[6]);
    }
    if (0x80 & res->flags) {
        SpriteTransfer_DeleteCharTransferTask(res->charRes[7]);
    }
    if (0x100 & res->flags) {
        SpriteTransfer_DeleteCharTransferTask(res->charRes[8]);
    }
    if (0x200 & res->flags) {
        SpriteTransfer_DeleteCharTransferTask(res->charRes[9]);
    }
    if (0x400 & res->flags) {
        SpriteTransfer_DeleteCharTransferTask(res->charRes[10]);
    }
    if (0x800 & res->flags) {
        SpriteTransfer_DeleteCharTransferTask(res->charRes[11]);
    }
    SpriteTransfer_DeletePlttTransferTask(res->plttRes[0]);
    plttResObj = res->plttRes[1];
    if (plttResObj != NULL) {
        SpriteTransfer_DeletePlttTransferTask(plttResObj);
    }
    v1 = 0;
    do {
        Destroy2DGfxResObjMan(res->resMan[v1]);
        v1 += 1;
    } while (v1 < 4);
    Heap_Free(res);
}

u8 ov96_021E9CF4(UnkStruct_ov96_021E9A78 *res, s32 flag) {
    if (res == NULL) {
        return 0U;
    }
    return (u8)(res->flags & flag);
}

GF_2DGfxResMan *ov96_021E9D08(UnkStruct_ov96_021E9A78 *res, s32 index) {
    return res->resMan[index];
}

void ov96_021E9D10(UnkStruct_ov96_021E9A78 *res) {
    s32 v0;
    s32 vram;

    if (res->flags & 1) {
        res->charRes[0] = AddCharResObjFromNarc(res->resMan[0], NARC_a_1_5_3, 3, 0, 0x14, 1, res->heapId);
        res->cellRes[0] = AddCellOrAnimResObjFromNarc(res->resMan[2], NARC_a_1_5_3, 2, 0, 0x14, GF_GFX_RES_TYPE_CELL, res->heapId);
        res->animRes[0] = AddCellOrAnimResObjFromNarc(res->resMan[3], NARC_a_1_5_3, 1, 0, 0x14, GF_GFX_RES_TYPE_ANIM, res->heapId);
        SpriteTransfer_CreateCharTransferTask_AllocAtEnd(res->charRes[0]);
    }
    if (4 & res->flags) {
        res->charRes[1] = AddCharResObjFromNarc(res->resMan[0], NARC_a_1_5_3, 6, 0, 0x15, res->vram, res->heapId);
        res->cellRes[1] = AddCellOrAnimResObjFromNarc(res->resMan[2], NARC_a_1_5_3, 5, 0, 0x15, GF_GFX_RES_TYPE_CELL, res->heapId);
        res->animRes[1] = AddCellOrAnimResObjFromNarc(res->resMan[3], NARC_a_1_5_3, 4, 0, 0x15, GF_GFX_RES_TYPE_ANIM, res->heapId);
        SpriteTransfer_CreateCharTransferTask_AllocAtEnd(res->charRes[1]);
    }
    if (2 & res->flags) {
        res->charRes[2] = AddCharResObjFromNarc(res->resMan[0], NARC_a_1_5_3, 9, 0, 0x16, res->vram, res->heapId);
        res->cellRes[2] = AddCellOrAnimResObjFromNarc(res->resMan[2], NARC_a_1_5_3, 8, 0, 0x16, GF_GFX_RES_TYPE_CELL, res->heapId);
        res->animRes[2] = AddCellOrAnimResObjFromNarc(res->resMan[3], NARC_a_1_5_3, 7, 0, 0x16, GF_GFX_RES_TYPE_ANIM, res->heapId);
        SpriteTransfer_CreateCharTransferTask_AllocAtEnd(res->charRes[2]);
    }
    if (res->flags & 0x10) {
        res->charRes[4] = AddCharResObjFromNarc(res->resMan[0], NARC_a_1_5_3, 0x10, 0, 0x1A, 1, res->heapId);
        res->cellRes[4] = AddCellOrAnimResObjFromNarc(res->resMan[2], NARC_a_1_5_3, 0xF, 0, 0x1A, GF_GFX_RES_TYPE_CELL, res->heapId);
        res->animRes[4] = AddCellOrAnimResObjFromNarc(res->resMan[3], NARC_a_1_5_3, 0xE, 0, 0x1A, GF_GFX_RES_TYPE_ANIM, res->heapId);
        SpriteTransfer_CreateCharTransferTask_AllocAtEnd(res->charRes[4]);
    }
    if (0x20 & res->flags) {
        res->charRes[5] = AddCharResObjFromNarc(res->resMan[0], NARC_a_1_5_3, 0x13, 0, 0x1B, 1, res->heapId);
        res->cellRes[5] = AddCellOrAnimResObjFromNarc(res->resMan[2], NARC_a_1_5_3, 0x12, 0, 0x1B, GF_GFX_RES_TYPE_CELL, res->heapId);
        res->animRes[5] = AddCellOrAnimResObjFromNarc(res->resMan[3], NARC_a_1_5_3, 0x11, 0, 0x1B, GF_GFX_RES_TYPE_ANIM, res->heapId);
        SpriteTransfer_CreateCharTransferTask_AllocAtEnd(res->charRes[5]);
    }
    if (8 & res->flags) {
        res->charRes[3] = AddCharResObjFromNarc(res->resMan[0], NARC_a_1_5_3, 0xD, 0, 0x19, 2, res->heapId);
        res->cellRes[3] = AddCellOrAnimResObjFromNarc(res->resMan[2], NARC_a_1_5_3, 0xC, 0, 0x19, GF_GFX_RES_TYPE_CELL, res->heapId);
        res->animRes[3] = AddCellOrAnimResObjFromNarc(res->resMan[3], NARC_a_1_5_3, 0xB, 0, 0x19, GF_GFX_RES_TYPE_ANIM, res->heapId);
        SpriteTransfer_CreateCharTransferTask_AllocAtEnd(res->charRes[3]);
    }
    if (0x40 & res->flags) {
        res->charRes[6] = AddCharResObjFromNarc(res->resMan[0], NARC_a_1_5_3, 0x16, 0, 0x1C, 1, res->heapId);
        res->cellRes[6] = AddCellOrAnimResObjFromNarc(res->resMan[2], NARC_a_1_5_3, 0x15, 0, 0x1C, GF_GFX_RES_TYPE_CELL, res->heapId);
        res->animRes[6] = AddCellOrAnimResObjFromNarc(res->resMan[3], NARC_a_1_5_3, 0x14, 0, 0x1C, GF_GFX_RES_TYPE_ANIM, res->heapId);
        SpriteTransfer_CreateCharTransferTask_AllocAtEnd(res->charRes[6]);
    }
    if (0x80 & res->flags) {
        res->charRes[7] = AddCharResObjFromNarc(res->resMan[0], NARC_a_1_5_3, 0x19, 0, 0x1D, 1, res->heapId);
        res->cellRes[7] = AddCellOrAnimResObjFromNarc(res->resMan[2], NARC_a_1_5_3, 0x18, 0, 0x1D, GF_GFX_RES_TYPE_CELL, res->heapId);
        res->animRes[7] = AddCellOrAnimResObjFromNarc(res->resMan[3], NARC_a_1_5_3, 0x17, 0, 0x1D, GF_GFX_RES_TYPE_ANIM, res->heapId);
        SpriteTransfer_CreateCharTransferTask_AllocAtEnd(res->charRes[7]);
    }
    if (0x100 & res->flags) {
        res->charRes[8] = AddCharResObjFromNarc(res->resMan[0], NARC_a_1_5_3, 0x1C, 0, 0x1E, 2, res->heapId);
        res->cellRes[8] = AddCellOrAnimResObjFromNarc(res->resMan[2], NARC_a_1_5_3, 0x1B, 0, 0x1E, GF_GFX_RES_TYPE_CELL, res->heapId);
        res->animRes[8] = AddCellOrAnimResObjFromNarc(res->resMan[3], NARC_a_1_5_3, 0x1A, 0, 0x1E, GF_GFX_RES_TYPE_ANIM, res->heapId);
        SpriteTransfer_CreateCharTransferTask_AllocAtEnd(res->charRes[8]);
    }
    if (0x200 & res->flags) {
        res->charRes[9] = AddCharResObjFromNarc(res->resMan[0], NARC_a_1_5_3, 0x1F, 0, 0x1F, 1, res->heapId);
        res->cellRes[9] = AddCellOrAnimResObjFromNarc(res->resMan[2], NARC_a_1_5_3, 0x1E, 0, 0x1F, GF_GFX_RES_TYPE_CELL, res->heapId);
        res->animRes[9] = AddCellOrAnimResObjFromNarc(res->resMan[3], NARC_a_1_5_3, 0x1D, 0, 0x1F, GF_GFX_RES_TYPE_ANIM, res->heapId);
        SpriteTransfer_CreateCharTransferTask_AllocAtEnd(res->charRes[9]);
    }
    if (0x400 & res->flags) {
        res->charRes[10] = AddCharResObjFromNarc(res->resMan[0], NARC_a_1_5_3, 0x22, 0, 0x1F, 2, res->heapId);
        res->cellRes[10] = AddCellOrAnimResObjFromNarc(res->resMan[2], NARC_a_1_5_3, 0x21, 0, 0x1F, GF_GFX_RES_TYPE_CELL, res->heapId);
        res->animRes[10] = AddCellOrAnimResObjFromNarc(res->resMan[3], NARC_a_1_5_3, 0x20, 0, 0x1F, GF_GFX_RES_TYPE_ANIM, res->heapId);
        SpriteTransfer_CreateCharTransferTask_AllocAtEnd(res->charRes[10]);
    }
    if (0x800 & res->flags) {
        res->charRes[11] = AddCharResObjFromNarc(res->resMan[0], NARC_a_1_5_3, 0x25, 0, 0x20, 1, res->heapId);
        res->cellRes[11] = AddCellOrAnimResObjFromNarc(res->resMan[2], NARC_a_1_5_3, 0x24, 0, 0x20, GF_GFX_RES_TYPE_CELL, res->heapId);
        res->animRes[11] = AddCellOrAnimResObjFromNarc(res->resMan[3], NARC_a_1_5_3, 0x23, 0, 0x20, GF_GFX_RES_TYPE_ANIM, res->heapId);
        SpriteTransfer_CreateCharTransferTask_AllocAtEnd(res->charRes[11]);
    }
    vram = res->vram;
    if (vram == 2) {
        vram = 3;
    }
    res->plttRes[0] = AddPlttResObjFromNarc(res->resMan[1], NARC_a_1_5_3, 0, 0, 0x17, vram, 1, res->heapId);
    SpriteTransfer_CreateExtPlttTransferTask(res->plttRes[0]);
    res->plttRes[1] = NULL;
    v0 = res->flags;
    if ((v0 & 8) || (0x100 & v0) || (0x400 & v0)) {
        res->plttRes[1] = AddPlttResObjFromNarc(res->resMan[1], NARC_a_1_5_3, 0xA, 0, 0x18, 2, 1, res->heapId);
        SpriteTransfer_CreateExtPlttTransferTask(res->plttRes[1]);
    }
}
