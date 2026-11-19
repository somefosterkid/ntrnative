#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_08_battle_bag.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "struct_defs/resdat_narc_structs.h"
#include "gf_gfx_planes.h"
#include "battle/battle.h"
#include "overlay_41_sprite_resource_collection.h"
#include "battle/battle_cursor.h"
#include "battle/battle_bag_context.h"

struct Bag;

struct BagItem;

struct BattleBag;

struct BattleBagContext;

struct BattleSystem;

struct BgConfig;

struct BgTemplate;


struct MessagePrinter;

struct GraphicsModes;

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

typedef struct BagItem {
    u16 item;
    u16 quantity;
} BagItem;

enum BgType {
    BG_TYPE_STATIC = 0,
    BG_TYPE_AFFINE,
    BG_TYPE_STATIC_WITH_AFFINE,
};

void InitBgFromTemplate(BgConfig *bgConfig, u8 bgLayer, const BgTemplate *bgTemplate, u8 bgType);

void BG_ClearCharDataRange(u8 bgLayer, u32 size, u32 offset, enum HeapID heapID);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

void ScheduleBgTilemapBufferTransfer(BgConfig *bgConfig, u8 bgLayer);

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

void ov08_02223248(BattleBag *battleBag, enum BattleBagScreen screen);

void ov08_02223390(BattleSystem *battleSys, u16 item, u16 category, enum HeapID heapID);

void ov08_02223000(BattleBag *battleBag);

extern const GraphicsModes ov08_02225AE8;

extern const BgTemplate ov08_02225B30;

extern const BgTemplate ov08_02225B14;

extern const BgTemplate ov08_02225AF8;

void ov08_02223000(BattleBag *battleBag) {
    {
        GraphicsModes graphicsMode = ov08_02225AE8;

        SetScreenModeAndDisable(&graphicsMode, SCREEN_SUB);
    }

    {
        BgTemplate background = ov08_02225B30;

        InitBgFromTemplate(battleBag->background, GF_BG_LYR_SUB_2, &background, BG_TYPE_STATIC);
    }

    {
        BgTemplate background = ov08_02225B14;

        InitBgFromTemplate(battleBag->background, GF_BG_LYR_SUB_1, &background, BG_TYPE_STATIC);
        BgClearTilemapBufferAndCommit(battleBag->background, GF_BG_LYR_SUB_1);
    }

    {
        BgTemplate background = ov08_02225AF8;

        InitBgFromTemplate(battleBag->background, GF_BG_LYR_SUB_0, &background, BG_TYPE_STATIC);
        BgClearTilemapBufferAndCommit(battleBag->background, GF_BG_LYR_SUB_0);
    }

    BG_ClearCharDataRange(GF_BG_LYR_SUB_1, ((16) * sizeof(u16)), 0, battleBag->context->heapID);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, ((16) * sizeof(u16)), 0, battleBag->context->heapID);
    ScheduleBgTilemapBufferTransfer(battleBag->background, GF_BG_LYR_SUB_1);
    ScheduleBgTilemapBufferTransfer(battleBag->background, GF_BG_LYR_SUB_0);
}
