#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "list_menu_items.h"
#include "message_format.h"
#include "move.h"
#include "pm_string.h"
#include "string_util.h"
#include "text.h"
#include "overlay_68_move_relearner.h"

void ov68_021E66A0(MoveRelearner *relearner, u8 x, u8 y, u8 width, u8 height, u8 srcX, u8 srcY);

void ov68_021E66F0(MoveRelearner *param_1, s32 param_2, s8 param_3);

void ov68_021E66F0(MoveRelearner *param_1, s32 param_2, s8 param_3) {
    u8 bVar1;
    u32 y;
    s32 iVar2;
    y = param_3 * 0x20;
    if ((param_2 + param_3) >= ((s32)((u32)(param_1->numMoves)))) {
        ov68_021E66A0(param_1, 0, (((s8)param_3) * 4) + 4, 0x10, 4, 0, 4);
        return;
    }
    iVar2 = param_2 + param_3;
    AddTextPrinterParameterizedWithColor(&param_1->windows[10], FONT_SYSTEM, param_1->moveList[iVar2].text, 0, y, 0xff, 0xf0e00, (PrinterCallback_t)0x0);
    AddTextPrinterParameterizedWithColor(&param_1->windows[10], FONT_SYSTEM, param_1->ppLabel, 0x10, y + 0x10, 0xff, 0x10200, (PrinterCallback_t)0x0);
    bVar1 = GetMoveMaxPP((u16)param_1->moveList[iVar2].value, 0);
    BufferIntegerAsString(param_1->msgFormat, 0, (u32)bVar1, 2, PRINTING_MODE_RIGHT_ALIGN, 1);
    BufferIntegerAsString(param_1->msgFormat, 1, (u32)bVar1, 2, PRINTING_MODE_LEFT_ALIGN, 1);
    StringExpandPlaceholders(param_1->msgFormat, param_1->string, param_1->ppFormat);
    AddTextPrinterParameterizedWithColor(&param_1->windows[10], FONT_SYSTEM, param_1->string, 0x2d, y + 0x10, 0xff, 0x10200, (PrinterCallback_t)0x0);
}
