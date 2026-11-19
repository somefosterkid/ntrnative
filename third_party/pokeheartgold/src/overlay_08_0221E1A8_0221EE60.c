#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "struct_defs/resdat_narc_structs.h"
#include "string_util.h"
#include "font.h"
#include "battle/battle.h"
#include "overlay_41_sprite_resource_collection.h"
#include "battle/battle_party_context.h"

struct SysTask;

struct SysTaskQueue;

void ScheduleWindowCopyToVram(Window *window);

u8 GetWindowWidth(Window *window);

enum CharsetMode {

    CHARSET_MODE_JP,

    CHARSET_MODE_EN,
};

void String_Delete(String *string);

typedef u32 TextColor;

u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontID, const String *string, u32 xOffset, u32 yOffset, u32 renderDelay, TextColor color, PrinterCallback_t callback);

void ov08_0221EE60(BattleParty *battleParty, u32 windowIndex, u32 movePowerStat);

void ov08_0221EE60(BattleParty *battleParty, u32 windowIndex, u32 movePowerStat) {
    Window *window = &battleParty->windows[windowIndex];

    if (movePowerStat <= 1) {
        String *string = NewString_ReadMsgData(battleParty->messageLoader, 50);
        u16 stringWidth = FontID_String_GetWidth(FONT_SYSTEM, string, 0);
        u16 xOffset = GetWindowWidth(window) * 8 - stringWidth;

        AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, string, xOffset, 0, 0xFF, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
        String_Delete(string);
    } else {
        String *string = NewString_ReadMsgData(battleParty->messageLoader, 49);
        String *formattedString = String_New(8, battleParty->context->heapID);
        u16 stringWidth;
        u16 xOffset;

        BufferIntegerAsString(battleParty->stringTemplate, 0, movePowerStat, 3, PRINTING_MODE_LEFT_ALIGN, CHARSET_MODE_EN);
        StringExpandPlaceholders(battleParty->stringTemplate, formattedString, string);

        stringWidth = FontID_String_GetWidth(FONT_SYSTEM, formattedString, 0);
        xOffset = GetWindowWidth(window) * 8 - stringWidth;

        AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, formattedString, xOffset, 0, 0xFF, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
        String_Delete(string);
        String_Delete(formattedString);
    }

    ScheduleWindowCopyToVram(window);
}
