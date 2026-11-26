#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "options.h"
#include "overlay_86_frontier_records_app.h"
#include "render_window.h"

void ov86_021E74F0(FrontierRecordsApp *param_1) {
    u32 uVar1;

    uVar1 = Options_GetFrame(param_1->options);
    LoadUserFrameGfx2(param_1->bgConfig, GF_BG_LYR_MAIN_CNT, 0x3e2, 0xe, (u8)uVar1, HEAP_ID_121);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_121);
    return;
}

void ov86_021E752C(FrontierRecordsApp *param_1) {
    DrawFrameAndWindow2(&param_1->windows[23], 1, 0x3e2, 0xe);
    FillWindowPixelBuffer(&param_1->windows[23], 0xf);
    ov86_021E6024(param_1, 0x17, 0x2a, 0, 0, FONT_MESSAGE, 0x1020f, 0);
    ScheduleWindowCopyToVram(&param_1->windows[23]);
    return;
}

void ov86_021E757C(FrontierRecordsApp *arg0) {
    ClearFrameAndWindow2(&arg0->windows[23], 1);
    ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 4U);
}
