#include "global.h"
#include "sprite_resources_header.h"

#include "bg_window.h"
#include "heap.h"
#include "overlay_08_battle_bag.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "struct_defs/resdat_narc_structs.h"
#include "gf_gfx_planes.h"
#include "battle/battle.h"
#include "overlay_41_sprite_resource_collection.h"
#include "battle/battle_cursor.h"
#include "overlay_08_battle_party.h"
#include "battle/battle_bag_context.h"

struct Bag;

struct BagItem;

struct BattleBag;

struct BattleBagContext;

struct BattleSubMenuButtonDimensions;

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

typedef struct BattleSubMenuButtonDimensions {
    u8 xCoord;
    u8 yCoord;
    u8 width;
    u8 height;
} BattleSubMenuButtonDimensions;


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

extern void ov08_02224768(BattleBag *battleBag, u16 *buttonData, u8 button, u8 buttonState, u8 screen);

void UpdateSpritePositions(BattleBag *battleBag, u8 button, enum BattleSubMenuButtonState buttonState);

void ov08_022247E4(BattleBag *battleBag, u8 button, u8 buttonState, u8 screen);

extern const BattleSubMenuButtonDimensions ov08_02225E9C[17];

void ov08_022247E4(BattleBag *battleBag, u8 button, u8 buttonState, u8 screen) {
    u16 *buttonData = Heap_Alloc(battleBag->context->heapID, ov08_02225E9C[button].width * ov08_02225E9C[button].height * sizeof(u16));

    ov08_02224768(battleBag, buttonData, button, buttonState, screen);

    LoadRectToBgTilemapRect(battleBag->background, GF_BG_LYR_SUB_2, buttonData, ov08_02225E9C[button].xCoord, ov08_02225E9C[button].yCoord, ov08_02225E9C[button].width, ov08_02225E9C[button].height);
    ScheduleBgTilemapBufferTransfer(battleBag->background, GF_BG_LYR_SUB_2);
    Heap_Free(buttonData);
}
