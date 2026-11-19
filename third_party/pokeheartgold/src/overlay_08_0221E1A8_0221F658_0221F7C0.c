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
#include "msgdata.h"
#include "battle/battle.h"
#include "overlay_08_battle_party.h"
#include "battle/battle_party_context.h"
#include "battle/battle_cursor.h"

struct BallSeal;

struct BattleParty;

struct BattlePartyContext;

struct BattlePartyPokemon;

struct BattlePartyPokemonMove;

struct BattleSubMenuCursor;

struct BattleSystem;

struct BgConfig;


struct MessagePrinter;

struct GridMenuCursorPosition;

struct ManagedSprite;

struct MessageBank;

struct MessageBankEntry;

struct MsgData;

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

void FillWindowPixelBuffer(Window *window, u8 fillValue);

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

enum BattlePokemonMovesScreenWindow {
    BATTLE_POKEMON_MOVES_SCREEN_WINDOW_POKEMON_NAME = 0,
    BATTLE_POKEMON_MOVES_SCREEN_WINDOW_MOVE_1,
    BATTLE_POKEMON_MOVES_SCREEN_WINDOW_MOVE_2,
    BATTLE_POKEMON_MOVES_SCREEN_WINDOW_MOVE_3,
    BATTLE_POKEMON_MOVES_SCREEN_WINDOW_MOVE_4,
    BATTLE_POKEMON_MOVES_SCREEN_WINDOW_SUMMARY,
    BATTLE_POKEMON_MOVES_SCREEN_WINDOW_ALT_POKEMON_NAME,
    BATTLE_POKEMON_MOVES_SCREEN_WINDOW_ALT_MOVE_1,
    BATTLE_POKEMON_MOVES_SCREEN_WINDOW_ALT_MOVE_2,
    BATTLE_POKEMON_MOVES_SCREEN_WINDOW_ALT_MOVE_3,
    BATTLE_POKEMON_MOVES_SCREEN_WINDOW_ALT_MOVE_4,
};

typedef u32 TextColor;

extern const u32 ov08_02224FE0[5];

void ov08_0221DDCC(BattleParty *battleParty, u32 windowIndex, enum Font font, u16 partyIndex, u8 xOffset, u8 yOffset);

void PrintMoveName(BattleParty *battleParty, int move, u32 windowIndex, u32 textID, u16 font, u16 yOffset, TextColor color);

void PrintButtonText(BattleParty *battleParty, u32 windowIndex, u32 textID);

void PrintMoveContestEffect(BattleParty *battleParty, u32 windowIndex, int move);

void ov08_0221F284(BattleParty *battleParty, BattlePartyPokemonMove *move, u32 windowIndex);

void ov08_0221F7C0(BattleParty *battleParty);

void ov08_0221E244(BattleParty *battleParty, int move, u32 windowIndex, u32 textID, u16 font, u16 yOffset, TextColor color);

void ov08_0221E3A4(BattleParty *battleParty, u32 windowIndex, u32 textID);

void ov08_0221F7C0(BattleParty *battleParty) {
    u16 windowIndexOffset = BATTLE_POKEMON_MOVES_SCREEN_WINDOW_ALT_POKEMON_NAME * battleParty->useAltSummaryWindows;

    FillWindowPixelBuffer(&battleParty->windows[BATTLE_POKEMON_MOVES_SCREEN_WINDOW_POKEMON_NAME + windowIndexOffset], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_POKEMON_MOVES_SCREEN_WINDOW_MOVE_1 + windowIndexOffset], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_POKEMON_MOVES_SCREEN_WINDOW_MOVE_2 + windowIndexOffset], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_POKEMON_MOVES_SCREEN_WINDOW_MOVE_3 + windowIndexOffset], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_POKEMON_MOVES_SCREEN_WINDOW_MOVE_4 + windowIndexOffset], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_POKEMON_MOVES_SCREEN_WINDOW_SUMMARY], 0);

    ov08_0221DDCC(battleParty, 0 + windowIndexOffset, FONT_SYSTEM, battleParty->context->selectedPartyIndex, 0, 0);

    for (u16 i = 0; i < 4; i++) {
        BattlePartyPokemonMove *move = &battleParty->partyPokemon[battleParty->context->selectedPartyIndex].moves[i];

        if (move->move == MOVE_NONE) {
            continue;
        }

        ov08_0221E244(battleParty, (int)(move->move), BATTLE_POKEMON_MOVES_SCREEN_WINDOW_MOVE_1 + windowIndexOffset + i, ov08_02224FE0[i], 4, 7, ((TextColor)(((7 & 0xFF) << 16) | ((8 & 0xFF) << 8) | ((9 & 0xFF) << 0))));
        ov08_0221F284(battleParty, move, BATTLE_POKEMON_MOVES_SCREEN_WINDOW_MOVE_1 + windowIndexOffset + i);
    }

    ov08_0221E3A4(battleParty, BATTLE_POKEMON_MOVES_SCREEN_WINDOW_SUMMARY, 18);

    ScheduleWindowCopyToVram(&battleParty->windows[BATTLE_POKEMON_MOVES_SCREEN_WINDOW_MOVE_1 + windowIndexOffset]);
    ScheduleWindowCopyToVram(&battleParty->windows[BATTLE_POKEMON_MOVES_SCREEN_WINDOW_MOVE_2 + windowIndexOffset]);
    ScheduleWindowCopyToVram(&battleParty->windows[BATTLE_POKEMON_MOVES_SCREEN_WINDOW_MOVE_3 + windowIndexOffset]);
    ScheduleWindowCopyToVram(&battleParty->windows[BATTLE_POKEMON_MOVES_SCREEN_WINDOW_MOVE_4 + windowIndexOffset]);

    battleParty->useAltSummaryWindows ^= 1;
}
