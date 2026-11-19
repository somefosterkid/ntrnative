#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "move.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "unk_0208C3E4.h"
#include "unk_02088288.h"
#include "pokemon_summary_app.h"

u32 sub_0208D884(PokemonSummary *param_1, s32 param_2) {
    u16 moveId;
    Window *new_var;
    u8 uVar2;
    u8 uVar1;
    u8 bVar3;
    u32 uVar4;
    u32 uVar5;
    u32 uStack_18;
    new_var = &param_1->pageWindows[param_2 + 8];
    if (param_2 != 4) {
        moveId = param_1->mon.moves[param_2];
        uVar2 = param_1->mon.pp[param_2];
        uVar1 = param_1->mon.maxPP[param_2];
    } else {
        moveId = param_1->args->moveToLearn;
        uVar1 = (uVar2 = GetMoveMaxPP(moveId, 0));
    }
    uStack_18 = (u32)moveId;
    ReadMsgDataIntoString(param_1->moveNames, uStack_18, param_1->buffer);
    AddTextPrinterParameterizedWithColor(new_var, FONT_SYSTEM, param_1->buffer, 1, 2, 0xff, 0x10200, (PrinterCallback_t)0x0);
    if (uStack_18 != 0) {
        ReadMsgDataIntoString(param_1->msgData, 0x87, param_1->buffer);
        AddTextPrinterParameterizedWithColor(new_var, FONT_SYSTEM, param_1->buffer, 0x10, 0x10, 0xff, 0x10200, (PrinterCallback_t)0x0);
        uVar4 = sub_0208C8C8(param_1, param_2 + 8, 0x75, param_2 + 0x88, param_2 + 0x8d, uVar2, uVar1, 2, 0x3c, 0x10);
        return uVar4;
    }
    ReadMsgDataIntoString(param_1->msgData, 0x99, param_1->buffer);
    uVar5 = FontID_String_GetWidth(FONT_SYSTEM, param_1->buffer, 0);
    bVar3 = AddTextPrinterParameterizedWithColor(new_var, FONT_SYSTEM, param_1->buffer, 0x3c - (uVar5 >> 1), 0x10, 0xff, 0x10200, (PrinterCallback_t)0x0);
    return (u32)bVar3;
}
