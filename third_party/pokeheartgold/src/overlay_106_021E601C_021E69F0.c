#include "global.h"

#include "gf_gfx_loader.h"
#include "heap.h"
#include "overlay_106_arceus_cinematic.h"

void ov106_021E69F0(ArceusCinematic *arg0, s32 arg1, s32 arg2);

void ov106_021E69F0(ArceusCinematic *arg0, s32 arg1, s32 arg2) {
    NNSG2dPaletteData *ppPlttData;
    ArceusBgPalette *destp;
    void *ptr;

    ptr = GfGfxLoader_GetPlttDataFromOpenNarc(arg0->narc, arg2 + 0x88, &ppPlttData, HEAP_ID_153);
    destp = &arg0->bgPalettes[arg1];
    destp->size = (u32)ppPlttData->szByte;
    MIi_CpuCopy32(ppPlttData->pRawData, destp, destp->size);
    Heap_Free(ptr);
}
