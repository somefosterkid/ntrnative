#include "global.h"

#include "constants/heap.h"

#include "error_handling.h"
#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "unk_02013534.h"
#include "unk_02013534_font_system.h"

UnkStruct_02013534 *FontSystem_NewInit(int a0, enum HeapID heapId) {
    int count;
    UnkStruct_02012744 *ret;
    NNSG2dCellDataBank **pData;
    NNSG2dCellDataBank **pBank;
    int i;
    u32 size;

    count = a0;
    ret = Heap_Alloc(heapId, sizeof(UnkStruct_02012744));
    if (ret == NULL) {
        GF_AssertFail();
    }
    i = 0;
    pData = ret->unk_30;
    pBank = (NNSG2dCellDataBank **)ret->unk_00;
    do {
        *pBank = GfGfxLoader_GetCellBank(NARC_a_0_3_5, i, 0, pData, heapId);
        if (*pBank == NULL) {
            GF_AssertFail();
        }
        pData++;
        pBank++;
        i++;
    } while (i < 12);
    size = count * sizeof(FontOAM);
    ret->unk_60 = Heap_Alloc(heapId, size);
    if (ret->unk_60 == NULL) {
        GF_AssertFail();
    }
    ret->unk_64 = count;
    memset(ret->unk_60, 0, size);
    return (UnkStruct_02013534 *)ret;
}
