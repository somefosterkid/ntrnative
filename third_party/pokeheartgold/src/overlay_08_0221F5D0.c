#include "global.h"

#include "struct_defs/resdat_narc_structs.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_3d_loader.h"
#include "message_printer.h"
#include "palette.h"
#include "pm_string.h"
#include "seal_case.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02020A0C.h"
#include "constants/heap.h"
#include "font_types_def.h"
#include "sys_task.h"
#include "text.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "pokemon_types_def.h"
#include "battle/battle_party_context.h"
#include "overlay_08_battle_party.h"
#include "msgdata.h"
#include "pokemon.h"
#include "message_format.h"
#include "battle/battle.h"

struct BattleSystem;

struct BattleCursor;

typedef u16 charcode_t;

typedef struct MessageBankEntry {
    u32 offset;
    u32 length;
} MessageBankEntry;

typedef struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
} MessageBank;

struct MsgData;

typedef union MailIconData {
    struct {
        u16 spriteIndex : 12;
        u16 palIndex : 4;
    } asStruct;
    u16 asValue;
} MailIconData;

typedef struct StringTemplateArgHeader {
    u8 unused_00;
    u8 unused_01;
    u8 unused_02;
    u8 unused_03;
} StringTemplateArgHeader;

typedef struct StringTemplateArg {
    StringTemplateArgHeader header;
    String *string;
} StringTemplateArg;

enum BattleSelectPokemonScreenWindow {
    BATTLE_SELECT_POKEMON_SCREEN_WINDOW_POKEMON_NAME = 0,
    BATTLE_SELECT_POKEMON_SCREEN_WINDOW_SHIFT,
    BATTLE_SELECT_POKEMON_SCREEN_WINDOW_SUMMARY,
    BATTLE_SELECT_POKEMON_SCREEN_WINDOW_CHECK_MOVES,
};

void ov08_0221F658(BattleParty *battleParty, u32 partyIndex);

void ov08_0221E3A4(BattleParty *battleParty, u32 windowIndex, u32 textID);

void ov08_0221F5D0(BattleParty *battleParty);

enum Gender {
    GENDER_MALE = 0,
    GENDER_FEMALE = 1,
    GENDER_NONE = 2,
};
typedef u32 TextColor;
extern const u32 ov08_02224FF4[6];

void ov08_0221F5D0(BattleParty *battleParty) {
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_SELECT_POKEMON_SCREEN_WINDOW_POKEMON_NAME], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_SELECT_POKEMON_SCREEN_WINDOW_SHIFT], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_SELECT_POKEMON_SCREEN_WINDOW_SUMMARY], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_SELECT_POKEMON_SCREEN_WINDOW_CHECK_MOVES], 0);

    ov08_0221F658(battleParty, battleParty->context->selectedPartyIndex);

    ov08_0221E3A4(battleParty, BATTLE_SELECT_POKEMON_SCREEN_WINDOW_SHIFT, 15);

    if (battleParty->partyPokemon[battleParty->context->selectedPartyIndex].isEgg == 0) {
        ov08_0221E3A4(battleParty, BATTLE_SELECT_POKEMON_SCREEN_WINDOW_SUMMARY, 18);
        ov08_0221E3A4(battleParty, BATTLE_SELECT_POKEMON_SCREEN_WINDOW_CHECK_MOVES, 19);
    } else {
        ScheduleWindowCopyToVram(&battleParty->windows[BATTLE_SELECT_POKEMON_SCREEN_WINDOW_SUMMARY]);
        ScheduleWindowCopyToVram(&battleParty->windows[BATTLE_SELECT_POKEMON_SCREEN_WINDOW_CHECK_MOVES]);
    }
}

void ov08_0221F658(BattleParty *battleParty, u32 partyIndex) {
    Window *window = &battleParty->windows[BATTLE_SELECT_POKEMON_SCREEN_WINDOW_POKEMON_NAME];
    BattlePartyPokemon *pokemon = &battleParty->partyPokemon[partyIndex];
    String *string = String_New(12, battleParty->context->heapID);
    String *formattedString = NewString_ReadMsgData(battleParty->messageLoader, ov08_02224FF4[partyIndex]);
    u8 stringWidth;
    u8 formattedStringWidth;
    u8 genderIconOffset;
    u8 xOffset;

    BufferBoxMonNickname(battleParty->stringTemplate, 0, Mon_GetBoxMon(pokemon->mon));
    StringExpandPlaceholders(battleParty->stringTemplate, string, formattedString);
    String_Delete(formattedString);

    formattedString = 0L;

    if (pokemon->hideGenderMarker == 0 && pokemon->isEgg == 0) {
        if (pokemon->gender == GENDER_MALE) {
            formattedString = NewString_ReadMsgData(battleParty->messageLoader, 16);
        } else if (pokemon->gender == GENDER_FEMALE) {
            formattedString = NewString_ReadMsgData(battleParty->messageLoader, 17);
        }
    }

    stringWidth = FontID_String_GetWidth((enum Font)4, string, 0);

    if (formattedString == 0L) {
        formattedStringWidth = 0;
        genderIconOffset = 0;
    } else {
        formattedStringWidth = FontID_String_GetWidth(FONT_SYSTEM, formattedString, 0);
        genderIconOffset = 8;
    }

    xOffset = (GetWindowWidth(window) * 8 - stringWidth - formattedStringWidth - genderIconOffset) / 2;

    AddTextPrinterParameterizedWithColor(window, (enum Font)4, string, xOffset, 7, 0xFF, ((TextColor)(((7 & 0xFF) << 16) | ((8 & 0xFF) << 8) | ((9 & 0xFF) << 0))), 0L);
    String_Delete(string);

    if (formattedString != 0L) {
        if (pokemon->gender == GENDER_MALE) {
            AddTextPrinterParameterizedWithColor(
                window, FONT_SYSTEM, formattedString, xOffset + stringWidth + genderIconOffset, 8, 0xFF, ((TextColor)(((10 & 0xFF) << 16) | ((11 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
        } else {
            AddTextPrinterParameterizedWithColor(
                window, FONT_SYSTEM, formattedString, xOffset + stringWidth + genderIconOffset, 8, 0xFF, ((TextColor)(((12 & 0xFF) << 16) | ((13 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
        }

        String_Delete(formattedString);
    }

    ScheduleWindowCopyToVram(window);
}
