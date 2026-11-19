#include "global.h"

#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"

void ov33_0225D84C(BgConfig **arg0);

void ov33_0225D84C(BgConfig **arg0) {
    NNSG2dScreenData *sp10;
    int new_var;
    NARC *temp_r0;
    void *temp_r6;
    temp_r0 = NARC_New(NARC_a_2_3_7, HEAP_ID_8);
    GfGfxLoader_GXLoadPalFromOpenNarc(temp_r0, 0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0xA0U, HEAP_ID_8);
    GfGfxLoader_LoadCharDataFromOpenNarc(temp_r0, 1, *arg0, GF_BG_LYR_SUB_1, 0U, 0U, 0, HEAP_ID_8);
    new_var = 6;
    GfGfxLoader_LoadScrnDataFromOpenNarc(temp_r0, 9, *arg0, GF_BG_LYR_SUB_2, 0U, 0U, 0, HEAP_ID_8);
    temp_r6 = GfGfxLoader_GetScrnDataFromOpenNarc(temp_r0, 0xA, 0, &sp10, HEAP_ID_8);
    MIi_CpuCopy16(sp10->rawData + new_var * 0x10, arg0 + 14, 0x180U);
    Heap_Free(temp_r6);
    NARC_Delete(temp_r0);
}
