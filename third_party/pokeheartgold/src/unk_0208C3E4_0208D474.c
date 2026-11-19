#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "pokemon_summary_app.h"
#include "unk_0208C3E4.h"
#include "unk_02088288.h"

void sub_0208D474(PokemonSummary *param_1) {
    if (param_1->args->unk12 != 2) {
        ScheduleWindowCopyToVram(&param_1->windows[4]);
    }
    FillWindowPixelBuffer(&param_1->pageWindows[8], 0);
    FillWindowPixelBuffer(&param_1->pageWindows[9], 0);
    FillWindowPixelBuffer(&param_1->pageWindows[10], 0);
    FillWindowPixelBuffer(&param_1->pageWindows[11], 0);
    sub_0208D884(param_1, 0);
    sub_0208D884(param_1, 1);
    sub_0208D884(param_1, 2);
    sub_0208D884(param_1, 3);
    ScheduleWindowCopyToVram(&param_1->pageWindows[8]);
    ScheduleWindowCopyToVram(&param_1->pageWindows[9]);
    ScheduleWindowCopyToVram(&param_1->pageWindows[10]);
    ScheduleWindowCopyToVram(&param_1->pageWindows[11]);
    ScheduleWindowCopyToVram(&param_1->windows[23]);
    return;
}

void sub_0208D520(PokemonSummary *param_1) {
    ScheduleWindowCopyToVram(&param_1->windows[5]);
    if (param_1->args->isFlag982Set != 0) {
        FillWindowPixelBuffer(&param_1->pageWindows[3], 0);
        FillWindowPixelBuffer(&param_1->pageWindows[4], 0);
        FillWindowPixelBuffer(&param_1->pageWindows[5], 0);
        FillWindowPixelBuffer(&param_1->pageWindows[6], 0);
        FillWindowPixelBuffer(&param_1->pageWindows[7], 0);
        ReadMsgDataIntoString(param_1->msgData, 0xbb, param_1->buffer);
        AddTextPrinterParameterizedWithColor(&param_1->pageWindows[3], FONT_SYSTEM, param_1->buffer, 4, 0, 0xff, 0x10200, (PrinterCallback_t)0x0);
        ReadMsgDataIntoString(param_1->msgData, 0xbc, param_1->buffer);
        AddTextPrinterParameterizedWithColor(&param_1->pageWindows[4], FONT_SYSTEM, param_1->buffer, 4, 0, 0xff, 0x10200, (PrinterCallback_t)0x0);
        ReadMsgDataIntoString(param_1->msgData, 0xbd, param_1->buffer);
        AddTextPrinterParameterizedWithColor(&param_1->pageWindows[5], FONT_SYSTEM, param_1->buffer, 4, 0, 0xff, 0x10200, (PrinterCallback_t)0x0);
        ReadMsgDataIntoString(param_1->msgData, 0xbe, param_1->buffer);
        AddTextPrinterParameterizedWithColor(&param_1->pageWindows[6], FONT_SYSTEM, param_1->buffer, 4, 0, 0xff, 0x10200, (PrinterCallback_t)0x0);
        ReadMsgDataIntoString(param_1->msgData, 0xbf, param_1->buffer);
        AddTextPrinterParameterizedWithColor(&param_1->pageWindows[7], FONT_SYSTEM, param_1->buffer, 4, 0, 0xff, 0x10200, (PrinterCallback_t)0x0);
        ScheduleWindowCopyToVram(&param_1->pageWindows[3]);
        ScheduleWindowCopyToVram(&param_1->pageWindows[4]);
        ScheduleWindowCopyToVram(&param_1->pageWindows[5]);
        ScheduleWindowCopyToVram(&param_1->pageWindows[6]);
        ScheduleWindowCopyToVram(&param_1->pageWindows[7]);
    }
    return;
}

void sub_0208D6B8(PokemonSummary *arg0) {
    ScheduleWindowCopyToVram(&arg0->windows[30]);
    ScheduleWindowCopyToVram(&arg0->windows[29]);
    ScheduleWindowCopyToVram(&arg0->windows[23]);
    FillWindowPixelBuffer(arg0->pageWindows, 0U);
    sub_0208C87C(arg0, 0xB7, arg0->ribbonCount, 3, 0);
    sub_0208C778(arg0, arg0->pageWindows, 0x10200, 0);
    ScheduleWindowCopyToVram(arg0->pageWindows);
    BgCommitTilemapBufferToVram(arg0->bgConfig, 4U);
}
