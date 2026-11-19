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

enum CharsetMode {

    CHARSET_MODE_JP,

    CHARSET_MODE_EN,
};

void String_Delete(String *string);

typedef u32 TextColor;

u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontID, const String *string, u32 xOffset, u32 yOffset, u32 renderDelay, TextColor color, PrinterCallback_t callback);

void ov08_0221F284(BattleParty *battleParty, BattlePartyPokemonMove *move, u32 windowIndex);

void ov08_0221F284(BattleParty *battleParty, BattlePartyPokemonMove *move, u32 windowIndex) {
    Window *window = &battleParty->windows[windowIndex];
    String *formattedString = String_New(6, battleParty->context->heapID);
    String *string = NewString_ReadMsgData(battleParty->messageLoader, 43);
    u32 stringWidth;

    AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, string, 40, 24, 0xFF, ((TextColor)(((15 & 0xFF) << 16) | ((14 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
    String_Delete(string);

    string = NewString_ReadMsgData(battleParty->messageLoader, 46);
    stringWidth = FontID_String_GetWidth(FONT_SYSTEM, string, 0);

    AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, string, 80, 24, 0xFF, ((TextColor)(((15 & 0xFF) << 16) | ((14 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
    String_Delete(string);

    string = NewString_ReadMsgData(battleParty->messageLoader, 45);

    BufferIntegerAsString(battleParty->stringTemplate, 0, move->maxPP, 2, PRINTING_MODE_LEFT_ALIGN, CHARSET_MODE_EN);
    StringExpandPlaceholders(battleParty->stringTemplate, formattedString, string);
    AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, formattedString, 80 + stringWidth, 24, 0xFF, ((TextColor)(((15 & 0xFF) << 16) | ((14 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
    String_Delete(string);

    string = NewString_ReadMsgData(battleParty->messageLoader, 44);

    BufferIntegerAsString(battleParty->stringTemplate, 0, move->currentPP, 2, PRINTING_MODE_LEFT_ALIGN, CHARSET_MODE_EN);
    StringExpandPlaceholders(battleParty->stringTemplate, formattedString, string);

    stringWidth = FontID_String_GetWidth(FONT_SYSTEM, formattedString, 0);

    AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, formattedString, 80 - stringWidth, 24, 0xFF, ((TextColor)(((15 & 0xFF) << 16) | ((14 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
    String_Delete(string);
    String_Delete(formattedString);
    ScheduleWindowCopyToVram(window);
}

void FillWindowPixelRect(Window *window, u8 color, u16 x, u16 y, u16 width, u16 height);

void ov08_0221F3D0(BattleParty *battleParty, BattlePartyPokemonMove *move, u32 windowIndex);

void ov08_0221F3D0(BattleParty *battleParty, BattlePartyPokemonMove *move, u32 windowIndex) {
    Window *window = &battleParty->windows[windowIndex];
    String *formattedString = String_New(6, battleParty->context->heapID);
    String *string = NewString_ReadMsgData(battleParty->messageLoader, 43);
    u32 stringWidth = FontID_String_GetWidth(FONT_SYSTEM, string, 0);

    String_Delete(string);
    FillWindowPixelRect(window, 0, 40 + stringWidth, 24, 80 - (40 + stringWidth), 16);

    string = NewString_ReadMsgData(battleParty->messageLoader, 44);

    BufferIntegerAsString(battleParty->stringTemplate, 0, move->currentPP, 2, PRINTING_MODE_LEFT_ALIGN, CHARSET_MODE_EN);
    StringExpandPlaceholders(battleParty->stringTemplate, formattedString, string);

    stringWidth = FontID_String_GetWidth(FONT_SYSTEM, formattedString, 0);

    AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, formattedString, 80 - stringWidth, 24, 0xFF, ((TextColor)(((15 & 0xFF) << 16) | ((14 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
    String_Delete(string);
    String_Delete(formattedString);
    ScheduleWindowCopyToVram(window);
}
