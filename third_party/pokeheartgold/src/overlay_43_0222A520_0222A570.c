#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "overlay_43_palpad.h"

extern const GraphicsModes ov43_0222EDC8;

extern const BgTemplate ov43_0222EFFC[];

void ov43_0222A570(PalPadGfx *gfx, enum HeapID heapID);

void ov43_0222A570(PalPadGfx *gfx, enum HeapID heapID) {
    s32 i;
    const BgTemplate *template;
    GraphicsModes modes;

    gfx->bgConfig = BgConfig_Alloc(heapID);
    modes = ov43_0222EDC8;
    SetBothScreensModesAndDisable(&modes);
    template = ov43_0222EFFC;
    i = 0;
    do {
        InitBgFromTemplate(gfx->bgConfig, i, template, 0);
        BG_ClearCharDataRange(i, 0x20, 0, heapID);
        BgClearTilemapBufferAndCommit(gfx->bgConfig, i);
        i++;
        template ++;
    } while (i < 7);
    GfGfxLoader_GXLoadPalFromOpenNarc(gfx->narc, 5, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x160, heapID);
    GfGfxLoader_GXLoadPalFromOpenNarc(gfx->narc, 4, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0xa0, heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(gfx->narc, 7, gfx->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, 1, heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(gfx->narc, 7, gfx->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 1, heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(gfx->narc, 6, gfx->bgConfig, GF_BG_LYR_SUB_2, 0, 0, 1, heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(gfx->narc, 10, gfx->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, 1, heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(gfx->narc, 8, gfx->bgConfig, GF_BG_LYR_SUB_2, 0, 0, 1, heapID);
}
