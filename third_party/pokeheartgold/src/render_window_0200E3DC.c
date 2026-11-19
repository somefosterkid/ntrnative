#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"

typedef enum GFPalSlotOffset GFPalSlotOffset;

void LoadUserFrameGfx1(BgConfig *bg_config, GFBgLayer layer, u16 baseTile, u8 palette_num, u8 frame, enum HeapID heapID);

void LoadUserFrameGfx1(BgConfig *bg_config, GFBgLayer layer, u16 baseTile, u8 palette_num, u8 frame, enum HeapID heapID) {
    s32 memberNo;
    GfGfxLoader_LoadCharData(NARC_a_0_3_8, (u32)(frame != 0), bg_config, layer, (u32)baseTile, 0, 0, heapID);
    if (frame == 2) {
        memberNo = 0x2e;
    } else {
        memberNo = 0x19;
    }
    if (((unsigned int)layer) < GF_BG_LYR_MAIN_CNT) {
        GfGfxLoader_GXLoadPal((NarcId)((GFPalSlotOffset)NARC_a_0_3_8), memberNo, GF_PAL_LOCATION_MAIN_BG, (GFPalSlotOffset)(((u32)palette_num) << 5), (GFPalSlotOffset)0x20, heapID);
        return;
    }
    GfGfxLoader_GXLoadPal((NarcId)((GFPalSlotOffset)NARC_a_0_3_8), memberNo, GF_PAL_LOCATION_SUB_BG, (GFPalSlotOffset)(((u32)palette_num) << 5), 0x20, heapID);
    return;
}
