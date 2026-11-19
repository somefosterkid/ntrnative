#ifndef POKEHEARTGOLD_OVERLAY_08_BATTLE_PARTY_H
#define POKEHEARTGOLD_OVERLAY_08_BATTLE_PARTY_H

#include "bg_window.h"
#include "pokemon_types_def.h"

// BattleParty: the battle party / summary sub-menu work object (overlay 08), SysTask data from ov08_0221BE20.

struct BattlePartyContext;
struct BattleSubMenuCursor;
struct ManagedSprite;
struct MessageFormat;
struct MessagePrinter;
struct MsgData;
struct PaletteData;
struct SpriteManager;
struct String;

enum BattleSubMenuButtonState {
    BATTLE_SUB_MENU_BUTTON_STATE_UNPRESSED = 0,
    BATTLE_SUB_MENU_BUTTON_STATE_PRESSING,
    BATTLE_SUB_MENU_BUTTON_STATE_PRESSED,
    BATTLE_SUB_MENU_BUTTON_STATE_DISABLED,

    BATTLE_SUB_MENU_BUTTON_STATE_MAX,
    BATTLE_SUB_MENU_BUTTON_STATE_MAX_CANNOT_DISABLE = BATTLE_SUB_MENU_BUTTON_STATE_DISABLED,
};

typedef struct BattlePartyPokemonMove {
    u16 move;
    u8 currentPP;
    u8 maxPP;
    u8 type;
    u8 class;
    u8 accuracy;
    u8 power;
} BattlePartyPokemonMove; // DS 0x8

typedef struct BattlePartyPokemon {
    Pokemon *mon;
    u16 species;
    u16 attack;
    u16 defence;
    u16 speed;
    u16 spAtk;
    u16 spDef;
    u16 curHP;
    u16 maxHP;
    u8 type_1;
    u8 type_2;
    u8 level : 7;
    u8 hideGenderMarker : 1;
    u8 gender : 3;
    u8 summaryStatus : 4;
    u8 isEgg : 1;
    u16 ability;
    u16 heldItem;
    u32 exp;
    u32 currentLevelBaseExp;
    u32 nextLevelExp;
    u8 cool;
    u8 beauty;
    u8 cute;
    u8 smart;
    u8 tough;
    u8 mail;
    u8 form;
    BattlePartyPokemonMove moves[4];
} BattlePartyPokemon; // DS 0x50

typedef struct BattleParty {
    struct BattlePartyContext *context;
    BattlePartyPokemon partyPokemon[6];
    struct BgConfig *background;
    struct PaletteData *palette;
    u16 partyPokemonButtonData[BATTLE_SUB_MENU_BUTTON_STATE_MAX][16 * 6];
    u16 altPartyPokemonButtonData[BATTLE_SUB_MENU_BUTTON_STATE_MAX][16 * 6];
    u16 summaryCheckMovesButtonData[BATTLE_SUB_MENU_BUTTON_STATE_MAX][13 * 5];
    u16 prevPokemonButtonData[BATTLE_SUB_MENU_BUTTON_STATE_MAX][5 * 5];
    u16 nextPokemonButtonData[BATTLE_SUB_MENU_BUTTON_STATE_MAX][5 * 5];
    u16 cancelButtonData[BATTLE_SUB_MENU_BUTTON_STATE_MAX][5 * 5];
    u16 shiftPokemonButtonData[BATTLE_SUB_MENU_BUTTON_STATE_MAX_CANNOT_DISABLE][30 * 17];
    u16 moveButtonData[BATTLE_SUB_MENU_BUTTON_STATE_MAX][16 * 6];
    u16 learnMoveConfirmButtonData[BATTLE_SUB_MENU_BUTTON_STATE_MAX_CANNOT_DISABLE][26 * 5];
    u16 contestStatsButtonData[BATTLE_SUB_MENU_BUTTON_STATE_MAX_CANNOT_DISABLE][9 * 4];
    u16 moveSummaryScreenMoveButtonData[BATTLE_SUB_MENU_BUTTON_STATE_MAX_CANNOT_DISABLE][5 * 2];
    u16 screenPaletteData[16 * sizeof(u16)];
    u8 pressedButtonState;
    u8 sLotoId;
    u8 pressedButton;
    u8 unused2 : 4;
    u8 useAltButtons : 3;
    u8 isAButtonPressed : 1;
    struct MessagePrinter *unused3;
    struct MsgData *messageLoader;
    struct MessageFormat *stringTemplate;
    struct String *string;
    struct SpriteManager *spriteManager;
    struct ManagedSprite *sprites[38];
    Window messageBoxWindows[2];
    Window *windows;
    u8 numWindows;
    u8 useAltSummaryWindows;
    u8 partySlotLearningMove;
    u8 displayingContestStats : 4;
    u8 hasVisitedContestHall : 4;
    u8 currentState;
    u8 queuedState;
    u8 currentScreen;
    u8 textPrinterID;
    u8 useItemState;
    u16 selectedPokemonCurrentHP;
    u16 selectedPokemonCurrentMovePPs[4];
    struct BattleSubMenuCursor *cursor;
    u8 selectPokemonPreviousScreenButton;
    u8 learnMovePreviousScreenButton;
    u8 confirmLearnMovePreviousScreenButton;
    u8 battlerSlot;
} BattleParty; // DS 0x2090

#endif // POKEHEARTGOLD_OVERLAY_08_BATTLE_PARTY_H
