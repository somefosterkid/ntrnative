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

extern u32 ov08_02224C94(BattleSubMenuCursor *cursor);

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

enum BattlePokemonPartyScreenButton {
    BATTLE_POKEMON_PARTY_SCREEN_BUTTON_POKEMON_1 = 0,
    BATTLE_POKEMON_PARTY_SCREEN_BUTTON_POKEMON_2,
    BATTLE_POKEMON_PARTY_SCREEN_BUTTON_POKEMON_3,
    BATTLE_POKEMON_PARTY_SCREEN_BUTTON_POKEMON_4,
    BATTLE_POKEMON_PARTY_SCREEN_BUTTON_POKEMON_5,
    BATTLE_POKEMON_PARTY_SCREEN_BUTTON_POKEMON_6,
    BATTLE_POKEMON_PARTY_SCREEN_BUTTON_CANCEL,
};

extern u8 ov08_0221D5DC(BattleParty *battleParty, s32 partySlot);

typedef union {
    struct {
        u8 top;
        u8 bottom;
        u8 left;
        u8 right;
    } rect;

    struct {
        u8 code;
        u8 x;
        u8 y;
        u8 r;
    } circle;
} TouchScreenRect, TouchscreenHitbox;

extern void ov08_022217C8(BattleParty *battleParty);

extern int ov08_0221D5D0(BattleParty *battleParty, const TouchScreenRect *rect);

extern const TouchScreenRect ov08_02224F1C[8];

void DrawXPBar(BattleParty *battleParty, enum BattlePartyScreen screen);

void ChangeBattlePartyScreen(BattleParty *battleParty, enum BattlePartyScreen screen);

void DrawScreenBackground(BattleParty *battleParty, enum BattlePartyScreen screen);

void ov08_02223390(BattleSystem *battleSys, u16 item, u16 category, enum HeapID heapID);

BOOL ov08_0221D438(BattleParty *battleParty);

BOOL ov08_0221D438(BattleParty *battleParty) {
    enum BattlePokemonPartyScreenButton buttonPressed = (enum BattlePokemonPartyScreenButton)(ov08_0221D5D0(battleParty, ov08_02224F1C));

    if (buttonPressed == -1) {
        buttonPressed = (enum BattlePokemonPartyScreenButton)(ov08_02224C94(battleParty->cursor));

        if (buttonPressed == 0xfffffffe) {
            buttonPressed = BATTLE_POKEMON_PARTY_SCREEN_BUTTON_CANCEL;
        } else if (buttonPressed == 0xffffffff) {
            return 0;
        }

        if (buttonPressed == BATTLE_POKEMON_PARTY_SCREEN_BUTTON_CANCEL || ov08_0221D5DC(battleParty, buttonPressed) != 0) {
            battleParty->context->selectedPartyIndex = (u8)buttonPressed;
            return 1;
        }
    } else {
        if (buttonPressed == BATTLE_POKEMON_PARTY_SCREEN_BUTTON_CANCEL || ov08_0221D5DC(battleParty, buttonPressed) != 0) {
            battleParty->context->selectedPartyIndex = (u8)buttonPressed;

            ov08_022217C8(battleParty);
            return 1;
        }
    }

    return 0;
}
