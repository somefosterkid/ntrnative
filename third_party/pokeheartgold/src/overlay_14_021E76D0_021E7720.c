#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "overlay_14_box_funcs.h"

typedef enum GFPalSlotOffset GFPalSlotOffset;

void ov14_021E7720(void *param_1, s32 param_2, s32 param_3) {
    GfGfxLoader_GXLoadPal(NARC_a_0_1_9, param_2 + 0x28, GF_PAL_LOCATION_MAIN_BG, (GFPalSlotOffset)(param_3 << 5), (GFPalSlotOffset)(0x20), HEAP_ID_10);
    return;
}
