#include "global.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "filesystem.h"

struct NARC;

NARC *NARC_New(enum NarcId narcID, enum HeapID heapID);

void NARC_Delete(NARC *narc);

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

void Heap_Free(void *ptr);

void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

void ov80_02234DC4(u8 fraction, u8 source);

void ov80_02234DC4(u8 fraction, u8 source) {
    u32 narcIdx, size;
    if (source == 0) {
        narcIdx = 162;
        size = (15 * 16) * 2;
    } else {
        narcIdx = 161;
        size = (9 * 16) * 2;
    }

    u16 *blendResult = Heap_Alloc(HEAP_ID_101, 0x1000 * 2);
    memset(blendResult, 0, 0x1000 * 2);

    NNSG2dPaletteData *plttData;
    NARC *narc = NARC_New((enum NarcId)183, HEAP_ID_101);
    void *pltt = GfGfxLoader_GetPlttDataFromOpenNarc(narc, narcIdx, &plttData, HEAP_ID_101);

    BlendPalette(plttData->pRawData, blendResult, 0x1000, fraction, 0x0);
    DC_FlushRange(blendResult, 0x1000 * 2);

    GX_BeginLoadBGExtPltt();
    GX_LoadBGExtPltt(blendResult, 0x6000, size);
    GX_EndLoadBGExtPltt();

    NARC_Delete(narc);
    Heap_Free(blendResult);
    Heap_Free(pltt);
}
