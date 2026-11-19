#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "error_handling.h"
#include "font.h"
#include "gf_gfx_planes.h"

typedef enum HeapID HeapID;

typedef enum GFPalSlotOffset GFPalSlotOffset;

extern WindowTemplate ov96_0221C2A0;

void ov96_021F7878(Window *arg0, BgConfig *arg1, enum HeapID arg2);

void ov96_021F7878(Window *arg0, BgConfig *arg1, enum HeapID arg2) {
    if (arg1 == NULL) {
        GF_AssertFail();
    }
    AddWindow(arg1, arg0, &ov96_0221C2A0);
    BG_FillCharDataRange(arg1, GF_BG_LYR_SUB_0_F, 0U, 1U, 0U);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, arg2);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BGEXT, (GFPalSlotOffset)(0x4000), (HeapID)((GFPalSlotOffset)(arg2)));
}
