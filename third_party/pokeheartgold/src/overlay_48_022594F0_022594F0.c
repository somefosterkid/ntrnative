#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "options.h"
#include "render_window.h"
#include "system.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_48_unk_struct_ov69_0225_d35_c.h"
#include "struct_defs/resdat_narc_structs.h"
#include "overlay_41_sprite_resource_collection.h"

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

struct G3DPipelineBuffers {
    enum HeapID heapID;
    void *paletteBuffer;
    void *textureBuffer;
};

extern GraphicsModes ov48_0225B190;

extern s32 ov48_0225B1B0[];

extern BgTemplate ov48_0225B2A4[];

void ov48_022594F0(UnkStruct_ov69_0225D35C *param0, Options *options, u32 heapID);

void ov48_022594F0(UnkStruct_ov69_0225D35C *param0, Options *options, u32 heapID) {
    int i;
    u8 temp_r4;

    SetBothScreensModesAndDisable(&ov48_0225B190);
    param0->unk_00 = BgConfig_Alloc((enum HeapID)heapID);
    gSystem.screensFlipped = 1;
    GfGfx_SwapDisplay();
    for (i = 0; i < 5; i++) {
        InitBgFromTemplate(param0->unk_00, (u8)ov48_0225B1B0[i], &ov48_0225B2A4[i], 0);
        BG_ClearCharDataRange((u8)ov48_0225B1B0[i], 0x20, 0, (enum HeapID)heapID);
        BgClearTilemapBufferAndCommit(param0->unk_00, (u8)ov48_0225B1B0[i]);
    }
    GfGfxLoader_GXLoadPalFromOpenNarc(param0->unk_1A8, 5, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0U, (enum HeapID)heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(param0->unk_1A8, 0xB, param0->unk_00, GF_BG_LYR_SUB_2_F, 0U, 0U, 0, (enum HeapID)heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(param0->unk_1A8, 0xC, param0->unk_00, GF_BG_LYR_SUB_2_F, 0U, 0U, 0, (enum HeapID)heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(param0->unk_1A8, 4, param0->unk_00, GF_BG_LYR_SUB_2, 0U, 0U, 0, (enum HeapID)heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(param0->unk_1A8, 0x12, param0->unk_00, GF_BG_LYR_SUB_3, 0U, 0U, 0, (enum HeapID)heapID);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_1_OFFSET, (enum HeapID)heapID);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_11_OFFSET, (enum HeapID)heapID);
    LoadUserFrameGfx1(param0->unk_00, GF_BG_LYR_SUB_0_F, 0x1FU, 0U, 0U, (enum HeapID)heapID);
    LoadUserFrameGfx1(param0->unk_00, GF_BG_LYR_SUB_0_F, 0x1FU, 0U, 0U, (enum HeapID)heapID);
    temp_r4 = (u8)Options_GetFrame(options);
    LoadUserFrameGfx2(param0->unk_00, GF_BG_LYR_SUB_2_F, 0xAU, 0xAU, (u8)temp_r4, (enum HeapID)heapID);
    LoadUserFrameGfx2(param0->unk_00, GF_BG_LYR_SUB_0_F, 1U, 2U, (u8)temp_r4, (enum HeapID)heapID);
    BG_SetMaskColor(0U, 0x72CAU);
}
