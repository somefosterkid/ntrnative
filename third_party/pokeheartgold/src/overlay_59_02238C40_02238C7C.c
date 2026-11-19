#include "global.h"
#include "overlay_59_apricorn_app.h"

#include "bg_window.h"
#include "font.h"
#include "pm_string.h"
#include "text.h"

u8 ov59_02239EA8(Ov59ApricornApp *arg0);

void ov59_02239D08(Ov59ApricornApp *, u32);

void ov59_02238C7C(Ov59ApricornApp *param_1);

void ov59_02238C7C(Ov59ApricornApp *param_1) {
    u8 bVar1;
    u32 uVar2;
    Window *new_var;
#if defined(SDK_PORT) || defined(NONMATCHING)
    FillWindowPixelBuffer(&param_1->windows[4], 0xf);
#else
    FillWindowPixelBuffer((Window *)(((s32)param_1) + 0x168), 0xf);
#endif
    bVar1 = ov59_02239EA8(param_1);
    uVar2 = (u32)bVar1;
    if (uVar2 == 8) {
#if defined(SDK_PORT) || defined(NONMATCHING)
        ScheduleWindowCopyToVram(&param_1->windows[4]);
#else
        ScheduleWindowCopyToVram((Window *)(((s32)param_1) + 0x168));
#endif
        return;
    }
    if (uVar2 <= (7 - 1)) {
        if (param_1->apricornCounts[uVar2] == 0) {
#if defined(SDK_PORT) || defined(NONMATCHING)
            ScheduleWindowCopyToVram(&param_1->windows[4]);
#else
            ScheduleWindowCopyToVram((Window *)(((s32)param_1) + 0x168));
#endif
            return;
        }
        ov59_02239D08(param_1, uVar2 + 0xd);
    } else if ((param_1->unk_14) != 0) {
        ov59_02239D08(param_1, 0x15);
    } else {
        ov59_02239D08(param_1, 0x14);
    }
#if defined(SDK_PORT) || defined(NONMATCHING)
    new_var = &param_1->windows[4];
#else
    new_var = (Window *)(((s32)param_1) + 0x168);
#endif
    AddTextPrinterParameterized(new_var, FONT_SYSTEM, param_1->unk_68, 0, 0, 0xff, (PrinterCallback_t)0x0);
    ScheduleWindowCopyToVram(new_var);
    return;
}
