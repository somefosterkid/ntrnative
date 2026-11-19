#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_planes.h"

extern u8 ov96_0221C028[];

void ov96_021F4558(enum HeapID *param_1);

void ov96_021F4558(enum HeapID *param_1) {
    u32 uVar1;

    uVar1 = 0;
    do {
        AddWindow((BgConfig *)param_1[2], (Window *)(param_1 + 3 + uVar1 * 4), (WindowTemplate *)(uVar1 * 8 + ((u8 *)ov96_0221C028)));
        uVar1 = uVar1 + 1 & 0xff;
    } while (uVar1 < 5);
    BG_FillCharDataRange((BgConfig *)param_1[2], GF_BG_LYR_MAIN_CNT, 0, 1, 0);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_15_OFFSET, *param_1);
    return;
}
