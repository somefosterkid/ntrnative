#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"

void ov49_0225BABC(void *param_1, u32 *param_2, NARC *param_3, s32 param_4, enum HeapID param_5);

void ov49_0225BB10(void);

void ov49_0225BABC(void *param_1, u32 *param_2, NARC *param_3, s32 param_4, enum HeapID param_5) {
    if (param_4 == 0) {
        GfGfxLoader_GXLoadPalFromOpenNarc(param_3, 0x56, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, param_5);
    } else {
        GfGfxLoader_GXLoadPalFromOpenNarc(param_3, 0x57, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, param_5);
    }
    GfGfxLoader_LoadCharDataFromOpenNarc(param_3, 0x36, (BgConfig *)*param_2, GF_BG_LYR_MAIN_CNT, 0, 0, 0, param_5);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_5_OFFSET, param_5);
    return;
}

void ov49_0225BB10(void) {
}
