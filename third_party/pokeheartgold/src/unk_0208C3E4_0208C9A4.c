#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "unk_0208C3E4.h"
#include "unk_02088288.h"
#include "pokemon_summary_app.h"

void sub_0208C9A4(PokemonSummary *param_1) {
    sub_0208C850(param_1, 0, 7, 0xe0f00, 0);
    sub_0208C850(param_1, 1, 0x17, 0xe0f00, 0);
    sub_0208C850(param_1, 2, 0x6d, 0xe0f00, 0);
    sub_0208C850(param_1, 3, 0x7e, 0xe0f00, 0);
    sub_0208C850(param_1, 4, 0x80, 0xe0f00, 0);
    if (param_1->args->isFlag982Set != 0) {
        sub_0208C850(param_1, 5, 0x9d, 0xe0f00, 0);
    } else {
        FillWindowPixelBuffer(&param_1->windows[5], 0);
    }
    sub_0208C850(param_1, 6, 4, 0xe0f00, 0);
    sub_0208C850(param_1, 7, 8, 0xe0f00, 0);
    sub_0208C850(param_1, 8, 10, 0xe0f00, 0);
    sub_0208C850(param_1, 9, 0xc, 0xe0f00, 0);
    sub_0208C850(param_1, 10, 0xd, 0xe0f00, 0);
    sub_0208C850(param_1, 0xb, 0xf, 0xe0f00, 0);
    sub_0208C850(param_1, 0xc, 0x11, 0xe0f00, 0);
    sub_0208C850(param_1, 0xd, 0x13, 0xe0f00, 0);
    sub_0208C850(param_1, 0xf, 0x6e, 0xe0f00, 0);
    sub_0208C7F8(param_1, 0x10, 0x6f, 0, 0);
    sub_0208C7F8(param_1, 0x11, 0x70, 1, 0);
    sub_0208C7F8(param_1, 0x12, 0x71, 3, 0);
    sub_0208C7F8(param_1, 0x13, 0x72, 4, 0);
    sub_0208C7F8(param_1, 0x14, 0x73, 2, 0);
    ReadMsgDataIntoString(param_1->msgData, 0x74, param_1->buffer);
    AddTextPrinterParameterizedWithColor(&param_1->windows[21], FONT_SYSTEM, param_1->buffer, 3, 0, 0xff, 0xe0f00, (PrinterCallback_t)0x0);
    ReadMsgDataIntoString(param_1->msgData, 0x92, param_1->buffer);
    AddTextPrinterParameterizedWithColor(&param_1->windows[23], FONT_UNK_4, param_1->buffer, 0, 0, 0xff, 0xe0f00, (PrinterCallback_t)0x0);
    sub_0208C850(param_1, 0x18, 0x95, 0xe0f00, 0);
    sub_0208C850(param_1, 0x19, 0x93, 0xe0f00, 0);
    sub_0208C850(param_1, 0x1a, 0x94, 0xe0f00, 0);
    sub_0208C850(param_1, 0x1b, 0xa2, 0xe0f00, 2);
    sub_0208C850(param_1, 0x1c, 0xa0, 0x10200, 2);
    sub_0208C850(param_1, 0x1d, 0xb6, 0x10200, 0);
    sub_0208C850(param_1, 0x1e, 0xb3, 0xe0f00, 0);
    return;
}

void sub_0208CBD4(PokemonSummary *arg0) {
    s8 v0;

    FillBgTilemapRect(arg0->bgConfig, 1U, 0U, 0U, 0U, 0x20U, 2U, 0x10U);
    FillBgTilemapRect(arg0->bgConfig, 1U, 0U, 0U, 2U, 0x12U, 0x16U, 0x10U);
    FillBgTilemapRect(arg0->bgConfig, 4U, 0U, 0U, 1U, 0x12U, 0x17U, 0x10U);
    v0 = arg0->page;
    switch (v0) {
    case 0:
        sub_0208CC88(arg0);
        sub_0208D0A4(arg0);
        return;
    case 1:
        sub_0208D178(arg0);
        sub_0208D474(arg0);
        return;
    case 2:
        sub_0208D520(arg0);
        sub_0208D6B8(arg0);
        return;
    case 3:
        sub_0208D728(arg0);
    default:
        return;
    }
}
