#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_113.h"

void ov113_021E6238(UnownReportData *param_1, UnownReportScreen *param_2, u16 param_3);

u8 ov113_021E6274(Window *param_1, String *param_2, enum Font param_3, u32 param_4, u16 param_5);

void ov113_021E6238(UnownReportData *param_1, UnownReportScreen *param_2, u16 param_3) {
    CopyToBgTilemapRect(param_1->bgConfig, param_3, 0, 0, 0x20, 0x18, param_2->tilemap, 0, 0, param_2->width, param_2->height);
    ScheduleBgTilemapBufferTransfer(param_1->bgConfig, param_3);
    return;
}

u8 ov113_021E6274(Window *param_1, String *param_2, enum Font param_3, u32 param_4, u16 param_5) {
    u8 uVar1;

    uVar1 = AddTextPrinterParameterizedWithColor(param_1, param_3, param_2, param_4, (u32)param_5, 0xff, 0x10200, (PrinterCallback_t)0x0);
    return uVar1;
}
