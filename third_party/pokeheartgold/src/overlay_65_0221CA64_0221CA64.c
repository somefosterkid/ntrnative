#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "sprite.h"

extern const u32 ov65_0221FEEC[];

void ov65_0221CA64(u8 *data, int idx, int pos);

void ov65_0221CA64(u8 *data, int idx, int pos) {
    Sprite **sprites = (Sprite **)(data + 0x40C);
    Sprite_SetAnimCtrlSeq(sprites[idx], pos + 7);
    if (idx == 2) {
        GfGfxLoader_GXLoadPal(NARC_a_1_6_2, ov65_0221FEEC[pos - 1], GF_PAL_LOCATION_SUB_OBJ, GF_PAL_SLOT_11_OFFSET, 0x20, HEAP_ID_26);
        Sprite_SetPaletteOverride(sprites[idx], 11);
    } else if (idx == 3) {
        GfGfxLoader_GXLoadPal(NARC_a_1_6_2, ov65_0221FEEC[pos - 1], GF_PAL_LOCATION_SUB_OBJ, GF_PAL_SLOT_12_OFFSET, 0x20, HEAP_ID_26);
        Sprite_SetPaletteOverride(sprites[idx], 12);
    }
}
