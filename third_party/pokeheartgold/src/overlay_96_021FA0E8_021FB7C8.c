#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "overlay_96_unk_struct_ov96_021_f94a8.h"

extern WindowTemplate ov96_0221C3FC;

void ov96_021FB7C8(UnkStruct_ov96_021F94A8 *arg0);

void ov96_021FB7C8(UnkStruct_ov96_021F94A8 *arg0) {
    AddWindow(arg0->bgConfig, &arg0->window, &ov96_0221C3FC);
    BG_FillCharDataRange(arg0->bgConfig, GF_BG_LYR_MAIN_3, 0U, 1U, 0U);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, arg0->heapID);
    FillWindowPixelBuffer(&arg0->window, 0U);
}
