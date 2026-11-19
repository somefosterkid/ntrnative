#include "global.h"

#include "gf_gfx_loader.h"
#include "bg_window.h"
#include "filesystem.h"
#include "heap.h"
#include "overlay_99_pokeathlon_medals.h"

void ov99_021E7DA4(void *arg0);

extern u8 ov99_021EA348;
void ov99_021E7EBC(void **param_1);

void ov99_021E7DA4(void *arg0) {
    GfGfxLoader_GXLoadPalFromOpenNarc(((PokeathlonMedals *)arg0)->narc, 0x2A, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x40U, ((PokeathlonMedals *)arg0)->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(((PokeathlonMedals *)arg0)->narc, 0x2C, *(BgConfig **)arg0, GF_BG_LYR_MAIN_FIRST, 0U, 0U, 1, ((PokeathlonMedals *)arg0)->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(((PokeathlonMedals *)arg0)->narc, 0x2E, *(BgConfig **)arg0, GF_BG_LYR_MAIN_FIRST, 0U, 0U, 1, ((PokeathlonMedals *)arg0)->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(((PokeathlonMedals *)arg0)->narc, 0x2C, *(BgConfig **)arg0, GF_BG_LYR_SUB_1_F, 0U, 0U, 1, ((PokeathlonMedals *)arg0)->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(((PokeathlonMedals *)arg0)->narc, 0x2F, *(BgConfig **)arg0, GF_BG_LYR_SUB_1_F, 0U, 0U, 1, ((PokeathlonMedals *)arg0)->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(((PokeathlonMedals *)arg0)->narc, 0x30, *(BgConfig **)arg0, GF_BG_LYR_MAIN_3, 1U, 0U, 1, ((PokeathlonMedals *)arg0)->heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(((PokeathlonMedals *)arg0)->narc, 0x2B, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x40U, ((PokeathlonMedals *)arg0)->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(((PokeathlonMedals *)arg0)->narc, 0x2D, *(BgConfig **)arg0, GF_BG_LYR_SUB_2_F, 0U, 0U, 1, ((PokeathlonMedals *)arg0)->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(((PokeathlonMedals *)arg0)->narc, 0x31, *(BgConfig **)arg0, GF_BG_LYR_SUB_2_F, 0U, 0U, 1, ((PokeathlonMedals *)arg0)->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(((PokeathlonMedals *)arg0)->narc, 0x2D, *(BgConfig **)arg0, GF_BG_LYR_SUB_2, 0U, 0U, 1, ((PokeathlonMedals *)arg0)->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(((PokeathlonMedals *)arg0)->narc, 0x32, *(BgConfig **)arg0, GF_BG_LYR_SUB_2, 0U, 0U, 1, ((PokeathlonMedals *)arg0)->heapId);
}

void ov99_021E7EBC(void **param_1) {
    u32 uVar1;

    NARC_Delete(((PokeathlonMedals *)param_1)->zukanNarc);
    NARC_Delete(((PokeathlonMedals *)param_1)->narc);
    uVar1 = 0;
    do {
        FreeBgTilemapBuffer(((PokeathlonMedals *)param_1)->bgConfig, (&ov99_021EA348)[uVar1]);
        uVar1 = uVar1 + 1 & 0xff;
    } while (uVar1 < 7);
    Heap_Free(((PokeathlonMedals *)param_1)->bgConfig);
    return;
}
