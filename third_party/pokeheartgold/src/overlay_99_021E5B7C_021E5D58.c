#include "global.h"
struct PokeathlonCourseRecord;

#include "gf_gfx_loader.h"
#include "overlay_99_pokeathlon_course_record.h"

void ov99_021E5D58(struct PokeathlonCourseRecord *arg0);

void ov99_021E5D58(PokeathlonCourseRecord *arg0) {
    GfGfxLoader_GXLoadPalFromOpenNarc(arg0->narc, 0x1E, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0U, *((enum HeapID *)&arg0->heapId));
    GfGfxLoader_LoadCharDataFromOpenNarc(arg0->narc, 0x1F, arg0->bgConfig, GF_BG_LYR_SUB_0_F, 0U, 0U, 1, *((enum HeapID *)&arg0->heapId));
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg0->narc, 0x20, arg0->bgConfig, GF_BG_LYR_SUB_0_F, 0U, 0U, 1, *((enum HeapID *)&arg0->heapId));
    GfGfxLoader_LoadCharDataFromOpenNarc(arg0->narc, 0x3E, arg0->bgConfig, GF_BG_LYR_SUB_1_F, 0U, 0U, 1, *((enum HeapID *)&arg0->heapId));
    GfGfxLoader_GXLoadPalFromOpenNarc(arg0->narc, 0x24, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, 0x20U, *((enum HeapID *)&arg0->heapId));
    GfGfxLoader_LoadCharDataFromOpenNarc(arg0->narc, 0x25, arg0->bgConfig, GF_BG_LYR_MAIN_3, 0U, 0U, 1, *((enum HeapID *)&arg0->heapId));
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg0->narc, 0x26, arg0->bgConfig, GF_BG_LYR_MAIN_3, 0U, 0U, 1, *((enum HeapID *)&arg0->heapId));
    GfGfxLoader_GXLoadPalFromOpenNarc(arg0->narc, 0x21, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0U, *((enum HeapID *)&arg0->heapId));
    GfGfxLoader_LoadCharDataFromOpenNarc(arg0->narc, 0x22, arg0->bgConfig, GF_BG_LYR_SUB_2_F, 0U, 0U, 1, *((enum HeapID *)&arg0->heapId));
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg0->narc, 0x23, arg0->bgConfig, GF_BG_LYR_SUB_2_F, 0U, 0U, 1, *((enum HeapID *)&arg0->heapId));
    GfGfxLoader_LoadCharDataFromOpenNarc(arg0->narc, 0x44, arg0->bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 1, *((enum HeapID *)&arg0->heapId));
    GfGfxLoader_GXLoadPalFromOpenNarc(arg0->narc, 0x27, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_15_OFFSET, 0x20U, *((enum HeapID *)&arg0->heapId));
    GfGfxLoader_LoadCharDataFromOpenNarc(arg0->narc, 0x28, arg0->bgConfig, GF_BG_LYR_SUB_3, 0U, 0U, 1, *((enum HeapID *)&arg0->heapId));
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg0->narc, 0x29, arg0->bgConfig, GF_BG_LYR_SUB_3, 0U, 0U, 1, *((enum HeapID *)&arg0->heapId));
}
