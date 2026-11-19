#include "global.h"
#include "overlay_59_apricorn_app.h"
#include "bg_window.h"
#include "text.h"

s32 ov59_02239D08(Ov59ApricornApp *, s32);

void ov59_02238C40(Ov59ApricornApp *arg0, s32 arg1);

void ov59_02238C40(Ov59ApricornApp *arg0, s32 arg1) {
    void * sum;
    sum = &arg0->windows[1];
    FillWindowPixelBuffer(sum, 0U);
    ov59_02239D08(arg0, arg1);
    AddTextPrinterParameterized(sum, FONT_SYSTEM, arg0->unk_68, 0U, 0U, 0xFFU, NULL);
    ScheduleWindowCopyToVram(sum);
}
