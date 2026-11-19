#include "global.h"
#include "sprite_resources_header.h"
#include "constants/heap.h"
#include "struct_defs/resdat_narc_structs.h"
#include "font.h"
#include "palette.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "sprite_system.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "seal_case.h"
#include "pokemon_types_def.h"
#include "message_format.h"
#include "battle/battle_party_context.h"
#include "overlay_08_battle_party.h"
#include "pokemon.h"
#include "battle/battle.h"

struct BallSeal;

struct BattleParty;

struct BattlePartyContext;

struct BattlePartyPokemon;

struct BattlePartyPokemonMove;

struct BattleSubMenuCursor;

struct BattleSystem;

struct BgConfig;

struct BoxPokemon;


struct MessagePrinter;

struct GridMenuCursorPosition;

struct ManagedSprite;

struct MessageBank;

struct MessageBankEntry;

struct MsgData;

struct NARC;

struct PaletteData;

struct SelectedPaletteData;

struct Party;

struct Pokemon;

struct SpriteManager;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct String;

struct MessageFormat;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct SysTask;

struct SysTaskQueue;

struct Window;


typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

typedef struct GridMenuCursorPosition {
    u8 xCoord1;
    u8 yCoord1;
    u8 xCoord2;
    u8 yCoord2;
    u8 upIndex;
    u8 downIndex;
    u8 leftIndex;
    u8 rightIndex;
} GridMenuCursorPosition;

void ScheduleWindowCopyToVram(Window *window);

u8 GetWindowWidth(Window *window);

typedef u16 charcode_t;

void String_Delete(String *string);

typedef struct MessageBankEntry {
    u32 offset;
    u32 length;
} MessageBankEntry;

typedef struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
} MessageBank;


typedef struct BallSeal {
    u8 type;
    u8 x;
    u8 y;
} BallSeal;

enum Gender {
    GENDER_MALE = 0,
    GENDER_FEMALE = 1,
    GENDER_NONE = 2,
};

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

void BufferBoxMonNickname(MessageFormat *template, u32 idx, BoxPokemon *boxMon);

typedef u32 TextColor;

u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontID, const String *string, u32 xOffset, u32 yOffset, u32 renderDelay, TextColor color, PrinterCallback_t callback);

void PrintMoveName(BattleParty *battleParty, int move, u32 windowIndex, u32 textID, u16 font, u16 yOffset, TextColor color);

void PrintMoveContestEffect(BattleParty *battleParty, u32 windowIndex, int move);

extern const u32 ov08_02224FF4[];

void ov08_0221DDCC(BattleParty *battleParty, u32 windowIndex, enum Font font, u16 partyIndex, u8 xOffset, u8 yOffset);

void ov08_0221DDCC(BattleParty *battleParty, u32 windowIndex, enum Font font, u16 partyIndex, u8 xOffset, u8 yOffset) {
    Window *window = &battleParty->windows[windowIndex];
    BattlePartyPokemon *pokemon = &battleParty->partyPokemon[partyIndex];
    String *formattedString = String_New(12, battleParty->context->heapID);
    String *string = NewString_ReadMsgData(battleParty->messageLoader, ov08_02224FF4[partyIndex]);

    BufferBoxMonNickname(battleParty->stringTemplate, 0, Mon_GetBoxMon(pokemon->mon));
    StringExpandPlaceholders(battleParty->stringTemplate, formattedString, string);

    if (font == FONT_SYSTEM) {
        AddTextPrinterParameterizedWithColor(window, font, formattedString, xOffset, yOffset, 0xFF, ((TextColor)(((15 & 0xFF) << 16) | ((14 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
    } else {
        AddTextPrinterParameterizedWithColor(window, font, formattedString, xOffset, yOffset, 0xFF, ((TextColor)(((7 & 0xFF) << 16) | ((8 & 0xFF) << 8) | ((9 & 0xFF) << 0))), 0L);
    }

    String_Delete(string);
    String_Delete(formattedString);

    if (pokemon->hideGenderMarker == 0 && pokemon->isEgg == 0) {
        u32 genderIconXOffset;
        if (pokemon->gender == GENDER_MALE) {
            string = NewString_ReadMsgData(battleParty->messageLoader, 16);
            genderIconXOffset = GetWindowWidth(window) * 8 - FontID_String_GetWidth(FONT_SYSTEM, string, 0);

            if (font == FONT_SYSTEM) {
                AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, string, genderIconXOffset, yOffset, 0xFF, ((TextColor)(((7 & 0xFF) << 16) | ((8 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
            } else {
                AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, string, genderIconXOffset, yOffset, 0xFF, ((TextColor)(((10 & 0xFF) << 16) | ((11 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
            }

            String_Delete(string);
        } else if (pokemon->gender == GENDER_FEMALE) {
            string = NewString_ReadMsgData(battleParty->messageLoader, 17);
            genderIconXOffset = GetWindowWidth(window) * 8 - FontID_String_GetWidth(FONT_SYSTEM, string, 0);

            if (font == FONT_SYSTEM) {
                AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, string, genderIconXOffset, yOffset, 0xFF, ((TextColor)(((3 & 0xFF) << 16) | ((4 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
            } else {
                AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, string, genderIconXOffset, yOffset, 0xFF, ((TextColor)(((12 & 0xFF) << 16) | ((13 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
            }

            String_Delete(string);
        }
    }

    ScheduleWindowCopyToVram(window);
}
