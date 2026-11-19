#include "global.h"
#include "overlay_59_apricorn_app.h"

#include "bg_window.h"
#include "font.h"
#include "pm_string.h"
#include "render_text.h"
#include "render_window.h"
#include "text.h"

s32 ov59_02239D08(Ov59ApricornApp *, s32 arg1);

void ov59_02238CFC(Ov59ApricornApp *arg0, s32 arg1, u32 arg2);

void ov59_02238CFC(Ov59ApricornApp *arg0, s32 arg1, u32 arg2) {
    void *new_var;
    void *new_var2;
    new_var2 = &arg0->windows[0];
    new_var = new_var2;
    FillWindowPixelBuffer(new_var, 0xFU);
    DrawFrameAndWindow2(new_var, 1, 1U, 0xDU);
    ov59_02239D08(arg0, arg1);
    if ((arg2 != 0xFF) && (arg2 != 0)) {
        TextFlags_SetCanTouchSpeedUpPrint(1);
        arg0->printerId = AddTextPrinterParameterized(new_var, FONT_MESSAGE, arg0->unk_68, 0U, 0U, arg2, 0L);
    } else {
        AddTextPrinterParameterized(new_var, FONT_MESSAGE, arg0->unk_68, 0U, 0U, arg2, 0L);
        arg0->printerId = 0xFFU;
    }
    ScheduleWindowCopyToVram(new_var2);
}
