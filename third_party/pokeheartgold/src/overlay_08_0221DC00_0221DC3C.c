#include "global.h"
#include "sprite_resources_header.h"

#include "bg_window.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "struct_defs/resdat_narc_structs.h"
#include "gf_gfx_planes.h"
#include "font.h"
#include "battle/battle.h"
#include "overlay_41_sprite_resource_collection.h"
#include "battle/battle_party_context.h"
#include "battle/battle_cursor.h"

struct BallSeal;

struct BattleParty;

struct BattlePartyContext;

struct BattlePartyPokemon;

struct BattlePartyPokemonMove;

struct BattleSystem;


struct MessagePrinter;

struct GridMenuCursorPosition;

struct ManagedSprite;

struct MessageBank;

struct MessageBankEntry;

struct SelectedPaletteData;

struct Party;

struct Pokemon;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct String;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct SysTask;

struct SysTaskQueue;

struct WindowTemplate;

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

enum BattlePartyScreen {
    BATTLE_PARTY_SCREEN_POKEMON_PARTY = 0,
    BATTLE_PARTY_SCREEN_SELECT_POKEMON,
    BATTLE_PARTY_SCREEN_POKEMON_SUMMARY,
    BATTLE_PARTY_SCREEN_POKEMON_MOVES,
    BATTLE_PARTY_SCREEN_MOVE_SUMMARY,
    BATTLE_PARTY_SCREEN_RESTORE_MOVE_PP,
    BATTLE_PARTY_SCREEN_LEARN_MOVE,
    BATTLE_PARTY_SCREEN_CONFIRM_LEARN_MOVE,
    BATTLE_PARTY_SCREEN_LEARN_MOVE_CONTEST_STATS,
    BATTLE_PARTY_SCREEN_CONFIRM_LEARN_MOVE_CONTEST_STATS,
};

typedef u32 TextColor;

extern const WindowTemplate ov08_02225084[6];

extern const WindowTemplate ov08_0222500C[4];

extern const WindowTemplate ov08_0222522C[35];

extern const WindowTemplate ov08_022250EC[11];

extern const WindowTemplate ov08_022251A4[17];

extern const WindowTemplate ov08_0222502C[5];

extern const WindowTemplate ov08_02225054[6];

extern const WindowTemplate ov08_02225144[12];

extern const WindowTemplate ov08_022250B4[7];

void ov08_0221DDCC(BattleParty *battleParty, u32 windowIndex, enum Font font, u16 partyIndex, u8 xOffset, u8 yOffset);

void ov08_0221E244(BattleParty *battleParty, int move, u32 windowIndex, u32 textID, u16 font, u16 yOffset, TextColor color);

void PrintMoveContestEffect(BattleParty *battleParty, u32 windowIndex, int move);

void ov08_0221DC3C(BattleParty *battleParty, enum BattlePartyScreen screen);

void ov08_0221DC3C(BattleParty *battleParty, enum BattlePartyScreen screen) {
    const WindowTemplate *windowTemplates;

    switch (screen) {
    case BATTLE_PARTY_SCREEN_POKEMON_PARTY:
        windowTemplates = ov08_02225084;
        battleParty->numWindows = 6;
        break;
    case BATTLE_PARTY_SCREEN_SELECT_POKEMON:
        windowTemplates = ov08_0222500C;
        battleParty->numWindows = 4;
        break;
    case BATTLE_PARTY_SCREEN_POKEMON_SUMMARY:
        windowTemplates = ov08_0222522C;
        battleParty->numWindows = 35;
        break;
    case BATTLE_PARTY_SCREEN_POKEMON_MOVES:
        windowTemplates = ov08_022250EC;
        battleParty->numWindows = 11;
        break;
    case BATTLE_PARTY_SCREEN_MOVE_SUMMARY:
        windowTemplates = ov08_022251A4;
        battleParty->numWindows = 17;
        break;
    case BATTLE_PARTY_SCREEN_RESTORE_MOVE_PP:
        windowTemplates = ov08_0222502C;
        battleParty->numWindows = 5;
        break;
    case BATTLE_PARTY_SCREEN_LEARN_MOVE:
    case BATTLE_PARTY_SCREEN_LEARN_MOVE_CONTEST_STATS:
        windowTemplates = ov08_02225054;
        battleParty->numWindows = 6;
        break;
    case BATTLE_PARTY_SCREEN_CONFIRM_LEARN_MOVE:
        windowTemplates = ov08_02225144;
        battleParty->numWindows = 12;
        break;
    case BATTLE_PARTY_SCREEN_CONFIRM_LEARN_MOVE_CONTEST_STATS:
        windowTemplates = ov08_022250B4;
        battleParty->numWindows = 7;
        break;
    }

    battleParty->windows = AllocWindows(battleParty->context->heapID, battleParty->numWindows);

    for (u8 i = 0; i < battleParty->numWindows; i++) {
        AddWindow(battleParty->background, &battleParty->windows[i], &windowTemplates[i]);
    }
}
