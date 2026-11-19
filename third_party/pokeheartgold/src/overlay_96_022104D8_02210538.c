#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "error_handling.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "overlay_96_app_0221040c.h"

extern WindowTemplate ov96_0221D15C[];

void ov96_02210538(Ov96App_0221040C *param_1) {
    u32 uVar1;

    if (param_1 == 0) {
        GF_AssertFail();
    }
    if (param_1->bgConfig == NULL) {
        GF_AssertFail();
    }
    uVar1 = 0;
    do {
        AddWindow(param_1->bgConfig, &param_1->windows[uVar1], &ov96_0221D15C[uVar1]);
        uVar1 = uVar1 + 1 & 0xff;
    } while (uVar1 < 4);
    BG_FillCharDataRange(param_1->bgConfig, GF_BG_LYR_SUB_3, 0, 1, 0);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_15_OFFSET, param_1->heapId);
    return;
}
