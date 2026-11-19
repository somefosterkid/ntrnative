#include "global.h"
struct UnkStruct_ov96_021EF610;

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "overlay_96_unk_struct_ov96_021_ef610.h"

typedef enum GFPalSlotOffset GFPalSlotOffset;

void ov96_021EE644(enum HeapID *param_1);

void ov96_021EF3A8(struct UnkStruct_ov96_021EF610 *param_1);

void ov96_021EF3A8(UnkStruct_ov96_021EF610 *param_1) {
    LoadFontPal0(GF_PAL_LOCATION_SUB_BGEXT, (GFPalSlotOffset)(0x4000), *((enum HeapID *)&param_1->unk_C));
    ov96_021EE644(*((enum HeapID * *)&param_1->unk_34));
    GfGfxLoader_GXLoadPalFromOpenNarc(*((NARC * *)&param_1->unk_8), 4, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, *((enum HeapID *)&param_1->unk_C));
    GfGfxLoader_LoadCharDataFromOpenNarc(*((NARC * *)&param_1->unk_8), 5, *((BgConfig * *)&param_1->unk_4), GF_BG_LYR_MAIN_CNT, 0, 0, 1, *((enum HeapID *)&param_1->unk_C));
    if (*((s32 *)&param_1->unk_28) != 0) {
        GfGfxLoader_LoadScrnDataFromOpenNarc(*((NARC * *)&param_1->unk_8), 10, *((BgConfig * *)&param_1->unk_4), GF_BG_LYR_MAIN_CNT, 0, 0, 1, *((enum HeapID *)&param_1->unk_C));
        return;
    }
    GfGfxLoader_LoadScrnDataFromOpenNarc(*((NARC * *)&param_1->unk_8), 9, *((BgConfig * *)&param_1->unk_4), GF_BG_LYR_MAIN_CNT, 0, 0, 1, *((enum HeapID *)&param_1->unk_C));
    return;
}
