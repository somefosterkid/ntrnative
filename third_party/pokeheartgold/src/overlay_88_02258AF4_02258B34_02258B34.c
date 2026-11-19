#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "system.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_88_wifi_plaza_news.h"
#include "struct_defs/resdat_narc_structs.h"

extern GraphicsModes ov88_02259934;

extern s32 ov88_02259944;

extern BgTemplate ov88_022599C0;

void ov88_02258B34(WiFiPlazaNewsGraphics *graphics, enum HeapID heapID);

void ov88_02258B34(WiFiPlazaNewsGraphics *graphics, enum HeapID heapID) {
    s32 sp10;
    BgTemplate *var_r6;
    s32 *var_r4;
    s32 temp_r0;

    SetBothScreensModesAndDisable(&ov88_02259934);
    graphics->bgConfig = BgConfig_Alloc(heapID);
    gSystem.screensFlipped = 0;
    GfGfx_SwapDisplay();
    var_r6 = &ov88_022599C0;
    var_r4 = &ov88_02259944;
    sp10 = 0;
    do {
        InitBgFromTemplate(graphics->bgConfig, (u8)*var_r4, var_r6, 0U);
        BG_ClearCharDataRange((u8)*var_r4, 0x20U, 0U, heapID);
        BgClearTilemapBufferAndCommit(graphics->bgConfig, (u8)*var_r4);
        var_r6 = (void *)((u8 *)var_r6 + (0x1C));
        var_r4 = (s32 *)((u8 *)var_r4 + 4);
        temp_r0 = sp10 + 1;
        sp10 = temp_r0;
    } while (temp_r0 < 5);
    GfGfxLoader_GXLoadPalFromOpenNarc(graphics->narc, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0U, heapID);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_10_OFFSET, heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(graphics->narc, 1, graphics->bgConfig, GF_BG_LYR_SUB_0_F, 0U, 0U, 0, heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(graphics->narc, 3, graphics->bgConfig, GF_BG_LYR_SUB_0_F, 0U, 0U, 0, heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(graphics->narc, 4, graphics->bgConfig, GF_BG_LYR_SUB_1_F, 0U, 0U, 0, heapID);
    GfGfxLoader_GXLoadPalFromOpenNarc(graphics->narc, 0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0U, heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(graphics->narc, 2, graphics->bgConfig, GF_BG_LYR_SUB_2_F, 0U, 0U, 0, heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(graphics->narc, 6, graphics->bgConfig, GF_BG_LYR_SUB_2_F, 0U, 0U, 0, heapID);
}
