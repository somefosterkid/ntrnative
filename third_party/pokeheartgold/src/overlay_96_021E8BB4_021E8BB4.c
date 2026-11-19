#include "global.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "filesystem_files_def.h"
#include "heap.h"
#include "overlay_96_sprite_res_man.h"

s32 ov96_021E91B8(s32, s32, s32);

void ov96_021E8BB4(Ov96SpriteResDesc *desc, enum HeapID heapID, void *dest) {
    NNSG3dResFileHeader *file;
    NNSG3dResTex *tex;
    void *pltt;

    file = AllocAndReadWholeNarcMemberByIdPair(NARC_data_mmodel_mmodel, ov96_021E91B8(desc->species, desc->unk_2, desc->unk_7), heapID);
    tex = NNS_G3dGetTex(file);
    pltt = ((u8 *)tex) + tex->plttInfo.ofsPlttData;
    if (desc->unk_6 != 0) {
        pltt += 0x20;
    }
    MI_CpuCopy8(pltt, dest, 0x20U);
    Heap_Free(file);
}
