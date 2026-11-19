#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "constants/heap.h"
#include "struct_defs/resdat_narc_structs.h"
#include "battle/battle.h"
#include "overlay_41_sprite_resource_collection.h"
#include "battle/battle_party_context.h"
#include "battle/battle_cursor.h"

struct BallSeal;

struct BattleParty;

struct BattlePartyContext;

struct BattlePartyPokemon;

struct BattlePartyPokemonMove;

struct BattleSubMenuCursor;

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

enum MoveClass {
    CLASS_PHYSICAL = 0,
    CLASS_SPECIAL = 1,
    CLASS_STATUS = 2,
};

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

void ov08_02224B98(BattleSubMenuCursor *cursor, u8 positionIndex);

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

void ov08_02220B3C(BattleParty *battleParty, ManagedSprite *sprite, u16 moveClass);

void ov08_022216CC(BattleParty *battleParty);

void ov08_022216F4(BattleParty *battleParty);

void ov08_02224BCC(BattleSubMenuCursor *cursor, const GridMenuCursorPosition *positions);

extern const GridMenuCursorPosition *const ov08_022254BC[];

void ov08_0222171C(BattleParty *battleParty, enum BattlePartyScreen screen);

void ov08_0222171C(BattleParty *battleParty, enum BattlePartyScreen screen) {
    ov08_02224BCC(battleParty->cursor, ov08_022254BC[screen]);

    switch (screen) {
    case BATTLE_PARTY_SCREEN_POKEMON_PARTY:
        ov08_02224B98(battleParty->cursor, battleParty->context->selectedPartyIndex);
        battleParty->selectPokemonPreviousScreenButton = 0;
        battleParty->context->selectedMoveSlot = 0;
        break;
    case BATTLE_PARTY_SCREEN_SELECT_POKEMON:
        ov08_02224B98(battleParty->cursor, battleParty->selectPokemonPreviousScreenButton);
        battleParty->context->selectedMoveSlot = 0;
        break;
    case BATTLE_PARTY_SCREEN_POKEMON_MOVES:
    case BATTLE_PARTY_SCREEN_MOVE_SUMMARY:
        ov08_02224B98(battleParty->cursor, battleParty->context->selectedMoveSlot);
        break;
    case BATTLE_PARTY_SCREEN_LEARN_MOVE:
    case BATTLE_PARTY_SCREEN_LEARN_MOVE_CONTEST_STATS:
        ov08_022216CC(battleParty);
        ov08_02224B98(battleParty->cursor, battleParty->learnMovePreviousScreenButton);
        break;
    case BATTLE_PARTY_SCREEN_CONFIRM_LEARN_MOVE:
    case BATTLE_PARTY_SCREEN_CONFIRM_LEARN_MOVE_CONTEST_STATS:
        ov08_022216F4(battleParty);
        ov08_02224B98(battleParty->cursor, battleParty->confirmLearnMovePreviousScreenButton);
        break;
    case BATTLE_PARTY_SCREEN_POKEMON_SUMMARY:
    case BATTLE_PARTY_SCREEN_RESTORE_MOVE_PP:
        break;
    }
}
