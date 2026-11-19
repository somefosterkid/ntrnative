#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_08_battle_party.h"
#include "struct_defs/resdat_narc_structs.h"
#include "string_util.h"
#include "font.h"
#include "battle/battle.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_08_battle_sub_menu_cursor.h"
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

enum BattlePokemonSummaryScreenWindow {
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_POKEMON_NAME = 0,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ABILITY_NAME,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ABILITY_DESC,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_HELD_ITEM,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_HP,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ATTACK,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_DEFENSE,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_SPEED,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_SP_ATK,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_SP_DEF,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_HEALTH_BAR,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_LEVEL,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_EXP_TO_NEXT_LVL,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_HP_LABEL,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ATTACK_LABEL,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_DEFENSE_LABEL,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_SPEED_LABEL,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_SP_ATK_LABEL,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_SP_DEF_LABEL,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_LEVEL_LABEL,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_EXP_TO_NEXT_LVL_LABEL,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_CHECK_MOVES,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ALT_POKEMON_NAME,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ALT_ABILITY_NAME,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ALT_ABILITY_DESC,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ALT_HELD_ITEM,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ALT_HP,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ALT_ATTACK,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ALT_DEFENSE,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ALT_SPEED,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ALT_SP_ATK,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ALT_SP_DEF,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ALT_HEALTH_BAR,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ALT_LEVEL,
    BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ALT_EXP_TO_NEXT_LVL,
};

typedef u32 TextColor;

u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontID, const String *string, u32 xOffset, u32 yOffset, u32 renderDelay, TextColor color, PrinterCallback_t callback);

void ov08_0221E408(BattleParty *battleParty, u32 partyIndex);

