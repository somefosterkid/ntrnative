#ifndef POKEHEARTGOLD_OVERLAY_08_BATTLE_BAG_H
#define POKEHEARTGOLD_OVERLAY_08_BATTLE_BAG_H

// BattleBag: the in-battle bag menu's work object (overlay 08), SysTask data from ov08_022225D4, DS size 0x115c.
// Typed whole from the per-file copies (hk593): the button data tables are [state][cells], the item list is
// [pocket][slot] (the promote.py merge had them transposed). Uses the real Window / ItemSlot / MessageFormat
// (hk601: the m2c type-soup files 02222B8C and 022237C4 no longer carry their own copies).
// State counts: 3 = BATTLE_SUB_MENU_BUTTON_STATE_MAX_CANNOT_DISABLE, 4 = BATTLE_SUB_MENU_BUTTON_STATE_MAX.

#include "bg_window.h"
#include "item.h"
#include "message_format.h"

struct BattleBagContext;
struct BattleFinger;
struct BattleSubMenuCursor;
struct ManagedSprite;
struct FontSpecialCharsContext;
struct MsgData;
struct PaletteData;
struct SpriteManager;

enum BattleBagPocketMenuWindow {
    BATTLE_BAG_POCKET_MENU_WINDOW_ITEM_SLOT_1_NAME = 0,
    BATTLE_BAG_POCKET_MENU_WINDOW_ITEM_SLOT_1_AMOUNT,
    BATTLE_BAG_POCKET_MENU_WINDOW_ITEM_SLOT_2_NAME,
    BATTLE_BAG_POCKET_MENU_WINDOW_ITEM_SLOT_2_AMOUNT,
    BATTLE_BAG_POCKET_MENU_WINDOW_ITEM_SLOT_3_NAME,
    BATTLE_BAG_POCKET_MENU_WINDOW_ITEM_SLOT_3_AMOUNT,
    BATTLE_BAG_POCKET_MENU_WINDOW_ITEM_SLOT_4_NAME,
    BATTLE_BAG_POCKET_MENU_WINDOW_ITEM_SLOT_4_AMOUNT,
    BATTLE_BAG_POCKET_MENU_WINDOW_ITEM_SLOT_5_NAME,
    BATTLE_BAG_POCKET_MENU_WINDOW_ITEM_SLOT_5_AMOUNT,
    BATTLE_BAG_POCKET_MENU_WINDOW_ITEM_SLOT_6_NAME,
    BATTLE_BAG_POCKET_MENU_WINDOW_ITEM_SLOT_6_AMOUNT,
    BATTLE_BAG_POCKET_MENU_WINDOW_ALT_ITEM_SLOT_1_NAME,
    BATTLE_BAG_POCKET_MENU_WINDOW_ALT_ITEM_SLOT_1_AMOUNT,
    BATTLE_BAG_POCKET_MENU_WINDOW_ALT_ITEM_SLOT_2_NAME,
    BATTLE_BAG_POCKET_MENU_WINDOW_ALT_ITEM_SLOT_2_AMOUNT,
    BATTLE_BAG_POCKET_MENU_WINDOW_ALT_ITEM_SLOT_3_NAME,
    BATTLE_BAG_POCKET_MENU_WINDOW_ALT_ITEM_SLOT_3_AMOUNT,
    BATTLE_BAG_POCKET_MENU_WINDOW_ALT_ITEM_SLOT_4_NAME,
    BATTLE_BAG_POCKET_MENU_WINDOW_ALT_ITEM_SLOT_4_AMOUNT,
    BATTLE_BAG_POCKET_MENU_WINDOW_ALT_ITEM_SLOT_5_NAME,
    BATTLE_BAG_POCKET_MENU_WINDOW_ALT_ITEM_SLOT_5_AMOUNT,
    BATTLE_BAG_POCKET_MENU_WINDOW_ALT_ITEM_SLOT_6_NAME,
    BATTLE_BAG_POCKET_MENU_WINDOW_ALT_ITEM_SLOT_6_AMOUNT,
    BATTLE_BAG_POCKET_MENU_WINDOW_POCKET_NAME,
    BATTLE_BAG_POCKET_MENU_WINDOW_PAGE_NUMS,
};

enum BattleBagUseItemMenuWindow {
    BATTLE_BAG_USE_ITEM_MENU_WINDOW_ITEM_NAME = 0,
    BATTLE_BAG_USE_ITEM_MENU_WINDOW_ITEM_AMOUNT,
    BATTLE_BAG_USE_ITEM_MENU_WINDOW_ITEM_DESC,
    BATTLE_BAG_USE_ITEM_MENU_WINDOW_ITEM_USE,
};

typedef struct BattleBag {
    struct BattleBagContext *context;
    struct BgConfig *background;
    struct PaletteData *palette;
    struct FontSpecialCharsContext *sLotoId;
    struct MsgData *messageLoader;
    MessageFormat *stringTemplate;
    String *string;
    Window messageBoxWindow;
    Window *windows;
    u8 numWindows;
    u8 useAltPocketMenuWindows;
    u8 textPrinterID;
    u8 pad_33[1];
    struct BattleSubMenuCursor *cursor;
    struct BattleFinger *catchTutorialCursor;
    ItemSlot items[5][36];
    struct SpriteManager *spriteManager;
    struct ManagedSprite *sprites[6];
    u16 menuPocketButtonData[3][(16 * 9)];
    u16 useItemButtonData[4][(26 * 5)];
    u16 cancelButtonData[3][(5 * 5)];
    u16 pocketItemButtonData[4][(16 * 6)];
    u16 pocketPrevPageButtonData[4][(5 * 5)];
    u16 pocketNextPageButtonData[4][(5 * 5)];
    u16 menuScreenRecoverHPPocketIconData[3][(4 * 4)];
    u16 menuScreenRecoverStatusPocketIconData[3][(4 * 4)];
    u16 menuScreenPokeBallsPocketIconData[3][(4 * 4)];
    u16 menuScreenBattleItemsIconData[3][(4 * 4)];
    u8 pressedButtonState;
    u8 unused2;
    u8 pressedButton;
    u8 isAButtonPressed : 4;
    u8 unused3 : 4;
    u8 unused4[8];
    u8 currentState;
    u8 queuedState;
    u8 currentScreen;
    u8 currentBattlePocket;
    s8 queuedBattlePocketPageChange;
    u8 numBattlePocketItems[5];
    u8 numBattlePocketPages[5];
    u8 catchTutorialState;
    u8 catchTutorialTickCount;
    u8 pad_115B[1];
} BattleBag;

// sizeof(BattleBag) == 0x115c on the DS

#endif // POKEHEARTGOLD_OVERLAY_08_BATTLE_BAG_H
