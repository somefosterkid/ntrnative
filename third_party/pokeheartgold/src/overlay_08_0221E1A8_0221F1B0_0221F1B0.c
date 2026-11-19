#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "struct_defs/resdat_narc_structs.h"
#include "font.h"
#include "battle/battle.h"
#include "overlay_41_sprite_resource_collection.h"
#include "battle/battle_party_context.h"
#include "battle/battle_cursor.h"

struct BallSeal;

struct BattleParty;


struct MessagePrinter;

struct GridMenuCursorPosition;

struct ManagedSprite;

struct MessageBank;

struct MessageBankEntry;

struct SelectedPaletteData;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct String;

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

typedef u32 TextColor;

u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontID, const String *string, u32 xOffset, u32 yOffset, u32 renderDelay, TextColor color, PrinterCallback_t callback);

void ov08_0221DDCC(BattleParty *battleParty, u32 windowIndex, enum Font font, u16 partyIndex, u8 xOffset, u8 yOffset);

void ov08_0221E244(BattleParty *battleParty, int move, u32 windowIndex, u32 textID, u16 font, u16 yOffset, TextColor color);

void PrintMoveContestEffect(BattleParty *battleParty, u32 windowIndex, int move);

void ov08_0221F1B0(BattleParty *battleParty, u32 windowIndex);

void ov08_0221F1B0(BattleParty *battleParty, u32 windowIndex) {
    Window *window = &battleParty->windows[windowIndex];
    String *string;
    u32 stringWidth;

    if (battleParty->context->selectedMoveSlot == 4) {
        string = NewString_ReadMsgData(battleParty->messageLoader, 59);
    } else {
        string = NewString_ReadMsgData(battleParty->messageLoader, 58);
    }

    stringWidth = FontID_String_GetWidth((enum Font)4, string, 0);

    AddTextPrinterParameterizedWithColor(window, (enum Font)4, string, (96 - stringWidth) / 2, 5, 0xFF, ((TextColor)(((7 & 0xFF) << 16) | ((8 & 0xFF) << 8) | ((9 & 0xFF) << 0))), 0L);
    String_Delete(string);
    ScheduleWindowCopyToVram(window);
}
