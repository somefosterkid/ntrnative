#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_08_battle_bag.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "battle/battle.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_08_battle_party.h"
#include "struct_defs/resdat_narc_structs.h"
#include "battle/battle_bag_context.h"

struct Bag;

struct BagItem;

struct BattleBag;

struct BattleBagContext;

struct BattleSystem;

struct GridMenuCursorPosition;

struct Indicator;

struct ManagedSprite;

struct MessageBank;

struct MessageBankEntry;

struct SpriteManager;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct SpriteSystem;

struct String;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct SysTask;

struct PlayerProfile;

typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

SpriteManager *SpriteManager_New(SpriteSystem *spriteSys);

BOOL SpriteSystem_InitSprites(SpriteSystem *spriteSys, SpriteManager *spriteMan, int maxSprites);

BOOL SpriteSystem_InitManagerWithCapacities(SpriteSystem *spriteSys, SpriteManager *spriteMan, const SpriteResourceCountsListUnion *capacities);

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

SpriteSystem *BattleSystem_GetSpriteSystem(BattleSystem *battleSys);

void ov08_02224134(BattleBag *battleBag, enum BattleBagScreen screen);

void ov08_02223D34(BattleBag *battleBag);

void ov08_02223D34(BattleBag *battleBag) {
    SpriteResourceCountsListUnion capacities = { .asArray[GF_GFX_RES_TYPE_CHAR] = 8, .asArray[GF_GFX_RES_TYPE_PLTT] = 8, .asArray[GF_GFX_RES_TYPE_CELL] = 3, .asArray[GF_GFX_RES_TYPE_ANIM] = 3, .asArray[GF_GFX_RES_TYPE_MCEL] = 0, .asArray[GF_GFX_RES_TYPE_MANM] = 0 };
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(battleBag->context->battleSys);

    battleBag->spriteManager = SpriteManager_New(spriteSystem);

    SpriteSystem_InitSprites(spriteSystem, battleBag->spriteManager, 12);
    SpriteSystem_InitManagerWithCapacities(spriteSystem, battleBag->spriteManager, &capacities);
}
