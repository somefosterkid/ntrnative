#include "global.h"

#include "gf_gfx_loader.h"

typedef struct UnkStruct_ov96_021ED158 {
    enum HeapID unk_0;
    u8 unk_4[0x178];
    s32 unk_17C;
} UnkStruct_ov96_021ED158;

void ov96_021ED158(UnkStruct_ov96_021ED158 *arg0, s32 arg1);

void ov96_021ED158(UnkStruct_ov96_021ED158 *arg0, s32 arg1) {
    arg0->unk_17C = arg1;
    GfGfxLoader_GXLoadPal(NARC_a_1_5_4, 0xC, GF_PAL_LOCATION_MAIN_OBJ, GF_PAL_SLOT_11_OFFSET, 0x20U, arg0->unk_0);
}
