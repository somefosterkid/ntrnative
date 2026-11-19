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

void FillWindowPixelBuffer(Window *window, u8 val);

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

enum BattleMoveSummaryScreenWindow {
    BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_MOVE_NAME = 0,
    BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_PP,
    BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_ACCURACY,
    BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_POWER,
    BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_DESCRIPTION,
    BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_CATEGORY,
    BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_POKEMON_NAME,
    BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_PP_LABEL,
    BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_ACCURACY_LABEL,
    BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_POWER_LABEL,
    BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_CATEGORY_LABEL,
    BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_ALT_MOVE_NAME,
    BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_ALT_PP,
    BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_ALT_ACCURACY,
    BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_ALT_POWER,
    BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_ALT_DESCRIPTION,
    BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_ALT_CATEGORY,
    BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_ALT_POKEMON_NAME,
};

typedef u32 TextColor;

extern const u32 ov08_02224FE0[5];

void ov08_0221DDCC(BattleParty *battleParty, u32 windowIndex, enum Font font, u16 partyIndex, u8 xOffset, u8 yOffset);

void ov08_0221E244(BattleParty *battleParty, int move, u32 windowIndex, u32 textID, u16 font, u16 yOffset, TextColor color);

void ov08_0221E2E8(BattleParty *battleParty, u16 windowIndex, u8 xOffset, u8 yOffset);

void ov08_0221ECE8(BattleParty *battleParty, u32 windowIndex);

void ov08_0221ED30(BattleParty *battleParty, u32 windowIndex, u32 moveAccuracyStat);

void ov08_0221EE18(BattleParty *battleParty, u32 windowIndex);

void ov08_0221EE60(BattleParty *battleParty, u32 windowIndex, u32 movePowerStat);

void ov08_0221EF48(BattleParty *battleParty, u32 windowIndex, u32 textID);

void PrintMoveContestEffect(BattleParty *battleParty, u32 windowIndex, int move);

void ov08_0221EFA8(BattleParty *battleParty, u32 windowIndex);

void ov08_0221F00C(BattleParty *battleParty, u32 windowIndex, u32 moveCategory);

void ov08_0221F07C(BattleParty *battleParty, u32 windowIndex, u32 currentPP, u32 maxPP);

void ov08_0221FB18(BattleParty *battleParty);

void ov08_0221FB18(BattleParty *battleParty) {
    u32 windowIndexOffset = BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_ALT_MOVE_NAME * battleParty->useAltSummaryWindows;
    BattlePartyPokemonMove *move;

    FillWindowPixelBuffer(&battleParty->windows[BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_POKEMON_NAME], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_PP_LABEL], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_ACCURACY_LABEL], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_POWER_LABEL], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_CATEGORY_LABEL], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_PP + windowIndexOffset], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_MOVE_NAME + windowIndexOffset], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_ACCURACY + windowIndexOffset], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_POWER + windowIndexOffset], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_CATEGORY + windowIndexOffset], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_DESCRIPTION + windowIndexOffset], 0);

    move = &battleParty->partyPokemon[battleParty->context->selectedPartyIndex].moves[battleParty->context->selectedMoveSlot];

    ov08_0221DDCC(battleParty, BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_POKEMON_NAME, FONT_SYSTEM, battleParty->context->selectedPartyIndex, 0, 0);
    ov08_0221E2E8(battleParty, BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_PP_LABEL, 0, 0);
    ov08_0221E244(battleParty, (int)(move->move), BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_MOVE_NAME + windowIndexOffset, ov08_02224FE0[battleParty->context->selectedMoveSlot], FONT_SYSTEM, 0, ((TextColor)(((15 & 0xFF) << 16) | ((14 & 0xFF) << 8) | ((0 & 0xFF) << 0))));
    ov08_0221ECE8(battleParty, BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_ACCURACY_LABEL);
    ov08_0221ED30(battleParty, BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_ACCURACY + windowIndexOffset, move->accuracy);
    ov08_0221EE18(battleParty, BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_POWER_LABEL);
    ov08_0221EE60(battleParty, BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_POWER + windowIndexOffset, move->power);
    ov08_0221EF48(battleParty, BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_DESCRIPTION + windowIndexOffset, move->move);
    ov08_0221EFA8(battleParty, BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_CATEGORY_LABEL);
    ov08_0221F00C(battleParty, BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_CATEGORY + windowIndexOffset, move->class);
    ov08_0221F07C(battleParty, BATTLE_MOVE_SUMMARY_SCREEN_WINDOW_PP + windowIndexOffset, move->currentPP, move->maxPP);

    battleParty->useAltSummaryWindows ^= 1;
}
