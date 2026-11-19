#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "constants/heap.h"
#include "struct_defs/resdat_narc_structs.h"
#include "battle/battle.h"
#include "overlay_41_sprite_resource_collection.h"
#include "battle/battle_cursor.h"
#include "battle/battle_party_context.h"

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

enum BattlePokemonPartyScreenButton {
    BATTLE_POKEMON_PARTY_SCREEN_BUTTON_POKEMON_1 = 0,
    BATTLE_POKEMON_PARTY_SCREEN_BUTTON_POKEMON_2,
    BATTLE_POKEMON_PARTY_SCREEN_BUTTON_POKEMON_3,
    BATTLE_POKEMON_PARTY_SCREEN_BUTTON_POKEMON_4,
    BATTLE_POKEMON_PARTY_SCREEN_BUTTON_POKEMON_5,
    BATTLE_POKEMON_PARTY_SCREEN_BUTTON_POKEMON_6,
    BATTLE_POKEMON_PARTY_SCREEN_BUTTON_CANCEL,
};

enum BattleSelectPokemonScreenButton {
    BATTLE_POKEMON_SELECT_POKEMON_SCREEN_BUTTON_SHIFT = 0,
    BATTLE_POKEMON_SELECT_POKEMON_SCREEN_BUTTON_SUMMARY,
    BATTLE_POKEMON_SELECT_POKEMON_SCREEN_BUTTON_CHECK_MOVES,
    BATTLE_POKEMON_SELECT_POKEMON_SCREEN_BUTTON_CANCEL,
};

enum BattlePokemonMovesScreenButton {
    BATTLE_POKEMON_MOVES_SCREEN_BUTTON_MOVE_1 = 0,
    BATTLE_POKEMON_MOVES_SCREEN_BUTTON_MOVE_2,
    BATTLE_POKEMON_MOVES_SCREEN_BUTTON_MOVE_3,
    BATTLE_POKEMON_MOVES_SCREEN_BUTTON_MOVE_4,
    BATTLE_POKEMON_MOVES_SCREEN_BUTTON_PREV_POKEMON,
    BATTLE_POKEMON_MOVES_SCREEN_BUTTON_NEXT_POKEMON,
    BATTLE_POKEMON_MOVES_SCREEN_BUTTON_SUMMARY,
    BATTLE_POKEMON_MOVES_SCREEN_BUTTON_CANCEL,
};

enum BattleMoveSummaryScreenButton {
    BATTLE_MOVE_SUMMARY_SCREEN_BUTTON_MOVE_1 = 0,
    BATTLE_MOVE_SUMMARY_SCREEN_BUTTON_MOVE_2,
    BATTLE_MOVE_SUMMARY_SCREEN_BUTTON_MOVE_3,
    BATTLE_MOVE_SUMMARY_SCREEN_BUTTON_MOVE_4,
    BATTLE_MOVE_SUMMARY_SCREEN_BUTTON_CANCEL,
};

enum BattleRestoreMovePPScreenButton {
    BATTLE_RESTORE_MOVE_PP_SCREEN_BUTTON_MOVE_1 = 0,
    BATTLE_RESTORE_MOVE_PP_SCREEN_BUTTON_MOVE_2,
    BATTLE_RESTORE_MOVE_PP_SCREEN_BUTTON_MOVE_3,
    BATTLE_RESTORE_MOVE_PP_SCREEN_BUTTON_MOVE_4,
    BATTLE_RESTORE_MOVE_PP_SCREEN_BUTTON_CANCEL,
};

enum BattleLearnMoveScreenButton {
    BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_1 = 0,
    BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_2,
    BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_3,
    BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_4,
    BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_TO_LEARN,
    BATTLE_LEARN_MOVE_SCREEN_BUTTON_CONTEST_STATS,
    BATTLE_LEARN_MOVE_SCREEN_BUTTON_CANCEL,
};

