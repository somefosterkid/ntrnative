#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "error_handling.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "overlay_96_unk_struct_ov96_021_ef610.h"

typedef enum GFPalSlotOffset GFPalSlotOffset;

void ov96_021EF430(void *arg0, s32 arg1);

void ov96_021EF430(void *arg0, s32 arg1) {
    s32 new_var;
    s32 sp10[4];
    s32 var_r4;
    void *var_r5;
    var_r4 = arg1;
    var_r5 = arg0;
    if (var_r4 >= 0xA) {
        GF_AssertFail();
    }
    new_var = 0x1000;
    GfGfxLoader_GXLoadPalFromOpenNarc(*((NARC **)&((UnkStruct_ov96_021EF610 *)var_r5)->unk_8), var_r4 + 0x20, GF_PAL_LOCATION_SUB_BGEXT, (GFPalSlotOffset)0x6000, (GFPalSlotOffset)0U, *((enum HeapID *)&((UnkStruct_ov96_021EF610 *)var_r5)->unk_C));
    GfGfxLoader_LoadCharDataFromOpenNarc(*((NARC **)&((UnkStruct_ov96_021EF610 *)var_r5)->unk_8), var_r4 + 0x2A, *((BgConfig **)&((UnkStruct_ov96_021EF610 *)var_r5)->unk_4), GF_BG_LYR_SUB_3, 0U, 0U, 1, *((enum HeapID *)&((UnkStruct_ov96_021EF610 *)var_r5)->unk_C));
    var_r4 = var_r4 + 0x34;
    GfGfxLoader_LoadScrnDataFromOpenNarc(*((NARC **)&((UnkStruct_ov96_021EF610 *)var_r5)->unk_8), var_r4, *((BgConfig **)&((UnkStruct_ov96_021EF610 *)var_r5)->unk_4), GF_BG_LYR_SUB_3, 0U, 0U, 1, *((enum HeapID *)&((UnkStruct_ov96_021EF610 *)var_r5)->unk_C));
    BG_SetMaskColor(4U, 0U);
    sp10[0] = new_var;
    sp10[1] = 0;
    sp10[2] = 0;
    sp10[3] = new_var;
    OS_WaitVBlankIntr();
    SetBgAffine(*((BgConfig **)&((UnkStruct_ov96_021EF610 *)var_r5)->unk_4), 7U, (MtxFx22 *)sp10, 0, 0);
    Bg_SetTextDimAndAffineParams(*((BgConfig **)&((UnkStruct_ov96_021EF610 *)var_r5)->unk_4), 7U, BG_POS_OP_SET_XSCALE, 0, (MtxFx22 *)sp10, 0, 0);
}
