#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "heap.h"

void ov02_0224A598(BgConfig *arg0, NARC *arg1, u32 arg2, NNSG2dCharacterData **arg3);

void ov02_0224A598(BgConfig *bgConfig, NARC *narc, u32 file_id, NNSG2dCharacterData **ppCharData) {
    NNSG2dCharacterData *nNSG2dCharacterData;
    void *v0;

    v0 = NARC_AllocAndReadWholeMember(narc, file_id, HEAP_ID_FIELD1);
    NNS_G2dGetUnpackedCharacterData(v0, ppCharData);
    nNSG2dCharacterData = *ppCharData;
    BG_LoadCharTilesData(bgConfig, 3U, nNSG2dCharacterData->pRawData, nNSG2dCharacterData->szByte, 0U);
    Heap_Free(v0);
}