enum Button {
    BUTTON_POKEMON_PARTY_SCREEN_POKEMON_1 = BATTLE_POKEMON_PARTY_SCREEN_BUTTON_POKEMON_1,
    BUTTON_POKEMON_PARTY_SCREEN_POKEMON_2 = BATTLE_POKEMON_PARTY_SCREEN_BUTTON_POKEMON_2,
    BUTTON_POKEMON_PARTY_SCREEN_POKEMON_3 = BATTLE_POKEMON_PARTY_SCREEN_BUTTON_POKEMON_3,
    BUTTON_POKEMON_PARTY_SCREEN_POKEMON_4 = BATTLE_POKEMON_PARTY_SCREEN_BUTTON_POKEMON_4,
    BUTTON_POKEMON_PARTY_SCREEN_POKEMON_5 = BATTLE_POKEMON_PARTY_SCREEN_BUTTON_POKEMON_5,
    BUTTON_POKEMON_PARTY_SCREEN_POKEMON_6 = BATTLE_POKEMON_PARTY_SCREEN_BUTTON_POKEMON_6,
    BUTTON_CANCEL = 6,
    BUTTON_SELECT_POKEMON_SCREEN_SHIFT = 7 + BATTLE_POKEMON_SELECT_POKEMON_SCREEN_BUTTON_SHIFT,
    BUTTON_SELECT_POKEMON_SCREEN_SUMMARY = 8,
    BUTTON_POKEMON_MOVES_SCREEN_SUMMARY,
    BUTTON_SELECT_POKEMON_SCREEN_CHECK_MOVES = 10,
    BUTTON_POKEMON_SUMMARY_SCREEN_CHECK_MOVES,
    BUTTON_PREV_POKEMON = 12,
    BUTTON_NEXT_POKEMON,
    BUTTON_POKEMON_MOVES_SCREEN_MOVE_1 = 14 + BATTLE_POKEMON_MOVES_SCREEN_BUTTON_MOVE_1,
    BUTTON_POKEMON_MOVES_SCREEN_MOVE_2 = 14 + BATTLE_POKEMON_MOVES_SCREEN_BUTTON_MOVE_2,
    BUTTON_POKEMON_MOVES_SCREEN_MOVE_3 = 14 + BATTLE_POKEMON_MOVES_SCREEN_BUTTON_MOVE_3,
    BUTTON_POKEMON_MOVES_SCREEN_MOVE_4 = 14 + BATTLE_POKEMON_MOVES_SCREEN_BUTTON_MOVE_4,
    BUTTON_CONTEST_STATS = 18,
    BUTTON_RESTORE_MOVE_PP_SCREEN_MOVE_1 = 19 + BATTLE_RESTORE_MOVE_PP_SCREEN_BUTTON_MOVE_1,
    BUTTON_RESTORE_MOVE_PP_SCREEN_MOVE_2 = 19 + BATTLE_RESTORE_MOVE_PP_SCREEN_BUTTON_MOVE_2,
    BUTTON_RESTORE_MOVE_PP_SCREEN_MOVE_3 = 19 + BATTLE_RESTORE_MOVE_PP_SCREEN_BUTTON_MOVE_3,
    BUTTON_RESTORE_MOVE_PP_SCREEN_MOVE_4 = 19 + BATTLE_RESTORE_MOVE_PP_SCREEN_BUTTON_MOVE_4,
    BUTTON_LEARN_MOVE_SCREEN_MOVE_1 = 23 + BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_1,
    BUTTON_LEARN_MOVE_SCREEN_MOVE_2 = 23 + BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_2,
    BUTTON_LEARN_MOVE_SCREEN_MOVE_3 = 23 + BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_3,
    BUTTON_LEARN_MOVE_SCREEN_MOVE_4 = 23 + BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_4,
    BUTTON_LEARN_MOVE_SCREEN_MOVE_TO_LEARN = 23 + BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_TO_LEARN,
    BUTTON_CONFIRM_LEARN_MOVE_SCREEN_CONFIRM = 28,
    BUTTON_CONFIRM_LEARN_MOVE_CONTEST_STATS_SCREEN_CONFIRM,
    BUTTON_MOVE_SUMMARY_SCREEN_MOVE_1 = 30 + BATTLE_MOVE_SUMMARY_SCREEN_BUTTON_MOVE_1,
    BUTTON_MOVE_SUMMARY_SCREEN_MOVE_2 = 30 + BATTLE_MOVE_SUMMARY_SCREEN_BUTTON_MOVE_2,
    BUTTON_MOVE_SUMMARY_SCREEN_MOVE_3 = 30 + BATTLE_MOVE_SUMMARY_SCREEN_BUTTON_MOVE_3,
    BUTTON_MOVE_SUMMARY_SCREEN_MOVE_4 = 30 + BATTLE_MOVE_SUMMARY_SCREEN_BUTTON_MOVE_4,
};

