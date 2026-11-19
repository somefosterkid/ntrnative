#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_08_battle_bag.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "constants/heap.h"
#include "struct_defs/resdat_narc_structs.h"
#include "battle/battle.h"
#include "overlay_41_sprite_resource_collection.h"
#include "battle/battle_cursor.h"
#include "battle/battle_bag_context.h"

struct Bag;

struct BagItem;

struct BattleBag;

struct BattleBagContext;

struct BattleSubMenuCursor;

struct BattleSystem;


struct MessagePrinter;

struct GridMenuCursorPosition;

struct Indicator;

struct ManagedSprite;

struct MessageBank;

struct MessageBankEntry;

struct PaletteData;

struct SelectedPaletteData;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct String;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct SysTask;

struct SysTaskQueue;

struct PlayerProfile;

u16 PaletteData_GetSelectedBuffersBitmask(PaletteData *paletteData);

typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

typedef struct Indicator {
    ManagedSprite *sprite;
    SysTask *animTask;
    int x;
    int y;
    int bounceAngle;
    fx32 subscreenOffset;
    s16 exitTimer;
    u8 exitPending;
    u8 hasDropped;
    u8 isExiting;
    u8 exitStep;
    u8 exitFrameCount;
} Indicator;


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

u32 ov08_02224C94(BattleSubMenuCursor *cursor);

typedef struct BagItem {
    u16 item;
    u16 quantity;
} BagItem;

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

enum BattleBagMenuScreenButton {
    BATTLE_BAG_MENU_SCREEN_BUTTON_RECOVER_HP_POCKET = 0,
    BATTLE_BAG_MENU_SCREEN_BUTTON_RECOVER_STATUS_POCKET,
    BATTLE_BAG_MENU_SCREEN_BUTTON_POKE_BALLS_POCKET,
    BATTLE_BAG_MENU_SCREEN_BUTTON_BATTLE_ITEMS_POCKET,
    BATTLE_BAG_MENU_SCREEN_BUTTON_LAST_USED_ITEM,
    BATTLE_BAG_MENU_SCREEN_BUTTON_CANCEL,
};

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

void ov08_02224938(BattleBag *battleBag, u8 pressedButton, u8 unused);

void ov08_0222417C(BattleBag *battleBag);

enum BattlePocketIndex {
    BATTLE_POCKET_INDEX_RECOVER_HP_PP = 0,
    BATTLE_POCKET_INDEX_RECOVER_STATUS,
    BATTLE_POCKET_INDEX_POKE_BALLS,
    BATTLE_POCKET_INDEX_BATTLE_ITEMS,
    BATTLE_POCKET_INDEX_NONE,
};

void ov08_02223BA8(BattleBag *battleBag);

BOOL PlaySE(u16 seqID);

int ov08_02223368(BattleBag *battleBag, const TouchScreenRect *rect);

extern const TouchScreenRect ov08_02225B4C[7];

void ov08_02223390(BattleSystem *battleSys, u16 item, u16 category, enum HeapID heapID);

u8 ov08_02222840(BattleBag *battleBag);

u8 ov08_02222840(BattleBag *battleBag) {
    if (PaletteData_GetSelectedBuffersBitmask(battleBag->palette) != 0) {
        return 1;
    }

    enum BattleBagMenuScreenButton menuButtonPressed = (enum BattleBagMenuScreenButton)(ov08_02223368(battleBag, ov08_02225B4C));

    if (menuButtonPressed == -1) {
        menuButtonPressed = (enum BattleBagMenuScreenButton)(ov08_02224C94(battleBag->cursor));

        if (menuButtonPressed == 0xfffffffe) {
            menuButtonPressed = BATTLE_BAG_MENU_SCREEN_BUTTON_CANCEL;
        }
    } else {
        ov08_0222417C(battleBag);
    }

    switch (menuButtonPressed) {
    case BATTLE_BAG_MENU_SCREEN_BUTTON_RECOVER_HP_POCKET:
    case BATTLE_BAG_MENU_SCREEN_BUTTON_RECOVER_STATUS_POCKET:
    case BATTLE_BAG_MENU_SCREEN_BUTTON_POKE_BALLS_POCKET:
    case BATTLE_BAG_MENU_SCREEN_BUTTON_BATTLE_ITEMS_POCKET:
        PlaySE(1501);
        battleBag->currentBattlePocket = menuButtonPressed;
        battleBag->queuedState = 5;
        ov08_02224938(battleBag, menuButtonPressed, 0);
        return 11;
    case BATTLE_BAG_MENU_SCREEN_BUTTON_LAST_USED_ITEM:
        if (battleBag->context->lastUsedItem != ITEM_NONE) {
            PlaySE(1501);
            battleBag->currentBattlePocket = battleBag->context->lastUsedItemPocket;
            battleBag->queuedState = 6;
            ov08_02223BA8(battleBag);
            ov08_02224938(battleBag, BATTLE_BAG_MENU_SCREEN_BUTTON_LAST_USED_ITEM, 0);
            return 11;
        }
        break;
    case BATTLE_BAG_MENU_SCREEN_BUTTON_CANCEL:
        PlaySE(1501);
        battleBag->context->selectedBattleBagItem = ITEM_NONE;
        battleBag->context->selectedBattleBagPocket = BATTLE_POCKET_INDEX_NONE;
        ov08_02224938(battleBag, BATTLE_BAG_MENU_SCREEN_BUTTON_CANCEL, 0);
        return 13;
    }

    return 1;
}
