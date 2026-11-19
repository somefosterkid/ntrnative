#include "global.h"
struct PokeathlonEventRecord;

#include "gf_gfx_loader.h"
#include "overlay_99_pokeathlon_event_record.h"

void ov99_021E860C(struct PokeathlonEventRecord *arg0);

void ov99_021E860C(PokeathlonEventRecord *arg0) {
    GfGfxLoader_GXLoadPalFromOpenNarc(arg0->narc, 0x33, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0U, *((enum HeapID *)&arg0->heapId));
    GfGfxLoader_LoadCharDataFromOpenNarc(arg0->narc, 0x34, arg0->bgConfig, GF_BG_LYR_MAIN_FIRST, 0U, 0U, 1, *((enum HeapID *)&arg0->heapId));
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg0->narc, 0x35, arg0->bgConfig, GF_BG_LYR_MAIN_FIRST, 0U, 0U, 1, *((enum HeapID *)&arg0->heapId));
    GfGfxLoader_LoadCharDataFromOpenNarc(arg0->narc, 0x37, arg0->bgConfig, GF_BG_LYR_SUB_1_F, 1U, 0U, 1, *((enum HeapID *)&arg0->heapId));
    GfGfxLoader_GXLoadPalFromOpenNarc(arg0->narc, 0x33, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0U, *((enum HeapID *)&arg0->heapId));
    GfGfxLoader_LoadCharDataFromOpenNarc(arg0->narc, 0x34, arg0->bgConfig, GF_BG_LYR_SUB_2_F, 0U, 0U, 1, *((enum HeapID *)&arg0->heapId));
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg0->narc, 0x36, arg0->bgConfig, GF_BG_LYR_SUB_2_F, 0U, 0U, 1, *((enum HeapID *)&arg0->heapId));
    GfGfxLoader_LoadCharDataFromOpenNarc(arg0->narc, 0x37, arg0->bgConfig, GF_BG_LYR_SUB_2, 1U, 0U, 1, *((enum HeapID *)&arg0->heapId));
}
