#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "error_handling.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "overlay_96_ov96_digit_work.h"

extern const WindowTemplate ov96_0221D8F8[];

void ov96_02219A08(Ov96DigitWork *work);

void ov96_02219A08(Ov96DigitWork *work) {
    u32 uVar1;

    if (work == 0) {
        GF_AssertFail();
    }
    if (work->bgConfig == 0) {
        GF_AssertFail();
    }
    uVar1 = 0;
    do {
        AddWindow(work->bgConfig, &work->windows[uVar1], &ov96_0221D8F8[uVar1]);
        uVar1 = uVar1 + 1 & 0xff;
    } while (uVar1 < 3);
    BG_FillCharDataRange(work->bgConfig, GF_BG_LYR_SUB_1, 0, 1, 0);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_15_OFFSET, work->heapId);
    return;
}
