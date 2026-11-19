#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "unk_0208C3E4.h"
#include "unk_02088288.h"
#include "pokemon_summary_app.h"

void sub_0208DC68(PokemonSummary *param_1, s32 param_2) {
    if (param_2 == 1) {
        FillWindowPixelBuffer(&param_1->pageWindows[16], 0);
        ReadMsgDataIntoString(param_1->msgData, 0xc1, param_1->buffer);
        AddTextPrinterParameterizedWithColor(&param_1->pageWindows[16], FONT_UNK_4, param_1->buffer, 0, 0, 0xff, 0xe0f00, (PrinterCallback_t)0x0);
        ClearWindowTilemapAndScheduleTransfer(&param_1->pageWindows[12]);
        ScheduleWindowCopyToVram(&param_1->pageWindows[16]);
        return;
    }
    ClearWindowTilemapAndScheduleTransfer(&param_1->pageWindows[16]);
    if (param_1->args->moveToLearn != 0) {
        FillWindowPixelBuffer(&param_1->pageWindows[12], 0);
        sub_0208D884(param_1, 4);
        ScheduleWindowCopyToVram(&param_1->pageWindows[12]);
    }
    return;
}

void sub_0208DD20(PokemonSummary *param_1, s32 param_2) {
    if (param_2 == 1) {
        FillWindowPixelBuffer(&param_1->pageWindows[17], 0);
        ReadMsgDataIntoString(param_1->msgData, 0xc2, param_1->buffer);
        AddTextPrinterParameterizedWithColor(&param_1->pageWindows[17], FONT_UNK_4, param_1->buffer, 0, 0, 0xff, 0xe0f00, (PrinterCallback_t)0x0);
        ScheduleWindowCopyToVram(&param_1->pageWindows[17]);
        return;
    }
    ClearWindowTilemapAndScheduleTransfer(&param_1->pageWindows[17]);
    return;
}
