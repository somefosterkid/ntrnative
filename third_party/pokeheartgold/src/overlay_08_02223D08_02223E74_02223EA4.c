#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_08_battle_bag.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "battle/battle.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_08_battle_party.h"
#include "struct_defs/resdat_narc_structs.h"
#include "battle/battle_bag_context.h"

struct BattleSystem;

struct SpriteList;

struct SysTask;

struct SysTaskQueue;

typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

ManagedSprite *SpriteSystem_NewSprite(SpriteSystem *spriteSys, SpriteManager *spriteMan, const ManagedSpriteTemplate *template);

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

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

struct BattleCursor;

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

struct MsgData;

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

SpriteSystem *BattleSystem_GetSpriteSystem(BattleSystem *battleSys);

typedef struct SpriteResourcesAndPriority {
    int charResID;
    int plttResID;
    int cellResID;
    int animResID;
    int priority;
} SpriteResourcesAndPriority;

extern const SpriteResourcesAndPriority ov08_02225DDC[6];

ManagedSprite *ov08_02223EA4(BattleBag *battleBag, u32 spriteIndex);

ManagedSprite *ov08_02223EA4(BattleBag *battleBag, u32 spriteIndex) {
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(battleBag->context->battleSys);
    ManagedSpriteTemplate template;

    template.x = 0;
    template.y = 0;
    template.z = 0;
    template.animation = 0;
    template.drawPriority = ov08_02225DDC[spriteIndex].priority;
    template.pal = 0;
    template.vram = NNS_G2D_VRAM_TYPE_2DSUB;
    template.resIdList[GF_GFX_RES_TYPE_CHAR] = ov08_02225DDC[spriteIndex].charResID;
    template.resIdList[GF_GFX_RES_TYPE_PLTT] = ov08_02225DDC[spriteIndex].plttResID;
    template.resIdList[GF_GFX_RES_TYPE_CELL] = ov08_02225DDC[spriteIndex].cellResID;
    template.resIdList[GF_GFX_RES_TYPE_ANIM] = ov08_02225DDC[spriteIndex].animResID;
    template.bgPriority = 1;
    template.vramTransfer = FALSE;

    return SpriteSystem_NewSprite(spriteSystem, battleBag->spriteManager, &template);
}
