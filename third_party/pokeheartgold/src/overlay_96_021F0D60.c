#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "overlay_96_ov96_arena_work.h"

s32 ov96_021F3CBC(void *);

void ov96_021F0D60(Ov96ArenaWork *arg0);

void ov96_021F0D60(Ov96ArenaWork *arg0) {
    NNSG2dScreenData *ppScrnData;
    NNSG2dScreenData *v0;
    NNSG2dScreenData *v1;
    void *ptr;

    GfGfxLoader_LoadCharData(NARC_a_1_6_7, 5, arg0->bgConfig, GF_BG_LYR_MAIN_FIRST, 0U, 0U, 0, arg0->heapId);
    GfGfxLoader_LoadCharData(NARC_a_1_6_7, 9, arg0->bgConfig, GF_BG_LYR_SUB_1_F, 0U, 0U, 0, arg0->heapId);
    GfGfxLoader_LoadCharData(NARC_a_1_6_7, 0xC, arg0->bgConfig, GF_BG_LYR_SUB_1, 0U, 0x1000U, 0, arg0->heapId);
    GfGfxLoader_LoadCharData(NARC_a_1_6_7, 0x16, arg0->bgConfig, GF_BG_LYR_SUB_3, 0U, 0U, 0, arg0->heapId);
    GfGfxLoader_LoadCharData(NARC_a_1_6_7, 0xF, arg0->bgConfig, GF_BG_LYR_SUB_2, 0x80U, 0U, 0, arg0->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_1_6_7, 6, arg0->bgConfig, GF_BG_LYR_MAIN_FIRST, 0U, 0U, 0, arg0->heapId);
    ptr = GfGfxLoader_GetScrnData(NARC_a_1_6_7, 0xA, 0, &ppScrnData, arg0->heapId);
    BG_LoadScreenTilemapData(arg0->bgConfig, 2U, ppScrnData->rawData, ppScrnData->szByte);
    BgTilemapRectChangePalette(arg0->bgConfig, 2U, 0U, 0U, 0x20U, 0x18U, 1U);
    ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 2U);
    Heap_Free(ptr);
    arg0->unk_734 = GfGfxLoader_GetScrnData(NARC_a_1_6_7, 7, 0, &arg0->unk_738, arg0->heapId);
    v0 = arg0->unk_738;
    BG_LoadScreenTilemapData(arg0->bgConfig, 1U, v0->rawData, v0->szByte);
    v1 = arg0->unk_738;
    BgCopyOrUncompressTilemapBufferRangeToVram(arg0->bgConfig, 1U, v1->rawData, v1->szByte, 0U);
    ov96_021F3CBC(arg0->unk_774);
    GfGfxLoader_GXLoadPal(NARC_a_1_6_7, 4, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x20U, arg0->heapId);
    GfGfxLoader_GXLoadPal(NARC_a_1_6_7, 8, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_1_OFFSET, 0x20U, arg0->heapId);
    GfGfxLoader_GXLoadPal(NARC_a_1_6_7, 0x15, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x40U, arg0->heapId);
    GfGfxLoader_GXLoadPal(NARC_a_1_6_7, 0xB, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_2_OFFSET, 0x20U, arg0->heapId);
    GfGfxLoader_GXLoadPal(NARC_a_1_6_7, 0xE, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_3_OFFSET, 0x20U, arg0->heapId);
    reg_G2_BG2OFS = 0x800000;
}
