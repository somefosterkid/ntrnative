#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "overlay_108_safari_area_customizer.h"

void ov108_021E7B74(SafariAreaCustomizer *arg0, NARC *narc, s32 arg2, s32 arg3);

void ov108_021E7B74(SafariAreaCustomizer *arg0, NARC *narc, s32 arg2, s32 arg3) {
    NNSG2dCharacterData *ppCharData;
    void *ptr;

    ptr = GfGfxLoader_GetCharDataFromOpenNarc(narc, arg3 + 0x12, 0, &ppCharData, arg0->heapID);
    BG_LoadCharTilesData(arg0->bgConfig, 5U, ppCharData->pRawData, 0x1000U, (arg2 << 6) + 1);
    Heap_Free(ptr);
}
