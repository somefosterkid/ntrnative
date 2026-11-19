#include "global.h"

#include "bg_window.h"
#include "error_handling.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "overlay_111.h"

extern u8 _021E6B74;

void ov111_021E5FD4(BugContestSwapMon *arg0);

void ov111_021E5FD4(BugContestSwapMon *arg0) {
    s32 v0;
    u8 *v1;

    NARC_Delete(arg0->narc);
    v1 = &_021E6B74;
    v0 = 0;
    do {
        FreeBgTilemapBuffer(arg0->bgConfig, *v1);
        v0 += 1;
        v1 += 1;
    } while (v0 < 5);
    Heap_Free(arg0->bgConfig);
}

void ov111_021E6000(BugContestSwapMon *arg0);

void ov111_021E6000(BugContestSwapMon *arg0) {
    if (arg0->bgConfig == NULL) {
        GF_AssertFail();
    }
    GfGfxLoader_GXLoadPalFromOpenNarc(arg0->narc, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x200U, arg0->heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(arg0->narc, 1, arg0->bgConfig, GF_BG_LYR_MAIN_FIRST, 0U, 0U, 1, arg0->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg0->narc, 2, arg0->bgConfig, GF_BG_LYR_MAIN_FIRST, 0U, 0U, 1, arg0->heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(arg0->narc, 4, arg0->bgConfig, GF_BG_LYR_SUB_1_F, 1U, 0U, 1, arg0->heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(arg0->narc, 4, arg0->bgConfig, GF_BG_LYR_MAIN_3, 1U, 0U, 1, arg0->heapID);
    GfGfxLoader_GXLoadPalFromOpenNarc(arg0->narc, 0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x200U, arg0->heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(arg0->narc, 1, arg0->bgConfig, GF_BG_LYR_SUB_2_F, 0U, 0U, 1, arg0->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg0->narc, 3, arg0->bgConfig, GF_BG_LYR_SUB_2_F, 0U, 0U, 1, arg0->heapID);
}