void ov08_0221E408(BattleParty *battleParty, u32 partyIndex) {
    BattlePartyPokemon *pokemon = &battleParty->partyPokemon[partyIndex];
    u16 windowIndexOffset = BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ALT_POKEMON_NAME * battleParty->useAltSummaryWindows;
    String *string = NewString_ReadMsgData(battleParty->messageLoader, 23);
    String *formattedString;
    u16 xOffset;

    AddTextPrinterParameterizedWithColor(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_LEVEL_LABEL], FONT_SYSTEM, string, 0, 0, 0xFF, ((TextColor)(((15 & 0xFF) << 16) | ((14 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
    String_Delete(string);

    string = NewString_ReadMsgData(battleParty->messageLoader, 24);
    formattedString = String_New(8, battleParty->context->heapID);

    BufferIntegerAsString(battleParty->stringTemplate, 0, pokemon->level, 3, PRINTING_MODE_LEFT_ALIGN, CHARSET_MODE_EN);
    StringExpandPlaceholders(battleParty->stringTemplate, formattedString, string);
    AddTextPrinterParameterizedWithColor(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_LEVEL + windowIndexOffset], FONT_SYSTEM, formattedString, 0, 0, 0xFF, ((TextColor)(((15 & 0xFF) << 16) | ((14 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
    String_Delete(string);
    String_Delete(formattedString);

    string = NewString_ReadMsgData(battleParty->messageLoader, 25);

    AddTextPrinterParameterizedWithColor(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_EXP_TO_NEXT_LVL_LABEL], FONT_SYSTEM, string, 0, 0, 0xFF, ((TextColor)(((15 & 0xFF) << 16) | ((14 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
    String_Delete(string);

    string = NewString_ReadMsgData(battleParty->messageLoader, 26);
    formattedString = String_New(14, battleParty->context->heapID);

    if (pokemon->level < 100) {
        BufferIntegerAsString(
            battleParty->stringTemplate, 0, pokemon->nextLevelExp - pokemon->exp, 6, PRINTING_MODE_RIGHT_ALIGN, CHARSET_MODE_EN);
    } else {
        BufferIntegerAsString(
            battleParty->stringTemplate, 0, 0, 6, PRINTING_MODE_RIGHT_ALIGN, CHARSET_MODE_EN);
    }

    StringExpandPlaceholders(battleParty->stringTemplate, formattedString, string);

    xOffset = GetWindowWidth(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_EXP_TO_NEXT_LVL + windowIndexOffset]) * 8 - FontID_String_GetWidth(FONT_SYSTEM, formattedString, 0);

    AddTextPrinterParameterizedWithColor(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_EXP_TO_NEXT_LVL + windowIndexOffset], FONT_SYSTEM, formattedString, xOffset, 0, 0xFF, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
    String_Delete(string);
    String_Delete(formattedString);
    ScheduleWindowCopyToVram(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_LEVEL_LABEL]);
    ScheduleWindowCopyToVram(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_LEVEL + windowIndexOffset]);
    ScheduleWindowCopyToVram(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_EXP_TO_NEXT_LVL_LABEL]);
    ScheduleWindowCopyToVram(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_EXP_TO_NEXT_LVL + windowIndexOffset]);
}

void ov08_0221E5DC(BattleParty *battleParty, u32 partyIndex);

void ov08_0221E5DC(BattleParty *battleParty, u32 partyIndex) {
    BattlePartyPokemon *pokemon = &battleParty->partyPokemon[partyIndex];
    u16 windowIndexOffset = BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ALT_POKEMON_NAME * battleParty->useAltSummaryWindows;
    String *string = NewString_ReadMsgData(battleParty->messageLoader, 32);
    String *formattedString;
    u8 stringWidth;
    u8 xOffset;

    AddTextPrinterParameterizedWithColor(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ATTACK_LABEL], FONT_SYSTEM, string, 0, 0, 0xFF, ((TextColor)(((15 & 0xFF) << 16) | ((14 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
    String_Delete(string);

    string = NewString_ReadMsgData(battleParty->messageLoader, 33);
    formattedString = String_New(8, battleParty->context->heapID);

    BufferIntegerAsString(battleParty->stringTemplate, 0, pokemon->attack, 3, PRINTING_MODE_LEFT_ALIGN, CHARSET_MODE_EN);
    StringExpandPlaceholders(battleParty->stringTemplate, formattedString, string);

    stringWidth = FontID_String_GetWidth(FONT_SYSTEM, formattedString, 0);
    xOffset = GetWindowWidth(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ATTACK + windowIndexOffset]) * 8 - stringWidth;

    AddTextPrinterParameterizedWithColor(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ATTACK + windowIndexOffset], FONT_SYSTEM, formattedString, xOffset, 0, 0xFF, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
    String_Delete(string);
    String_Delete(formattedString);
    ScheduleWindowCopyToVram(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ATTACK_LABEL]);
    ScheduleWindowCopyToVram(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ATTACK + windowIndexOffset]);
}

void ov08_0221E6D8(BattleParty *battleParty, u32 partyIndex);

void ov08_0221E6D8(BattleParty *battleParty, u32 partyIndex) {
    BattlePartyPokemon *pokemon = &battleParty->partyPokemon[partyIndex];
    u16 windowIndexOffset = BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ALT_POKEMON_NAME * battleParty->useAltSummaryWindows;
    String *string = NewString_ReadMsgData(battleParty->messageLoader, 34);
    String *formattedString;
    u8 stringWidth;
    u8 xOffset;

    AddTextPrinterParameterizedWithColor(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_DEFENSE_LABEL], FONT_SYSTEM, string, 0, 0, 0xFF, ((TextColor)(((15 & 0xFF) << 16) | ((14 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
    String_Delete(string);

    string = NewString_ReadMsgData(battleParty->messageLoader, 35);
    formattedString = String_New(8, battleParty->context->heapID);

    BufferIntegerAsString(battleParty->stringTemplate, 0, pokemon->defence, 3, PRINTING_MODE_LEFT_ALIGN, CHARSET_MODE_EN);
    StringExpandPlaceholders(battleParty->stringTemplate, formattedString, string);

    stringWidth = FontID_String_GetWidth(FONT_SYSTEM, formattedString, 0);
    xOffset = GetWindowWidth(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_DEFENSE + windowIndexOffset]) * 8 - stringWidth;

    AddTextPrinterParameterizedWithColor(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_DEFENSE + windowIndexOffset], FONT_SYSTEM, formattedString, xOffset, 0, 0xFF, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
    String_Delete(string);
    String_Delete(formattedString);
    ScheduleWindowCopyToVram(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_DEFENSE_LABEL]);
    ScheduleWindowCopyToVram(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_DEFENSE + windowIndexOffset]);
}

void ov08_0221E7D4(BattleParty *battleParty, u32 partyIndex);

void ov08_0221E7D4(BattleParty *battleParty, u32 partyIndex) {
    BattlePartyPokemon *pokemon = &battleParty->partyPokemon[partyIndex];
    u16 windowIndexOffset = BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ALT_POKEMON_NAME * battleParty->useAltSummaryWindows;
    String *string = NewString_ReadMsgData(battleParty->messageLoader, 40);
    String *formattedString;
    u8 stringWidth;
    u8 xOffset;

    AddTextPrinterParameterizedWithColor(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_SPEED_LABEL], FONT_SYSTEM, string, 0, 0, 0xFF, ((TextColor)(((15 & 0xFF) << 16) | ((14 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
    String_Delete(string);

    string = NewString_ReadMsgData(battleParty->messageLoader, 41);
    formattedString = String_New(8, battleParty->context->heapID);

    BufferIntegerAsString(battleParty->stringTemplate, 0, pokemon->speed, 3, PRINTING_MODE_LEFT_ALIGN, CHARSET_MODE_EN);
    StringExpandPlaceholders(battleParty->stringTemplate, formattedString, string);

    stringWidth = FontID_String_GetWidth(FONT_SYSTEM, formattedString, 0);
    xOffset = GetWindowWidth(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_SPEED + windowIndexOffset]) * 8 - stringWidth;

    AddTextPrinterParameterizedWithColor(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_SPEED + windowIndexOffset], FONT_SYSTEM, formattedString, xOffset, 0, 0xFF, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
    String_Delete(string);
    String_Delete(formattedString);
    ScheduleWindowCopyToVram(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_SPEED_LABEL]);
    ScheduleWindowCopyToVram(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_SPEED + windowIndexOffset]);
}

void ov08_0221E8D4(BattleParty *battleParty, u32 partyIndex);

void ov08_0221E8D4(BattleParty *battleParty, u32 partyIndex) {
    BattlePartyPokemon *pokemon = &battleParty->partyPokemon[partyIndex];
    u16 windowIndexOffset = BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ALT_POKEMON_NAME * battleParty->useAltSummaryWindows;
    String *string = NewString_ReadMsgData(battleParty->messageLoader, 36);
    String *formattedString;
    u8 stringWidth;
    u8 xOffset;

    AddTextPrinterParameterizedWithColor(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_SP_ATK_LABEL], FONT_SYSTEM, string, 0, 0, 0xFF, ((TextColor)(((15 & 0xFF) << 16) | ((14 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
    String_Delete(string);

    string = NewString_ReadMsgData(battleParty->messageLoader, 37);
    formattedString = String_New(8, battleParty->context->heapID);

    BufferIntegerAsString(battleParty->stringTemplate, 0, pokemon->spAtk, 3, PRINTING_MODE_LEFT_ALIGN, CHARSET_MODE_EN);
    StringExpandPlaceholders(battleParty->stringTemplate, formattedString, string);

    stringWidth = FontID_String_GetWidth(FONT_SYSTEM, formattedString, 0);
    xOffset = GetWindowWidth(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_SP_ATK + windowIndexOffset]) * 8 - stringWidth;

    AddTextPrinterParameterizedWithColor(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_SP_ATK + windowIndexOffset], FONT_SYSTEM, formattedString, xOffset, 0, 0xFF, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
    String_Delete(string);
    String_Delete(formattedString);
    ScheduleWindowCopyToVram(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_SP_ATK_LABEL]);
    ScheduleWindowCopyToVram(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_SP_ATK + windowIndexOffset]);
}

void ov08_0221E9D4(BattleParty *battleParty, u32 partyIndex);

void ov08_0221E9D4(BattleParty *battleParty, u32 partyIndex) {
    BattlePartyPokemon *pokemon = &battleParty->partyPokemon[partyIndex];
    u16 windowIndexOffset = BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ALT_POKEMON_NAME * battleParty->useAltSummaryWindows;
    String *string = NewString_ReadMsgData(battleParty->messageLoader, 38);
    String *formattedString;
    u8 stringWidth;
    u8 xOffset;

    AddTextPrinterParameterizedWithColor(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_SP_DEF_LABEL], FONT_SYSTEM, string, 0, 0, 0xFF, ((TextColor)(((15 & 0xFF) << 16) | ((14 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
    String_Delete(string);

    string = NewString_ReadMsgData(battleParty->messageLoader, 39);
    formattedString = String_New(8, battleParty->context->heapID);

    BufferIntegerAsString(battleParty->stringTemplate, 0, pokemon->spDef, 3, PRINTING_MODE_LEFT_ALIGN, CHARSET_MODE_EN);
    StringExpandPlaceholders(battleParty->stringTemplate, formattedString, string);

    stringWidth = FontID_String_GetWidth(FONT_SYSTEM, formattedString, 0);
    xOffset = GetWindowWidth(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_SP_DEF + windowIndexOffset]) * 8 - stringWidth;

    AddTextPrinterParameterizedWithColor(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_SP_DEF + windowIndexOffset], FONT_SYSTEM, formattedString, xOffset, 0, 0xFF, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
    String_Delete(string);
    String_Delete(formattedString);
    ScheduleWindowCopyToVram(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_SP_DEF_LABEL]);
    ScheduleWindowCopyToVram(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_SP_DEF + windowIndexOffset]);
}

void ov08_0221EAD4(BattleParty *battleParty, u32 partyIndex);

void ov08_0221EAD4(BattleParty *battleParty, u32 partyIndex) {
    BattlePartyPokemon *pokemon = &battleParty->partyPokemon[partyIndex];
    u16 windowIndexOffset = BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_ALT_POKEMON_NAME * battleParty->useAltSummaryWindows;
    String *string = NewString_ReadMsgData(battleParty->messageLoader, 28);
    String *formattedString;
    u32 stringWidth, formattedStringWidth;
    u16 xOffset;

    AddTextPrinterParameterizedWithColor(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_HP_LABEL], FONT_SYSTEM, string, 0, 0, 0xFF, ((TextColor)(((15 & 0xFF) << 16) | ((14 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
    String_Delete(string);

    string = NewString_ReadMsgData(battleParty->messageLoader, 31);
    stringWidth = FontID_String_GetWidth(FONT_SYSTEM, string, 0);
    xOffset = (GetWindowWidth(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_HP]) * 8 - stringWidth) / 2;

    AddTextPrinterParameterizedWithColor(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_HP + windowIndexOffset], FONT_SYSTEM, string, xOffset, 0, 0xFF, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
    String_Delete(string);

    string = NewString_ReadMsgData(battleParty->messageLoader, 29);
    formattedString = String_New(8, battleParty->context->heapID);

    BufferIntegerAsString(battleParty->stringTemplate, 0, pokemon->curHP, 3, PRINTING_MODE_LEFT_ALIGN, CHARSET_MODE_EN);
    StringExpandPlaceholders(battleParty->stringTemplate, formattedString, string);

    formattedStringWidth = FontID_String_GetWidth(FONT_SYSTEM, formattedString, 0);

    AddTextPrinterParameterizedWithColor(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_HP + windowIndexOffset], FONT_SYSTEM, formattedString, xOffset - formattedStringWidth, 0, 0xFF, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
    String_Delete(string);
    String_Delete(formattedString);

    string = NewString_ReadMsgData(battleParty->messageLoader, 30);
    formattedString = String_New(8, battleParty->context->heapID);

    BufferIntegerAsString(battleParty->stringTemplate, 0, pokemon->maxHP, 3, PRINTING_MODE_LEFT_ALIGN, CHARSET_MODE_EN);
    StringExpandPlaceholders(battleParty->stringTemplate, formattedString, string);
    AddTextPrinterParameterizedWithColor(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_HP + windowIndexOffset], FONT_SYSTEM, formattedString, xOffset + stringWidth, 0, 0xFF, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
    String_Delete(string);
    String_Delete(formattedString);
    ScheduleWindowCopyToVram(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_HP_LABEL]);
    ScheduleWindowCopyToVram(&battleParty->windows[BATTLE_POKEMON_SUMMARY_SCREEN_WINDOW_HP + windowIndexOffset]);
}
