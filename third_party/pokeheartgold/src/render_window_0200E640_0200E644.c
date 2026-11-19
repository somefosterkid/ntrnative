#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "render_window.h"

typedef enum GFPalSlotOffset GFPalSlotOffset;

s32 sub_0200E63C(u32);

void LoadUserFrameGfx2(BgConfig *bgConfig, GFBgLayer layer, u16 baseTile, u8 paletteNum, u8 frame, enum HeapID heapID) {
    s32 memberNo;
    u32 frame_00;
    u32 uVar1;
    frame_00 = (u32)frame;
    memberNo = sub_0200E63C(frame_00);
    uVar1 = GF_BG_LYR_MAIN_CNT;
    GfGfxLoader_LoadCharData(NARC_a_0_3_8, memberNo, bgConfig, layer, (u32)baseTile, 0, 0, heapID);
    if (layer < uVar1) {
        uVar1 = sub_0200E640(frame_00);
        GfGfxLoader_GXLoadPal((NarcId)((GFPalSlotOffset)NARC_a_0_3_8), uVar1, GF_PAL_LOCATION_MAIN_BG, (GFPalSlotOffset)(((u32)paletteNum) << 5), (GFPalSlotOffset)0x20, heapID);
        return;
    }
    uVar1 = sub_0200E640(frame_00);
    GfGfxLoader_GXLoadPal((NarcId)((GFPalSlotOffset)NARC_a_0_3_8), uVar1, GF_PAL_LOCATION_SUB_BG, (GFPalSlotOffset)(((u32)paletteNum) << 5), 0x20, heapID);
    return;
}
