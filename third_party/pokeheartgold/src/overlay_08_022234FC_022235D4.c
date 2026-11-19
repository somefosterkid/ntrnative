#include "global.h"
#include "sprite_resources_header.h"
#include "constants/heap.h"
#include "struct_defs/resdat_narc_structs.h"
#include "font.h"
#include "palette.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "sprite_system.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "bag_types_def.h"
#include "player_data.h"
#include "message_format.h"
#include "battle/battle.h"
#include "overlay_08_battle_party.h"
#include "battle/battle_bag_context.h"
#include "overlay_08_battle_bag.h"

struct Bag;

struct BagItem;

struct BattleBag;

struct BattleBagContext;

struct BattleSubMenuCursor;

struct BattleSystem;

struct BgConfig;

struct BattleCursor;

struct MessagePrinter;

struct GridMenuCursorPosition;

struct Indicator;

struct ManagedSprite;

struct MessageBank;

struct MessageBankEntry;

struct MsgData;

struct NARC;

struct PaletteData;

struct SelectedPaletteData;

struct SpriteManager;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct String;

struct MessageFormat;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct SysTask;

struct SysTaskQueue;

struct PlayerProfile;

struct Window;

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

void ScheduleWindowCopyToVram(Window *window);

void FillWindowPixelBuffer(Window *window, u8 val);

u8 GetWindowWidth(Window *window);

typedef u16 charcode_t;

void String_Delete(String *string);

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

void BufferItemName(MessageFormat *template, u32 idx, u32 item);

typedef u32 TextColor;

u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontID, const String *string, u32 xOffset, u32 yOffset, u32 renderDelay, TextColor color, PrinterCallback_t callback);

void PrintTextToWindow(BattleBag *battleBag, u8 windowIndex, u32 textID, enum Font font, u32 yOffset, TextColor color);

void ov08_02223678(BattleBag *battleBag, u32 itemIndex, u32 slot, u32 windowIndex, enum Font font, u32 yOffset, TextColor color);

extern const struct {
    u32 name;
    u32 amount;
} ov08_02225BE0[];

void ov08_022235D4(BattleBag *battleBag, u32 itemIndex, u32 slot, u32 windowIndex, enum Font font, TextColor color);

void ov08_022235D4(BattleBag *battleBag, u32 itemIndex, u32 slot, u32 windowIndex, enum Font font, TextColor color) {
    Window *window = &battleBag->windows[windowIndex];

    FillWindowPixelBuffer(window, 0);

    if (battleBag->items[battleBag->currentBattlePocket][itemIndex].id != ITEM_NONE) {
        String *string = NewString_ReadMsgData(battleBag->messageLoader, ov08_02225BE0[slot].name);
        u32 width;
        u32 xOffset;

        BufferItemName(battleBag->stringTemplate, 0, battleBag->items[battleBag->currentBattlePocket][itemIndex].id);
        StringExpandPlaceholders(battleBag->stringTemplate, battleBag->string, string);

        width = FontID_String_GetWidth(font, battleBag->string, 0);
        xOffset = (GetWindowWidth(window) * 8 - width) / 2;

        AddTextPrinterParameterizedWithColor(window, font, battleBag->string, xOffset, 7, 0xFF, color, 0L);
        String_Delete(string);
    }

    ScheduleWindowCopyToVram(window);
}
