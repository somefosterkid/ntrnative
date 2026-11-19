#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"

typedef struct UnkStruct_ov96_021EE644 {
    enum HeapID unk_0;
    BgConfig *unk_4;
    s32 unk_8;
    NARC *unk_C;
} UnkStruct_ov96_021EE644;

typedef enum GFPalSlotOffset GFPalSlotOffset;

void ov96_021EE644(UnkStruct_ov96_021EE644 *arg0);

void ov96_021EE644(UnkStruct_ov96_021EE644 *arg0) {
    u8 temp_r4;

    temp_r4 = (u8) arg0->unk_8;
    GfGfxLoader_GXLoadPalFromOpenNarc(arg0->unk_C, 0xB, GF_PAL_LOCATION_SUB_BGEXT, (GFPalSlotOffset)(0x6000), (GFPalSlotOffset)(0U), arg0->unk_0);
    GfGfxLoader_LoadCharDataFromOpenNarc(arg0->unk_C, temp_r4 + 0xC, arg0->unk_4, GF_BG_LYR_SUB_3, 0U, 0U, 1, arg0->unk_0);
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg0->unk_C, temp_r4 + 0x16, arg0->unk_4, GF_BG_LYR_SUB_3, 0U, 0U, 1, arg0->unk_0);
}
