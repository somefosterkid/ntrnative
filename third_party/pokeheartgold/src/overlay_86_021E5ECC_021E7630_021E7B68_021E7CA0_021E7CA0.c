#include "global.h"

#include "bg_window.h"
#include "overlay_86_frontier_records_app.h"

void ov86_021E7CA0(FrontierRecordsApp *arg0) {
    s32 temp_r5;
    s32 new_var;
    FillWindowPixelBuffer(&arg0->windows[21], 0U);
    new_var = GetWindowWidth(&arg0->windows[21]);
    temp_r5 = new_var * 8;
    ov86_021E6024(arg0, 0x15, (arg0->category) + 0x38, temp_r5 / 2, 0, FONT_UNK_4, 0xF0100, 2);
    ScheduleWindowCopyToVram(&arg0->windows[21]);
}
