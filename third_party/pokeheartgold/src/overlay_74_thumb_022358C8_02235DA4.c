#include "global.h"

#include "gf_gfx_loader.h"

void ov74_02235DA4(void *arg0);

void ov74_02235DA4(void *arg0) {
    GfGfxLoader_GXLoadPal(NARC_a_1_1_3, 0x21, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_8_OFFSET, 0xC0U, *(enum HeapID *)((u8 *)arg0 + 8));
}
