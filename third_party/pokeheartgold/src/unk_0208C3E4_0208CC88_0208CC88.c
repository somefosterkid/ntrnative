#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "msgdata.h"
#include "pm_string.h"
#include "pokedex_util.h"
#include "text.h"
#include "unk_0208C3E4.h"
#include "unk_02088288.h"
#include "pokemon_summary_app.h"

void sub_0208CC88(PokemonSummary *param_1) {
    u32 uVar1;

    ScheduleWindowCopyToVram(&param_1->windows[23]);
    if (param_1->mon.isEgg != 0) {
        ClearWindowTilemapAndScheduleTransfer(param_1->windows);
        ClearWindowTilemapAndScheduleTransfer(&param_1->windows[7]);
        ClearWindowTilemapAndScheduleTransfer(&param_1->windows[8]);
        ClearWindowTilemapAndScheduleTransfer(&param_1->windows[9]);
        ClearWindowTilemapAndScheduleTransfer(&param_1->windows[10]);
        ClearWindowTilemapAndScheduleTransfer(&param_1->windows[11]);
        ClearWindowTilemapAndScheduleTransfer(&param_1->windows[12]);
        ClearWindowTilemapAndScheduleTransfer(&param_1->windows[13]);
        ClearWindowTilemapAndScheduleTransfer(&param_1->windows[14]);
        ClearWindowTilemapAndScheduleTransfer(param_1->pageWindows);
        ClearWindowTilemapAndScheduleTransfer(&param_1->pageWindows[1]);
        ClearWindowTilemapAndScheduleTransfer(&param_1->pageWindows[2]);
        ClearWindowTilemapAndScheduleTransfer(&param_1->pageWindows[3]);
        ClearWindowTilemapAndScheduleTransfer(&param_1->pageWindows[4]);
        ClearWindowTilemapAndScheduleTransfer(&param_1->pageWindows[5]);
        return;
    }
    ScheduleWindowCopyToVram(param_1->windows);
    ScheduleWindowCopyToVram(&param_1->windows[7]);
    ScheduleWindowCopyToVram(&param_1->windows[8]);
    ScheduleWindowCopyToVram(&param_1->windows[9]);
    ScheduleWindowCopyToVram(&param_1->windows[10]);
    ScheduleWindowCopyToVram(&param_1->windows[11]);
    ScheduleWindowCopyToVram(&param_1->windows[12]);
    ScheduleWindowCopyToVram(&param_1->windows[13]);
    ScheduleWindowCopyToVram(&param_1->windows[14]);
    FillWindowPixelBuffer(param_1->pageWindows, 0);
    FillWindowPixelBuffer(&param_1->pageWindows[1], 0);
    FillWindowPixelBuffer(&param_1->pageWindows[2], 0);
    FillWindowPixelBuffer(&param_1->pageWindows[3], 0);
    FillWindowPixelBuffer(&param_1->pageWindows[4], 0);
    FillWindowPixelBuffer(&param_1->pageWindows[5], 0);
    uVar1 = Pokedex_ConvertToCurrentDexNo(param_1->args->natDexEnabled, param_1->mon.species);
    if (uVar1 != 0) {
        sub_0208C87C(param_1, 9, uVar1, 3, 2);
    } else {
        ReadMsgDataIntoString(param_1->msgData, 0x16, param_1->buffer);
    }
    if (param_1->mon.shiny == 0) {
        sub_0208C778(param_1, param_1->pageWindows, 0x10200, 2);
    } else {
        sub_0208C778(param_1, param_1->pageWindows, 0x50600, 2);
    }
    uVar1 = FontID_String_GetCenterAlignmentX(FONT_SYSTEM, param_1->mon.speciesName, 0, 0x48);
    AddTextPrinterParameterizedWithColor(&param_1->pageWindows[1], FONT_SYSTEM, param_1->mon.speciesName, uVar1, 0, 0xff, 0x10200, (PrinterCallback_t)0x0);
    if (param_1->mon.otGender == 0) {
        uVar1 = FontID_String_GetCenterAlignmentX(FONT_SYSTEM, param_1->mon.otName, 0, 0x48);
        AddTextPrinterParameterizedWithColor(&param_1->pageWindows[2], FONT_SYSTEM, param_1->mon.otName, uVar1, 0, 0xff, 0x30400, (PrinterCallback_t)0x0);
    } else {
        uVar1 = FontID_String_GetCenterAlignmentX(FONT_SYSTEM, param_1->mon.otName, 0, 0x48);
        AddTextPrinterParameterizedWithColor(&param_1->pageWindows[2], FONT_SYSTEM, param_1->mon.otName, uVar1, 0, 0xff, 0x50600, (PrinterCallback_t)0x0);
    }
    sub_0208C87C(param_1, 0x10, param_1->mon.otId & 0xffff, 5, 2);
    sub_0208C778(param_1, &param_1->pageWindows[3], 0x10200, 2);
    sub_0208C87C(param_1, 0x12, param_1->mon.exp, 7, 0);
    sub_0208C778(param_1, &param_1->pageWindows[4], 0x10200, 1);
    if (param_1->mon.level < 100) {
        sub_0208C87C(param_1, 0x15, param_1->mon.nextLevelExp - param_1->mon.exp, 7, 0);
    } else {
        sub_0208C87C(param_1, 0x15, 0, 7, 0);
    }
    sub_0208C778(param_1, &param_1->pageWindows[5], 0x10200, 1);
    ScheduleWindowCopyToVram(param_1->pageWindows);
    ScheduleWindowCopyToVram(&param_1->pageWindows[1]);
    ScheduleWindowCopyToVram(&param_1->pageWindows[2]);
    ScheduleWindowCopyToVram(&param_1->pageWindows[3]);
    ScheduleWindowCopyToVram(&param_1->pageWindows[4]);
    ScheduleWindowCopyToVram(&param_1->pageWindows[5]);
    return;
}
