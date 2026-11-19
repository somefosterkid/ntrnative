#include "global.h"

#include "bg_window.h"
#include "battle/battle_bag_context.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_08_battle_bag.h"
#include "pm_string.h"
#include "string_util.h"
#include "text.h"

void ov08_022237C4(BattleBag *battleBag);

void ov08_022237C4(BattleBag *battleBag) {
    FillWindowPixelBuffer(&battleBag->windows[BATTLE_BAG_POCKET_MENU_WINDOW_PAGE_NUMS], 0);

    Window *window = &battleBag->windows[BATTLE_BAG_POCKET_MENU_WINDOW_PAGE_NUMS];
    String *string = NewString_ReadMsgData(battleBag->messageLoader, 28);
    u32 width = FontID_String_GetWidth(FONT_SYSTEM, string, 0);
    u32 xOffset = (GetWindowWidth(window) * 8 - width) / 2;

    AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, string, xOffset, 4, 0xFF, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(string);

    string = NewString_ReadMsgData(battleBag->messageLoader, 29);
    BufferIntegerAsString(battleBag->stringTemplate, 0, battleBag->numBattlePocketPages[battleBag->currentBattlePocket] + 1, 2, PRINTING_MODE_LEFT_ALIGN, TRUE);
    StringExpandPlaceholders(battleBag->stringTemplate, battleBag->string, string);
    AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, battleBag->string, xOffset + width, 4, 0xFF, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(string);

    string = NewString_ReadMsgData(battleBag->messageLoader, 30);
    BufferIntegerAsString(battleBag->stringTemplate, 0, battleBag->context->pocketCurrentPages[battleBag->currentBattlePocket] + 1, 2, PRINTING_MODE_LEFT_ALIGN, TRUE);
    StringExpandPlaceholders(battleBag->stringTemplate, battleBag->string, string);

    width = FontID_String_GetWidth(FONT_SYSTEM, battleBag->string, 0);
    AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, battleBag->string, xOffset - width, 4, 0xFF, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(string);
    ScheduleWindowCopyToVram(window);
}