void RetrieveButtonData(BattleParty *battleParty, u16 *buttonData, u8 button, enum BattleSubMenuButtonState buttonState, u8 isAltButton);

void ov08_02221E6C(BattleParty *battleParty, u8 button, u8 buttonState, u8 isAltButton);

void ov08_02221EE8(BattleParty *battleParty, u8 button, enum BattleSubMenuButtonState buttonState);

void ov08_02221F98(BattleParty *battleParty, enum Button button, enum BattleSubMenuButtonState buttonState);

void ov08_022220FC(BattleParty *battleParty);

void ov08_022220FC(BattleParty *battleParty) {
    if (battleParty->isAButtonPressed == 0) {
        return;
    }

    switch (battleParty->pressedButtonState) {
    case BATTLE_SUB_MENU_BUTTON_STATE_UNPRESSED:
        ov08_02221E6C(battleParty, battleParty->pressedButton, BATTLE_SUB_MENU_BUTTON_STATE_PRESSING, battleParty->useAltButtons);
        ov08_02221EE8(battleParty, battleParty->pressedButton, BATTLE_SUB_MENU_BUTTON_STATE_PRESSING);
        ov08_02221F98((struct BattleParty *)(battleParty), (enum Button)(battleParty->pressedButton), BATTLE_SUB_MENU_BUTTON_STATE_PRESSING);
        battleParty->sLotoId = 0;
        battleParty->pressedButtonState = BATTLE_SUB_MENU_BUTTON_STATE_PRESSING;
        break;
    case BATTLE_SUB_MENU_BUTTON_STATE_PRESSING:
        ov08_02221E6C(battleParty, battleParty->pressedButton, BATTLE_SUB_MENU_BUTTON_STATE_PRESSED, battleParty->useAltButtons);
        ov08_02221EE8(battleParty, battleParty->pressedButton, BATTLE_SUB_MENU_BUTTON_STATE_PRESSED);
        ov08_02221F98((struct BattleParty *)(battleParty), (enum Button)(battleParty->pressedButton), BATTLE_SUB_MENU_BUTTON_STATE_PRESSED);
        battleParty->sLotoId = 0;
        battleParty->pressedButtonState = BATTLE_SUB_MENU_BUTTON_STATE_PRESSED;
        break;
    case BATTLE_SUB_MENU_BUTTON_STATE_PRESSED:
        ov08_02221E6C(battleParty, battleParty->pressedButton, BATTLE_SUB_MENU_BUTTON_STATE_UNPRESSED, battleParty->useAltButtons);
        ov08_02221EE8(battleParty, battleParty->pressedButton, BATTLE_SUB_MENU_BUTTON_STATE_UNPRESSED);
        ov08_02221F98((struct BattleParty *)(battleParty), (enum Button)(battleParty->pressedButton), BATTLE_SUB_MENU_BUTTON_STATE_UNPRESSED);
        battleParty->sLotoId = 0;
        battleParty->isAButtonPressed = 0;
        break;
    }
}
