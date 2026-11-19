#include "global.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "filesystem_files_def.h"
#include "heap.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02013FDC.h"

void ov96_021EED64(Sprite *param_1, void *param_2, u32 param_3);

void ov96_021EEA94(Sprite *param_1, u32 param_2, u32 param_3, enum HeapID param_4);

void ov96_021EEA94(Sprite *param_1, u32 param_2, u32 param_3, enum HeapID param_4) {
    u8 *texBase;
    u32 uVar4;
    NARC *narc;
    NNSG3dResTex *pNVar1;
    NNSG3dResDict *pNVar2;
    u32 *puVar3;
    NNSG3dResFileHeader *header;
    void *dest;
    NNS_G2D_VRAM_TYPE type;
    NNSG2dImagePaletteProxy *pImg;
    u32 uVar5;
    u32 uVar6;

    thunk_Sprite_SetDrawFlag(param_1, 0);
    narc = NARC_New(NARC_data_mmodel_mmodel, param_4);
    header = NARC_AllocAndReadWholeMember(narc, param_2, param_4);
    pNVar1 = NNS_G3dGetTex(header);
    if (pNVar1 != NULL) {
        pNVar2 = &pNVar1->dict;
        if ((pNVar2 != NULL) && (param_3 < (pNVar1->dict).numEntry)) {
            uVar5 = (pNVar1->dict).ofsEntry;
            u8 *ent = (u8 *)pNVar2 + uVar5;
            puVar3 = (u32 *)(ent + 4 + *(u16 *)ent * param_3);
        } else {
            puVar3 = NULL;
        }
    } else {
        puVar3 = NULL;
    }
    uVar5 = (*puVar3 & 0xffff) * 8;
    texBase = (u8 *)pNVar1 + pNVar1->texInfo.ofsTex;
    dest = Heap_AllocAtEnd(param_4, 0x200);
    sub_020145B4(texBase + uVar5, 4, 0, 0, 4, 4, dest);
    ov96_021EED64(param_1, dest, 0x200);
    Heap_Free(dest);
    type = Sprite_GetVramType(param_1);
    uVar6 = pNVar1->plttInfo.ofsPlttData;
    DC_FlushRange((u8 *)pNVar1 + uVar6, 0x20);
    pImg = Sprite_GetPaletteProxy(param_1);
    uVar4 = NNS_G2dGetImagePaletteLocation(pImg, type);
    if (type == NNS_G2D_VRAM_TYPE_2DMAIN) {
        GX_LoadOBJPltt((u8 *)pNVar1 + uVar6, uVar4, 0x20);
    } else {
        GXS_LoadOBJPltt((u8 *)pNVar1 + uVar6, uVar4, 0x20);
    }
    Heap_Free(header);
    NARC_Delete(narc);
    thunk_Sprite_SetDrawFlag(param_1, 1);
    return;
}
