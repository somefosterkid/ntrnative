#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "overlay_28.h"
#include "pm_string.h"
#include "text.h"

extern WindowTemplate ov28_0225EB52;

extern u8 ov28_0225EB7C[];

void ov28_0225D7E0(DowsingMchnApp *app) {
    u32 sp10;
    Window *var_r4;
    WindowTemplate *var_r6;
    s32 temp_r0;
    s32 i;

    FontID_Alloc(FONT_UNK_4, HEAP_ID_8);
    sp10 = 0;
    i = 0;
    var_r6 = &ov28_0225EB52;
    var_r4 = app->windows;
    do {
        AddWindow(app->bgConfig, var_r4, var_r6);
        FillWindowPixelBuffer(var_r4, 0U);
        AddTextPrinterParameterizedWithColor(var_r4, FONT_UNK_4, app->strings[i], 0U, 0U, 0xFFU, 0xF0100U, NULL);
        CopyWindowPixelsToVram_TextMode(var_r4);
        var_r6++;
        temp_r0 = sp10 + 1;
        var_r4++;
        i++;
        sp10 = temp_r0;
    } while (temp_r0 < 5U);
    ScheduleWindowCopyToVram(&app->windows[ov28_0225EB7C[app->windowSel]]);
    ScheduleWindowCopyToVram(&app->windows[4]);
}
