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

struct SelectedPaletteData;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct String;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct SysTask;

struct SysTaskQueue;

struct PlayerProfile;

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

void ov08_02224B98(BattleSubMenuCursor *cursor, u8 positionIndex);

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

enum BattleBagScreen {
    BATTLE_BAG_SCREEN_MENU = 0,
    BATTLE_BAG_SCREEN_POCKET_MENU,
    BATTLE_BAG_SCREEN_USE_ITEM,
};

void ov08_02224BCC(BattleSubMenuCursor *cursor, const GridMenuCursorPosition *positions);

extern const GridMenuCursorPosition *const ov08_02225CF8[];

void ov08_02224134(BattleBag *battleBag, enum BattleBagScreen screen);

void ov08_02224134(BattleBag *battleBag, enum BattleBagScreen screen) {
    ov08_02224BCC(battleBag->cursor, ov08_02225CF8[screen]);

    switch (screen) {
    case BATTLE_BAG_SCREEN_MENU:
        ov08_02224B98(battleBag->cursor, battleBag->currentBattlePocket);
        break;
    case BATTLE_BAG_SCREEN_POCKET_MENU:
        ov08_02224B98(battleBag->cursor, battleBag->context->pocketCurrentPagePositions[battleBag->currentBattlePocket]);
        break;
    case BATTLE_BAG_SCREEN_USE_ITEM:
        break;
    }
}
